// SigLib FIR Filter Example 2
// Copyright (c) 2024 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>       // SigLib DSP library
#include <gnuplot_c.h>    // Gnuplot/C

// Define constants
#define FILTER_LENGTH 128
#define SAMPLE_LENGTH 512
#define FFT_LENGTH 512
#define LOG2_FFT_LENGTH SAI_FftLengthLog2(FFT_LENGTH)    // Log2 FFT length,

// Declare global variables and arrays

// Initialise filter coefficients
static const SLData_t pFilterTaps[FILTER_LENGTH] = {
    -5.34266824816E-0004, 1.24568792546E-0003,  8.73278953630E-0004,  4.37786545396E-0004,  1.57576481250E-0003,  -1.38557006660E-0003,
    1.90710297624E-0004,  -1.85794214656E-0003, -2.00704829381E-0003, 2.40455797115E-0004,  -2.04022181601E-0003, 2.71242012844E-0003,
    8.82181294290E-0004,  2.06390803758E-0003,  3.46115122350E-0003,  -1.75033817834E-0003, 1.86645226647E-0003,  -4.19706616768E-0003,
    -2.84686649762E-0003, -1.38480747151E-0003, -4.84877083488E-0003, 4.15691399504E-0003,  -5.59004695862E-0004, 5.33078655121E-0003,
    5.64663098926E-0003,  -6.64415849217E-0004, 5.54512386900E-0003,  -7.26170537971E-0003, -2.32956984257E-0003, -5.38280517195E-0003,
    -8.92656985171E-0003, 4.46954409718E-0003,  -4.72452163230E-0003, 1.05439735389E-0002,  7.10658444744E-0003,  3.43906801399E-0003,
    1.19942117579E-0002,  -1.02552167173E-0002, 1.37716894902E-0003,  -1.31325730564E-0002, -1.39301212180E-0002, 1.64388322268E-0003,
    -1.37820329052E-0002, 1.81624192809E-0002,  5.87454453127E-0003,  1.37146029376E-0002,  2.30328314183E-0002,  -1.17097244170E-0002,
    1.26049642763E-0002,  -2.87440911303E-0002, -1.98723865258E-0002, -9.90924135116E-0003, -3.58018612026E-0002, 3.19153948705E-0002,
    -4.50363677305E-0003, 4.55694407950E-0002,  5.19287403766E-0002,  -6.69120481527E-0003, 6.26204030713E-0002,  -9.50703908154E-0002,
    -3.71802059842E-0002, -1.13988434152E-0001, -2.98431997465E-0001, 4.24370110064E-0001,  4.24370110064E-0001,  -2.98431997465E-0001,
    -1.13988434152E-0001, -3.71802059842E-0002, -9.50703908154E-0002, 6.26204030713E-0002,  -6.69120481527E-0003, 5.19287403766E-0002,
    4.55694407950E-0002,  -4.50363677305E-0003, 3.19153948705E-0002,  -3.58018612026E-0002, -9.90924135116E-0003, -1.98723865258E-0002,
    -2.87440911303E-0002, 1.26049642763E-0002,  -1.17097244170E-0002, 2.30328314183E-0002,  1.37146029376E-0002,  5.87454453127E-0003,
    1.81624192809E-0002,  -1.37820329052E-0002, 1.64388322268E-0003,  -1.39301212180E-0002, -1.31325730564E-0002, 1.37716894902E-0003,
    -1.02552167173E-0002, 1.19942117579E-0002,  3.43906801399E-0003,  7.10658444744E-0003,  1.05439735389E-0002,  -4.72452163230E-0003,
    4.46954409718E-0003,  -8.92656985171E-0003, -5.38280517195E-0003, -2.32956984257E-0003, -7.26170537971E-0003, 5.54512386900E-0003,
    -6.64415849217E-0004, 5.64663098926E-0003,  5.33078655121E-0003,  -5.59004695862E-0004, 4.15691399504E-0003,  -4.84877083488E-0003,
    -1.38480747151E-0003, -2.84686649762E-0003, -4.19706616768E-0003, 1.86645226647E-0003,  -1.75033817834E-0003, 3.46115122350E-0003,
    2.06390803758E-0003,  8.82181294290E-0004,  2.71242012844E-0003,  -2.04022181601E-0003, 2.40455797115E-0004,  -2.00704829381E-0003,
    -1.85794214656E-0003, 1.90710297624E-0004,  -1.38557006660E-0003, 1.57576481250E-0003,  4.37786545396E-0004,  8.73278953630E-0004,
    1.24568792546E-0003,  -5.34266824816E-0004};

static SLData_t pFilterState[FILTER_LENGTH];
static SLArrayIndex_t FilterIndex;

int main(void)
{
  h_GPC_Plot* h2DPlot;    // Plot object

  SLData_t* pSrc1 = SUF_VectorArrayAllocate(SAMPLE_LENGTH);
  SLData_t* pSrc2 = SUF_VectorArrayAllocate(SAMPLE_LENGTH);
  SLData_t* pImagData = SUF_VectorArrayAllocate(SAMPLE_LENGTH);
  SLData_t* pResults = SUF_VectorArrayAllocate(SAMPLE_LENGTH);
  SLData_t* pFFTCoeffs = SUF_FftCoefficientAllocate(FFT_LENGTH);
  if ((NULL == pSrc1) || (NULL == pSrc2) || (NULL == pImagData) || (NULL == pResults) || (NULL == pFFTCoeffs)) {
    printf("Memory allocation error in main()\n");
    exit(-1);
  }

  // Initialise FFT
  SIF_Fft(pFFTCoeffs,                 // Pointer to FFT coefficients
          SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit
                                      // reverse address table
          FFT_LENGTH);                // FFT length

  h2DPlot =                                          // Initialize plot
      gpc_init_2d("FIR Filter",                      // Plot title
                  "Impulse Response / Frequency",    // X-Axis label
                  "Magnitude",                       // Y-Axis label
                  GPC_AUTO_SCALE,                    // Scaling mode
                  GPC_SIGNED,                        // Sign mode
                  GPC_KEY_ENABLE);                   // Legend / key mode
  if (NULL == h2DPlot) {
    printf("\nPlot creation failure.\n");
    exit(-1);
  }

  SIF_Fir(pFilterState,      // Pointer to filter state array
          &FilterIndex,      // Pointer to filter index register
          FILTER_LENGTH);    // Filter length

  SDA_SignalGenerate(pSrc1,                   // Pointer to destination array
                     SIGLIB_WHITE_NOISE,      // Signal type - random white noise
                     0.9,                     // Signal peak level
                     SIGLIB_FILL,             // Fill (overwrite) or add to existing array contents
                     SIGLIB_ZERO,             // Signal frequency - Unused
                     SIGLIB_ZERO,             // D.C. Offset
                     SIGLIB_ZERO,             // Unused
                     SIGLIB_ZERO,             // Signal end value - Unused
                     SIGLIB_NULL_DATA_PTR,    // Unused
                     SIGLIB_NULL_DATA_PTR,    // Unused
                     SAMPLE_LENGTH);          // Output dataset length

  // Apply fir filter and store filtered data
  for (SLFixData_t i = 0; i < SAMPLE_LENGTH; i++) {
    *pSrc2++ = SDS_Fir(*pSrc1++,          // Input data sample to be filtered
                       pFilterState,      // Pointer to filter state array
                       pFilterTaps,       // Pointer to filter coefficients
                       &FilterIndex,      // Pointer to filter index register
                       FILTER_LENGTH);    // Filter length
  }

  pSrc1 -= SAMPLE_LENGTH;
  pSrc2 -= SAMPLE_LENGTH;

  gpc_plot_2d(h2DPlot,                        // Graph handle
              pSrc1,                          // Dataset
              SAMPLE_LENGTH,                  // Dataset length
              "Unfiltered Signal",            // Dataset title
              SIGLIB_ZERO,                    // Minimum X value
              (double)(SAMPLE_LENGTH - 1),    // Maximum X value
              "lines",                        // Graph type
              "blue",                         // Colour
              GPC_NEW);                       // New graph
  printf("\nUnfiltered Signal\nPlease hit <Carriage Return> to continue . . .");
  getchar();

  // Perform real FFT
  SDA_Rfft(pSrc1,                      // Pointer to real array
           pImagData,                  // Pointer to imaginary array
           pFFTCoeffs,                 // Pointer to FFT coefficients
           SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit
                                       // reverse address table
           FFT_LENGTH,                 // FFT length
           LOG2_FFT_LENGTH);           // log2 FFT length

  // Calculate real power from complex
  SDA_LogMagnitude(pSrc1,          // Pointer to real source array
                   pImagData,      // Pointer to imaginary source array
                   pResults,       // Pointer to log magnitude destination array
                   FFT_LENGTH);    // Dataset length

  gpc_plot_2d(h2DPlot,                         // Graph handle
              pResults,                        // Dataset
              FFT_LENGTH,                      // Dataset length
              "Unfiltered Signal Spectrum",    // Dataset title
              SIGLIB_ZERO,                     // Minimum X value
              (double)(SAMPLE_LENGTH - 1),     // Maximum X value
              "lines",                         // Graph type
              "blue",                          // Colour
              GPC_NEW);                        // New graph
  printf("\nUnfiltered Signal Spectrum\nPlease hit <Carriage Return> to "
         "continue . . .");
  getchar();

  gpc_plot_2d(h2DPlot,                        // Graph handle
              pSrc2,                          // Dataset
              SAMPLE_LENGTH,                  // Dataset length
              "Filtered Signal",              // Dataset title
              SIGLIB_ZERO,                    // Minimum X value
              (double)(SAMPLE_LENGTH - 1),    // Maximum X value
              "lines",                        // Graph type
              "blue",                         // Colour
              GPC_NEW);                       // New graph
  printf("\nFiltered Signal\nPlease hit <Carriage Return> to continue . . .");
  getchar();

  // Perform real FFT
  SDA_Rfft(pSrc2,                      // Pointer to real array
           pImagData,                  // Pointer to imaginary array
           pFFTCoeffs,                 // Pointer to FFT coefficients
           SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit
                                       // reverse address table
           FFT_LENGTH,                 // FFT length
           LOG2_FFT_LENGTH);           // log2 FFT length

  // Calculate real power from complex
  SDA_LogMagnitude(pSrc2,          // Pointer to real source array
                   pImagData,      // Pointer to imaginary source array
                   pResults,       // Pointer to log magnitude destination array
                   FFT_LENGTH);    // Dataset length

  gpc_plot_2d(h2DPlot,                        // Graph handle
              pResults,                       // Dataset
              FFT_LENGTH,                     // Dataset length
              "Filtered Signal Spectrum",     // Dataset title
              SIGLIB_ZERO,                    // Minimum X value
              (double)(SAMPLE_LENGTH - 1),    // Maximum X value
              "lines",                        // Graph type
              "blue",                         // Colour
              GPC_NEW);                       // New graph
  printf("\nFiltered Signal Spectrum\n");

  printf("\nHit <Carriage Return> to continue ....\n");
  getchar();    // Wait for <Carriage Return>
  gpc_close(h2DPlot);

  SUF_MemoryFree(pSrc1);    // Free memory
  SUF_MemoryFree(pSrc2);
  SUF_MemoryFree(pImagData);
  SUF_MemoryFree(pResults);
  SUF_MemoryFree(pFFTCoeffs);

  return (0);
}
