// SigLib - .WAV file alignment and display program
// Copyright (c) 2023 Delta Numerix All rights reserved.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <siglib_host_utils.h>                                      // Optionally includes conio.h and time.h subset functions
#include <math.h>
#include <siglib.h>                                                 // SigLib DSP library
#include <gnuplot_c.h>                                              // Gnuplot/C

static char     Filename1[80];
static char     Filename2[80];

static SLWavFileInfo_s wavInfo1, wavInfo2;


int main (
  int argc,
  char *argv[])
{
  h_GPC_Plot     *h2DPlot;                                          // Plot object

  SLArrayIndex_t  outputLength;

  if (argc != 3) {
    printf ("\nUsage:\nalignwav Filename1 Filename2\n\n");
    exit (-1);                                                      // Exit - usage error
  }

  strcpy (Filename1, argv[1]);
  strcpy (Filename2, argv[2]);

  printf ("Source file 1 = %s\n", Filename1);
  printf ("Source file 2 = %s\n", Filename2);

  printf ("\nAligning data arrays. This may take a little time ...\n");

  SLData_t       *pDataArray1 = SUF_VectorArrayAllocate (SUF_WavFileLength (Filename1));  // Input data arrays
  SLData_t       *pDataArray2 = SUF_VectorArrayAllocate (SUF_WavFileLength (Filename2));

  SLData_t       *pAlignedArray1 = SUF_VectorArrayAllocate (SUF_WavFileLength (Filename1) + SUF_WavFileLength (Filename2) - 1); // Aligned data arrays
  SLData_t       *pAlignedArray2 = SUF_VectorArrayAllocate (SUF_WavFileLength (Filename1) + SUF_WavFileLength (Filename2) - 1);

  wavInfo1 = SUF_WavReadFile (pDataArray1, Filename1);
  if (wavInfo1.NumberOfSamples == -1) {
    printf ("Error reading .WAV file header: %s\n", Filename1);
    exit (-1);
  }

  wavInfo2 = SUF_WavReadFile (pDataArray2, Filename2);
  if (wavInfo2.NumberOfSamples == -1) {
    printf ("Error reading .WAV file header: %s\n", Filename2);
    exit (-1);
  }

  outputLength = SDA_Align (pDataArray1,                            // Pointer to input array #1
                            pDataArray2,                            // Pointer to input array #2
                            pAlignedArray1,                         // Pointer to destination array #1
                            pAlignedArray2,                         // Pointer to destination array #2
                            SIGLIB_ALIGN_CROP,                      // Alignment mode
                            wavInfo1.NumberOfSamples,               // Length of array #1
                            wavInfo2.NumberOfSamples);              // Length of array #2

  printf ("Data File #1 read:    %d samples\n", wavInfo1.NumberOfSamples);
  printf ("Data File #2 read:    %d samples\n", wavInfo2.NumberOfSamples);
  printf ("Output Array Lengths: %d samples\n", outputLength);

  SUF_WavWriteFile (pAlignedArray1, "aligned_1.wav", wavInfo1, outputLength);
  SUF_WavWriteFile (pAlignedArray2, "aligned_2.wav", wavInfo2, outputLength);

  h2DPlot =                                                         // Initialize plot
    gpc_init_2d_dual_plot ("Plot wave",                             // Plot title
                           GPC_KEY_ENABLE);                         // Legend / key mode
  if (NULL == h2DPlot) {
    printf ("\nPlot creation failure.\n");
    exit (-1);
  }

  gpc_plot_2d_dual_plot (h2DPlot,                                   // Graph handle
                         "Time (sec)",                              // X-Axis label
                         SIGLIB_ZERO,                               // Minimum X value
                         (double) (outputLength / wavInfo1.SampleRate), // Maximum X value
                         pAlignedArray1,                            // Dataset #1
                         "Aligned Signal #1",                       // Dataset #1 title
                         "lines",                                   // Graph type #1
                         "blue",                                    // Colour #1
                         "Magnitude",                               // Y-Axis label #1
                         GPC_AUTO_SCALE,                            // Scaling mode #1
                         GPC_SIGNED,                                // Sign mode #1
                         pAlignedArray2,                            // Dataset #2
                         "Aligned Signal #2",                       // Dataset #2 title
                         "lines",                                   // Graph type #2
                         "orange",                                  // Colour #2
                         "Magnitude",                               // Y-Axis label #2
                         GPC_AUTO_SCALE,                            // Scaling mode #2
                         GPC_SIGNED,                                // Sign mode #2
                         outputLength);                             // Dataset lengths

  printf ("Hit <Carriage Return> to continue ...\n");
  getchar ();

  gpc_close (h2DPlot);

  free (pDataArray1);                                               // Free memory
  free (pDataArray2);
  free (pAlignedArray1);
  free (pAlignedArray2);

  return (0);
}
