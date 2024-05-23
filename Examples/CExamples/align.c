// SigLib - array alignment and display program
// The datasets are aligned using correlation
// Copyright (c) 2023 Delta Numerix All rights reserved.

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <siglib.h>               // SigLib DSP library
#include <gnuplot_c.h>            // Gnuplot/C
#include <siglib_host_utils.h>    // Optionally includes conio.h and time.h subset functions

#define PLOT_RESULTS 1    // Set to '1' to plot results, '0' otherwise

static const SLData_t AlignSrc1[] = {    // Input data sequence 1
    1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

static const SLData_t AlignSrc2[] = {    // Input data sequence 2
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 0.0, 0.0, 0.0};

#define ALIGN_SRC_1_LENGTH 20    // Input array #1 length
#define ALIGN_SRC_2_LENGTH 20    // Input array #2 length

static SLData_t Dst1[ALIGN_SRC_1_LENGTH + ALIGN_SRC_2_LENGTH - 1];    // Ensure there is more than enough for the aligned results
static SLData_t Dst2[ALIGN_SRC_1_LENGTH + ALIGN_SRC_2_LENGTH - 1];

void SIGLIB_FUNC_DECL SUF_PrintAlignedArrays(const SLData_t* pSrc1, const SLData_t* pSrc2, const SLArrayIndex_t ArrayLength)
{
  for (SLArrayIndex_t i = 0; i < ArrayLength; i++) {
    printf("[%d] = %1.6lf\t\t[%d] = %1.6lf\n", (SLInt32_t)i, *pSrc1++, (SLInt32_t)i, *pSrc2++);
  }
  printf("\n");

}    // End of SUF_PrintAlignedArrays()

int main(void)
{
  SLArrayIndex_t outputLength;

#if PLOT_RESULTS
  h_GPC_Plot* h2DPlot;    // Plot object

  h2DPlot =                           // Initialize plot
      gpc_init_2d("Correlation",      // Plot title
                  "Time",             // X-Axis label
                  "Magnitude",        // Y-Axis label
                  GPC_AUTO_SCALE,     // Scaling mode
                  GPC_SIGNED,         // Sign mode
                  GPC_KEY_ENABLE);    // Legend / key mode
  if (NULL == h2DPlot) {
    printf("\nPlot creation failure.\n");
    exit(-1);
  }
#endif

  SDA_Fill(Dst1, 11111111.,
           ALIGN_SRC_1_LENGTH + ALIGN_SRC_2_LENGTH - 1);    // Fill with dummy data to ensure that all elements are
                                                            // written correctly
  SDA_Fill(Dst2, 11111111., ALIGN_SRC_1_LENGTH + ALIGN_SRC_2_LENGTH - 1);

  outputLength = SDA_Align(AlignSrc1,              // Pointer to input array #1
                           AlignSrc2,              // Pointer to input array #2
                           Dst1,                   // Pointer to destination array #1
                           Dst2,                   // Pointer to destination array #2
                           SIGLIB_ALIGN_EXTEND,    // Alignment mode
                           ALIGN_SRC_1_LENGTH,     // Length of array #1
                           ALIGN_SRC_2_LENGTH);    // Length of array #2

  printf("Output Array Lengths = %d\n", outputLength);

  printf("\nOutput Array #1 =       Output Array #2 = \n");
  SUF_PrintAlignedArrays(Dst1, Dst2, outputLength);

  outputLength = SDA_Align(AlignSrc1,              // Pointer to input array #1
                           AlignSrc2,              // Pointer to input array #2
                           Dst1,                   // Pointer to destination array #1
                           Dst2,                   // Pointer to destination array #2
                           SIGLIB_ALIGN_CROP,      // Alignment mode
                           ALIGN_SRC_1_LENGTH,     // Length of array #1
                           ALIGN_SRC_2_LENGTH);    // Length of array #2

  printf("Output Array Lengths = %d\n", outputLength);

  printf("\nOutput Array #1 =       Output Array #2 = \n");
  SUF_PrintAlignedArrays(Dst1, Dst2, outputLength);

#if PLOT_RESULTS
  gpc_plot_2d(h2DPlot,                       // Graph handle
              Dst2,                          // Dataset
              outputLength,                  // Dataset length
              "Aligned Set #1",              // Dataset title
              (double)SIGLIB_ZERO,           // Minimum X value
              (double)(outputLength - 1),    // Maximum X value
              "lines",                       // Graph type
              "blue",                        // Colour
              GPC_NEW);                      // New graph

  gpc_plot_2d(h2DPlot,                       // Graph handle
              Dst2,                          // Dataset
              outputLength,                  // Dataset length
              "Aligned Set #1",              // Dataset title
              (double)SIGLIB_ZERO,           // Minimum X value
              (double)(outputLength - 1),    // Maximum X value
              "lines",                       // Graph type
              "red",                         // Colour
              GPC_ADD);                      // New graph

  printf("\nHit <Carriage Return> to continue ....\n");
  getchar();    // Wait for <Carriage Return>
  gpc_close(h2DPlot);
#endif

  return (0);
}
