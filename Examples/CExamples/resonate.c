// SigLib Resonator Example
// Copyright (c) 2024 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>       // SigLib DSP library
#include <gnuplot_c.h>    // Gnuplot/C

// Define constants
#define SAMPLE_LENGTH 256
#define FUNDAMENTAL_FREQ 0.02    // Fundamental frequency

// Declare global variables and arrays
static SLData_t ResonatorDelay[SIGLIB_RESONATOR_DELAY_LENGTH];
static SLData_t pSrc[SAMPLE_LENGTH], pDst[SAMPLE_LENGTH];

int main(void)
{
  h_GPC_Plot* h2DPlot;    // Plot object

  SLFixData_t FirstTimeFlag;
  SLData_t CosCoeff, SinCoeff;

  h2DPlot =                           // Initialize plot
      gpc_init_2d("Resonator",        // Plot title
                  "Time",             // X-Axis label
                  "Magnitude",        // Y-Axis label
                  GPC_AUTO_SCALE,     // Scaling mode
                  GPC_SIGNED,         // Sign mode
                  GPC_KEY_ENABLE);    // Legend / key mode
  if (NULL == h2DPlot) {
    printf("\nPlot creation failure.\n");
    exit(-1);
  }

  SDA_Impulse(pSrc,              // Pointer to destination array
              SAMPLE_LENGTH);    // Output dataset length

  gpc_plot_2d(h2DPlot,                        // Graph handle
              pSrc,                           // Dataset
              SAMPLE_LENGTH,                  // Dataset length
              "Impulse Input",                // Dataset title
              SIGLIB_ZERO,                    // Minimum X value
              (double)(SAMPLE_LENGTH - 1),    // Maximum X value
              "lines",                        // Graph type
              "blue",                         // Colour
              GPC_NEW);                       // New graph
  printf("\nImpulse Input\nPlease hit <Carriage Return> to continue . . .");
  getchar();

  SIF_Resonator(ResonatorDelay,      // Pointer to state array
                FUNDAMENTAL_FREQ,    // Resonant frequency
                &CosCoeff,           // Pointer to cosine coefficient
                &SinCoeff);          // Pointer to sine coefficient

  //  printf ("Cos Coeff = %lf, Sin  Coeff = %lf\n", CosCoeff, SinCoeff);

  SDA_Resonator(pSrc,              // Pointer to source array
                pDst,              // Pointer to destination array
                ResonatorDelay,    // Pointer to state array
                CosCoeff,          // Cosine coefficient
                SinCoeff,          // Sine coefficient
                SAMPLE_LENGTH);    // Dataset length

  gpc_plot_2d(h2DPlot,                        // Graph handle
              pDst,                           // Dataset
              SAMPLE_LENGTH,                  // Dataset length
              "Resonator Output",             // Dataset title
              SIGLIB_ZERO,                    // Minimum X value
              (double)(SAMPLE_LENGTH - 1),    // Maximum X value
              "lines",                        // Graph type
              "blue",                         // Colour
              GPC_NEW);                       // New graph
  printf("\nResonator Output, Normalized Frequency = %lf\nPlease hit <Carriage "
         "Return> to continue . . .",
         FUNDAMENTAL_FREQ);
  getchar();

  SDA_Zeros(pSrc,              // Pointer to destination array
            SAMPLE_LENGTH);    // Dataset length

  SDA_Resonator(pSrc,              // Pointer to source array
                pDst,              // Pointer to destination array
                ResonatorDelay,    // Pointer to state array
                CosCoeff,          // Cosine coefficient
                SinCoeff,          // Sine coefficient
                SAMPLE_LENGTH);    // Dataset length

  gpc_plot_2d(h2DPlot,                        // Graph handle
              pDst,                           // Dataset
              SAMPLE_LENGTH,                  // Dataset length
              "Resonator Output",             // Dataset title
              SIGLIB_ZERO,                    // Minimum X value
              (double)(SAMPLE_LENGTH - 1),    // Maximum X value
              "lines",                        // Graph type
              "blue",                         // Colour
              GPC_NEW);                       // New graph
  printf("\nResonator Output, Normalized Frequency = %lf\nPlease hit <Carriage "
         "Return> to continue . . .",
         FUNDAMENTAL_FREQ);
  getchar();

  SIF_Resonator1(ResonatorDelay,                   // Pointer to state array
                 FUNDAMENTAL_FREQ * SIGLIB_TWO,    // Resonant frequency
                 &CosCoeff,                        // Pointer to cosine coefficient
                 &SinCoeff,                        // Pointer to sine coefficient
                 &FirstTimeFlag);                  // First iteration flag

  SDA_Resonator1(pDst,              // Pointer to destination array
                 SIGLIB_ONE,        // Output signal magnitude
                 ResonatorDelay,    // Pointer to state array
                 &FirstTimeFlag,    // First iteration flag
                 CosCoeff,          // Cosine coefficient
                 SinCoeff,          // Sine coefficient
                 SAMPLE_LENGTH);    // Dataset length

  gpc_plot_2d(h2DPlot,                          // Graph handle
              pDst,                             // Dataset
              SAMPLE_LENGTH,                    // Dataset length
              "Standalone Resonator Output",    // Dataset title
              SIGLIB_ZERO,                      // Minimum X value
              (double)(SAMPLE_LENGTH - 1),      // Maximum X value
              "lines",                          // Graph type
              "blue",                           // Colour
              GPC_NEW);                         // New graph
  printf("\nStandalone Resonator Output, Normalized Frequency = %lf\nPlease "
         "hit <Carriage Return> to continue . . .",
         FUNDAMENTAL_FREQ * SIGLIB_TWO);
  getchar();

  SDA_Resonator1(pDst,              // Pointer to destination array
                 SIGLIB_ONE,        // Output signal magnitude
                 ResonatorDelay,    // Pointer to state array
                 &FirstTimeFlag,    // First iteration flag
                 CosCoeff,          // Cosine coefficient
                 SinCoeff,          // Sine coefficient
                 SAMPLE_LENGTH);    // Dataset length

  gpc_plot_2d(h2DPlot,                          // Graph handle
              pDst,                             // Dataset
              SAMPLE_LENGTH,                    // Dataset length
              "Standalone Resonator Output",    // Dataset title
              SIGLIB_ZERO,                      // Minimum X value
              (double)(SAMPLE_LENGTH - 1),      // Maximum X value
              "lines",                          // Graph type
              "blue",                           // Colour
              GPC_NEW);                         // New graph
  printf("\nStandalone Resonator Output, Normalized Frequency = %lf\nPlease "
         "hit <Carriage Return> to continue . . .",
         FUNDAMENTAL_FREQ * SIGLIB_TWO);
  getchar();

  SDA_Zeros(pDst,              // Pointer to destination array
            SAMPLE_LENGTH);    // Dataset length

  SIF_Resonator1(ResonatorDelay,                    // Pointer to state array
                 FUNDAMENTAL_FREQ * SIGLIB_FOUR,    // Resonant frequency
                 &CosCoeff,                         // Pointer to cosine coefficient
                 &SinCoeff,                         // Pointer to sine coefficient
                 &FirstTimeFlag);                   // First iteration flag

  SDA_Resonator1Add(pDst,              // Pointer to destination array
                    SIGLIB_ONE,        // Output signal magnitude
                    ResonatorDelay,    // Pointer to state array
                    &FirstTimeFlag,    // First iteration flag
                    CosCoeff,          // Cosine coefficient
                    SinCoeff,          // Sine coefficient
                    SAMPLE_LENGTH);    // Dataset length

  gpc_plot_2d(h2DPlot,                              // Graph handle
              pDst,                                 // Dataset
              SAMPLE_LENGTH,                        // Dataset length
              "Standalone Resonator Add Output",    // Dataset title
              SIGLIB_ZERO,                          // Minimum X value
              (double)(SAMPLE_LENGTH - 1),          // Maximum X value
              "lines",                              // Graph type
              "blue",                               // Colour
              GPC_NEW);                             // New graph
  printf("\nStandalone Resonator Add Output, Normalized Frequency = %lf\n", FUNDAMENTAL_FREQ * SIGLIB_FOUR);

  printf("\nHit <Carriage Return> to continue ....\n");
  getchar();    // Wait for <Carriage Return>
  gpc_close(h2DPlot);

  return (0);
}
