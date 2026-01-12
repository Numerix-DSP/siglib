// IIR Notch Filter Example
// Generates the notch filter with the specified magnitude for the pole
// Applies this notch to the .wav file and saves the results in filtered.wav
// Copyright (c) 2026 Delta Numerix All rights reserved.

// Transfer function of second-order notch filter
// H(z) = (1 - r^2) / (1 - 2r*cos(ω_0)*z^(-1) + r^2*z^(-2))
// where r = e^(-π*BW/fs) and ω_0 = 2*π*Fc/fs

// Computing radius (r) and frequency (ω_0)
// r = np.exp(-np.pi * notch_bandwidth / fs)
// w0 = 2 * np.pi * notch_center_freq / fs

// Include files
#include <stdio.h>
#include <siglib.h>       // SigLib DSP library
#include <gnuplot_c.h>    // Gnuplot/C

// Define constants
#define WAV_FILENAME "Alive_1kHz_Tone_10s_mono.wav"
#define TONE_FREQUENCY 1000.
#define NOTCH_BANDWIDTH 100.0    // Notch bandwidth

#define FILTER_ORDER 2                             // Filter order
#define FILTER_STAGES ((FILTER_ORDER + 1) >> 1)    // Number of 2nd-order filter stages

#define FFT_LENGTH 1024
#define LOG2_FFT_LENGTH SAI_FftLengthLog2(FFT_LENGTH)    // Log2 FFT length,

#define PLOT_LENGTH (FFT_LENGTH / 2)

// Declare global variables and arrays

static SLWavFileInfo_s wavInfo;

int main(void)
{
  h_GPC_Plot* h2DPlot;    // Plot object

  // Allocate memory
  SLData_t* pIIRCoeffs = SUF_IirCoefficientAllocate(FILTER_STAGES);
  SLData_t* pFilterState = SUF_IirStateArrayAllocate(FILTER_STAGES);
  SLData_t* pResults = SUF_VectorArrayAllocate(PLOT_LENGTH);
  SLData_t* pFFTCoeffs = SUF_FftCoefficientAllocate(FFT_LENGTH);

  SLArrayIndex_t sampleLength = SUF_WavFileLength(WAV_FILENAME);
  SLData_t* pSrc = SUF_VectorArrayAllocate(sampleLength);         // Input data arrays
  SLData_t* pRealData = SUF_VectorArrayAllocate(sampleLength);    // Input data arrays
  SLData_t* pImagData = SUF_VectorArrayAllocate(sampleLength);    // Input data arrays

  if ((NULL == pIIRCoeffs) || (NULL == pRealData) || (NULL == pImagData) || (NULL == pResults) || (NULL == pSrc) || (NULL == pFFTCoeffs)) {
    printf("\n\nMemory allocation failed\n\n");
    exit(0);
  }

  wavInfo = SUF_WavReadFile(pSrc, WAV_FILENAME);
  if (wavInfo.NumberOfSamples == -1) {
    printf("Error reading .WAV file header: %s\n", WAV_FILENAME);
    exit(-1);
  }

  printf("Sample rate: %d\n", wavInfo.SampleRate);

  // Generate the notch filter coefficients
  SLData_t pole_magnitude = SIF_IirNotchFilterBandwidthToRadius(NOTCH_BANDWIDTH / wavInfo.SampleRate);
  printf("pole_magnitude: %lf\n", pole_magnitude);

  SIF_IirNotchFilter2(pIIRCoeffs,                             // Pointer to filter coefficients
                      TONE_FREQUENCY / wavInfo.SampleRate,    // Centre frequency
                      pole_magnitude,                         // Pole magnitude
                      FILTER_ORDER);                          // Filter order

  printf("IIR Filter Coefficients:\n");
  SUF_PrintIIRCoefficients(pIIRCoeffs,        // Pointer to filter coefficients
                           FILTER_STAGES);    // Number of biquads

  SIF_Iir(pFilterState,      // Pointer to filter state array
          FILTER_STAGES);    // Number of second order stages
                             // Initialise FFT

  // Apply iir filter and store filtered data
  SDA_Iir(pSrc,             // Input array to be filtered
          pRealData,        // Filtered output array
          pFilterState,     // Pointer to filter state array
          pIIRCoeffs,       // Pointer to filter coefficients array
          FILTER_STAGES,    // Number of stages
          sampleLength);    // Dataset length

  SUF_WavWriteFile(pRealData, "filtered.wav", wavInfo, sampleLength);

  // Generate impulse response to compute the frequency response of the filter
  h2DPlot =                              // Initialize plot
      gpc_init_2d("IIR Notch Filter",    // Plot title
                  "Time / Frequency",    // X-Axis label
                  "Magnitude",           // Y-Axis label
                  GPC_AUTO_SCALE,        // Scaling mode
                  GPC_SIGNED,            // Sign mode
                  GPC_KEY_DISABLE);      // Legend / key mode
  if (NULL == h2DPlot) {
    printf("\nPlot creation failure.\n");
    exit(-1);
  }

  SIF_Iir(pFilterState,      // Pointer to filter state array
          FILTER_STAGES);    // Number of second order stages
                             // Initialise FFT

  SIF_Fft(pFFTCoeffs,                 // Pointer to FFT coefficients
          SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit
                                      // reverse address table
          FFT_LENGTH);                // FFT length

  // Generate test impulse
  SDA_Impulse(pSrc,           // Pointer to destination array
              FFT_LENGTH);    // Output dataset length

  // Apply IIR filter and store filtered data
  SDA_Iir(pSrc,             // Input array to be filtered
          pRealData,        // Filtered output array
          pFilterState,     // Pointer to filter state array
          pIIRCoeffs,       // Pointer to filter coefficients array
          FILTER_STAGES,    // Number of stages
          FFT_LENGTH);      // Dataset length

  // Generate frequency response
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

  gpc_plot_2d(h2DPlot,                              // Graph handle
              pResults,                             // Dataset
              PLOT_LENGTH,                          // Dataset length
              "Frequency response",                 // Dataset title
              SIGLIB_ZERO,                          // Minimum X value
              (wavInfo.SampleRate / SIGLIB_TWO),    // Maximum X value
              "lines",                              // Graph type
              "blue",                               // Colour
              GPC_NEW);                             // New graph
  printf("\nFrequency response\n");

  printf("\nHit <Carriage Return> to continue ....\n");
  getchar();    // Wait for <Carriage Return>
  gpc_close(h2DPlot);

  SUF_MemoryFree(pSrc);    // Free memory
  SUF_MemoryFree(pIIRCoeffs);
  SUF_MemoryFree(pFilterState);
  SUF_MemoryFree(pRealData);
  SUF_MemoryFree(pImagData);
  SUF_MemoryFree(pResults);
  SUF_MemoryFree(pFFTCoeffs);

  return (0);
}
