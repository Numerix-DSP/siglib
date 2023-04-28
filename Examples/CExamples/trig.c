// SigLib Fast Sin, Cos And Sincos Example
// Copyright (c) 2023 Alpha Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>                                                 // SigLib DSP library
#include <gnuplot_c.h>                                              // Gnuplot/C

// Define constants
#define SAMPLE_RATE                 10000.
#define CARRIER_FREQUENCY           500.
#define CARRIER_TABLE_LENGTH        1000
#define SAMPLE_LENGTH               32
#define SINE_LUT_LENGTH             500
#define COSINE_LUT_LENGTH           500
#define SINC_LUT_LENGTH             1001
#define SCALE_FACTOR                4.                              // Provides a range of input numbers to process
#define MAX_SINC_INPUT_MAGNITUDE    ((11. * SIGLIB_PI) / SCALE_FACTOR)

// Declare global variables and arrays
static SLData_t *pSinOutput, *pCosOutput, *pSinCosLUT;
static SLData_t *pSineLUT, *pCosineLUT, *pSincLUT;
static SLData_t LookUpTablePhase;
static SLData_t LookUpTablePhaseGain;


int main (
  void)
{
  h_GPC_Plot     *h2DPlot;                                          // Plot object

  SLArrayIndex_t  i;

  pSinOutput = SUF_VectorArrayAllocate (SAMPLE_LENGTH);
  pCosOutput = SUF_VectorArrayAllocate (SAMPLE_LENGTH);
  pSinCosLUT = SUF_FastSinCosArrayAllocate (CARRIER_TABLE_LENGTH);
  pSineLUT = SUF_VectorArrayAllocate (SINE_LUT_LENGTH);
  pCosineLUT = SUF_VectorArrayAllocate (COSINE_LUT_LENGTH);
  pSincLUT = SUF_VectorArrayAllocate (SINC_LUT_LENGTH);

  SIF_QuickSin (pSineLUT,                                           // Pointer to LUT array
                &LookUpTablePhaseGain,                              // Pointer to phase gain
                SINE_LUT_LENGTH);                                   // Table length

// SDS_QuickSin Parameters - (Angle, Sine table pointer, Phase gain)
  printf ("Fast sin and cos function tests.\n");
  printf ("x = %1.3lf, sin(x) = %1.3lf, SDS_QuickSin(x) = %1.3lf\n", (-4. * SIGLIB_PI) / SCALE_FACTOR, SDS_Sin ((-4. * SIGLIB_PI) / SCALE_FACTOR),
          SDS_QuickSin ((-4. * SIGLIB_PI) / SCALE_FACTOR, pSineLUT, LookUpTablePhaseGain));
  printf ("x = %1.3lf, sin(x) = %1.3lf, SDS_QuickSin(x) = %1.3lf\n", (-3. * SIGLIB_PI) / SCALE_FACTOR, SDS_Sin ((-3. * SIGLIB_PI) / SCALE_FACTOR),
          SDS_QuickSin ((-3. * SIGLIB_PI) / SCALE_FACTOR, pSineLUT, LookUpTablePhaseGain));
  printf ("x = %1.3lf, sin(x) = %1.3lf, SDS_QuickSin(x) = %1.3lf\n", (-2. * SIGLIB_PI) / SCALE_FACTOR, SDS_Sin ((-2. * SIGLIB_PI) / SCALE_FACTOR),
          SDS_QuickSin ((-2. * SIGLIB_PI) / SCALE_FACTOR, pSineLUT, LookUpTablePhaseGain));
  printf ("x = %1.3lf, sin(x) = %1.3lf, SDS_QuickSin(x) = %1.3lf\n", (-1. * SIGLIB_PI) / SCALE_FACTOR, SDS_Sin ((-1. * SIGLIB_PI) / SCALE_FACTOR),
          SDS_QuickSin ((-1. * SIGLIB_PI) / SCALE_FACTOR, pSineLUT, LookUpTablePhaseGain));
  printf ("x = %1.3lf, sin(x) = %1.3lf, SDS_QuickSin(x) = %1.3lf\n", (0. * SIGLIB_PI) / SCALE_FACTOR, SDS_Sin ((0. * SIGLIB_PI) / SCALE_FACTOR),
          SDS_QuickSin ((0. * SIGLIB_PI) / SCALE_FACTOR, pSineLUT, LookUpTablePhaseGain));
  printf ("x = %1.3lf, sin(x) = %1.3lf, SDS_QuickSin(x) = %1.3lf\n", (1. * SIGLIB_PI) / SCALE_FACTOR, SDS_Sin ((1. * SIGLIB_PI) / SCALE_FACTOR),
          SDS_QuickSin ((1. * SIGLIB_PI) / SCALE_FACTOR, pSineLUT, LookUpTablePhaseGain));
  printf ("x = %1.3lf, sin(x) = %1.3lf, SDS_QuickSin(x) = %1.3lf\n", (2. * SIGLIB_PI) / SCALE_FACTOR, SDS_Sin ((2. * SIGLIB_PI) / SCALE_FACTOR),
          SDS_QuickSin ((2. * SIGLIB_PI) / SCALE_FACTOR, pSineLUT, LookUpTablePhaseGain));
  printf ("x = %1.3lf, sin(x) = %1.3lf, SDS_QuickSin(x) = %1.3lf\n", (3. * SIGLIB_PI) / SCALE_FACTOR, SDS_Sin ((3. * SIGLIB_PI) / SCALE_FACTOR),
          SDS_QuickSin ((3. * SIGLIB_PI) / SCALE_FACTOR, pSineLUT, LookUpTablePhaseGain));
  printf ("x = %1.3lf, sin(x) = %1.3lf, SDS_QuickSin(x) = %1.3lf\n", (4. * SIGLIB_PI) / SCALE_FACTOR, SDS_Sin ((4. * SIGLIB_PI) / SCALE_FACTOR),
          SDS_QuickSin ((4. * SIGLIB_PI) / SCALE_FACTOR, pSineLUT, LookUpTablePhaseGain));
  printf ("x = %1.3lf, sin(x) = %1.3lf, SDS_QuickSin(x) = %1.3lf\n", (5. * SIGLIB_PI) / SCALE_FACTOR, SDS_Sin ((5. * SIGLIB_PI) / SCALE_FACTOR),
          SDS_QuickSin ((5. * SIGLIB_PI) / SCALE_FACTOR, pSineLUT, LookUpTablePhaseGain));
  printf ("x = %1.3lf, sin(x) = %1.3lf, SDS_QuickSin(x) = %1.3lf\n", (6. * SIGLIB_PI) / SCALE_FACTOR, SDS_Sin ((6. * SIGLIB_PI) / SCALE_FACTOR),
          SDS_QuickSin ((6. * SIGLIB_PI) / SCALE_FACTOR, pSineLUT, LookUpTablePhaseGain));
  printf ("x = %1.3lf, sin(x) = %1.3lf, SDS_QuickSin(x) = %1.3lf\n", (7. * SIGLIB_PI) / SCALE_FACTOR, SDS_Sin ((7. * SIGLIB_PI) / SCALE_FACTOR),
          SDS_QuickSin ((7. * SIGLIB_PI) / SCALE_FACTOR, pSineLUT, LookUpTablePhaseGain));
  printf ("x = %1.3lf, sin(x) = %1.3lf, SDS_QuickSin(x) = %1.3lf\n", (8. * SIGLIB_PI) / SCALE_FACTOR, SDS_Sin ((8. * SIGLIB_PI) / SCALE_FACTOR),
          SDS_QuickSin ((8. * SIGLIB_PI) / SCALE_FACTOR, pSineLUT, LookUpTablePhaseGain));
  printf ("x = %1.3lf, sin(x) = %1.3lf, SDS_QuickSin(x) = %1.3lf\n", (9. * SIGLIB_PI) / SCALE_FACTOR, SDS_Sin ((9. * SIGLIB_PI) / SCALE_FACTOR),
          SDS_QuickSin ((9. * SIGLIB_PI) / SCALE_FACTOR, pSineLUT, LookUpTablePhaseGain));
  printf ("x = %1.3lf, sin(x) = %1.3lf, SDS_QuickSin(x) = %1.3lf\n", (10. * SIGLIB_PI) / SCALE_FACTOR, SDS_Sin ((10. * SIGLIB_PI) / SCALE_FACTOR),
          SDS_QuickSin ((10. * SIGLIB_PI) / SCALE_FACTOR, pSineLUT, LookUpTablePhaseGain));
  printf ("Please hit <Carriage Return> to continue . . .\n");
  getchar ();

  SIF_QuickCos (pCosineLUT,                                         // Pointer to LUT array
                &LookUpTablePhaseGain,                              // Pointer to phase gain
                COSINE_LUT_LENGTH);                                 // Table length

// SDS_QuickCos Parameters - (Angle, Sine table pointer, Phase gain)
  printf ("x = %1.3lf, cos(x) = %1.3lf, SDS_QuickCos(x) = %1.3lf\n", (-4. * SIGLIB_PI) / SCALE_FACTOR, SDS_Cos ((-4. * SIGLIB_PI) / SCALE_FACTOR),
          SDS_QuickCos ((-4. * SIGLIB_PI) / SCALE_FACTOR, pCosineLUT, LookUpTablePhaseGain));
  printf ("x = %1.3lf, cos(x) = %1.3lf, SDS_QuickCos(x) = %1.3lf\n", (-3. * SIGLIB_PI) / SCALE_FACTOR, SDS_Cos ((-3. * SIGLIB_PI) / SCALE_FACTOR),
          SDS_QuickCos ((-3. * SIGLIB_PI) / SCALE_FACTOR, pCosineLUT, LookUpTablePhaseGain));
  printf ("x = %1.3lf, cos(x) = %1.3lf, SDS_QuickCos(x) = %1.3lf\n", (-2. * SIGLIB_PI) / SCALE_FACTOR, SDS_Cos ((-2. * SIGLIB_PI) / SCALE_FACTOR),
          SDS_QuickCos ((-2. * SIGLIB_PI) / SCALE_FACTOR, pCosineLUT, LookUpTablePhaseGain));
  printf ("x = %1.3lf, cos(x) = %1.3lf, SDS_QuickCos(x) = %1.3lf\n", (-1. * SIGLIB_PI) / SCALE_FACTOR, SDS_Cos ((-1. * SIGLIB_PI) / SCALE_FACTOR),
          SDS_QuickCos ((-1. * SIGLIB_PI) / SCALE_FACTOR, pCosineLUT, LookUpTablePhaseGain));
  printf ("x = %1.3lf, cos(x) = %1.3lf, SDS_QuickCos(x) = %1.3lf\n", (0. * SIGLIB_PI) / SCALE_FACTOR, SDS_Cos ((0. * SIGLIB_PI) / SCALE_FACTOR),
          SDS_QuickCos ((0. * SIGLIB_PI) / SCALE_FACTOR, pCosineLUT, LookUpTablePhaseGain));
  printf ("x = %1.3lf, cos(x) = %1.3lf, SDS_QuickCos(x) = %1.3lf\n", (1. * SIGLIB_PI) / SCALE_FACTOR, SDS_Cos ((1. * SIGLIB_PI) / SCALE_FACTOR),
          SDS_QuickCos ((1. * SIGLIB_PI) / SCALE_FACTOR, pCosineLUT, LookUpTablePhaseGain));
  printf ("x = %1.3lf, cos(x) = %1.3lf, SDS_QuickCos(x) = %1.3lf\n", (2. * SIGLIB_PI) / SCALE_FACTOR, SDS_Cos ((2. * SIGLIB_PI) / SCALE_FACTOR),
          SDS_QuickCos ((2. * SIGLIB_PI) / SCALE_FACTOR, pCosineLUT, LookUpTablePhaseGain));
  printf ("x = %1.3lf, cos(x) = %1.3lf, SDS_QuickCos(x) = %1.3lf\n", (3. * SIGLIB_PI) / SCALE_FACTOR, SDS_Cos ((3. * SIGLIB_PI) / SCALE_FACTOR),
          SDS_QuickCos ((3. * SIGLIB_PI) / SCALE_FACTOR, pCosineLUT, LookUpTablePhaseGain));
  printf ("x = %1.3lf, cos(x) = %1.3lf, SDS_QuickCos(x) = %1.3lf\n", (4. * SIGLIB_PI) / SCALE_FACTOR, SDS_Cos ((4. * SIGLIB_PI) / SCALE_FACTOR),
          SDS_QuickCos ((4. * SIGLIB_PI) / SCALE_FACTOR, pCosineLUT, LookUpTablePhaseGain));
  printf ("x = %1.3lf, cos(x) = %1.3lf, SDS_QuickCos(x) = %1.3lf\n", (5. * SIGLIB_PI) / SCALE_FACTOR, SDS_Cos ((5. * SIGLIB_PI) / SCALE_FACTOR),
          SDS_QuickCos ((5. * SIGLIB_PI) / SCALE_FACTOR, pCosineLUT, LookUpTablePhaseGain));
  printf ("x = %1.3lf, cos(x) = %1.3lf, SDS_QuickCos(x) = %1.3lf\n", (6. * SIGLIB_PI) / SCALE_FACTOR, SDS_Cos ((6. * SIGLIB_PI) / SCALE_FACTOR),
          SDS_QuickCos ((6. * SIGLIB_PI) / SCALE_FACTOR, pCosineLUT, LookUpTablePhaseGain));
  printf ("x = %1.3lf, cos(x) = %1.3lf, SDS_QuickCos(x) = %1.3lf\n", (7. * SIGLIB_PI) / SCALE_FACTOR, SDS_Cos ((7. * SIGLIB_PI) / SCALE_FACTOR),
          SDS_QuickCos ((7. * SIGLIB_PI) / SCALE_FACTOR, pCosineLUT, LookUpTablePhaseGain));
  printf ("x = %1.3lf, cos(x) = %1.3lf, SDS_QuickCos(x) = %1.3lf\n", (8. * SIGLIB_PI) / SCALE_FACTOR, SDS_Cos ((8. * SIGLIB_PI) / SCALE_FACTOR),
          SDS_QuickCos ((8. * SIGLIB_PI) / SCALE_FACTOR, pCosineLUT, LookUpTablePhaseGain));
  printf ("x = %1.3lf, cos(x) = %1.3lf, SDS_QuickCos(x) = %1.3lf\n", (9. * SIGLIB_PI) / SCALE_FACTOR, SDS_Cos ((9. * SIGLIB_PI) / SCALE_FACTOR),
          SDS_QuickCos ((9. * SIGLIB_PI) / SCALE_FACTOR, pCosineLUT, LookUpTablePhaseGain));
  printf ("x = %1.3lf, cos(x) = %1.3lf, SDS_QuickCos(x) = %1.3lf\n", (10. * SIGLIB_PI) / SCALE_FACTOR, SDS_Cos ((10. * SIGLIB_PI) / SCALE_FACTOR),
          SDS_QuickCos ((10. * SIGLIB_PI) / SCALE_FACTOR, pCosineLUT, LookUpTablePhaseGain));
  printf ("Please hit <Carriage Return> to continue . . .\n");
  getchar ();

  SIF_QuickSinc (pSincLUT,                                          // Pointer to Sinc look up table
                 &LookUpTablePhaseGain,                             // Pointer to phase gain
                 MAX_SINC_INPUT_MAGNITUDE,                          // Maximum input 'x' value
                 SINC_LUT_LENGTH);                                  // Look up table length

// SDS_QuickSinc Parameters - (Angle, Sine table pointer, Phase gain)
  printf ("x = %1.3lf, SDS_Sinc(x) = %1.3lf, SDS_QuickSinc(x) = %1.3lf\n", (-4. * SIGLIB_PI) / SCALE_FACTOR,
          SDS_Sinc ((-4. * SIGLIB_PI) / SCALE_FACTOR), SDS_QuickSinc ((-4. * SIGLIB_PI) / SCALE_FACTOR, pSincLUT, LookUpTablePhaseGain));
  printf ("x = %1.3lf, SDS_Sinc(x) = %1.3lf, SDS_QuickSinc(x) = %1.3lf\n", (-3. * SIGLIB_PI) / SCALE_FACTOR,
          SDS_Sinc ((-3. * SIGLIB_PI) / SCALE_FACTOR), SDS_QuickSinc ((-3. * SIGLIB_PI) / SCALE_FACTOR, pSincLUT, LookUpTablePhaseGain));
  printf ("x = %1.3lf, SDS_Sinc(x) = %1.3lf, SDS_QuickSinc(x) = %1.3lf\n", (-2. * SIGLIB_PI) / SCALE_FACTOR,
          SDS_Sinc ((-2. * SIGLIB_PI) / SCALE_FACTOR), SDS_QuickSinc ((-2. * SIGLIB_PI) / SCALE_FACTOR, pSincLUT, LookUpTablePhaseGain));
  printf ("x = %1.3lf, SDS_Sinc(x) = %1.3lf, SDS_QuickSinc(x) = %1.3lf\n", (-1. * SIGLIB_PI) / SCALE_FACTOR,
          SDS_Sinc ((-1. * SIGLIB_PI) / SCALE_FACTOR), SDS_QuickSinc ((-1. * SIGLIB_PI) / SCALE_FACTOR, pSincLUT, LookUpTablePhaseGain));
  printf ("x = %1.3lf, SDS_Sinc(x) = %1.3lf, SDS_QuickSinc(x) = %1.3lf\n", (0. * SIGLIB_PI) / SCALE_FACTOR,
          SDS_Sinc ((0. * SIGLIB_PI) / SCALE_FACTOR), SDS_QuickSinc ((0. * SIGLIB_PI) / SCALE_FACTOR, pSincLUT, LookUpTablePhaseGain));
  printf ("x = %1.3lf, SDS_Sinc(x) = %1.3lf, SDS_QuickSinc(x) = %1.3lf\n", (1. * SIGLIB_PI) / SCALE_FACTOR,
          SDS_Sinc ((1. * SIGLIB_PI) / SCALE_FACTOR), SDS_QuickSinc ((1. * SIGLIB_PI) / SCALE_FACTOR, pSincLUT, LookUpTablePhaseGain));
  printf ("x = %1.3lf, SDS_Sinc(x) = %1.3lf, SDS_QuickSinc(x) = %1.3lf\n", (2. * SIGLIB_PI) / SCALE_FACTOR,
          SDS_Sinc ((2. * SIGLIB_PI) / SCALE_FACTOR), SDS_QuickSinc ((2. * SIGLIB_PI) / SCALE_FACTOR, pSincLUT, LookUpTablePhaseGain));
  printf ("x = %1.3lf, SDS_Sinc(x) = %1.3lf, SDS_QuickSinc(x) = %1.3lf\n", (3. * SIGLIB_PI) / SCALE_FACTOR,
          SDS_Sinc ((3. * SIGLIB_PI) / SCALE_FACTOR), SDS_QuickSinc ((3. * SIGLIB_PI) / SCALE_FACTOR, pSincLUT, LookUpTablePhaseGain));
  printf ("x = %1.3lf, SDS_Sinc(x) = %1.3lf, SDS_QuickSinc(x) = %1.3lf\n", (4. * SIGLIB_PI) / SCALE_FACTOR,
          SDS_Sinc ((4. * SIGLIB_PI) / SCALE_FACTOR), SDS_QuickSinc ((4. * SIGLIB_PI) / SCALE_FACTOR, pSincLUT, LookUpTablePhaseGain));
  printf ("x = %1.3lf, SDS_Sinc(x) = %1.3lf, SDS_QuickSinc(x) = %1.3lf\n", (5. * SIGLIB_PI) / SCALE_FACTOR,
          SDS_Sinc ((5. * SIGLIB_PI) / SCALE_FACTOR), SDS_QuickSinc ((5. * SIGLIB_PI) / SCALE_FACTOR, pSincLUT, LookUpTablePhaseGain));
  printf ("x = %1.3lf, SDS_Sinc(x) = %1.3lf, SDS_QuickSinc(x) = %1.3lf\n", (6. * SIGLIB_PI) / SCALE_FACTOR,
          SDS_Sinc ((6. * SIGLIB_PI) / SCALE_FACTOR), SDS_QuickSinc ((6. * SIGLIB_PI) / SCALE_FACTOR, pSincLUT, LookUpTablePhaseGain));
  printf ("x = %1.3lf, SDS_Sinc(x) = %1.3lf, SDS_QuickSinc(x) = %1.3lf\n", (7. * SIGLIB_PI) / SCALE_FACTOR,
          SDS_Sinc ((7. * SIGLIB_PI) / SCALE_FACTOR), SDS_QuickSinc ((7. * SIGLIB_PI) / SCALE_FACTOR, pSincLUT, LookUpTablePhaseGain));
  printf ("x = %1.3lf, SDS_Sinc(x) = %1.3lf, SDS_QuickSinc(x) = %1.3lf\n", (8. * SIGLIB_PI) / SCALE_FACTOR,
          SDS_Sinc ((8. * SIGLIB_PI) / SCALE_FACTOR), SDS_QuickSinc ((8. * SIGLIB_PI) / SCALE_FACTOR, pSincLUT, LookUpTablePhaseGain));
  printf ("x = %1.3lf, SDS_Sinc(x) = %1.3lf, SDS_QuickSinc(x) = %1.3lf\n", (9. * SIGLIB_PI) / SCALE_FACTOR,
          SDS_Sinc ((9. * SIGLIB_PI) / SCALE_FACTOR), SDS_QuickSinc ((9. * SIGLIB_PI) / SCALE_FACTOR, pSincLUT, LookUpTablePhaseGain));
  printf ("x = %1.3lf, SDS_Sinc(x) = %1.3lf, SDS_QuickSinc(x) = %1.3lf\n", (10. * SIGLIB_PI) / SCALE_FACTOR,
          SDS_Sinc ((10. * SIGLIB_PI) / SCALE_FACTOR), SDS_QuickSinc ((10. * SIGLIB_PI) / SCALE_FACTOR, pSincLUT, LookUpTablePhaseGain));
  printf ("Please hit <Carriage Return> to continue . . .\n");
  getchar ();


  h2DPlot =                                                         // Initialize plot
    gpc_init_2d ("Fast Sin, Cos And Sincos",                        // Plot title
                 "Time",                                            // X-Axis label
                 "Magnitude",                                       // Y-Axis label
                 GPC_AUTO_SCALE,                                    // Scaling mode
                 GPC_SIGNED,                                        // Sign mode
                 GPC_KEY_ENABLE);                                   // Legend / key mode
  if (NULL == h2DPlot) {
    printf ("\nPlot creation failure.\n");
    exit (-1);
  }


  SIF_FastSin (pSinCosLUT,                                          // Pointer to LUT array
               CARRIER_TABLE_LENGTH);                               // Table length

// Generate sinusoid
  LookUpTablePhase = SIGLIB_ZERO;
  SDA_FastSin (pSinCosLUT,                                          // Sine table pointer
               pSinOutput,                                          // Sine wave destination pointer
               &LookUpTablePhase,                                   // Sine table phase
               CARRIER_FREQUENCY / SAMPLE_RATE,                     // Sine wave frequency
               CARRIER_TABLE_LENGTH,                                // Sine wave look up table length
               SAMPLE_LENGTH);                                      // Dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pSinOutput,                                          // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Sine Wave",                                         // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               ((double) (SAMPLE_LENGTH - 1) / SAMPLE_RATE),        // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nSine Wave\nPlease hit <Carriage Return> to continue . . .");
  getchar ();

// Generate sinusoid
  LookUpTablePhase = SIGLIB_ZERO;
  for (i = 0; i < SAMPLE_LENGTH; i++) {
    *(pSinOutput + i) = SDS_FastSin (pSinCosLUT,                    // Sine table pointer
                                     &LookUpTablePhase,             // Sine table phase
                                     CARRIER_FREQUENCY / SAMPLE_RATE, // Sine wave frequency
                                     CARRIER_TABLE_LENGTH);         // Sine wave look up table length
  }

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pSinOutput,                                          // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Sine Wave",                                         // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               ((double) (SAMPLE_LENGTH - 1) / SAMPLE_RATE),        // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nSine Wave\nPlease hit <Carriage Return> to continue . . .");
  getchar ();


  SIF_FastCos (pSinCosLUT,                                          // Pointer to LUT array
               CARRIER_TABLE_LENGTH);                               // Table length

// Generate fast cosine
  LookUpTablePhase = SIGLIB_ZERO;
  SDA_FastCos (pSinCosLUT,                                          // Cosine table pointer
               pCosOutput,                                          // Cosine wave destination pointer
               &LookUpTablePhase,                                   // Cosine table phase
               CARRIER_FREQUENCY / SAMPLE_RATE,                     // Cosine wave frequency
               CARRIER_TABLE_LENGTH,                                // Cosine wave look up table length
               SAMPLE_LENGTH);                                      // Dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pCosOutput,                                          // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Cosine Wave",                                       // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               ((double) (SAMPLE_LENGTH - 1) / SAMPLE_RATE),        // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nCosine Wave\nPlease hit <Carriage Return> to continue . . .");
  getchar ();

// Generate fast cosine
  LookUpTablePhase = SIGLIB_ZERO;
  for (i = 0; i < SAMPLE_LENGTH; i++) {
    *(pCosOutput + i) = SDS_FastCos (pSinCosLUT,                    // Cosine table pointer
                                     &LookUpTablePhase,             // Cosine table phase
                                     CARRIER_FREQUENCY / SAMPLE_RATE, // Cosine wave frequency
                                     CARRIER_TABLE_LENGTH);         // Cosine wave look up table length
  }

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pCosOutput,                                          // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Cosine Wave",                                       // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               ((double) (SAMPLE_LENGTH - 1) / SAMPLE_RATE),        // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nCosine Wave\nPlease hit <Carriage Return> to continue . . .");
  getchar ();


  SIF_FastSinCos (pSinCosLUT,                                       // Pointer to LUT array
                  CARRIER_TABLE_LENGTH);                            // Table length

// Generate sinusoid and cosinusoid
  LookUpTablePhase = SIGLIB_ZERO;
  SDA_FastSinCos (pSinCosLUT,                                       // Sine table pointer
                  pSinOutput,                                       // Sine wave destination pointer
                  pCosOutput,                                       // Cosine wave destination pointer
                  &LookUpTablePhase,                                // Sine table phase
                  CARRIER_FREQUENCY / SAMPLE_RATE,                  // Sine wave frequency
                  CARRIER_TABLE_LENGTH,                             // Sine wave period
                  SAMPLE_LENGTH);                                   // Dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pSinOutput,                                          // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Sine Wave",                                         // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               ((double) (SAMPLE_LENGTH - 1) / SAMPLE_RATE),        // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pCosOutput,                                          // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Cosine Wave",                                       // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               ((double) (SAMPLE_LENGTH - 1) / SAMPLE_RATE),        // Maximum X value
               "lines",                                             // Graph type
               "red",                                               // Colour
               GPC_ADD);                                            // New graph
  printf ("\nSine And Cosine Wave\nPlease hit <Carriage Return> to continue . . .");
  getchar ();

// Generate sinusoid and cosinusoid
  LookUpTablePhase = SIGLIB_ZERO;
  for (i = 0; i < SAMPLE_LENGTH; i++) {
    SDS_FastSinCos (pSinCosLUT,                                     // Sine table pointer
                    pSinOutput + i,                                 // Sine wave destination pointer
                    pCosOutput + i,                                 // Cosine wave destination pointer
                    &LookUpTablePhase,                              // Sine table phase
                    CARRIER_FREQUENCY / SAMPLE_RATE,                // Sine wave frequency
                    CARRIER_TABLE_LENGTH);                          // Sine wave period
  }

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pSinOutput,                                          // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Sine Wave",                                         // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               ((double) (SAMPLE_LENGTH - 1) / SAMPLE_RATE),        // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pCosOutput,                                          // Dataset
               SAMPLE_LENGTH,                                       // Dataset length
               "Cosine Wave",                                       // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               ((double) (SAMPLE_LENGTH - 1) / SAMPLE_RATE),        // Maximum X value
               "lines",                                             // Graph type
               "red",                                               // Colour
               GPC_ADD);                                            // New graph
  printf ("\nSine And Cosine Wave\nHit <Carriage Return> to continue ....\n");
  getchar ();                                                       // Wait for <Carriage Return>


  SIF_QuickSinc (pSincLUT,                                          // Pointer to Sinc look up table
                 &LookUpTablePhaseGain,                             // Pointer to phase gain
                 MAX_SINC_INPUT_MAGNITUDE,                          // Maximum input 'x' value
                 SINC_LUT_LENGTH);                                  // Look up table length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pSincLUT,                                            // Dataset
               SINC_LUT_LENGTH,                                     // Dataset length
               "Sin(x) / x",                                        // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) MAX_SINC_INPUT_MAGNITUDE,                   // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph

  printf ("\nSine And Cosine Wave\nHit <Carriage Return> to continue ....\n");
  getchar ();                                                       // Wait for <Carriage Return>


  gpc_close (h2DPlot);

  SUF_MemoryFree (pSinOutput);                                      // Free memory
  SUF_MemoryFree (pCosOutput);
  SUF_MemoryFree (pSinCosLUT);
  SUF_MemoryFree (pSineLUT);
  SUF_MemoryFree (pCosineLUT);
  SUF_MemoryFree (pSincLUT);

  exit (0);
}
