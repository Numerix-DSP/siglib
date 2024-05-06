// SigLib z-transform Example - plots the z-transform of an FIR or IIR filter
// Copyright (c) 2023 Delta Numerix All rights reserved.

// Include files
#include "plot_fd/plot_fd.h"    // Frequency domain plots
#include <gnuplot_c.h>          // Gnuplot/C
#include <siglib.h>             // SigLib DSP library
#include <stdio.h>

// Define constants
#define ENABLE_FIR 1         // Set to '1' to enable FIR filter
#define ENABLE_IIR 0         // Set to '1' to enable IIR filter
#define ENABLE_ALL_POLE 0    // Set to '1' to enable all-pole filter

#define ZT_LENGTH 512
#define ZT_SIZE (ZT_LENGTH * ZT_LENGTH)

#define ZT_START_END 1.5                              // z-transform calculated between +/- N
#define ZT_STEP ((2.0 * ZT_START_END) / ZT_LENGTH)    // z-transform step size

#define UNIT_CIRCLE_POINTS 1000

// Declare global variables and arrays
#if ENABLE_FIR
//#define   NUMERATOR_LENGTH      5
#  define NUMERATOR_LENGTH 10
#  define DENOMINATOR_LENGTH 0
// SLComplexRect_s   Numerator [] = {{1.0, 0.0}, {1.0, 0.0}, {1.0, 0.0}, {1.0,
// 0.0}, {1.0, 0.0}}; SLComplexRect_s   Numerator [] = {{1.0, 0.0}, {-1.0, 0.0},
// {1.0, 0.0}, {-1.0, 0.0}, {1.0, 0.0}};
static const SLComplexRect_s Numerator[] = {{1.0, 0.0},     {0.6, 0.0},      {0.36, 0.0},      {0.216, 0.0},      {0.1296, 0.0},
                                            {0.07776, 0.0}, {0.046656, 0.0}, {0.0279936, 0.0}, {0.01679616, 0.0}, {0.010077696, 0.0}};
static const SLComplexRect_s Denominator[] = {{0.0, 0.0}};

static char printString[] = ("FIR Filter\nNumerator [] = {{1.0, 0.0}, {0.6, 0.0}, {0.36, 0.0}, {0.216, "
                             "0.0}, {0.1296, 0.0},\n\t\t{0.07776, 0.0}, {0.046656, 0.0}, {0.0279936, "
                             "0.0}, {0.01679616, 0.0}, {0.010077696, 0.0}}\nDenominator [] = {{0.0, "
                             "0.0}}");
#endif

#if ENABLE_IIR
#  define NUMERATOR_LENGTH 3
#  define DENOMINATOR_LENGTH 3
static const SLComplexRect_s Numerator[] = {{1.0, 0.0}, {-1.414, 0.0}, {1.0, 0.0}};
static const SLComplexRect_s Denominator[] = {{1.0, 0.0}, {1.273, 0.0}, {-0.810, 0.0}};

static char printString[] = ("IIR Filter\nNumerator [] = {{1.0, 0.0}, {-1.414, 0.0}, {1.0, "
                             "0.0}}\nDenominator [] = {{1.0, 0.0}, {1.273, 0.0}, {-0.810, 0.0}}");
#endif

#if ENABLE_ALL_POLE
#  define NUMERATOR_LENGTH 0
#  define DENOMINATOR_LENGTH 2
static const SLComplexRect_s Numerator[] = {{0.0, 0.0}};
static const SLComplexRect_s Denominator[] = {{1.0, 0.0}, {0.6, 0.0}};

static char printString[] = ("All Pole Filter\nNumerator [] = {{0.0, 0.0}}\nDenominator [] = {{1.0, "
                             "0.0}, {0.6, 0.0}}");
#endif

int main(void)
{
  h_GPC_Plot* hImage;    // Plot object
  SLData_t RealPoint, ImagPoint;
  SLFixData_t RealIndex, ImagIndex;

  SLData_t* pZTransform = SUF_VectorArrayAllocate(ZT_SIZE);    // Allocate memory
  SLImageData_t* pImage = (SLImageData_t*)malloc((size_t)(ZT_SIZE * sizeof(SLImageData_t)));

  hImage =                                  // Initialize plot
      gpc_init_image("z-transform Plot",    // Plot title
                     ZT_LENGTH,             // X-axis length
                     ZT_LENGTH,             // Y-axis length
                     0,                     // Minimum Z value
                     255,                   // Maximum Z value
                     GPC_COLOUR,            // Colour mode
                     GPC_KEY_ENABLE);       // Legend / key mode
  if (NULL == hImage) {
    printf("\nPlot creation failure.\n");
    exit(-1);
  }

  // Create sub-carriers
  for (ImagIndex = 0; ImagIndex < ZT_LENGTH; ImagIndex++) {
    ImagPoint = ZT_START_END - (ImagIndex * ZT_STEP);

    for (RealIndex = 0; RealIndex < ZT_LENGTH; RealIndex++) {
      RealPoint = -ZT_START_END + (RealIndex * ZT_STEP);

      *(pZTransform + RealIndex + (ImagIndex * ZT_LENGTH)) = SDS_ZTransformDB(SCV_Rectangular(RealPoint, ImagPoint),    // z-Domain location
                                                                              Numerator,                                // Numerator
                                                                              Denominator,                              // Denominator
                                                                              NUMERATOR_LENGTH,                         // Numerator length
                                                                              DENOMINATOR_LENGTH);                      // Denominator length
    }
  }

  // Print min, max and locations
  printf("dB - Min[%d] = %lf, Max[%d] = %lf\n", SDA_MinIndex(pZTransform, ZT_SIZE), SDA_Min(pZTransform, ZT_SIZE),
         SDA_MaxIndex(pZTransform, ZT_SIZE), SDA_Max(pZTransform, ZT_SIZE));

  // Ensure signal is positive for display
  SDA_Add(pZTransform,                       // Source array pointer
          -SDA_Min(pZTransform, ZT_SIZE),    // Offset value
          pZTransform,                       // Destination array pointer
          ZT_SIZE);                          // Dataset length

  // Scale signal for display
  SDA_Scale(pZTransform,    // Source array pointer
            pZTransform,    // Destination array pointer
            255.0,          // Peak value
            ZT_SIZE);       // Dataset length

  // Plot the Axes and unit circle
  for (SLArrayIndex_t i = 0; i < ZT_LENGTH; i++) {
    *(pZTransform + i + (ZT_LENGTH * (ZT_LENGTH >> 1))) = 255.0;
    *(pZTransform + (i * ZT_LENGTH) + (ZT_LENGTH >> 1)) = 255.0;
  }

  for (SLArrayIndex_t i = 0; i < UNIT_CIRCLE_POINTS; i++) {
    SLComplexRect_s UnitCircle;
    UnitCircle = SCV_PolarToRectangular(SCV_Polar((SIGLIB_ONE / ZT_START_END) * ((SLData_t)(ZT_LENGTH >> 1)),
                                                  ((SLData_t)i) * SIGLIB_TWO_PI * (SIGLIB_ONE / ((SLData_t)UNIT_CIRCLE_POINTS))));
    *(pZTransform + (ZT_LENGTH * (ZT_LENGTH >> 1)) + (ZT_LENGTH >> 1) + (int)(UnitCircle.real) - ((int)(UnitCircle.imag) * ZT_LENGTH)) = 255.0;
  }

  // Convert floating point data to image for display
  for (SLArrayIndex_t i = 0; i < ZT_SIZE; i++) {
    *(pImage + i) = (SLImageData_t) * (pZTransform + i);
  }

  gpc_plot_image(hImage,           // Graph handle
                 pImage,           // Data array
                 "zTransform");    // Dataset title

  printf("%s", printString);

  printf("\nHit <Carriage Return> to continue ....\n");
  getchar();    // Wait for <Carriage Return>
  gpc_close(hImage);

  SUF_MemoryFree(pZTransform);    // Free memory
  SUF_MemoryFree(pImage);

  return (0);
}
