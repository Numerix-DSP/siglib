// SigLib Example to demonstrate detection of clipping in time and frequency domain
// Copyright (C) 2022 Delta Numerix

// Include files
#include <stdio.h>
#include <siglib_host_utils.h>                                      // Optionally includes conio.h and time.h subset functions
#include <siglib.h>                                                 // SigLib DSP library
#include <gnuplot_c.h>                                              // Gnuplot/C

// Define constants
#define SAMPLE_LENGTH       1024
#define DISPLAY_LENGTH      512
#define FFT_LENGTH          1024
#define LOG2_FFT_LENGTH     ((SLArrayIndex_t)SDS_Log2(FFT_LENGTH))
#define WINDOW_SIZE         FFT_LENGTH

#define INITIAL_MAGNITUDE   0.9995
#define INCREMENT_DECREMENT 0.0001
#define CLIP_LEVEL          1.0
#define GRAPH_PEAK_LEVEL    1.1

// Declare global variables and arrays


int main (
  void)
{
  h_GPC_Plot     *h2DPlotTime;                                      // Plot object
  h_GPC_Plot     *h2DPlotFreq;

  SLData_t        ZXPrevSign;                                       // Sign of last value in previous array, for zero crossing detect
  SLArrayIndex_t  TriggerLocation;                                  // Location of trigger in source array

  SLFixData_t     TriggerFlag = 1;
  enum SLLevelCrossingMode_t ZXFlag = SIGLIB_POSITIVE_LEVEL_CROSS;
  SLData_t        Magnitude = INITIAL_MAGNITUDE;
  SLData_t        previousMagnitude = INITIAL_MAGNITUDE;
  int             action;

  SLData_t       *pSrc = SUF_VectorArrayAllocate (SAMPLE_LENGTH);   // Allocate memory
  SLData_t       *pRealData = SUF_VectorArrayAllocate (FFT_LENGTH);
  SLData_t       *pImagData = SUF_VectorArrayAllocate (FFT_LENGTH);
  SLData_t       *pResults = SUF_VectorArrayAllocate (FFT_LENGTH);
  SLData_t       *pFFTCoeffs = SUF_FftCoefficientAllocate (FFT_LENGTH);
  SLData_t       *pWindowCoeffs = SUF_VectorArrayAllocate (FFT_LENGTH);

  if ((pSrc == NULL) || (pRealData == NULL) || (pImagData == NULL) || (pResults == NULL) || (pFFTCoeffs == NULL) || (pWindowCoeffs == NULL)) {
    printf ("\n\nMemory allocation failed\n\n");
    exit (0);
  }


  h2DPlotTime =                                                     // Initialize plot
    gpc_init_2d ("Clipping - Time Domain",                          // Plot title
                 "Time",                                            // X-Axis label
                 "Magnitude",                                       // Y-Axis label
                 SIGLIB_TWO,                                        // Scaling mode
                 GPC_SIGNED,                                        // Sign mode
                 GPC_KEY_DISABLE);                                  // Legend / key mode
  if (h2DPlotTime == NULL) {
    printf ("\nTime domain plot creation failure.\n");
    exit (1);
  }


  h2DPlotFreq =                                                     // Initialize plot
    gpc_init_2d ("Clipping - Frequency Domain",                     // Plot title
                 "Frequency",                                       // X-Axis label
                 "Magnitude",                                       // Y-Axis label
                 120.,                                              // Scaling mode
                 GPC_SIGNED,                                        // Sign mode
                 GPC_KEY_DISABLE);                                  // Legend / key mode
  if (h2DPlotFreq == NULL) {
    printf ("\nFrequency domain plot creation failure.\n");
    exit (1);
  }

  SLData_t        sinePhase = SIGLIB_ZERO;

// Generate Hanning window table
  SIF_Window (pWindowCoeffs,                                        // Pointer to window oefficient
              SIGLIB_HANNING,                                       // Window type
              SIGLIB_ZERO,                                          // Window coefficient
              WINDOW_SIZE);                                         // Window length

// Initialise FFT
  SIF_Fft (pFFTCoeffs,                                              // Pointer to FFT coefficients
           SIGLIB_BIT_REV_STANDARD,                                 // Pointer to bit reverse address table - NOT USED
           FFT_LENGTH);                                             // FFT length

  printf ("Clip level = %lf\n", CLIP_LEVEL);
  printf ("-/+ to decrease / increase signal level\n");
  printf ("T to toggle trigger\n");
  printf ("S to toggle +ve/-ve zero crossing\n");
  printf ("Any other key to exit ...\n");
  printf ("Signal magnitude = %lf\n", Magnitude);

  for (;;) {
    if (_kbhit ()) {
      action = tolower (_getch ());
      if (action == 't') {
        TriggerFlag ^= 1;
      }
      else if (action == 's') {
        if (ZXFlag == SIGLIB_POSITIVE_LEVEL_CROSS) {
          ZXFlag = SIGLIB_NEGATIVE_LEVEL_CROSS;
        }
        else {
          ZXFlag = SIGLIB_POSITIVE_LEVEL_CROSS;
        }
      }
      else if (action == '+') {
        Magnitude += INCREMENT_DECREMENT;
        if (Magnitude >= CLIP_LEVEL + 0.2) {
          Magnitude = CLIP_LEVEL + 0.2;
        }
      }
      else if ((action == '-') || (action == '_')) {
        Magnitude -= INCREMENT_DECREMENT;
        if (Magnitude <= CLIP_LEVEL - 0.2) {
          Magnitude = CLIP_LEVEL - 0.2;
        }
      }
      else {
        gpc_close (h2DPlotTime);
        gpc_close (h2DPlotFreq);

        SUF_MemoryFree (pSrc);                                      // Free memory
        SUF_MemoryFree (pRealData);
        SUF_MemoryFree (pImagData);
        SUF_MemoryFree (pResults);
        SUF_MemoryFree (pFFTCoeffs);
        SUF_MemoryFree (pWindowCoeffs);
        exit (0);
      }
    }
// Generate the carrier signal
    SDA_SignalGenerate (pSrc,                                       // Pointer to destination array
                        SIGLIB_SINE_WAVE,                           // Signal type - Sine wave
                        Magnitude,                                  // Signal peak level
                        SIGLIB_FILL,                                // Fill (overwrite) or add to existing array contents
                        0.019,                                      // Signal frequency
                        SIGLIB_ZERO,                                // D.C. Offset
                        SIGLIB_ZERO,                                // Unused
                        SIGLIB_ZERO,                                // Signal end value - Unused
                        &sinePhase,                                 // Signal phase - maintained across array boundaries
                        SIGLIB_NULL_DATA_PTR,                       // Unused
                        SAMPLE_LENGTH);                             // Output dataset length

    SDA_Clip (pSrc, pSrc, CLIP_LEVEL, SIGLIB_CLIP_BOTH, SAMPLE_LENGTH);
    SDA_Copy (pSrc, pRealData, SAMPLE_LENGTH);

// Apply window to real data
    SDA_Window (pRealData,                                          // Source array pointer
                pRealData,                                          // Destination array pointer
                pWindowCoeffs,                                      // Window array pointer
                FFT_LENGTH);                                        // Window size
// Perform real FFT
    SDA_Rfft (pRealData,                                            // Pointer to real array
              pImagData,                                            // Pointer to imaginary array
              pFFTCoeffs,                                           // Pointer to FFT coefficients
              SIGLIB_BIT_REV_STANDARD,                              // Pointer to bit reverse address table - NOT USED
              FFT_LENGTH,                                           // FFT length
              LOG2_FFT_LENGTH);                                     // log2 FFT length
// Calculate real power from complex
    SDA_LogMagnitude (pRealData,                                    // Pointer to real source array
                      pImagData,                                    // Pointer to imaginary source array
                      pResults,                                     // Pointer to log magnitude destination array
                      FFT_LENGTH);                                  // Dataset length



    if (TriggerFlag) {
      TriggerLocation = SDA_Trigger (pSrc,                          // Pointer to source array
                                     &ZXPrevSign,                   // Pointer to sign from last array processed
                                     0.1,                           // Trigger Level
                                     ZXFlag,                        // Zero crossing type - +ve, -ve, both
                                     SAMPLE_LENGTH);                // Dataset length
    }

    if (TriggerLocation > SAMPLE_LENGTH) {                          // If no trigger detected, just display from first sample
      TriggerLocation = 0;
    }

    if (TriggerLocation > DISPLAY_LENGTH) {                         // If trigger is detected after mid point, just display from mid point
      TriggerLocation = DISPLAY_LENGTH;
    }

    if (Magnitude != previousMagnitude) {
      printf ("Signal magnitude = %lf\n", Magnitude);
      previousMagnitude = Magnitude;
    }

    gpc_plot_2d (h2DPlotTime,                                       // Graph handle
                 pSrc + TriggerLocation,                            // Dataset
                 DISPLAY_LENGTH,                                    // Dataset length
                 "Time Domain Signal",                              // Dataset title
                 SIGLIB_ZERO,                                       // Minimum X value
                 (double) (DISPLAY_LENGTH - 1),                     // Maximum X value
                 "lines",                                           // Graph type
                 "blue",                                            // Colour
                 GPC_NEW);                                          // New graph

    gpc_plot_2d (h2DPlotFreq,                                       // Graph handle
                 pResults,                                          // Dataset
                 DISPLAY_LENGTH,                                    // Dataset length
                 "Frequency Domain Signal",                         // Dataset title
                 SIGLIB_ZERO,                                       // Minimum X value
                 (double) (DISPLAY_LENGTH - 1),                     // Maximum X value
                 "lines",                                           // Graph type
                 "orange",                                          // Colour
                 GPC_NEW);                                          // New graph
  }

  return (0);
}
