// SigLib Graphic Equalizer Example
// This example generates a graphic equalizer filter bank
// The filter bank includes entries for the number of filter bands
// and the number of gain levels for each filter band.
// The filter bands are:
//  - low-shelf       - The low-shelf filter
//  - (N-2) * peaking - The peaking filters
//  - high-shelf      - The high-shelf filter
// This code is based on the equations in Robert Bristow-Johnson's Audio
// Equalizer Cookbook:
//  https://webaudio.github.io/Audio-EQ-Cookbook/audio-eq-cookbook.html.
// Copyright (c) 2024 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>       // SigLib DSP library
#include <gnuplot_c.h>    // Gnuplot/C

// Define constants
#define SAMPLE_RATE 48000.    // Sample rate

#define GRAPHIC_EQ_GAIN_MIN_DB -12.    // Gain min value
#define GRAPHIC_EQ_GAIN_MAX_DB 12.     // Gain max value
#define GRAPHIC_EQ_GAIN_STEP_DB 1.     // Gain step value
#define GRAPHIC_EQ_NUM_GAIN_LEVELS \
  ((SLArrayIndex_t)((GRAPHIC_EQ_GAIN_MAX_DB - GRAPHIC_EQ_GAIN_MIN_DB) / GRAPHIC_EQ_GAIN_STEP_DB) + 1)    // Number of gain levels

#define GRAPHIC_EQ_NUM_BIQUADS_PER_BAND 1    // Second order filters

// 3 Band Tone Control (Bass, Middle and Treble)
// #define GRAPHIC_EQ_NUM_FREQ_BANDS       3
// static SLData_t filterCutOffFrequencies[] = {200., 700., 1200.};

// 5 Band Graphic Equalizer Parameters
// #define GRAPHIC_EQ_NUM_FREQ_BANDS       5
// static SLData_t filterCutOffFrequencies[] = {62.5, 250., 1000., 4000.,
// 16000.};

// 10 Band Graphic Equalizer Parameters
#define GRAPHIC_EQ_NUM_FREQ_BANDS 10
static SLData_t filterCutOffFrequencies[] = {31.25, 62.5, 125., 250., 500., 1000., 2000., 4000., 8000., 16000.};

#define SAMPLE_LENGTH 512
#define FFT_LENGTH SAMPLE_LENGTH
#define LOG2_FFT_LENGTH SAI_FftLengthLog2(FFT_LENGTH)    // Log2 FFT length,

#define HALF_FFT_LENGTH (FFT_LENGTH >> 1)

SLData_t graphic_eq_state_arrays[GRAPHIC_EQ_NUM_FREQ_BANDS][SIGLIB_IIR_DELAY_SIZE];

static SLArrayIndex_t graphic_eq_gains_table_index[] = {    // Specify the gains table index for each filter band
    10, 13, 10, 10, 13, 10, 10, 13, 10, 13};

int main(void)
{
  h_GPC_Plot* h2DPlot;    // Plot object

  SLData_t* pRealData = SUF_VectorArrayAllocate(FFT_LENGTH);
  SLData_t* pImagData = SUF_VectorArrayAllocate(FFT_LENGTH);
  SLData_t* pResults = SUF_VectorArrayAllocate(SAMPLE_LENGTH);
  SLData_t* pFFTCoeffs = SUF_FftCoefficientAllocate(FFT_LENGTH);

  // Dynamically allocate this array becasue C does not like using doubles for
  // array dimensions in static declarations
  SLData_t(*graphic_eq_coeffs)[GRAPHIC_EQ_NUM_FREQ_BANDS][GRAPHIC_EQ_NUM_GAIN_LEVELS][SIGLIB_IIR_COEFFS_PER_BIQUAD] =
      malloc(sizeof *graphic_eq_coeffs);
  if ((NULL == graphic_eq_coeffs)) {

    printf("\n\nMemory allocation failed\n\n");
    exit(0);
  }

  if ((NULL == pRealData) || (NULL == pImagData) || (NULL == pResults) || (NULL == pFFTCoeffs)) {

    printf("\n\nMemory allocation failed\n\n");
    exit(0);
  }

  h2DPlot =                                         // Initialize plot
      gpc_init_2d_logscalex("Graphic Equalizer",    // Plot title
                            "Frequency (Hz)",       // X-Axis label
                            "Magnitude",            // Y-Axis label
                            GPC_AUTO_SCALE,         // Scaling mode
                            GPC_SIGNED,             // Sign mode
                            GPC_KEY_ENABLE);        // Legend / key mode
  if (NULL == h2DPlot) {
    printf("\nPlot creation failure.\n");
    exit(-1);
  }

  // Initialise graphical equalizer filter bank
  SIF_GraphicEqualizerFilterBank(filterCutOffFrequencies,               // Pointer to filter cut-off frequency array
                                 (SLData_t*)graphic_eq_coeffs,          // Pointer to filter bank coefficients
                                 (SLData_t*)graphic_eq_state_arrays,    // Pointer to filter state array
                                 GRAPHIC_EQ_NUM_FREQ_BANDS,             // Number of frequency bands
                                 SAMPLE_RATE,                           // Sample rate
                                 GRAPHIC_EQ_GAIN_MIN_DB,                // Minimum gain for each frequncy band
                                 GRAPHIC_EQ_GAIN_STEP_DB,               // Gain step for each frequency band
                                 GRAPHIC_EQ_NUM_GAIN_LEVELS);           // Number of gain levels for each frequency band

  // Initialise FFT
  SIF_Fft(pFFTCoeffs,                 // Pointer to FFT coefficients
          SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit
                                      // reverse address table
          FFT_LENGTH);                // FFT length

  // Generate signal to be filtered
  SDA_Impulse(pRealData,         // Pointer to destination array
              SAMPLE_LENGTH);    // Output dataset length

  // Filter the data stream
  for (SLArrayIndex_t i = 0; i < SAMPLE_LENGTH; i++) {
    for (SLArrayIndex_t j = 0; j < GRAPHIC_EQ_NUM_FREQ_BANDS; j++) {
      pRealData[i] = SDS_Iir(pRealData[i],                                                    // Input data sample to be filtered
                             &graphic_eq_state_arrays[j][0],                                  // Pointer to filter state array
                             &(*graphic_eq_coeffs)[j][graphic_eq_gains_table_index[j]][0],    // Pointer to filter coefficients array
                             GRAPHIC_EQ_NUM_BIQUADS_PER_BAND);                                // Number of second order stages
    }
  }

  // Perform real FFT
  SDA_Rfft(pRealData,                  // Pointer to real array
           pImagData,                  // Pointer to imaginary array
           pFFTCoeffs,                 // Pointer to FFT coefficients
           SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit
                                       // reverse address table
           FFT_LENGTH,                 // FFT length
           LOG2_FFT_LENGTH);           // log2 FFT length

  // Calculate real power from complex
  SDA_LogMagnitude(pRealData,           // Pointer to real source array
                   pImagData,           // Pointer to imaginary source array
                   pResults,            // Pointer to log magnitude destination array
                   HALF_FFT_LENGTH);    // Dataset length

  gpc_plot_2d(h2DPlot,                               // Graph handle
              pResults,                              // Dataset
              HALF_FFT_LENGTH,                       // Dataset length
              "Frequency Response",                  // Dataset title
              SIGLIB_ZERO,                           // Minimum X value
              (double)(SAMPLE_RATE / SIGLIB_TWO),    // Maximum X value
              "lines",                               // Graph type
              "blue",                                // Colour
              GPC_NEW);                              // New graph
  printf("\nGraphical Equalizer Frequency Response\nPlease hit <Carriage "
         "Return> to continue . . .");
  getchar();

  gpc_close(h2DPlot);

  SUF_MemoryFree(pRealData);    // Free memory
  SUF_MemoryFree(pImagData);
  SUF_MemoryFree(pResults);
  SUF_MemoryFree(pFFTCoeffs);

  return (0);
}
