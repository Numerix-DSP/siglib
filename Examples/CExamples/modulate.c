// SigLib Modulation Example
// simulated sampling is at 40kHz with a modulation freq of 12kHz

// Shift is acomplished by a positive shift of mod. freq. + shift,
// this is high pass filtered to remove the lower side band and
// this is then shifted back down to baseband, where the signal is
// low pass filtered to remove any out of band signals.
// Copyright (c) 2026 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>       // SigLib DSP library
#include <gnuplot_c.h>    // Gnuplot/C

// Define constants
#define SAMPLE_LENGTH 512
#define FFT_LENGTH 512
#define LOG2_FFT_LENGTH SAI_FftLengthLog2(FFT_LENGTH)    // Log2 FFT length,

#define FREQUENCY_SHIFT -2000.0    // Shift in frequency (Hz) +/- values okay

#define SAMPLING_FREQ 40000.0       // 40 kHz sampling frequency desired
#define MODULATOR_1_FREQ 12000.0    // 12 kHz 1st stage modulation frequency
#define SIZE_COS_MOD1 10            // set as count before repeat of cos

#define PI_2 6.283185307               // 2.*PI
#define PI_BY_2 1.570796327            // PI/2
#define PI_2_BY_Fs 1.57079635e-4       // PI_2/SAMPLING_FREQ
#define PI_2_BY_Fs_MOD_1 1.88495562    // PI_2_BY_Fs*MODULATOR_1_FREQ
#define INVERSE_PI_2 0.159154943       // 1.0/2.0/PI
#define SCALE 1.52878906e-5            // 1.0/65536

// High pass filter coefficients, delay storage elements
#define FILTER_STAGES 4    // number of 2nd-order stages in hpf

// Declare global variables and arrays

// Coefficients: b(0), b(1), b(2), a(1), a(2)
static const SLData_t hpf_taps[FILTER_STAGES * SIGLIB_IIR_COEFFS_PER_BIQUAD] = {
    2.23961687447E-0001, -4.02333411719E-0001, 2.23961687447E-0001, 1.50911097475E+0000, 6.42400555318E-0001,
    2.23961687447E-0001, -1.95529517845E-0001, 2.23961687447E-0001, 1.12786907788E+0000, 7.63610351067E-0001,
    2.23961687447E-0001, -5.92230015925E-0002, 2.23961687447E-0001, 7.80337450065E-0001, 8.83615509653E-0001,
    2.23961687447E-0001, -5.91200419900E-0003, 2.23961687447E-0001, 6.19117222196E-0001, 9.66022175006E-0001};

static const SLData_t lpf_taps[FILTER_STAGES * SIGLIB_IIR_COEFFS_PER_BIQUAD] = {
    2.23961687447E-0001, 4.02333411719E-0001, 2.23961687447E-0001, -1.50911097475E+0000, 6.42400555318E-0001,
    2.23961687447E-0001, 1.95529517845E-0001, 2.23961687447E-0001, -1.12786907788E+0000, 7.63610351067E-0001,
    2.23961687447E-0001, 5.92230015925E-0002, 2.23961687447E-0001, -7.80337450065E-0001, 8.83615509653E-0001,
    2.23961687447E-0001, 5.91200419900E-0003, 2.23961687447E-0001, -6.19117222196E-0001, 9.66022175006E-0001};

static SLData_t hpf_delay[FILTER_STAGES * SIGLIB_IIR_DELAY_SIZE];
static SLData_t lpf_delay[FILTER_STAGES * SIGLIB_IIR_DELAY_SIZE];

static const SLData_t COS_MOD1[SIZE_COS_MOD1] = {1.0000000000,  -0.3090170443, -0.8090169327, 0.8090170869,  0.3090167948,
                                                 -1.0000000000, 0.3090172937,  0.8090167786,  -0.8090172410, -0.3090165454};

int main(void)
{
  h_GPC_Plot* h2DPlot;    // Plot object

  SLFixData_t cos_mod1_index;    // Size of the 1st stage modulator lookup table

  SLData_t alpha, freq_shift, omega;
  SLData_t sinePhase;
  SLData_t working_data;

  SLData_t* pSrc = SUF_VectorArrayAllocate(SAMPLE_LENGTH);
  SLData_t* pRealData = SUF_VectorArrayAllocate(FFT_LENGTH);
  SLData_t* pImagData = SUF_VectorArrayAllocate(FFT_LENGTH);
  SLData_t* pResults = SUF_VectorArrayAllocate(SAMPLE_LENGTH);
  SLData_t* pFFTCoeffs = SUF_FftCoefficientAllocate(FFT_LENGTH);

  if ((NULL == pSrc) || (NULL == pRealData) || (NULL == pImagData) || (NULL == pResults) || (NULL == pFFTCoeffs)) {

    printf("\n\nMemory allocation failed\n\n");
    exit(0);
  }

  h2DPlot =                                  // Initialize plot
      gpc_init_2d("Amplitude Modulation",    // Plot title
                  "Time / Frequency",        // X-Axis label
                  "Magnitude",               // Y-Axis label
                  GPC_AUTO_SCALE,            // Scaling mode
                  GPC_SIGNED,                // Sign mode
                  GPC_KEY_ENABLE);           // Legend / key mode
  if (NULL == h2DPlot) {
    printf("\nPlot creation failure.\n");
    exit(-1);
  }

  SIF_Iir(hpf_delay,         // Pointer to filter state array
          FILTER_STAGES);    // Number of second order stages
  SIF_Iir(lpf_delay,         // Pointer to filter state array
          FILTER_STAGES);    // Number of second order stages

  // Initialise FFT
  SIF_Fft(pFFTCoeffs,                 // Pointer to FFT coefficients
          SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit
                                      // reverse address table
          FFT_LENGTH);                // FFT length

  // Generate signal to be shifted
  SDA_SignalGenerate(pSrc,                    // Pointer to destination array
                     SIGLIB_SINE_WAVE,        // Signal type - Sine wave
                     SIGLIB_ONE,              // Signal peak level
                     SIGLIB_FILL,             // Fill (overwrite) or add to existing array contents
                     0.1,                     // Signal frequency
                     SIGLIB_ZERO,             // D.C. Offset
                     SIGLIB_ZERO,             // Unused
                     SIGLIB_ZERO,             // Signal end value - Unused
                     &sinePhase,              // Signal phase - maintained across array boundaries
                     SIGLIB_NULL_DATA_PTR,    // Unused
                     SAMPLE_LENGTH);          // Output dataset length

  SDA_Copy(pSrc,           // Pointer to source array
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

  // Calculate real power from complex
  SDA_LogMagnitude(pRealData,      // Pointer to real source array
                   pImagData,      // Pointer to imaginary source array
                   pResults,       // Pointer to log magnitude destination array
                   FFT_LENGTH);    // Dataset length

  gpc_plot_2d(h2DPlot,                        // Graph handle
              pSrc,                           // Dataset
              FFT_LENGTH,                     // Dataset length
              "Source Signal",                // Dataset title
              SIGLIB_ZERO,                    // Minimum X value
              (double)(SAMPLE_LENGTH - 1),    // Maximum X value
              "lines",                        // Graph type
              "blue",                         // Colour
              GPC_NEW);                       // New graph
  printf("\nSource Signal\nPlease hit <Carriage Return> to continue . . .");
  getchar();

  gpc_plot_2d(h2DPlot,                        // Graph handle
              pResults,                       // Dataset
              FFT_LENGTH,                     // Dataset length
              "Source Spectrum",              // Dataset title
              SIGLIB_ZERO,                    // Minimum X value
              (double)(SAMPLE_LENGTH - 1),    // Maximum X value
              "lines",                        // Graph type
              "blue",                         // Colour
              GPC_NEW);                       // New graph
  printf("\nSource Spectrum\nPlease hit <Carriage Return> to continue . . .");
  getchar();

  // Initialize all variable locations
  freq_shift = FREQUENCY_SHIFT;    // Initialise frequency translation
  cos_mod1_index = 0;              // Initialize the 1st modulator index value

  alpha = PI_2_BY_Fs * (MODULATOR_1_FREQ - freq_shift);
  omega = SIGLIB_ZERO;    // omega is the increasing radian value for
                          // the 2nd modulation stage

  for (SLArrayIndex_t i = 0; i < SAMPLE_LENGTH; i++) {
    // perform the first stage modulation to move baseband
    // signal around MOD_1_FREQ
    working_data = COS_MOD1[cos_mod1_index++] * *(pSrc + i);
    if (cos_mod1_index >= SIZE_COS_MOD1) {
      cos_mod1_index = 0;
    }

    // High pass filter the data to reject the lower side band
    working_data = SDS_Iir(working_data,      // Input data sample to be filtered
                           hpf_delay,         // Pointer to filter state array
                           hpf_taps,          // Pointer to filter coefficients array
                           FILTER_STAGES);    // Number of stages

    // Second stage modulation to move back to baseband
    working_data *= SDS_Cos(omega);
    omega += alpha;    // Increase phase

    working_data *= SIGLIB_FOUR;    // Gain the signal

    // Low pass filter the data to reject the unwanted side band
    *(pResults + i) = SDS_Iir(working_data,      // Input data sample to be filtered
                              lpf_delay,         // Pointer to filter state array
                              lpf_taps,          // Pointer to filter coefficients array
                              FILTER_STAGES);    // Number of stages
  }

  gpc_plot_2d(h2DPlot,                        // Graph handle
              pResults,                       // Dataset
              FFT_LENGTH,                     // Dataset length
              "Shifted Signal",               // Dataset title
              SIGLIB_ZERO,                    // Minimum X value
              (double)(SAMPLE_LENGTH - 1),    // Maximum X value
              "lines",                        // Graph type
              "blue",                         // Colour
              GPC_NEW);                       // New graph
  printf("\nShifted Signal\nPlease hit <Carriage Return> to continue . . .");
  getchar();

  SDA_Copy(pResults,       // Pointer to source array
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

  // Calculate real power from complex
  SDA_LogMagnitude(pRealData,      // Pointer to real source array
                   pImagData,      // Pointer to imaginary source array
                   pResults,       // Pointer to log magnitude destination array
                   FFT_LENGTH);    // Dataset length

  gpc_plot_2d(h2DPlot,                        // Graph handle
              pResults,                       // Dataset
              FFT_LENGTH,                     // Dataset length
              "Shifted Spectrum",             // Dataset title
              SIGLIB_ZERO,                    // Minimum X value
              (double)(SAMPLE_LENGTH - 1),    // Maximum X value
              "lines",                        // Graph type
              "blue",                         // Colour
              GPC_NEW);                       // New graph
  printf("\nShifted Spectrum\nHit <Carriage Return> to continue ....\n");
  getchar();    // Wait for <Carriage Return>
  gpc_close(h2DPlot);

  SUF_MemoryFree(pSrc);    // Free memory
  SUF_MemoryFree(pRealData);
  SUF_MemoryFree(pImagData);
  SUF_MemoryFree(pResults);
  SUF_MemoryFree(pFFTCoeffs);

  return (0);
}
