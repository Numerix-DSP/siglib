// SigLib Comb Filter Example
// Copyright (c) 2023 Delta Numerix All rights reserved.

// This program demonstrates the impulse and frequency response of four
// different arrangents of comb filter. The flow diagrams for each are:

// Comb filter type 1
//         |-----|     - |-----|
// IN ---->|Delay|------>|     |
//     |   |-----|       |     |
//     |               + |  +  |----> OUT
//     |---------------->|     |
// |-----|

// Comb filter type 2
//         |-----|
// IN ---->|comb |------> OUT
//         |-----|

// Comb filter type 3
//         |-----|      |-----|
// IN ---->|Comb1|----->|Comb2|-->|-----|
//     |   |-----|  |   |-----|   |     |
//     |            ------------->|  +  |----> OUT
//     |                          |     |
//     -------------------------->|-----|

// Comb filter type 4
//         |------|      |------|
// IN ---->|Delay1|----->|Delay2|-->|-----|
//     |   |------|  |   |------|   |     |
//     |             -------------->|  +  |----> OUT
//     |                            |     |
//     ---------------------------->|-----|

// Include files
#include <stdio.h>
#include <siglib.h>       // SigLib DSP library
#include <gnuplot_c.h>    // Gnuplot/C

// Define constants
#define SAMPLE_LENGTH 512
#define FFT_LENGTH SAMPLE_LENGTH
#define LOG2_FFT_LENGTH SAI_FftLengthLog2(FFT_LENGTH)    // Log2 FFT length,

// Declare global variables and arrays

int main(void)
{
  h_GPC_Plot* h2DPlot;    // Plot object

  h2DPlot =                              // Initialize plot
      gpc_init_2d("Comb Filter",         // Plot title
                  "Time / Frequency",    // X-Axis label
                  "Magnitude",           // Y-Axis label
                  GPC_AUTO_SCALE,        // Scaling mode
                  GPC_SIGNED,            // Sign mode
                  GPC_KEY_ENABLE);       // Legend / key mode
  if (NULL == h2DPlot) {
    printf("\nPlot creation failure.\n");
    exit(-1);
  }

  SLArrayIndex_t combFilterLength;
  SLArrayIndex_t combFilterIndex1, combFilterIndex2;
  SLData_t combFilterSum1, combFilterSum2;
  SLData_t K1, K2, K3;

  printf("\n\n\nThis program demonstrates the impulse and frequency\n");
  printf("response of four different arrangents of comb filter\n\n.");

  printf("Enter a comb filter length < 512 =>");
  scanf("%d", &combFilterLength);
  printf("\nComb filter length => %d\n", combFilterLength);
  getchar();    // Clear keyboard buffer

  SLData_t* pSrc1 = SUF_VectorArrayAllocate(SAMPLE_LENGTH);
  SLData_t* pSrc2 = SUF_VectorArrayAllocate(SAMPLE_LENGTH);
  SLData_t* pSrc3 = SUF_VectorArrayAllocate(SAMPLE_LENGTH);
  SLData_t* pImagData = SUF_VectorArrayAllocate(SAMPLE_LENGTH);
  SLData_t* pMagnitude = SUF_VectorArrayAllocate(SAMPLE_LENGTH);
  SLData_t* pPhase = SUF_VectorArrayAllocate(SAMPLE_LENGTH);
  SLData_t* pCombFilterStateArray1 = SUF_VectorArrayAllocate(combFilterLength);
  SLData_t* pCombFilterStateArray2 = SUF_VectorArrayAllocate(combFilterLength);
  SLData_t* pFFTCoeffs = SUF_FftCoefficientAllocate(FFT_LENGTH);

  if ((NULL == pSrc1) || (NULL == pSrc2) || (NULL == pSrc3) || (NULL == pImagData) || (NULL == pMagnitude) || (NULL == pPhase) ||
      (NULL == pCombFilterStateArray1) || (NULL == pCombFilterStateArray2) || (NULL == pFFTCoeffs)) {
    printf("Memory allocation error in main()\n");
    exit(-1);
  }

  // Initialise FFT
  SIF_Fft(pFFTCoeffs,                 // Pointer to FFT coefficients
          SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit
                                      // reverse address table
          FFT_LENGTH);                // FFT length

  // Comb filter type 1
  printf("\n\n\nComb filter type 1\n\n");
  printf("        |-----|     -\n");
  printf("IN ---->|Delay|------>|-----|\n");
  printf("    |   |-----|       |     |\n");
  printf("    |                 |     |\n");
  printf("    |                 |  +  |----> OUT\n");
  printf("    |                 |     |\n");
  printf("    |               + |     |\n");
  printf("    ----------------->|-----|\n");
  printf("\n Comb filter length => %d", combFilterLength);

  SIF_FixedDelay(pCombFilterStateArray1,    // Pointer to delay state array
                 &combFilterIndex1,         // Pointer to delay state index
                 combFilterLength);         // Delay length

  // Generate an impulse
  SDA_SignalGenerate(pSrc1,                   // Pointer to destination array
                     SIGLIB_IMPULSE,          // Signal type - Impulse function
                     SIGLIB_ONE,              // Signal peak level
                     SIGLIB_FILL,             // Fill (overwrite) or add to existing array contents
                     SIGLIB_ZERO,             // Signal frequency - Unused
                     SIGLIB_ZERO,             // D.C. Offset
                     SIGLIB_ZERO,             // Delay (samples periods) applied to impulse
                     SIGLIB_ZERO,             // Signal end value - Unused
                     SIGLIB_NULL_DATA_PTR,    // Unused
                     SIGLIB_NULL_DATA_PTR,    // Unused
                     SAMPLE_LENGTH);          // Output dataset length

  // Apply comb filter and store filtered data
  for (SLArrayIndex_t i = 0; i < SAMPLE_LENGTH; i++) {
    *pSrc2++ = *pSrc1 - SDS_FixedDelay(*pSrc1,                    // Input data sample
                                       pCombFilterStateArray1,    // Pointer to delay state array
                                       &combFilterIndex1,         // Pointer to delay state index
                                       combFilterLength);         // Delay length
    pSrc1++;
  }

  pSrc2 -= SAMPLE_LENGTH;    // Reset pointers
  pSrc1 -= SAMPLE_LENGTH;

  gpc_plot_2d(h2DPlot,                        // Graph handle
              pSrc2,                          // Dataset
              SAMPLE_LENGTH,                  // Dataset length
              "Impulse Response",             // Dataset title
              SIGLIB_ZERO,                    // Minimum X value
              (double)(SAMPLE_LENGTH - 1),    // Maximum X value
              "lines",                        // Graph type
              "blue",                         // Colour
              GPC_NEW);                       // New graph
  printf("\nImpulse Response\nPlease hit <Carriage Return> to continue . . .");
  getchar();

  // Perform real FFT
  SDA_Rfft(pSrc2,                      // Pointer to real array
           pImagData,                  // Pointer to imaginary array
           pFFTCoeffs,                 // Pointer to FFT coefficients
           SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit
                                       // reverse address table
           FFT_LENGTH,                 // FFT length
           LOG2_FFT_LENGTH);           // log2 FFT length

  SDA_RectangularToPolar(pSrc2,             // Pointer to source real array
                         pImagData,         // Pointer to source imaginary array
                         pMagnitude,        // Pointer to destination magnitude array
                         pPhase,            // Pointer to destination phase array
                         SAMPLE_LENGTH);    // Dataset length
  SDA_Multiply(pMagnitude,                  // Pointer to source array
               65536.,                      // Multiplier
               pMagnitude,                  // Pointer to destination array
               SAMPLE_LENGTH);              // Dataset length
  SDA_20Log10(pMagnitude,                   // Pointer to source array
              pMagnitude,                   // Pointer to destination array
              SAMPLE_LENGTH);               // Dataset length

  gpc_plot_2d(h2DPlot,                        // Graph handle
              pMagnitude,                     // Dataset
              SAMPLE_LENGTH,                  // Dataset length
              "Magnitude  (dB)",              // Dataset title
              SIGLIB_ZERO,                    // Minimum X value
              (double)(SAMPLE_LENGTH - 1),    // Maximum X value
              "lines",                        // Graph type
              "blue",                         // Colour
              GPC_NEW);                       // New graph
  printf("\nMagnitude Response\nPlease hit <Carriage Return> to continue . . .");
  getchar();

  gpc_plot_2d(h2DPlot,                        // Graph handle
              pPhase,                         // Dataset
              SAMPLE_LENGTH,                  // Dataset length
              "Phase",                        // Dataset title
              SIGLIB_ZERO,                    // Minimum X value
              (double)(SAMPLE_LENGTH - 1),    // Maximum X value
              "lines",                        // Graph type
              "red",                          // Colour
              GPC_NEW);                       // New graph
  printf("\nPhase Response\nPlease hit <Carriage Return> to continue . . .");
  getchar();

  // Comb filter type 2
  printf("\n\n\nComb filter type 2\n\n");
  printf("        |-----|\n");
  printf("IN ---->|comb |------> OUT\n");
  printf("        |-----|\n");
  printf("\n Comb filter length => %d", combFilterLength);

  SIF_Comb(pCombFilterStateArray1,    // Pointer to filter delay state array
           &combFilterIndex1,         // Pointer to filter index register
           &combFilterSum1,           // Pointer to filter sum register
           combFilterLength);         // Filter length

  // Generate an impulse
  SDA_SignalGenerate(pSrc1,                   // Pointer to destination array
                     SIGLIB_IMPULSE,          // Signal type - Impulse function
                     SIGLIB_ONE,              // Signal peak level
                     SIGLIB_FILL,             // Fill (overwrite) or add to existing array contents
                     SIGLIB_ZERO,             // Signal frequency - Unused
                     SIGLIB_ZERO,             // D.C. Offset
                     SIGLIB_ZERO,             // Delay (samples periods) applied to impulse
                     SIGLIB_ZERO,             // Signal end value - Unused
                     SIGLIB_NULL_DATA_PTR,    // Unused
                     SIGLIB_NULL_DATA_PTR,    // Unused
                     SAMPLE_LENGTH);          // Output dataset length

  // Apply comb filter and store filtered data
  for (SLArrayIndex_t i = 0; i < SAMPLE_LENGTH; i++) {
    *pSrc2++ = SDS_Comb(*pSrc1++,                  // Input data sample to be filtered
                        pCombFilterStateArray1,    // Pointer to filter state array
                        &combFilterIndex1,         // Pointer to filter index register
                        &combFilterSum1,           // Pointer to filter sum register
                        combFilterLength);         // Filter length
  }

  pSrc2 -= SAMPLE_LENGTH;    // Reset pointers
  pSrc1 -= SAMPLE_LENGTH;

  gpc_plot_2d(h2DPlot,                        // Graph handle
              pSrc2,                          // Dataset
              SAMPLE_LENGTH,                  // Dataset length
              "Impulse Response",             // Dataset title
              SIGLIB_ZERO,                    // Minimum X value
              (double)(SAMPLE_LENGTH - 1),    // Maximum X value
              "lines",                        // Graph type
              "blue",                         // Colour
              GPC_NEW);                       // New graph
  printf("\nImpulse Response\nPlease hit <Carriage Return> to continue . . .");
  getchar();

  // Perform real FFT
  SDA_Rfft(pSrc2,                      // Pointer to real array
           pImagData,                  // Pointer to imaginary array
           pFFTCoeffs,                 // Pointer to FFT coefficients
           SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit
                                       // reverse address table
           FFT_LENGTH,                 // FFT length
           LOG2_FFT_LENGTH);           // log2 FFT length

  SDA_RectangularToPolar(pSrc2,             // Pointer to source real array
                         pImagData,         // Pointer to source imaginary array
                         pMagnitude,        // Pointer to destination magnitude array
                         pPhase,            // Pointer to destination phase array
                         SAMPLE_LENGTH);    // Dataset length
  SDA_Multiply(pMagnitude,                  // Pointer to source array
               65536.,                      // Multiplier
               pMagnitude,                  // Pointer to destination array
               SAMPLE_LENGTH);              // Dataset length
  SDA_20Log10(pMagnitude,                   // Pointer to source array
              pMagnitude,                   // Pointer to destination array
              SAMPLE_LENGTH);               // Dataset length

  gpc_plot_2d(h2DPlot,                        // Graph handle
              pMagnitude,                     // Dataset
              SAMPLE_LENGTH,                  // Dataset length
              "Magnitude  (dB)",              // Dataset title
              SIGLIB_ZERO,                    // Minimum X value
              (double)(SAMPLE_LENGTH - 1),    // Maximum X value
              "lines",                        // Graph type
              "blue",                         // Colour
              GPC_NEW);                       // New graph
  printf("\nMagnitude Response\nPlease hit <Carriage Return> to continue . . .");
  getchar();

  gpc_plot_2d(h2DPlot,                        // Graph handle
              pPhase,                         // Dataset
              SAMPLE_LENGTH,                  // Dataset length
              "Phase",                        // Dataset title
              SIGLIB_ZERO,                    // Minimum X value
              (double)(SAMPLE_LENGTH - 1),    // Maximum X value
              "lines",                        // Graph type
              "red",                          // Colour
              GPC_NEW);                       // New graph
  printf("\nPhase Response\nPlease hit <Carriage Return> to continue . . .");
  getchar();

  // Comb filter type 3
  printf("\n\n\nComb filter type 3\n\n");
  printf("        |-----|      |-----|\n");
  printf("IN ---->|Comb1|----->|Comb2|-->|-----|\n");
  printf("    |   |-----|  |   |-----|   |     |\n");
  printf("    |            |             |     |\n");
  printf("    |            ------------->|  +  |----> OUT\n");
  printf("    |                          |     |\n");
  printf("    |                          |     |\n");
  printf("    -------------------------->|-----|\n");
  printf("\n Comb filter length => %d", combFilterLength);

  SIF_Comb(pCombFilterStateArray1,    // Pointer to filter delay state array
           &combFilterIndex1,         // Pointer to filter index register
           &combFilterSum1,           // Pointer to filter sum register
           combFilterLength);         // Filter length
  SIF_Comb(pCombFilterStateArray2,    // Pointer to filter delay state array
           &combFilterIndex2,         // Pointer to filter index register
           &combFilterSum2,           // Pointer to filter sum register
           combFilterLength);         // Filter length

  // Generate an impulse
  SDA_SignalGenerate(pSrc1,                   // Pointer to destination array
                     SIGLIB_IMPULSE,          // Signal type - Impulse function
                     SIGLIB_ONE,              // Signal peak level
                     SIGLIB_FILL,             // Fill (overwrite) or add to existing array contents
                     SIGLIB_ZERO,             // Signal frequency - Unused
                     SIGLIB_ZERO,             // D.C. Offset
                     SIGLIB_ZERO,             // Delay (samples periods) applied to impulse
                     SIGLIB_ZERO,             // Signal end value - Unused
                     SIGLIB_NULL_DATA_PTR,    // Unused
                     SIGLIB_NULL_DATA_PTR,    // Unused
                     SAMPLE_LENGTH);          // Output dataset length

  for (SLArrayIndex_t i = 0; i < SAMPLE_LENGTH; i++) {
    // Apply comb filter 1 and store filtered data
    *pSrc2 = SDS_Comb(*pSrc1,                    // Input data sample to be filtered
                      pCombFilterStateArray1,    // Pointer to filter state array
                      &combFilterIndex1,         // Pointer to filter index register
                      &combFilterSum1,           // Pointer to filter sum register
                      combFilterLength);         // Filter length

    // Apply comb filter 1 and store filtered data
    *pSrc3 = SDS_Comb(*pSrc2,                    // Input data sample to be filtered
                      pCombFilterStateArray2,    // Pointer to filter state array
                      &combFilterIndex2,         // Pointer to filter index register
                      &combFilterSum2,           // Pointer to filter sum register
                      combFilterLength);         // Filter length

    *pSrc3 = *pSrc3 + *pSrc1++;
    *pSrc3 = *pSrc3 + *pSrc2++;
    pSrc3++;
  }

  pSrc1 -= SAMPLE_LENGTH;
  pSrc2 -= SAMPLE_LENGTH;
  pSrc3 -= SAMPLE_LENGTH;

  SDA_Scale(pSrc3,             // Pointer to source array
            pSrc3,             // Pointer to destination array
            SIGLIB_ONE,        // Peak level
            SAMPLE_LENGTH);    // Dataset length

  gpc_plot_2d(h2DPlot,                        // Graph handle
              pSrc3,                          // Dataset
              SAMPLE_LENGTH,                  // Dataset length
              "Impulse Response",             // Dataset title
              SIGLIB_ZERO,                    // Minimum X value
              (double)(SAMPLE_LENGTH - 1),    // Maximum X value
              "lines",                        // Graph type
              "blue",                         // Colour
              GPC_NEW);                       // New graph
  printf("\nImpulse Response\nPlease hit <Carriage Return> to continue . . .");
  getchar();

  // Perform real FFT
  SDA_Rfft(pSrc3,                      // Pointer to real array
           pImagData,                  // Pointer to imaginary array
           pFFTCoeffs,                 // Pointer to FFT coefficients
           SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit
                                       // reverse address table
           FFT_LENGTH,                 // FFT length
           LOG2_FFT_LENGTH);           // log2 FFT length

  SDA_RectangularToPolar(pSrc3,             // Pointer to source real array
                         pImagData,         // Pointer to source imaginary array
                         pMagnitude,        // Pointer to destination magnitude array
                         pPhase,            // Pointer to destination phase array
                         SAMPLE_LENGTH);    // Dataset length
  SDA_Multiply(pMagnitude,                  // Pointer to source array
               65536.,                      // Multiplier
               pMagnitude,                  // Pointer to destination array
               SAMPLE_LENGTH);              // Dataset length
  SDA_20Log10(pMagnitude,                   // Pointer to source array
              pMagnitude,                   // Pointer to destination array
              SAMPLE_LENGTH);               // Dataset length

  gpc_plot_2d(h2DPlot,                        // Graph handle
              pMagnitude,                     // Dataset
              SAMPLE_LENGTH,                  // Dataset length
              "Magnitude  (dB)",              // Dataset title
              SIGLIB_ZERO,                    // Minimum X value
              (double)(SAMPLE_LENGTH - 1),    // Maximum X value
              "lines",                        // Graph type
              "blue",                         // Colour
              GPC_NEW);                       // New graph
  printf("\nMagnitude Response\nPlease hit <Carriage Return> to continue . . .");
  getchar();

  gpc_plot_2d(h2DPlot,                        // Graph handle
              pPhase,                         // Dataset
              SAMPLE_LENGTH,                  // Dataset length
              "Phase",                        // Dataset title
              SIGLIB_ZERO,                    // Minimum X value
              (double)(SAMPLE_LENGTH - 1),    // Maximum X value
              "lines",                        // Graph type
              "red",                          // Colour
              GPC_NEW);                       // New graph
  printf("\nPhase Response\nPlease hit <Carriage Return> to continue . . .");
  getchar();

  // Comb filter type 4
  printf("\n\n\nComb filter type 4\n");
  printf("        |------|      |------|\n");
  printf("IN ---->|Delay1|----->|Delay2|-->|-----|\n");
  printf("    |   |------|  |   |------|   |     |\n");
  printf("    |             |              |     |\n");
  printf("    |             -------------->|  +  |----> OUT\n");
  printf("    |                            |     |\n");
  printf("    |                            |     |\n");
  printf("    ---------------------------->|-----|\n");
  printf("\n Comb filter length => %d", combFilterLength);

  // Initialize delays
  SIF_FixedDelay(pCombFilterStateArray1,    // Pointer to delay state array
                 &combFilterIndex1,         // Pointer to delay state index
                 combFilterLength);         // Delay length
  SIF_FixedDelay(pCombFilterStateArray2,    // Pointer to delay state array
                 &combFilterIndex2,         // Pointer to delay state index
                 combFilterLength);         // Delay length

  K1 = SIGLIB_QUARTER;
  K2 = SIGLIB_MINUS_HALF;
  K3 = SIGLIB_QUARTER;

  //    K1 = SIGLIB_HALF;
  //    K2 = SIGLIB_HALF;
  //    K3 = SIGLIB_ZERO;

  // Generate an impulse
  SDA_SignalGenerate(pSrc1,                   // Pointer to destination array
                     SIGLIB_IMPULSE,          // Signal type - Impulse function
                     SIGLIB_ONE,              // Signal peak level
                     SIGLIB_FILL,             // Fill (overwrite) or add to existing array contents
                     SIGLIB_ZERO,             // Signal frequency - Unused
                     SIGLIB_ZERO,             // D.C. Offset
                     SIGLIB_ZERO,             // Delay (samples periods) applied to impulse
                     SIGLIB_ZERO,             // Signal end value - Unused
                     SIGLIB_NULL_DATA_PTR,    // Unused
                     SIGLIB_NULL_DATA_PTR,    // Unused
                     SAMPLE_LENGTH);          // Output dataset length

  for (SLArrayIndex_t i = 0; i < SAMPLE_LENGTH; i++) {
    // Apply comb filter 1 and store filtered data
    *pSrc2 = SDS_FixedDelay(*pSrc1,                    // Input data sample
                            pCombFilterStateArray1,    // Pointer to delay state array
                            &combFilterIndex1,         // Pointer to delay state index
                            combFilterLength);         // Delay length

    // Apply comb filter 1 and store filtered data
    *pSrc3 = K3 * SDS_FixedDelay(*pSrc2,                    // Input data sample
                                 pCombFilterStateArray2,    // Pointer to delay state array
                                 &combFilterIndex2,         // Pointer to delay state index
                                 combFilterLength);         // Delay length

    *pSrc3 = *pSrc3 + (*pSrc1++ * K1);
    *pSrc3 = *pSrc3 + (*pSrc2++ * K2);
    pSrc3++;
  }

  pSrc1 -= SAMPLE_LENGTH;
  pSrc2 -= SAMPLE_LENGTH;
  pSrc3 -= SAMPLE_LENGTH;

  SDA_Scale(pSrc3,             // Pointer to source array
            pSrc3,             // Pointer to destination array
            SIGLIB_ONE,        // Peak level
            SAMPLE_LENGTH);    // Dataset length

  gpc_plot_2d(h2DPlot,                        // Graph handle
              pSrc3,                          // Dataset
              SAMPLE_LENGTH,                  // Dataset length
              "Impulse Response",             // Dataset title
              SIGLIB_ZERO,                    // Minimum X value
              (double)(SAMPLE_LENGTH - 1),    // Maximum X value
              "lines",                        // Graph type
              "blue",                         // Colour
              GPC_NEW);                       // New graph
  printf("\nImpulse Response\nPlease hit <Carriage Return> to continue . . .");
  getchar();

  // Perform real FFT
  SDA_Rfft(pSrc3,                      // Pointer to real array
           pImagData,                  // Pointer to imaginary array
           pFFTCoeffs,                 // Pointer to FFT coefficients
           SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit
                                       // reverse address table
           FFT_LENGTH,                 // FFT length
           LOG2_FFT_LENGTH);           // log2 FFT length

  SDA_RectangularToPolar(pSrc3,             // Pointer to source real array
                         pImagData,         // Pointer to source imaginary array
                         pMagnitude,        // Pointer to destination magnitude array
                         pPhase,            // Pointer to destination phase array
                         SAMPLE_LENGTH);    // Dataset length
  SDA_Multiply(pMagnitude,                  // Pointer to source array
               65536.,                      // Multiplier
               pMagnitude,                  // Pointer to destination array
               SAMPLE_LENGTH);              // Dataset length
  SDA_20Log10(pMagnitude,                   // Pointer to source array
              pMagnitude,                   // Pointer to destination array
              SAMPLE_LENGTH);               // Dataset length

  gpc_plot_2d(h2DPlot,                        // Graph handle
              pMagnitude,                     // Dataset
              SAMPLE_LENGTH,                  // Dataset length
              "Magnitude  (dB)",              // Dataset title
              SIGLIB_ZERO,                    // Minimum X value
              (double)(SAMPLE_LENGTH - 1),    // Maximum X value
              "lines",                        // Graph type
              "blue",                         // Colour
              GPC_NEW);                       // New graph
  printf("\nMagnitude Response\nPlease hit <Carriage Return> to continue . . .");
  getchar();

  gpc_plot_2d(h2DPlot,                        // Graph handle
              pPhase,                         // Dataset
              SAMPLE_LENGTH,                  // Dataset length
              "Phase",                        // Dataset title
              SIGLIB_ZERO,                    // Minimum X value
              (double)(SAMPLE_LENGTH - 1),    // Maximum X value
              "lines",                        // Graph type
              "red",                          // Colour
              GPC_NEW);                       // New graph
  printf("\nPhase Response\n");

  printf("\nHit <Carriage Return> to continue ....\n");
  getchar();    // Wait for <Carriage Return>
  gpc_close(h2DPlot);

  SUF_MemoryFree(pImagData);    // Free all memory
  SUF_MemoryFree(pMagnitude);
  SUF_MemoryFree(pPhase);
  SUF_MemoryFree(pSrc1);
  SUF_MemoryFree(pSrc2);
  SUF_MemoryFree(pSrc3);
  SUF_MemoryFree(pCombFilterStateArray1);
  SUF_MemoryFree(pCombFilterStateArray2);
  SUF_MemoryFree(pFFTCoeffs);

  return (0);
}
