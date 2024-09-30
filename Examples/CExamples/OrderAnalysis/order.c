// SigLib - Program to read vibration data file and generate an order analysis
// This program re-orders the data into an ordergram
// Copyright (c) 2024 Delta Numerix All rights reserved.

// Include files
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <siglib.h>               // SigLib DSP library
#include <gnuplot_c.h>            // Gnuplot/C
#include <siglib_host_utils.h>    // Optionally includes conio.h and time.h subset functions
#include "readdat.h"

#define ENABLE_DEBUG_LOG 0    // Set to '1' to enable debug logging and '0' to disable

// Define constants
#define SAMPLE_LENGTH 4096    // Length of array read from input file
#define FFT_LENGTH 4096
#define LOG2_FFT_LENGTH SAI_FftLengthLog2(FFT_LENGTH)    // Log2 FFT length,

#define GRAPH_X_AXIS_LENGTH 240

// Parameters for quick sinc look up table
#define SINC_NUMBER_OF_ADJ_SAMPLES 3    // Number of adjacent samples
#define SINC_LUT_LENGTH 1001

#define RESULT_LENGTH (FFT_LENGTH >> 1)        // Only need to store the lower 1/2 of the FFT output
#define OVERLAP_LENGTH (SAMPLE_LENGTH >> 2)    // 25 % overlap

#define SAMPLE_PERIOD (SIGLIB_ONE / SampleRate)    // Sample rate of data in vibration data file

#define MAX_NUMBER_OF_FRAMES 400    // Maximum number of frames processed

#define FIRST_ORDER_FREQUENCY 20.    // Destination frequency of first order
#define TENTH_ORDER_BIN \
  ((SLArrayIndex_t)((FIRST_ORDER_FREQUENCY * 10. * ((SLData_t)FFT_LENGTH)) / ((SLData_t)SampleRate)))    // Destination FFT bin of first order

#define BASE_ORDER 10                    // Base order
#define NUMBER_OF_ORDERS 10              // Maximum number of orders to extract
#define ORDER_NUMBER_OF_ADJ_SAMPLES 3    // Number of adjacent samples to search

#define DB_SCALE (1e5)    // Input signal scaling - bar to dB*/
// #define LINEAR_SCALE              (1e-5)                             // Input
// signal scaling - dB to Bar

#define NUMBER_OF_ORDERS_TO_SUM 5    // Number of orders to Sum

// Declare global variables and arrays
static SLData_t LookUpTablePhaseGain;
static SLData_t SincLUT[SINC_LUT_LENGTH];

static char Filename[80];

int main(int argc, char* argv[])
{
  h_GPC_Plot* hOrderPlot;    // Plot objects
  h_GPC_Plot* hAverageSpectrumGraph;
  h_GPC_Plot* hOrdergram;

  SLArrayIndex_t sampleCount;
  FILE* fpInputFile;
  SLArrayIndex_t FrameNumber = SIGLIB_AI_ZERO;    // Number of frames processed
  SLArrayIndex_t OverlapSrcArrayIndex;
  SLArrayIndex_t LogMagnitudeFlag, XAxisTimeFlag;

  SLData_t WindowInverseCoherentGain = SIGLIB_ONE;
  SLData_t Speed;
  SLData_t SampleRate;

  SLData_t* pInputData = SUF_VectorArrayAllocate(SAMPLE_LENGTH);                               // Input data array
  SLData_t* pOverlapArray = SUF_VectorArrayAllocate(OVERLAP_LENGTH);                           // Overlap data array
  SLData_t* pWindowCoeffs = SUF_VectorArrayAllocate(FFT_LENGTH);                               // Window coeffs data array
  SLData_t* pOverlappedData = SUF_VectorArrayAllocate(SAMPLE_LENGTH);                          // Overlapped data array
  SLData_t* pOrderAnalysisInternalArray = SUF_OrderAnalysisArrayAllocate(FFT_LENGTH);          // Order analysis internal processing array
  SLData_t* pOrderMagnitudeResults = SUF_VectorArrayAllocate(RESULT_LENGTH);                   // Results data array
  SLData_t* pFFTCoeffs = SUF_FftCoefficientAllocate(FFT_LENGTH);                               // FFT coefficient data array
  SLData_t* pOrderArray = SUF_VectorArrayAllocate(NUMBER_OF_ORDERS * MAX_NUMBER_OF_FRAMES);    // Order results data array
  SLData_t* pSumLevelArray = SUF_VectorArrayAllocate(MAX_NUMBER_OF_FRAMES);                    // Sum level results data array
  SLData_t* pRealAverage = SUF_VectorArrayAllocate(RESULT_LENGTH);                             // Average order spectrum data array
  SLData_t* pImagAverage = SUF_VectorArrayAllocate(RESULT_LENGTH);                             // Average order spectrum data array
  SLData_t* pSpeed = SUF_VectorArrayAllocate(MAX_NUMBER_OF_FRAMES);                            // Speed data array

  if ((NULL == pInputData) || (NULL == pOverlapArray) || (NULL == pWindowCoeffs) || (NULL == pOverlappedData) ||
      (NULL == pOrderAnalysisInternalArray) || (NULL == pOrderMagnitudeResults) || (NULL == pFFTCoeffs) || (NULL == pOrderArray) ||
      (NULL == pSumLevelArray) || (NULL == pRealAverage) || (NULL == pImagAverage) || (NULL == pSpeed)) {

    printf("Memory allocation error\n");
    exit(0);
  }

#if ENABLE_DEBUG_LOG
  SUF_ClearDebugfprintf();
#endif

  // Reset the copy with overlap
  SIF_CopyWithOverlap(&OverlapSrcArrayIndex);    // Pointer to overlap source array index

  SIF_OrderAnalysis(SincLUT,                       // Pointer to sinc LUT array
                    &LookUpTablePhaseGain,         // Pointer to phase gain
                    SINC_NUMBER_OF_ADJ_SAMPLES,    // Number of adjacent samples
                    SINC_LUT_LENGTH,               // Look up table length
                    pWindowCoeffs,                 // Window coefficients pointer
                    SIGLIB_BLACKMAN_HARRIS,        // Window type
                    SIGLIB_ZERO,                   // Window coefficient
                    &WindowInverseCoherentGain,    // Window inverse coherent gain
                    pFFTCoeffs,                    // Pointer to FFT coefficients
                    SIGLIB_BIT_REV_STANDARD,       // Bit reverse mode flag / Pointer
                                                   // to bit reverse address table
                    pRealAverage,                  // Pointer to real average array
                    pImagAverage,                  // Pointer to imaginary average array
                    FFT_LENGTH);                   // FFT length

  if (argc != 5) {
    printf("\nUsage:\norder SampleRate D/L T/S filename\n");
    printf("    D/L indicates dB or linear scaling\n");
    printf("    T/S indicates time or total speed on x-axis\n\n");
    exit(-1);    // Exit - usage error
  }

  SampleRate = atof(argv[1]);    // Initilize sample rate

  printf("SampleRate = %1.1lf\n", SampleRate);

  if (toupper(argv[2][0]) == 'D') {    // Should we display in dB or linear
    LogMagnitudeFlag = SIGLIB_TRUE;
  } else if (toupper(argv[2][0]) == 'L') {
    LogMagnitudeFlag = SIGLIB_FALSE;
  } else {
    printf("Error: Scaling option should be D or L for dB or linear\n");
    exit(-1);
  }

  if (toupper(argv[3][0]) == 'T') {    // Should we display time or speed on the x-axis
    XAxisTimeFlag = SIGLIB_TRUE;
  } else if (toupper(argv[3][0]) == 'S') {
    XAxisTimeFlag = SIGLIB_FALSE;
  } else {
    printf("Error: X-Axis option should be T or S for time or speed\n");
    exit(-1);
  }

  strcpy(Filename, argv[4]);
  printf("Source file = %s\n", Filename);

  if ((fpInputFile = fopen(Filename, "r")) == NULL) {    // Note this file is text
    printf("Error opening input vibration data file\n");
    exit(-1);
  }

  if (LogMagnitudeFlag == SIGLIB_TRUE) {                 // Create 3D graph with different
                                                         // scaling for log and linear plots
    hOrdergram =                                         // Initialize plot
        gpc_init_spectrogram("Machine Ordergram",        // Plot title
                             "Time",                     // X-Axis label
                             "Order",                    // Y-Axis label
                             GRAPH_X_AXIS_LENGTH,        // X-axis length
                             RESULT_LENGTH,              // Y-axis length
                             SIGLIB_ZERO,                // Minimum Y value
                             SampleRate / SIGLIB_TWO,    // Maximum Y value
                             SIGLIB_ZERO,                // Minimum Z value
                             100.0,                      // Maximum Z value
                             GPC_COLOUR,                 // Graph mode
                             GPC_KEY_ENABLE);            // Legend / key mode
    if (NULL == hOrdergram) {
      printf("\nPlot creation failure.\n");
      exit(-1);
    }
  } else {
    hOrdergram =                                         // Initialize plot
        gpc_init_spectrogram("Machine Ordergram",        // Plot title
                             "Time",                     // X-Axis label
                             "Order",                    // Y-Axis label
                             GRAPH_X_AXIS_LENGTH,        // X-axis length
                             RESULT_LENGTH,              // Y-axis length
                             SIGLIB_ZERO,                // Minimum Y value
                             SampleRate / SIGLIB_TWO,    // Maximum Y value
                             SIGLIB_ZERO,                // Minimum Z value
                             0.1,                        // Maximum Z value
                             GPC_COLOUR,                 // Graph mode
                             GPC_KEY_ENABLE);            // Legend / key mode
    if (NULL == hOrdergram) {
      printf("\nPlot creation failure.\n");
      exit(-1);
    }
  }

  hOrderPlot =                        // Initialize plot
      gpc_init_2d("Order Plot",       // Plot title
                  "Time",             // X-Axis label
                  "Magnitude",        // Y-Axis label
                  GPC_AUTO_SCALE,     // Scaling mode
                  GPC_POSITIVE,       // Sign mode
                  GPC_KEY_ENABLE);    // Legend / key mode
  if (NULL == hOrderPlot) {
    printf("\nPlot creation failure.\n");
    exit(-1);
  }

  hAverageSpectrumGraph =                // Initialize plot
      gpc_init_2d("Average Spectrum",    // Plot title
                  "Frequency",           // X-Axis label
                  "Magnitude",           // Y-Axis label
                  GPC_AUTO_SCALE,        // Scaling mode
                  GPC_SIGNED,            // Sign mode
                  GPC_KEY_ENABLE);       // Legend / key mode
  if (NULL == hOrderPlot) {
    printf("\nPlot creation failure.\n");
    exit(-1);
  }

#if ENABLE_DEBUG_LOG
  SUF_Debugfprintf("Starting order analysis\n");
#endif

  while ((sampleCount = read_vibration_data(pInputData, fpInputFile, &Speed, SAMPLE_LENGTH)) == SAMPLE_LENGTH) {
#if ENABLE_DEBUG_LOG
    SUF_Debugfprintf("FN1 = %d\n", FrameNumber);
    SUF_Debugfprintf("sampleCount = %d\n", sampleCount);
#endif

    // Apply the overlap to the data
    while (SDA_CopyWithOverlap(pInputData,               // Pointer to source array
                               pOverlappedData,          // Pointer to destination array
                               pOverlapArray,            // Pointer to overlap array
                               &OverlapSrcArrayIndex,    // Pointer to source array index
                               SAMPLE_LENGTH,            // Source dataset length
                               OVERLAP_LENGTH,           // Overlap length
                               SAMPLE_LENGTH)            // Destination dataset length
           < SAMPLE_LENGTH) {
      pSpeed[FrameNumber] = Speed;    // Save speed

#if ENABLE_DEBUG_LOG
      SUF_Debugfprintf("FrameNumber = %d\n", FrameNumber);
      SUF_Debugfprintf("Calling SDA_OrderAnalysis\n");
#endif

      // Process and extract order information
      *(pSumLevelArray + FrameNumber) = SDA_OrderAnalysis(pOverlappedData,                // Pointer to source array
                                                          pOrderAnalysisInternalArray,    // Pointer to local processing array
                                                          pOrderMagnitudeResults,         // Pointer to destination array
                                                          SincLUT,                        // Pointer to LUT array
                                                          LookUpTablePhaseGain,           // Look up table phase gain
                                                          FIRST_ORDER_FREQUENCY,          // First order frequency
                                                          Speed / 60.0,                   // Speed - revolutions per second
                                                          SINC_NUMBER_OF_ADJ_SAMPLES,     // Number of adjacent samples for
                                                                                          // interpolation
                                                          pWindowCoeffs,                  // Pointer to window coefficients
                                                          WindowInverseCoherentGain,      // Window inverse coherent gain
                                                          pFFTCoeffs,                     // Pointer to FFT coefficients
                                                          SIGLIB_BIT_REV_STANDARD,        // Bit reverse mode flag / Pointer to bit
                                                                                          // reverse address table
                                                          pRealAverage,                   // Pointer to real average array
                                                          pImagAverage,                   // Pointer to imaginary average array
                                                          LogMagnitudeFlag,               // Log magnitude flag
                                                          pOrderArray + (FrameNumber * NUMBER_OF_ORDERS),    // Pointer to order array
                                                          BASE_ORDER,                                        // Base order
                                                          NUMBER_OF_ORDERS,                                  // Number of orders to extract
                                                          ORDER_NUMBER_OF_ADJ_SAMPLES,                       // Number of adjacent samples to search
                                                          SAMPLE_PERIOD,                                     // Sample period
                                                          SIGLIB_SIGNAL_INCOHERENT,    // Signal source type for summing orders
                                                          DB_SCALE,                    // dB scale
                                                          NUMBER_OF_ORDERS_TO_SUM,     // Number of orders to sum
                                                          SAMPLE_LENGTH,               // Source dataset length
                                                          FFT_LENGTH,                  // FFT length
                                                          LOG2_FFT_LENGTH);            // log2 FFT length

      // Plot results
      gpc_plot_spectrogram(hOrdergram,                // Graph handle
                           pOrderMagnitudeResults,    // Dataset
                           "Ordergram Plot",          // Dataset title
                           SIGLIB_ZERO,               // Minimum X value
                           RESULT_LENGTH - 1);        // Maximum X value
      //                                ((double)NUMBER_OF_SAMPLES) /
      //                                SampleRate); // Maximum X value

      FrameNumber++;
    }
  }

  fclose(fpInputFile);    // Close input file

#if ENABLE_DEBUG_LOG
  SUF_Debugfprintf("Max frame number = %d\n", FrameNumber);
  SUF_Debugfprintf("Plotting orders\n");
#endif

  //  Plotting orders
  for (SLArrayIndex_t i = 0; i < NUMBER_OF_ORDERS; i++) {
    if (XAxisTimeFlag == SIGLIB_TRUE) {    // Display time on x-axis
      SLData_t PlotArray[500];
      for (SLArrayIndex_t j = 0; j < FrameNumber; j++) {    // Write out results for each order in turn
                                                            // Time (s), Order magnitude
        PlotArray[j] = *(pOrderArray + (j * NUMBER_OF_ORDERS) + i);
      }
      if (i == 0) {
        gpc_plot_2d(hOrderPlot,                                  // Graph handle
                    PlotArray,                                   // Dataset
                    FrameNumber,                                 // Dataset length
                    "Order results",                             // Dataset title
                    SIGLIB_ZERO,                                 // Minimum X value
                    (double)(FrameNumber - 1),                   // Maximum X value
                    "lines",                                     // Graph type
                    gpcPlotColours[i % GPC_NUM_PLOT_COLOURS],    // Colour
                    GPC_NEW);                                    // New graph
      } else {
        gpc_plot_2d(hOrderPlot,                                  // Graph handle
                    PlotArray,                                   // Dataset
                    FrameNumber,                                 // Dataset length
                    "Order results",                             // Dataset title
                    SIGLIB_ZERO,                                 // Minimum X value
                    (double)(FrameNumber - 1),                   // Maximum X value
                    "lines",                                     // Graph type
                    gpcPlotColours[i % GPC_NUM_PLOT_COLOURS],    // Colour
                    GPC_ADD);                                    // New graph
      }
    } else {    // Display speed on x-axis
      SLData_t PlotArray[500];
      for (SLArrayIndex_t j = 0; j < FrameNumber; j++) {    // Write out results for each order in turn
                                                            // Speed (rpm), Order magnitude
        PlotArray[j] = *(pOrderArray + (j * NUMBER_OF_ORDERS) + i);
      }
      if (i == 0) {
        gpc_plot_2d(hOrderPlot,                                  // Graph handle
                    PlotArray,                                   // Dataset
                    FrameNumber,                                 // Dataset length
                    "Order results",                             // Dataset title
                    SIGLIB_ZERO,                                 // Minimum X value
                    (double)(FrameNumber - 1),                   // Maximum X value
                    "lines",                                     // Graph type
                    gpcPlotColours[i % GPC_NUM_PLOT_COLOURS],    // Colour
                    GPC_NEW);                                    // New graph
      } else {
        gpc_plot_2d(hOrderPlot,                                  // Graph handle
                    PlotArray,                                   // Dataset
                    FrameNumber,                                 // Dataset length
                    "Order results",                             // Dataset title
                    SIGLIB_ZERO,                                 // Minimum X value
                    (double)(FrameNumber - 1),                   // Maximum X value
                    "lines",                                     // Graph type
                    gpcPlotColours[i % GPC_NUM_PLOT_COLOURS],    // Colour
                    GPC_ADD);                                    // New graph
      }
    }
  }

#if ENABLE_DEBUG_LOG
  SUF_Debugfprintf("Plot speed\n");
#endif

  if (XAxisTimeFlag == SIGLIB_TRUE) {    // Only plot speed if we are plotting orders against time
    // Plot speed - different scaling for variable and fixed speed
    printf("Maximum speed = %lf\n", SDA_Max(pSpeed, FrameNumber));

    if (LogMagnitudeFlag == SIGLIB_TRUE) {    // Plot ordergram graph with different speed scaling for
                                              // log and linear plots
      SDA_Divide(pSpeed, 100.0, pSpeed, FrameNumber);

      gpc_plot_2d(hOrderPlot,                   // Graph handle
                  pSpeed,                       // Dataset
                  FrameNumber,                  // Dataset length
                  "Speed x 100",                // Dataset title
                  SIGLIB_ZERO,                  // Minimum X value
                  (double)(FrameNumber - 1),    // Maximum X value
                  "lines",                      // Graph type
                  "yellow",                     // Colour
                  GPC_ADD);                     // New graph
    } else {
      SDA_Divide(pSpeed, 10000.0, pSpeed, FrameNumber);

      gpc_plot_2d(hOrderPlot,                   // Graph handle
                  pSpeed,                       // Dataset
                  FrameNumber,                  // Dataset length
                  "Speed x 10000",              // Dataset title
                  SIGLIB_ZERO,                  // Minimum X value
                  (double)(FrameNumber - 1),    // Maximum X value
                  "lines",                      // Graph type
                  "yellow",                     // Colour
                  GPC_ADD);                     // New graph
    }
  }

  if (XAxisTimeFlag == SIGLIB_TRUE) {         // Display time on x-axis
    gpc_plot_2d(hOrderPlot,                   // Graph handle
                pSumLevelArray,               // Dataset
                FrameNumber,                  // Dataset length
                "Order Sum",                  // Dataset title
                SIGLIB_ZERO,                  // Minimum X value
                (double)(FrameNumber - 1),    // Maximum X value
                "lines",                      // Graph type
                "red",                        // Colour
                GPC_ADD);                     // New graph
  } else {                                    // Display speed on x-axis
    gpc_plot_2d(hOrderPlot,                   // Graph handle
                pSumLevelArray,               // Dataset
                FrameNumber,                  // Dataset length
                "Order Sum",                  // Dataset title
                SIGLIB_ZERO,                  // Minimum X value
                (double)(FrameNumber - 1),    // Maximum X value
                "lines",                      // Graph type
                "red",                        // Colour
                GPC_ADD);                     // New graph
  }

#if ENABLE_DEBUG_LOG
  SUF_Debugfprintf("Order sum calculated\n");
#endif

  // Calculate average spectrum
  SDA_ComplexScalarDivide(pRealAverage,             // Pointer to real source array
                          pImagAverage,             // Pointer to imaginary source array
                          (SLData_t)FrameNumber,    // Scalar divisor
                          pRealAverage,             // Pointer to real destination array
                          pImagAverage,             // Pointer to imaginary destination array
                          RESULT_LENGTH);           // Dataset lengths

  if (LogMagnitudeFlag == SIGLIB_TRUE) {        // Calc real power fm complex
    SDA_LogMagnitude(pRealAverage,              // Pointer to real source array
                     pImagAverage,              // Pointer to imaginary source array
                     pOrderMagnitudeResults,    // Pointer to log magnitude destination array
                     RESULT_LENGTH);            // Dataset length
  } else {
    SDA_Magnitude(pRealAverage,              // Pointer to real source array
                  pImagAverage,              // Pointer to imaginary source array
                  pOrderMagnitudeResults,    // Pointer to magnitude destination array
                  RESULT_LENGTH);            // Dataset length
  }

#if ENABLE_DEBUG_LOG
  SUF_Debugfprintf("Calculating spectrum average\n");
#endif

  printf("Spectrum average\n");
  for (SLArrayIndex_t i = 1; i <= NUMBER_OF_ORDERS; i++) {    // Extract the required orders from the results
    printf("Order %d = %lf\n", BASE_ORDER * i,
           SDA_ExtractOrder(pOrderMagnitudeResults,         // Pointer to source array
                            BASE_ORDER * i,                 // Order to extract
                            ORDER_NUMBER_OF_ADJ_SAMPLES,    // Number of samples to search
                                                            // either side of centre
                            FIRST_ORDER_FREQUENCY,          // First order frequency
                            FFT_LENGTH,                     // FFT length
                            SAMPLE_PERIOD,                  // Sample period
                            RESULT_LENGTH));                // Input dataset length
  }
  printf("\n");

#if ENABLE_DEBUG_LOG
  SUF_Debugfprintf("Plot average spectrum\n");
#endif

  // Plot average spectrum
  gpc_plot_2d(hAverageSpectrumGraph,                                                  // Graph handle
              pRealAverage,                                                           // Dataset
              RESULT_LENGTH,                                                          // Dataset length
              "Average spectrum",                                                     // Dataset title
              SIGLIB_ZERO,                                                            // Minimum X value
              ((double)(RESULT_LENGTH - 1) / (SampleRate / ((double)FFT_LENGTH))),    // Maximum X value
              "lines",                                                                // Graph type
              "red",                                                                  // Colour
              GPC_NEW);                                                               // New graph

#if ENABLE_DEBUG_LOG
  SUF_Debugfprintf("Number of frames processed = %d\n", FrameNumber);
#endif

  printf("Number of frames processed = %d\n", FrameNumber);

  // Indicate end of plot
  gpc_plot_spectrogram(hOrdergram,          // Graph handle
                       GPC_END_PLOT,        // Dataset
                       "Ordergram Plot",    // Dataset title
                       SIGLIB_ZERO,         // Minimum X value
                       SIGLIB_ZERO);        // Maximum X value

  printf("\nHit <Carriage Return> to continue ....\n");
  getchar();    // Wait for <Carriage Return>
  gpc_close(hOrderPlot);
  gpc_close(hAverageSpectrumGraph);
  gpc_close(hOrdergram);

  SUF_MemoryFree(pInputData);    // Free memory
  SUF_MemoryFree(pOverlapArray);
  SUF_MemoryFree(pWindowCoeffs);
  SUF_MemoryFree(pOverlappedData);
  SUF_MemoryFree(pOrderAnalysisInternalArray);
  SUF_MemoryFree(pOrderMagnitudeResults);
  SUF_MemoryFree(pFFTCoeffs);
  SUF_MemoryFree(pOrderArray);
  SUF_MemoryFree(pSumLevelArray);
  SUF_MemoryFree(pRealAverage);
  SUF_MemoryFree(pImagAverage);
  SUF_MemoryFree(pSpeed);

  return (0);
}
