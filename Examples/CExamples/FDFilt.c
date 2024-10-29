// SigLib Frequency Domain Overlapped FIR Filter Example
// This program filters a signal in the time domain and then in
// the frequency domain using the overlap-add and overlap-save
// techniques
// Copyright (c) 2024 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>       // SigLib DSP library
#include <gnuplot_c.h>    // Gnuplot/C

// Define constants
#define FILTER_LENGTH 64
#define SAMPLE_LENGTH 512
#define INVERSE_SAMPLE_LENGTH (SIGLIB_ONE / ((SLData_t)SAMPLE_LENGTH))

#define FFT_LENGTH 128
#define LOG2_FFT_LENGTH SAI_FftLengthLog2(FFT_LENGTH)    // Log2 FFT length,
#define TIME_DOMAIN_DATA_LENGTH 64                       // Each iteration processes this many data samples

// Declare global variables and arrays
// Initialise filter coefficients
static const SLData_t TimeDomainCoeffs[FILTER_LENGTH] = {
    -3.783E-3, 2.803E-3, 2.648E-3, 2.891E-3, 3.397E-3, 4.094E-3, 4.942E-3, 5.917E-3, 7.005E-3, 8.196E-3, 9.479E-3, 1.084E-2, 1.229E-2,
    1.379E-2,  1.535E-2, 1.695E-2, 1.857E-2, 2.021E-2, 2.183E-2, 2.344E-2, 2.500E-2, 2.651E-2, 2.795E-2, 2.929E-2, 3.053E-2, 3.165E-2,
    3.264E-2,  3.348E-2, 3.417E-2, 3.469E-2, 3.504E-2, 3.522E-2, 3.522E-2, 3.504E-2, 3.469E-2, 3.417E-2, 3.348E-2, 3.264E-2, 3.165E-2,
    3.053E-2,  2.929E-2, 2.795E-2, 2.651E-2, 2.500E-2, 2.344E-2, 2.183E-2, 2.021E-2, 1.857E-2, 1.695E-2, 1.535E-2, 1.379E-2, 1.229E-2,
    1.084E-2,  9.479E-3, 8.196E-3, 7.005E-3, 5.917E-3, 4.942E-3, 4.094E-3, 3.397E-3, 2.891E-3, 2.648E-3, 2.803E-3, -3.783E-3};

int main(void)
{
  h_GPC_Plot* h2DPlot;    // Plot object

  SLData_t InverseFFTLength;
  SLArrayIndex_t filterIndex;

  // Allocate memory
  SLData_t* pSrc = SUF_VectorArrayAllocate(SAMPLE_LENGTH + FFT_LENGTH);    // Allow for overlap - overlap and save
  SLData_t* pFilterState = SUF_VectorArrayAllocate(FILTER_LENGTH);
  SLData_t* pTDFilt = SUF_VectorArrayAllocate(SAMPLE_LENGTH);
  SLData_t* pFDFilt = SUF_VectorArrayAllocate(SAMPLE_LENGTH + FFT_LENGTH);    // Allow for overlap - overlap and add

  SLData_t* RealFreqDomainCoeffs = SUF_VectorArrayAllocate(FFT_LENGTH);    // Allocate working arrays
  SLData_t* ImagFreqDomainCoeffs = SUF_VectorArrayAllocate(FFT_LENGTH);
  SLData_t* OverlapArrayPtr = SUF_VectorArrayAllocate(FFT_LENGTH);    // Allocate overlap storage
  SLData_t* TempArrayPtr = SUF_VectorArrayAllocate(FFT_LENGTH);
  SLData_t* pFFTCoeffs = SUF_FftCoefficientAllocate(FFT_LENGTH);

  // Clear array to ensure zero padded - overlap and save
  SDA_Zeros(pSrc,                                  // Pointer to destination array
            SAMPLE_LENGTH + FFT_LENGTH);           // Dataset length
  h2DPlot =                                        // Initialize plot
      gpc_init_2d("Frequency Domain Filtering",    // Plot title
                  "Time",                          // X-Axis label
                  "Magnitude",                     // Y-Axis label
                  GPC_AUTO_SCALE,                  // Scaling mode
                  GPC_SIGNED,                      // Sign mode
                  GPC_KEY_ENABLE);                 // Legend / key mode
  if (NULL == h2DPlot) {
    printf("\nPlot creation failure.\n");
    exit(-1);
  }

  SLData_t sinePhase = SIGLIB_ZERO;
  SLData_t cosinePhase = SIGLIB_ZERO;

  // Generate a noisy sinewave
  SDA_SignalGenerate(pSrc,                    // Pointer to destination array
                     SIGLIB_SINE_WAVE,        // Signal type - Sine wave
                     SIGLIB_HALF,             // Signal peak level
                     SIGLIB_FILL,             // Fill (overwrite) or add to existing array contents
                     0.01,                    // Signal frequency
                     SIGLIB_ZERO,             // D.C. Offset
                     SIGLIB_ZERO,             // Unused
                     SIGLIB_ZERO,             // Signal end value - Unused
                     &sinePhase,              // Signal phase - maintained across array boundaries
                     SIGLIB_NULL_DATA_PTR,    // Unused
                     SAMPLE_LENGTH);          // Output dataset length

  SDA_SignalGenerate(pSrc,                    // Pointer to destination array
                     SIGLIB_COSINE_WAVE,      // Signal type - Cosine wave
                     SIGLIB_HALF,             // Signal peak level
                     SIGLIB_ADD,              // Fill (overwrite) or add to existing array contents
                     0.01,                    // Signal frequency
                     SIGLIB_ZERO,             // D.C. Offset
                     SIGLIB_ZERO,             // Unused
                     SIGLIB_ZERO,             // Signal end value - Unused
                     &cosinePhase,            // Signal phase - maintained across array boundaries
                     SIGLIB_NULL_DATA_PTR,    // Unused
                     SAMPLE_LENGTH);          // Output dataset length

  SDA_SignalGenerate(pSrc,                    // Pointer to destination array
                     SIGLIB_WHITE_NOISE,      // Signal type - random white noise
                     0.2,                     // Signal peak level
                     SIGLIB_ADD,              // Fill (overwrite) or add to existing array contents
                     SIGLIB_ZERO,             // Signal frequency - Unused
                     SIGLIB_ZERO,             // D.C. Offset
                     SIGLIB_ZERO,             // Unused
                     SIGLIB_ZERO,             // Signal end value - Unused
                     SIGLIB_NULL_DATA_PTR,    // Unused
                     SIGLIB_NULL_DATA_PTR,    // Unused
                     SAMPLE_LENGTH);          // Output dataset length

  gpc_plot_2d(h2DPlot,                        // Graph handle
              pSrc,                           // Dataset
              SAMPLE_LENGTH,                  // Dataset length
              "Unfiltered Signal",            // Dataset title
              SIGLIB_ZERO,                    // Minimum X value
              (double)(SAMPLE_LENGTH - 1),    // Maximum X value
              "lines",                        // Graph type
              "blue",                         // Colour
              GPC_NEW);                       // New graph

  // Initialise FIR filter
  SIF_Fir(pFilterState,      // Pointer to filter state array
          &filterIndex,      // Pointer to filter index register
          FILTER_LENGTH);    // Filter length

  // Apply fir filter
  SDA_Fir(pSrc,                // Input array to be filtered
          pTDFilt,             // Filtered output array
          pFilterState,        // Pointer to filter state array
          TimeDomainCoeffs,    // Pointer to filter coefficients
          &filterIndex,        // Pointer to filter index register
          FILTER_LENGTH,       // Filter length
          SAMPLE_LENGTH);      // Dataset length

  gpc_plot_2d(h2DPlot,                          // Graph handle
              pTDFilt,                          // Dataset
              SAMPLE_LENGTH,                    // Dataset length
              "Time Domain Filtered Signal",    // Dataset title
              SIGLIB_ZERO,                      // Minimum X value
              (double)(SAMPLE_LENGTH - 1),      // Maximum X value
              "lines",                          // Graph type
              "red",                            // Colour
              GPC_ADD);                         // New graph
  printf("\nTime Domain Filtered Signal\nPlease hit <Carriage Return> to "
         "continue . . .");
  getchar();

  SIF_FirOverlapAdd(TimeDomainCoeffs,               // Time Domain Coefficients pointer
                    RealFreqDomainCoeffs,           // Ptr to Real Freq Domain Coefficients pointer
                    ImagFreqDomainCoeffs,           // Ptr to Imaginary Freq Domain Coefficients pointer
                    OverlapArrayPtr,                // Ptr to overlap array pointer
                    pFFTCoeffs,                     // Pointer to FFT coeffs
                    SIGLIB_NULL_ARRAY_INDEX_PTR,    // Pointer to FFT bit reverse address table
                    &InverseFFTLength,              // Pointer to inverse FFT Length
                    FFT_LENGTH,                     // FFT Length
                    LOG2_FFT_LENGTH,                // Log10 FFT Length
                    FILTER_LENGTH);                 // Filter length

  // Apply frequency domain (overlap and add) FIR filter
  for (SLArrayIndex_t i = 0; i < SAMPLE_LENGTH; i += TIME_DOMAIN_DATA_LENGTH) {
    SDA_FirOverlapAdd(pSrc + i,                       // Source data pointer
                      pFDFilt + i,                    // Destination data pointer
                      RealFreqDomainCoeffs,           // Real Freq Domain Coefficients pointer
                      ImagFreqDomainCoeffs,           // Imaginary Freq Domain Coefficients pointer
                      OverlapArrayPtr,                // Overlap array pointer
                      TempArrayPtr,                   // Temporary array pointer
                      pFFTCoeffs,                     // Pointer to FFT coeffs
                      SIGLIB_NULL_ARRAY_INDEX_PTR,    // Pointer to FFT bit reverse address table
                      InverseFFTLength,               // Inverse FFT Length
                      FFT_LENGTH,                     // FFT Length
                      LOG2_FFT_LENGTH,                // Log 10 FFT Length
                      FILTER_LENGTH,                  // Filter length
                      TIME_DOMAIN_DATA_LENGTH);       // Dataset length
  }
  // Calculate mean square error
  SLData_t MSE = SDA_MeanSquareError(pTDFilt,                  // Pointer to source array 1
                                     pFDFilt,                  // Pointer to source array 2
                                     INVERSE_SAMPLE_LENGTH,    // Inverse of the dataset length
                                     SAMPLE_LENGTH);           // Dataset length

  gpc_plot_2d(h2DPlot,                                                 // Graph handle
              pFDFilt,                                                 // Dataset
              SAMPLE_LENGTH,                                           // Dataset length
              "Frequency Domain (Overlap And Add) Filtered Signal",    // Dataset title
              SIGLIB_ZERO,                                             // Minimum X value
              (double)(SAMPLE_LENGTH - 1),                             // Maximum X value
              "lines",                                                 // Graph type
              "magenta",                                               // Colour
              GPC_ADD);                                                // New graph
  printf("\nFrequency Domain (Overlap And Add) Filtered Signal MSE = "
         "%e\nPlease hit <Carriage Return> to continue . . .",
         MSE);
  getchar();

  SIF_FirOverlapSave(TimeDomainCoeffs,               // Time Domain Coefficients pointer
                     RealFreqDomainCoeffs,           // Ptr to Real Freq Domain Coefficients pointer
                     ImagFreqDomainCoeffs,           // Ptr to Imaginary Freq Domain Coefficients pointer
                     OverlapArrayPtr,                // Ptr to overlap array pointer
                     pFFTCoeffs,                     // Pointer to FFT coeffs
                     SIGLIB_NULL_ARRAY_INDEX_PTR,    // Pointer to FFT bit reverse address table
                     &InverseFFTLength,              // Pointer to inverse FFT Length
                     FFT_LENGTH,                     // FFT Length
                     LOG2_FFT_LENGTH,                // Log10 FFT Length
                     FILTER_LENGTH);                 // Filter length

  // Apply frequency domain (overlap and add) FIR filter
  for (SLArrayIndex_t i = 0; i < SAMPLE_LENGTH; i += TIME_DOMAIN_DATA_LENGTH) {
    SDA_FirOverlapSave(pSrc + i,                       // Source data pointer
                       pFDFilt + i,                    // Destination data pointer
                       RealFreqDomainCoeffs,           // Real Freq Domain Coefficients pointer
                       ImagFreqDomainCoeffs,           // Imaginary Freq Domain Coefficients pointer
                       OverlapArrayPtr,                // Overlap array pointer
                       TempArrayPtr,                   // Temporary array pointer
                       pFFTCoeffs,                     // Pointer to FFT coeffs
                       SIGLIB_NULL_ARRAY_INDEX_PTR,    // Pointer to FFT bit reverse address table
                       InverseFFTLength,               // Inverse FFT Length
                       FFT_LENGTH,                     // FFT Length
                       LOG2_FFT_LENGTH,                // Log 10 FFT Length
                       FILTER_LENGTH,                  // Filter length
                       TIME_DOMAIN_DATA_LENGTH);       // Dataset length
  }
  // Calculate mean square error
  MSE = SDA_MeanSquareError(pTDFilt,                  // Pointer to source array 1
                            pFDFilt,                  // Pointer to source array 2
                            INVERSE_SAMPLE_LENGTH,    // Inverse of the dataset length
                            SAMPLE_LENGTH);           // Dataset length

  gpc_plot_2d(h2DPlot,                                                  // Graph handle
              pFDFilt,                                                  // Dataset
              SAMPLE_LENGTH,                                            // Dataset length
              "Frequency Domain (Overlap And Save) Filtered Signal",    // Dataset title
              SIGLIB_ZERO,                                              // Minimum X value
              (double)(SAMPLE_LENGTH - 1),                              // Maximum X value
              "lines",                                                  // Graph type
              "green",                                                  // Colour
              GPC_ADD);                                                 // New graph

  printf("\nFrequency Domain (Overlap And Save) Filtered Signal MSE = %e\nHit "
         "<Carriage Return> to continue ....\n",
         MSE);
  getchar();    // Wait for <Carriage Return>
  gpc_close(h2DPlot);

  SUF_MemoryFree(pSrc);    // Free memory
  SUF_MemoryFree(pFilterState);
  SUF_MemoryFree(pTDFilt);
  SUF_MemoryFree(pFDFilt);

  SUF_MemoryFree(RealFreqDomainCoeffs);
  SUF_MemoryFree(ImagFreqDomainCoeffs);
  SUF_MemoryFree(OverlapArrayPtr);
  SUF_MemoryFree(TempArrayPtr);
  SUF_MemoryFree(pFFTCoeffs);

  return (0);
}
