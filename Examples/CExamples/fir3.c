// SigLib FIR Filter And Group Delay Example
// Copyright (c) 2023 Alpha Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>                                                 // SigLib DSP library
#include <gnuplot_c.h>                                              // Gnuplot/C

// Define constants
#define FILE_OUTPUT     0                                           // Define as 1 for output to file
#define SAMPLE_LENGTH   512
#define FILTER_LENGTH   128
#define FFT_LENGTH      512
#define LOG2_FFT_LENGTH SAI_FftLengthLog2(FFT_LENGTH)               // Log2 FFT length,
#define WINDOW_LENGTH   FFT_LENGTH

// Declare global variables and arrays

            // Initialise filter coefficients
static const SLData_t pFilterTaps[FILTER_LENGTH] = {
  -5.34266824816E-0004, 1.24568792546E-0003, 8.73278953630E-0004,
  4.37786545396E-0004, 1.57576481250E-0003, -1.38557006660E-0003,
  1.90710297624E-0004, -1.85794214656E-0003, -2.00704829381E-0003,
  2.40455797115E-0004, -2.04022181601E-0003, 2.71242012844E-0003,
  8.82181294290E-0004, 2.06390803758E-0003, 3.46115122350E-0003,
  -1.75033817834E-0003, 1.86645226647E-0003, -4.19706616768E-0003,
  -2.84686649762E-0003, -1.38480747151E-0003, -4.84877083488E-0003,
  4.15691399504E-0003, -5.59004695862E-0004, 5.33078655121E-0003,
  5.64663098926E-0003, -6.64415849217E-0004, 5.54512386900E-0003,
  -7.26170537971E-0003, -2.32956984257E-0003, -5.38280517195E-0003,
  -8.92656985171E-0003, 4.46954409718E-0003, -4.72452163230E-0003,
  1.05439735389E-0002, 7.10658444744E-0003, 3.43906801399E-0003,
  1.19942117579E-0002, -1.02552167173E-0002, 1.37716894902E-0003,
  -1.31325730564E-0002, -1.39301212180E-0002, 1.64388322268E-0003,
  -1.37820329052E-0002, 1.81624192809E-0002, 5.87454453127E-0003,
  1.37146029376E-0002, 2.30328314183E-0002, -1.17097244170E-0002,
  1.26049642763E-0002, -2.87440911303E-0002, -1.98723865258E-0002,
  -9.90924135116E-0003, -3.58018612026E-0002, 3.19153948705E-0002,
  -4.50363677305E-0003, 4.55694407950E-0002, 5.19287403766E-0002,
  -6.69120481527E-0003, 6.26204030713E-0002, -9.50703908154E-0002,
  -3.71802059842E-0002, -1.13988434152E-0001, -2.98431997465E-0001,
  4.24370110064E-0001, 4.24370110064E-0001, -2.98431997465E-0001,
  -1.13988434152E-0001, -3.71802059842E-0002, -9.50703908154E-0002,
  6.26204030713E-0002, -6.69120481527E-0003, 5.19287403766E-0002,
  4.55694407950E-0002, -4.50363677305E-0003, 3.19153948705E-0002,
  -3.58018612026E-0002, -9.90924135116E-0003, -1.98723865258E-0002,
  -2.87440911303E-0002, 1.26049642763E-0002, -1.17097244170E-0002,
  2.30328314183E-0002, 1.37146029376E-0002, 5.87454453127E-0003,
  1.81624192809E-0002, -1.37820329052E-0002, 1.64388322268E-0003,
  -1.39301212180E-0002, -1.31325730564E-0002, 1.37716894902E-0003,
  -1.02552167173E-0002, 1.19942117579E-0002, 3.43906801399E-0003,
  7.10658444744E-0003, 1.05439735389E-0002, -4.72452163230E-0003,
  4.46954409718E-0003, -8.92656985171E-0003, -5.38280517195E-0003,
  -2.32956984257E-0003, -7.26170537971E-0003, 5.54512386900E-0003,
  -6.64415849217E-0004, 5.64663098926E-0003, 5.33078655121E-0003,
  -5.59004695862E-0004, 4.15691399504E-0003, -4.84877083488E-0003,
  -1.38480747151E-0003, -2.84686649762E-0003, -4.19706616768E-0003,
  1.86645226647E-0003, -1.75033817834E-0003, 3.46115122350E-0003,
  2.06390803758E-0003, 8.82181294290E-0004, 2.71242012844E-0003,
  -2.04022181601E-0003, 2.40455797115E-0004, -2.00704829381E-0003,
  -1.85794214656E-0003, 1.90710297624E-0004, -1.38557006660E-0003,
  1.57576481250E-0003, 4.37786545396E-0004, 8.73278953630E-0004,
  1.24568792546E-0003, -5.34266824816E-0004
};

static SLData_t pFilterState[FILTER_LENGTH];
static SLArrayIndex_t FilterIndex;
static SLData_t *pReal, *pImag, *pMagnitude, *pPhase, *pFFTCoeffs;
static SLData_t ChirpPhase, ChirpValue;
static SLData_t PreviousPhase;

#if FILE_OUTPUT
static FILE    *fpOutputFile;
static const char fname1[] = "firlpf_t.sig";
static const char fname2[] = "firlpf_f.sig";
#endif


int main (
  void)
{
  h_GPC_Plot     *h2DPlot;                                          // Plot object

  pReal = SUF_VectorArrayAllocate (SAMPLE_LENGTH);
  pImag = SUF_VectorArrayAllocate (SAMPLE_LENGTH);
  pMagnitude = SUF_VectorArrayAllocate (SAMPLE_LENGTH);
  pPhase = SUF_VectorArrayAllocate (SAMPLE_LENGTH);
  pFFTCoeffs = SUF_FftCoefficientAllocate (FFT_LENGTH);

  if ((NULL == pReal) || (NULL == pImag) || (NULL == pMagnitude) || (NULL == pPhase) || (NULL == pFFTCoeffs)) {
    printf ("Memory allocation error in main()\n");
    exit (-1);
  }

  h2DPlot =                                                         // Initialize plot
    gpc_init_2d ("FIR Filter And Group Delay",                      // Plot title
                 "Impulse Response / Frequency",                    // X-Axis label
                 "Magnitude",                                       // Y-Axis label
                 GPC_AUTO_SCALE,                                    // Scaling mode
                 GPC_SIGNED,                                        // Sign mode
                 GPC_KEY_ENABLE);                                   // Legend / key mode
  if (NULL == h2DPlot) {
    printf ("\nPlot creation failure.\n");
    exit (-1);
  }

// Initialise FFT
  SIF_Fft (pFFTCoeffs,                                              // Pointer to FFT coefficients
           SIGLIB_BIT_REV_STANDARD,                                 // Bit reverse mode flag / Pointer to bit reverse address table
           FFT_LENGTH);                                             // FFT length
// Initialise FIR filter
  SIF_Fir (pFilterState,                                            // Pointer to filter state array
           &FilterIndex,                                            // Pointer to filter index register
           FILTER_LENGTH);                                          // Filter length

// Generate a linear source signal
  ChirpPhase = SIGLIB_ZERO;
  ChirpValue = SIGLIB_ZERO;
  SDA_SignalGenerate (pReal,                                        // Pointer to destination array
                      SIGLIB_CHIRP_LIN,                             // Signal type - Chirp with linear frequency ramp
                      SIGLIB_ONE,                                   // Signal peak level
                      SIGLIB_FILL,                                  // Fill (overwrite) or add to existing array contents
                      SIGLIB_ZERO,                                  // Signal lower frequency
                      SIGLIB_ZERO,                                  // D.C. Offset
                      SIGLIB_ONE / 1024.,                           // Frequency change per sample period
                      SIGLIB_HALF,                                  // Signal upper frequency
                      &ChirpPhase,                                  // Chirp phase - used for next iteration
                      &ChirpValue,                                  // Chirp current value - used for next iteration
                      SAMPLE_LENGTH);                               // Output dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pReal,                                               // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Linear Chirp Signal",                               // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nLinear Chirp Signal\nPlease hit <Carriage Return> to continue . . .");
  getchar ();

#if FILE_OUTPUT
  if ((fpOutputFile = fopen (fname1, "wb")) == NULL) {
    printf ("Can not open Dat file %s\n", fname1);
    exit (0);
  }

  SUF_SigWriteData (pFilterTaps, fpOutputFile, FILTER_LENGTH);

  fclose (fpOutputFile);
#endif

  SDA_Fir (pReal,                                                   // Input array to be filtered
           pReal,                                                   // Filtered output array
           pFilterState,                                            // Pointer to filter state array
           pFilterTaps,                                             // Pointer to filter coefficients
           &FilterIndex,                                            // Pointer to filter index register
           FILTER_LENGTH,                                           // Filter length
           SAMPLE_LENGTH);                                          // Dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pReal,                                               // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Filtered Linear Chirp Signal",                      // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nFiltered Linear Chirp Signal\nPlease hit <Carriage Return> to continue . . .");
  getchar ();

// Perform real FFT
  SDA_Rfft (pReal,                                                  // Pointer to real array
            pImag,                                                  // Pointer to imaginary array
            pFFTCoeffs,                                             // Pointer to FFT coefficients
            SIGLIB_BIT_REV_STANDARD,                                // Bit reverse mode flag / Pointer to bit reverse address table
            FFT_LENGTH,                                             // FFT length
            LOG2_FFT_LENGTH);                                       // log2 FFT length
// Calc polar from complex
  SDA_RectangularToPolar (pReal,                                    // Pointer to source real array
                          pImag,                                    // Pointer to source imaginary array
                          pMagnitude,                               // Pointer to destination magnitude array
                          pPhase,                                   // Pointer to destination phase array
                          FFT_LENGTH);                              // Dataset length
  SDA_20Log10 (pMagnitude,                                          // Pointer to source array
               pMagnitude,                                          // Pointer to destination array
               FFT_LENGTH);                                         // Dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pMagnitude,                                          // Dataset
               FFT_LENGTH,                                          // Dataset length
               "Spectrum Of Filtered Linear Chirp Signal",          // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nSpectrum Of Filtered Linear Chirp Signal\nPlease hit <Carriage Return> to continue . . .");
  getchar ();

#if FILE_OUTPUT
  if ((fpOutputFile = fopen (fname2, "wb")) == NULL) {
    printf ("Can not open Dat file %s\n", fname2);
    exit (0);
  }

  SUF_SigWriteData (pMagnitude, fpOutputFile, FFT_LENGTH);

  fclose (fpOutputFile);
#endif

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pPhase,                                              // Dataset
               FFT_LENGTH,                                          // Dataset length
               "Phase Of Filtered Linear Chirp Signal",             // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nPhase Of Filtered Linear Chirp Signal\nPlease hit <Carriage Return> to continue . . .");
  getchar ();

  PreviousPhase = SIGLIB_ZERO;
  SDA_GroupDelay (pPhase,                                           // Pointer to source array
                  pPhase,                                           // Pointer to destination array
                  &PreviousPhase,                                   // Pointer to previous phase value
                  SAMPLE_LENGTH);                                   // Dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pPhase,                                              // Dataset
               FFT_LENGTH,                                          // Dataset length
               "Group Delay Of Filtered Linear Chirp Signal",       // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (SAMPLE_LENGTH - 1),                        // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nGroup Delay Of Filtered Linear Chirp Signal\n");

  printf ("\nHit <Carriage Return> to continue ....\n");
  getchar ();                                                       // Wait for <Carriage Return>
  gpc_close (h2DPlot);

  SUF_MemoryFree (pReal);                                           // Free memory
  SUF_MemoryFree (pImag);
  SUF_MemoryFree (pMagnitude);
  SUF_MemoryFree (pPhase);
  SUF_MemoryFree (pFFTCoeffs);

  exit (0);
}
