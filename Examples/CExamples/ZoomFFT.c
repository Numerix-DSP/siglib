// SigLib Zoom FFT Example
// This program modulates 5 closely spaced sine waves, which are
// (with a reasonably sized FFT) indestinguishable. The Zoom FFT
// then magnifies the frequency range of interest, by a factor
// of 16 and performs a window'd complex FFT. This example also
// uses the spectrum reversal option to produce the correct
// output orientation.
// Copyright (c) 2026 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>       // SigLib DSP library
#include <gnuplot_c.h>    // Gnuplot/C

// Define constants
#define FFT_LENGTH 128
#define LOG2_FFT_LENGTH SAI_FftLengthLog2(FFT_LENGTH)    // Log2 FFT length,
#define HIGH_DECIMATE_RATIO 4
#define LPF_DECIMATE_RATIO 4
#define SOURCE_BUF_SIZE (FFT_LENGTH * HIGH_DECIMATE_RATIO * LPF_DECIMATE_RATIO)
#define INTERMEDIATE_LENGTH (SOURCE_BUF_SIZE / HIGH_DECIMATE_RATIO)    // Used for the intermed. data between HDF * LPF
#define CARRIER_FREQUENCY 0.24
#define SINE_BUF_SIZE 1024
#define COMB_FILTER_LENGTH 32
#define LPF_FILTER_LENGTH 48

// Declare global variables and arrays

// Low pass filter for final stage of down conversion Filter Spec:
// Design Type: FIR-Remez
// Sample rate: 1.0
// Number of coefficients: 48
// Fc1 = 0.200000, Fc2 = 0.000000, TBW = 0.050000, PBR = 1.000000, SBA
// = 70.000000
static const SLData_t LPFCoefficientArray[] = {
    -1.48330832437900479900e-03, -2.08167828285720857600e-03, 1.82056303050500334000e-03,  9.24651371854399821400e-03,  1.14685237504992231000e-02,
    2.77405262353953071400e-03,  -7.54152298415707109800e-03, -4.50378741128770396200e-03, 8.81105589241907433200e-03,  1.07876779749111143700e-02,
    -6.04189313536136057800e-03, -1.70201336011035347900e-02, -1.07303095652230493000e-04, 2.26877354870228670200e-02,  1.10640334356405349600e-02,
    -2.54399441530610837000e-02, -2.74961102564541745600e-02, 2.24145094347915005400e-02,  5.09944571101899960000e-02,  -8.32418594095427760700e-03,
    -8.80587888857019551800e-02, -3.47834716343741051600e-02, 1.86942179995934015800e-01,  4.02316775841422114000e-01,  4.02316775841422114000e-01,
    1.86942179995934015800e-01,  -3.47834716343741051600e-02, -8.80587888857019551800e-02, -8.32418594095427760700e-03, 5.09944571101899960000e-02,
    2.24145094347915005400e-02,  -2.74961102564541745600e-02, -2.54399441530610837000e-02, 1.10640334356405349600e-02,  2.26877354870228670200e-02,
    -1.07303095652230493000e-04, -1.70201336011035347900e-02, -6.04189313536136057800e-03, 1.07876779749111143700e-02,  8.81105589241907433200e-03,
    -4.50378741128770396200e-03, -7.54152298415707109800e-03, 2.77405262353953071400e-03,  1.14685237504992231000e-02,  9.24651371854399821400e-03,
    1.82056303050500334000e-03,  -2.08167828285720857600e-03, -1.48330832437900479900e-03};

static SLData_t pRealLPFStateArray[LPF_FILTER_LENGTH], pImagPFStateArray[LPF_FILTER_LENGTH];

int main(void)
{
  h_GPC_Plot* h2DPlot;    // Plot object
  SLData_t MixFreq;

  SLData_t RealCombFilterSum, ImagCombFilterSum;
  SLArrayIndex_t CombFilterPhase, SineTablePhase;

  SLArrayIndex_t RealLPFIndex, ImagLPFIndex;
  SLArrayIndex_t RealDecimatorIndex, ImagDecimatorIndex;

  // Allocate memory
  SLData_t* pRealData = SUF_VectorArrayAllocate(INTERMEDIATE_LENGTH);
  SLData_t* pImagData = SUF_VectorArrayAllocate(INTERMEDIATE_LENGTH);
  SLData_t* pInputData = SUF_VectorArrayAllocate(SOURCE_BUF_SIZE);
  SLData_t* pRealCombFilter = SUF_VectorArrayAllocate(COMB_FILTER_LENGTH);
  SLData_t* pImagCombFilter = SUF_VectorArrayAllocate(COMB_FILTER_LENGTH);
  SLData_t* pSineTable = SUF_VectorArrayAllocate(SINE_BUF_SIZE);
  SLData_t* pFFTCoeffs = SUF_FftCoefficientAllocate(FFT_LENGTH);
  SLData_t* pWindowCoeffs = SUF_VectorArrayAllocate(FFT_LENGTH);

  if ((NULL == pRealData) || (NULL == pImagData) || (NULL == pInputData) || (NULL == pRealCombFilter) || (NULL == pImagCombFilter) ||
      (NULL == pSineTable) || (NULL == pFFTCoeffs) || (NULL == pWindowCoeffs)) {

    printf("\n\nMemory allocation failed\n\n");
    exit(-1);
  }

  printf("\n\n\nThis program performs a zoom-FFT on a modulated data set.\n");
  printf("All frequencies are normalised to a sample rate of 1.0.\n\n");
  printf("Carrier frequency  = %lf\n", CARRIER_FREQUENCY);
  printf("Sideband strengths:\n");
  printf("        Freq.   Magn.\n");
  printf("        -0.009  0.2\n");
  printf("        -0.006  0.3\n");
  printf("        -0.003  0.4\n");
  printf("        0.0     1.0\n");
  printf("        +0.003  0.6\n");
  printf("        +0.006  0.7\n");
  printf("        +0.009  0.8\n\n");

  MixFreq = CARRIER_FREQUENCY;

#if COMMENT    // Uncomment these lines to allow the user to specify the mix
               // frequency
  printf("Enter mix frequency (<.5) =>");
  scanf("%lf", &MixFreq);
#endif

  h2DPlot =                              // Initialize plot
      gpc_init_2d("Zoom-FFT",            // Plot title
                  "Time / Frequency",    // X-Axis label
                  "Magnitude",           // Y-Axis label
                  GPC_AUTO_SCALE,        // Scaling mode
                  GPC_SIGNED,            // Sign mode
                  GPC_KEY_DISABLE);      // Legend / key mode
  if (NULL == h2DPlot) {
    printf("\nPlot creation failure.\n");
    exit(-1);
  }

  // Initialise FFT
  SIF_Fft(pFFTCoeffs,                 // Pointer to FFT coefficients
          SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit
                                      // reverse address table
          FFT_LENGTH);                // FFT length

  // Generate the source spectrum
  SLData_t sinePhase = SIGLIB_ZERO;
  SDA_SignalGenerate(pInputData,                   // Pointer to destination array
                     SIGLIB_SINE_WAVE,             // Signal type - Sine wave
                     SIGLIB_ONE,                   // Signal peak level
                     SIGLIB_FILL,                  // Fill (overwrite) or add to existing array contents
                     CARRIER_FREQUENCY,            // Signal frequency
                     SIGLIB_ZERO,                  // D.C. Offset
                     SIGLIB_ZERO,                  // Unused
                     SIGLIB_ZERO,                  // Signal end value - Unused
                     &sinePhase,                   // Signal phase - maintained across array boundaries
                     SIGLIB_NULL_DATA_PTR,         // Unused
                     SOURCE_BUF_SIZE);             // Output dataset length
  SDA_SignalGenerate(pInputData,                   // Pointer to destination array
                     SIGLIB_SINE_WAVE,             // Signal type - Sine wave
                     0.2,                          // Signal peak level
                     SIGLIB_ADD,                   // Fill (overwrite) or add to existing array contents
                     CARRIER_FREQUENCY - 0.009,    // Signal frequency
                     SIGLIB_ZERO,                  // D.C. Offset
                     SIGLIB_ZERO,                  // Unused
                     SIGLIB_ZERO,                  // Signal end value - Unused
                     &sinePhase,                   // Signal phase - maintained across array boundaries
                     SIGLIB_NULL_DATA_PTR,         // Unused
                     SOURCE_BUF_SIZE);             // Output dataset length
  SDA_SignalGenerate(pInputData,                   // Pointer to destination array
                     SIGLIB_SINE_WAVE,             // Signal type - Sine wave
                     0.3,                          // Signal peak level
                     SIGLIB_ADD,                   // Fill (overwrite) or add to existing array contents
                     CARRIER_FREQUENCY - 0.006,    // Signal frequency
                     SIGLIB_ZERO,                  // D.C. Offset
                     SIGLIB_ZERO,                  // Unused
                     SIGLIB_ZERO,                  // Signal end value - Unused
                     &sinePhase,                   // Signal phase - maintained across array boundaries
                     SIGLIB_NULL_DATA_PTR,         // Unused
                     SOURCE_BUF_SIZE);             // Output dataset length
  SDA_SignalGenerate(pInputData,                   // Pointer to destination array
                     SIGLIB_SINE_WAVE,             // Signal type - Sine wave
                     0.4,                          // Signal peak level
                     SIGLIB_ADD,                   // Fill (overwrite) or add to existing array contents
                     CARRIER_FREQUENCY - 0.003,    // Signal frequency
                     SIGLIB_ZERO,                  // D.C. Offset
                     SIGLIB_ZERO,                  // Unused
                     SIGLIB_ZERO,                  // Signal end value - Unused
                     &sinePhase,                   // Signal phase - maintained across array boundaries
                     SIGLIB_NULL_DATA_PTR,         // Unused
                     SOURCE_BUF_SIZE);             // Output dataset length
  SDA_SignalGenerate(pInputData,                   // Pointer to destination array
                     SIGLIB_SINE_WAVE,             // Signal type - Sine wave
                     0.6,                          // Signal peak level
                     SIGLIB_ADD,                   // Fill (overwrite) or add to existing array contents
                     CARRIER_FREQUENCY + 0.003,    // Signal frequency
                     SIGLIB_ZERO,                  // D.C. Offset
                     SIGLIB_ZERO,                  // Unused
                     SIGLIB_ZERO,                  // Signal end value - Unused
                     &sinePhase,                   // Signal phase - maintained across array boundaries
                     SIGLIB_NULL_DATA_PTR,         // Unused
                     SOURCE_BUF_SIZE);             // Output dataset length
  SDA_SignalGenerate(pInputData,                   // Pointer to destination array
                     SIGLIB_SINE_WAVE,             // Signal type - Sine wave
                     0.7,                          // Signal peak level
                     SIGLIB_ADD,                   // Fill (overwrite) or add to existing array contents
                     CARRIER_FREQUENCY + 0.006,    // Signal frequency
                     SIGLIB_ZERO,                  // D.C. Offset
                     SIGLIB_ZERO,                  // Unused
                     SIGLIB_ZERO,                  // Signal end value - Unused
                     &sinePhase,                   // Signal phase - maintained across array boundaries
                     SIGLIB_NULL_DATA_PTR,         // Unused
                     SOURCE_BUF_SIZE);             // Output dataset length
  SDA_SignalGenerate(pInputData,                   // Pointer to destination array
                     SIGLIB_SINE_WAVE,             // Signal type - Sine wave
                     0.8,                          // Signal peak level
                     SIGLIB_ADD,                   // Fill (overwrite) or add to existing array contents
                     CARRIER_FREQUENCY + 0.009,    // Signal frequency
                     SIGLIB_ZERO,                  // D.C. Offset
                     SIGLIB_ZERO,                  // Unused
                     SIGLIB_ZERO,                  // Signal end value - Unused
                     &sinePhase,                   // Signal phase - maintained across array boundaries
                     SIGLIB_NULL_DATA_PTR,         // Unused
                     SOURCE_BUF_SIZE);             // Output dataset length

  gpc_plot_2d(h2DPlot,                     // Graph handle
              pInputData,                  // Dataset
              FFT_LENGTH,                  // Dataset length
              "Modulated Signal",          // Dataset title
              SIGLIB_ZERO,                 // Minimum X value
              (double)(FFT_LENGTH - 1),    // Maximum X value
              "lines",                     // Graph type
              "blue",                      // Colour
              GPC_NEW);                    // New graph
  printf("\nModulated Signal\nPlease hit <Carriage Return> to continue . . .");
  getchar();

  // Copy for FFT
  SDA_Copy(pInputData,     // Pointer to source array
           pRealData,      // Pointer to destination array
           FFT_LENGTH);    // Dataset length

  // Perform real FFT
  SDA_Rfft(pRealData,                  // Pointer to real array
           pImagData,                  // Pointer to imaginary array
           pFFTCoeffs,                 // Pointer to FFT coefficients
           SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit
                                       // reverse address table
           FFT_LENGTH,                 // FFT length
           LOG2_FFT_LENGTH);           // log2 FFT length

  // Calculate real magnitude from complex
  SDA_Magnitude(pRealData,      // Pointer to real source array
                pImagData,      // Pointer to imaginary source array
                pRealData,      // Pointer to magnitude destination array
                FFT_LENGTH);    // Dataset length

  gpc_plot_2d(h2DPlot,                        // Graph handle
              pRealData,                      // Dataset
              FFT_LENGTH,                     // Dataset length
              "Modulated Signal Spectrum",    // Dataset title
              SIGLIB_ZERO,                    // Minimum X value
              (double)(FFT_LENGTH - 1),       // Maximum X value
              "lines",                        // Graph type
              "blue",                         // Colour
              GPC_NEW);                       // New graph
  printf("\nModulated Signal Spectrum\nPlease hit <Carriage Return> to "
         "continue . . .");
  getchar();

  // Initialize Zoom FFT
  SIF_ZoomFft(pRealCombFilter,            // Pointer to real comb filter state array
              &RealCombFilterSum,         // Real comb filter sum
              pImagCombFilter,            // Pointer to imaginary comb filter state array
              &ImagCombFilterSum,         // Imaginary comb filter sum
              &CombFilterPhase,           // Comb filter phase
              pSineTable,                 // Pointer to sine look-up table
              &SineTablePhase,            // Sine table phase for mixer
              &RealDecimatorIndex,        // Pointer to real decimator index
              &ImagDecimatorIndex,        // Pointer to imaginary decimator index
              &RealLPFIndex,              // Pointer to real LPF index
              &ImagLPFIndex,              // Pointer to imaginary LPF index
              pRealLPFStateArray,         // Pointer to real LPF state array
              pImagPFStateArray,          // Pointer to imaginary LPF state array
              pWindowCoeffs,              // Pointer to window look-up table
              pFFTCoeffs,                 // Pointer to FFT coefficient table
              SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit
                                          // reverse address table
              COMB_FILTER_LENGTH,         // Comb filter length
              SINE_BUF_SIZE,              // Mixer sine table size
              LPF_FILTER_LENGTH,          // FIR filter length
              FFT_LENGTH);                // FFT length
                                          // Perform Zoom FFT
  SDA_ZoomFft(pInputData,                 // Pointer to input array
              pRealData,                  // Pointer to real result array
              pImagData,                  // Pointer to imaginary result array
              pRealCombFilter,            // Pointer to real comb filter state array
              &RealCombFilterSum,         // Real comb filter sum
              pImagCombFilter,            // Pointer to imaginary comb filter state array
              &ImagCombFilterSum,         // Imaginary comb filter sum
              &CombFilterPhase,           // Comb filter phase
              pSineTable,                 // Pointer to sine look-up table
              &SineTablePhase,            // Pointer to sine table phase for mixer
              MixFreq,                    // Mix frequency
              COMB_FILTER_LENGTH,         // Length of comb filter
              SINE_BUF_SIZE,              // Sine table size for mixer
              HIGH_DECIMATE_RATIO,        // High decimation ratio
              pRealLPFStateArray,         // Pointer to real LPF state array
              pImagPFStateArray,          // Pointer to imaginary LPF state array
              LPFCoefficientArray,        // Pointer to LPF coefficients
              &RealDecimatorIndex,        // Pointer to real decimator index
              &ImagDecimatorIndex,        // Pointer to imaginary decimator index
              &RealLPFIndex,              // Pointer to real LPF index
              &ImagLPFIndex,              // Pointer to imaginary LPF index
              pWindowCoeffs,              // Pointer to window look-up table
              pFFTCoeffs,                 // Pointer to FFT coefficient table
              SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit
                                          // reverse address table
              SOURCE_BUF_SIZE,            // Input dataset length
              INTERMEDIATE_LENGTH,        // Intermediate dataset length
              LPF_FILTER_LENGTH,          // FIR filter length
              LPF_DECIMATE_RATIO,         // FIR decimation ratio
              SIGLIB_TRUE,                // Frequency reverse flag
              FFT_LENGTH,                 // FFT length
              LOG2_FFT_LENGTH);           // Log2 FFT length

  // Calculate real magnitude from complex
  SDA_Magnitude(pRealData,      // Pointer to real source array
                pImagData,      // Pointer to imaginary source array
                pRealData,      // Pointer to magnitude destination array
                FFT_LENGTH);    // Dataset length

  // Swap FFT halves to centralize mix frequency
  SDA_FftShift(pRealData,      // Pointer to source array
               pRealData,      // Pointer to destination array
               FFT_LENGTH);    // Dataset length

  gpc_plot_2d(h2DPlot,                     // Graph handle
              pRealData,                   // Dataset
              FFT_LENGTH,                  // Dataset length
              "Zoomed Spectrum (x16)",     // Dataset title
              SIGLIB_ZERO,                 // Minimum X value
              (double)(FFT_LENGTH - 1),    // Maximum X value
              "lines",                     // Graph type
              "blue",                      // Colour
              GPC_NEW);                    // New graph
  printf("\nZoomed Spectrum (x16)\n");

  printf("\nHit <Carriage Return> to continue ....\n");
  getchar();    // Wait for <Carriage Return>
  gpc_close(h2DPlot);

  SUF_MemoryFree(pRealData);    // Free memory
  SUF_MemoryFree(pImagData);
  SUF_MemoryFree(pInputData);
  SUF_MemoryFree(pRealCombFilter);
  SUF_MemoryFree(pImagCombFilter);
  SUF_MemoryFree(pSineTable);
  SUF_MemoryFree(pFFTCoeffs);
  SUF_MemoryFree(pWindowCoeffs);

  return (0);
}
