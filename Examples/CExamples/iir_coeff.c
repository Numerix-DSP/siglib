// SigLib - Calculates IIR Biquad Filter Coefficients
// Copyright (c) 2023 Delta Numerix All rights reserved.

// Include files
#include <math.h>
#include <stdio.h>
#include <siglib.h>       // SigLib DSP library
#include <gnuplot_c.h>    // Gnuplot/C

// Define constants
#define PLOT_TIME_DOMAIN 0    // Set to '1' to plot the time domain, '0' otherwise
#define PLOT_FREQ_DOMAIN 0    // Set to '1' to plot the frequency domain, '0' otherwise

#define IIR_FILTER_STAGES 1    // Half the size of the filter order
#define IMPULSE_RESPONSE_LENGTH 1024
#define FFT_LENGTH IMPULSE_RESPONSE_LENGTH
#define LOG2_FFT_LENGTH SAI_FftLengthLog2(FFT_LENGTH)    // Log2 FFT length,
#define PLOT_LENGTH (IMPULSE_RESPONSE_LENGTH / 2)

// Declare global variables and arrays
static SLData_t pIIRCoeffs[SIGLIB_IIR_COEFFS_PER_BIQUAD];

int main(int argc, char** argv)
{
  char filterType;
  SLData_t sampleRate, cutoffFreq, qFactor, gain = SIGLIB_ZERO;    // Paremeters
  SLData_t Max;

  if (argc == 5) {
    filterType = *argv[1];
    sampleRate = (SLData_t)atof(argv[2]);
    cutoffFreq = (SLData_t)atof(argv[3]);
    qFactor = (SLData_t)atof(argv[4]);
  } else if (argc == 6) {
    filterType = *argv[1];
    sampleRate = (SLData_t)atof(argv[2]);
    cutoffFreq = (SLData_t)atof(argv[3]);
    qFactor = (SLData_t)atof(argv[4]);
    gain = (SLData_t)atof(argv[SIGLIB_IIR_COEFFS_PER_BIQUAD]);
  } else {
    printf("Usage: SL_Coeff_Gen <filterType> <sampleRate> <cutoffFreq> "
           "<qFactor> <gain>\n");
    printf("Filter Types: L - Low-Pass\n");
    printf("              H - High-Pass\n");
    printf("              A - All-Pass\n");
    printf("              D - Band-Pass - constant skirt gain\n");
    printf("              E - Band-Pass - 0 dB peak gain\n");
    printf("              N - Notch\n");
    printf("              P - Peaking\n");
    printf("              B - Low-Shelf (Bass)\n");
    printf("              T - High-Shelf (Treble)\n\n");
    printf("Example: iir_coeff B 48000 8000 0.7071 3\n\n");

    filterType = 'B';
    sampleRate = 48000.;
    cutoffFreq = 8000.;
    qFactor = 0.7071;
    gain = 3.;
  }

  SLData_t* pFilterState = SUF_IirStateArrayAllocate(IIR_FILTER_STAGES);    // Allocate memory
  SLData_t* pRealData = SUF_VectorArrayAllocate(FFT_LENGTH);
  SLData_t* pImagData = SUF_VectorArrayAllocate(FFT_LENGTH);
  SLData_t* pFFTCoeffs = SUF_VectorArrayAllocate(FFT_LENGTH);
  SLData_t* pResults = SUF_VectorArrayAllocate(PLOT_LENGTH);

  if ((NULL == pFilterState) || (NULL == pRealData) || (NULL == pImagData) || (NULL == pFFTCoeffs) || (NULL == pResults)) {

    printf("\n\nMemory allocation failed\n\n");
    exit(0);
  }

#if (PLOT_TIME_DOMAIN || PLOT_FREQ_DOMAIN)
  h_GPC_Plot* h2DPlot;    // Plot object

  h2DPlot =                                           // Initialize plot
      gpc_init_2d("IIR Filter Frequency Response",    // Plot title
                  "Time / Frequency",                 // X-Axis label
                  "Magnitude",                        // Y-Axis label
                  GPC_AUTO_SCALE,                     // Scaling mode
                  GPC_SIGNED,                         // Sign mode
                  GPC_KEY_ENABLE);                    // Legend / key mode
  if (NULL == h2DPlot) {
    printf("\nPlot creation failure.\n");
    exit(-1);
  }
#endif

  switch (filterType) {
  case 'L':
    SIF_IirLowPassFilter(pIIRCoeffs, cutoffFreq / sampleRate, qFactor);
    break;
  case 'H':
    SIF_IirHighPassFilter(pIIRCoeffs, cutoffFreq / sampleRate, qFactor);
    break;
  case 'A':
    SIF_IirAllPassFilter(pIIRCoeffs, cutoffFreq / sampleRate, qFactor);
    break;
  case 'D':
    SIF_IirBandPassFilterConstantSkirtGain(pIIRCoeffs, cutoffFreq / sampleRate, qFactor);
    break;
  case 'E':
    SIF_IirBandPassFilter0dBPeakGain(pIIRCoeffs, cutoffFreq / sampleRate, qFactor);
    break;
  case 'N':
    SIF_IirNotchFilter(pIIRCoeffs, cutoffFreq / sampleRate, qFactor);
    break;
  case 'P':
    SIF_IirPeakingFilter(pIIRCoeffs, cutoffFreq / sampleRate, qFactor, gain);
    break;
  case 'B':
    SIF_IirLowShelfFilter(pIIRCoeffs, cutoffFreq / sampleRate, qFactor, gain);
    break;
  case 'T':
    SIF_IirHighShelfFilter(pIIRCoeffs, cutoffFreq / sampleRate, qFactor, gain);
    break;
  default:
    printf("Syntax: SL_Coeff_Gen <filterType> <sampleRate> <cutoffFreq> "
           "<qFactor> <gain>\n");
    printf("Filter Types: L - Low-Pass\n");
    printf("              H - High-Pass\n");
    printf("              A - All-Pass\n");
    printf("              D - Band-Pass - constant skirt gain\n");
    printf("              E - Band-Pass - 0 dB peak gain\n");
    printf("              N - Notch\n");
    printf("              P - Peaking\n");
    printf("              B - Bass-Shelf\n");
    printf("              T - Treble-Shelf\n");
    printf("Example: SL_Coeff_Gen B 48000 8000 0.7071 3\n\n");
    exit(-1);
  }

  SIF_Iir(pFilterState,          // Pointer to filter state array
          IIR_FILTER_STAGES);    // Number of second order stages

  // Initialise FFT
  SIF_Fft(pFFTCoeffs,                 // Pointer to FFT coefficients
          SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit
                                      // reverse address table
          FFT_LENGTH);                // FFT length

  // Generate test impulse
  SDA_SignalGenerate(pRealData,                   // Pointer to destination array
                     SIGLIB_IMPULSE,              // Signal type - Impulse function
                     SIGLIB_ONE,                  // Signal peak level
                     SIGLIB_FILL,                 // Fill (overwrite) or add to existing array contents
                     SIGLIB_ZERO,                 // Signal frequency - Unused
                     SIGLIB_ZERO,                 // D.C. Offset
                     SIGLIB_ZERO,                 // Delay (samples periods) applied to impulse
                     SIGLIB_ZERO,                 // Signal end value - Unused
                     SIGLIB_NULL_DATA_PTR,        // Unused
                     SIGLIB_NULL_DATA_PTR,        // Unused
                     IMPULSE_RESPONSE_LENGTH);    // Output dataset length

  // Apply iir filter and store filtered data
  SDA_Iir(pRealData,                   // Input array to be filtered
          pRealData,                   // Filtered output array
          pFilterState,                // Pointer to filter state array
          pIIRCoeffs,                  // Pointer to filter coefficients array
          IIR_FILTER_STAGES,           // Number of stages
          IMPULSE_RESPONSE_LENGTH);    // Dataset length

#if PLOT_TIME_DOMAIN
  gpc_plot_2d(h2DPlot,                      // Graph handle
              pRealData,                    // Dataset
              PLOT_LENGTH,                  // Dataset length
              "Filter Impulse Response",    // Dataset title
              SIGLIB_ZERO,                  // Minimum X value
              (double)(PLOT_LENGTH - 1),    // Maximum X value
              "lines",                      // Graph type
              "blue",                       // Colour
              GPC_NEW);                     // New graph
  printf("\nFilter Impulse Response\nPlease hit <Carriage Return> to continue "
         ". . .");
  getchar();
#endif

  printf("Coefficients - b(0), b(1), b(2), a(1), a(2): %lf, %lf, %lf, %lf, %lf\n", pIIRCoeffs[0], pIIRCoeffs[1], pIIRCoeffs[2], pIIRCoeffs[3],
         pIIRCoeffs[4]);

  // Generate frequency response
  // Perform real FFT
  SDA_Rfft(pRealData,                  // Pointer to real array
           pImagData,                  // Pointer to imaginary array
           pFFTCoeffs,                 // Pointer to FFT coefficients
           SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit
                                       // reverse address table
           FFT_LENGTH,                 // FFT length
           LOG2_FFT_LENGTH);           // log2 FFT length

  // Calculate real power from complex
  SDA_LogMagnitude(pRealData,       // Pointer to real source array
                   pImagData,       // Pointer to imaginary source array
                   pResults,        // Pointer to log magnitude destination array
                   PLOT_LENGTH);    // Dataset length

  Max = SDA_Max(pResults,        // Pointer to source array
                PLOT_LENGTH);    // Dataset length
                                 // printf ("Max = %lf\n", Max);
  SDA_Add(pResults,              // Pointer to source array
          -Max,                  // Offset
          pResults,              // Pointer to destination array
          PLOT_LENGTH);          // Dataset length

#if PLOT_FREQ_DOMAIN
  gpc_plot_2d(h2DPlot,                        // Graph handle
              pResults,                       // Dataset
              PLOT_LENGTH,                    // Dataset length
              "Filter Frequency Response",    // Dataset title
              SIGLIB_ZERO,                    // Minimum X value
              sampleRate / SIGLIB_TWO,        // Maximum X value
              "lines",                        // Graph type
              "blue",                         // Colour
              GPC_NEW);                       // New graph
  printf("\nFilter Frequency Response\n");
#endif

#if (PLOT_TIME_DOMAIN || PLOT_FREQ_DOMAIN)
  printf("\nHit <Carriage Return> to continue ....\n");
  getchar();    // Wait for <Carriage Return>
  gpc_close(h2DPlot);
#endif

  SUF_MemoryFree(pFilterState);    // Free memory
  SUF_MemoryFree(pRealData);
  SUF_MemoryFree(pImagData);
  SUF_MemoryFree(pResults);
  SUF_MemoryFree(pFFTCoeffs);

  return (0);
}
