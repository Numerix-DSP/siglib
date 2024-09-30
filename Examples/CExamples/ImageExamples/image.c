// SigLib imaging processing functions example
// Copyright (c) 2024 Delta Numerix All rights reserved.

// Include files
#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <siglib.h>
#include <gnuplot_c.h>    // Gnuplot/C

// Define constants
#define IMAGE_DIMENSION 256
#define IMAGE_SIZE (IMAGE_DIMENSION * IMAGE_DIMENSION)
#define LOG_IMAGE_DIMENSION 8
#define PEAK_PIXEL_LEVEL 255
#define THRESHOLD_LEVEL 9
#define HISTOGRAM_LENGTH (PEAK_PIXEL_LEVEL + 1)
#define INVERSE_IMAGE_DIMENSION (21.0 / IMAGE_DIMENSION)

// Declare global variables and arrays
static SLData_t FilterCoeffs[] = {0.0, 0.1, 0.0, 0.1, 0.5, 0.1, 0.0, 0.1, 0.0};

static SLData_t FFTCoeffs[IMAGE_DIMENSION];    // FFT coefficients array

static unsigned char imageLine[IMAGE_DIMENSION];    // Array for reading line of
                                                    // unsigned chars in from file

int main(int argc, char* argv[])
{
  FILE* fp;
  SLData_t Tmp;
  SLData_t HistogramMax;

  h_GPC_Plot* hImage;    // Declare image objects
  h_GPC_Plot* h2DPlot;

  if (argc != 2) {
    printf("Usage:\nimage imagefilename\n\n");
    return (1);
  }

  // Allocate memory
  SLImageData_t* pImage0 = (SLImageData_t*)malloc((size_t)(IMAGE_SIZE * sizeof(SLImageData_t)));
  SLImageData_t* pImage1 = (SLImageData_t*)malloc((size_t)(IMAGE_SIZE * sizeof(SLImageData_t)));
  SLImageData_t* pImagImage = (SLImageData_t*)malloc((size_t)(IMAGE_SIZE * sizeof(SLImageData_t)));

  SLData_t* pLineBuffer = SUF_VectorArrayAllocate(IMAGE_DIMENSION);
  SLData_t* pHistogram = SUF_VectorArrayAllocate(HISTOGRAM_LENGTH);

  SLData_t* pRealBuffer = SUF_VectorArrayAllocate(IMAGE_DIMENSION);
  SLData_t* pImagBuffer = SUF_VectorArrayAllocate(IMAGE_DIMENSION);

  if ((NULL == pImage0) || (NULL == pImage1) || (NULL == pImagImage) || (NULL == pLineBuffer) || (NULL == pHistogram) || (NULL == pRealBuffer) ||
      (NULL == pImagBuffer)) {

    printf("Memory allocation failure\n");
    exit(-1);
  }

  SLImageData_t* pImage0Start = pImage0;    // Initialize start pointers
  SLImageData_t* pImage1Start = pImage1;
  SLData_t* pLineBufferStart = pLineBuffer;

  SIF_Fft2d(FFTCoeffs, SIGLIB_NULL_ARRAY_INDEX_PTR, IMAGE_DIMENSION);

  if ((fp = fopen(argv[1], "rb")) == NULL) {
    printf("\nThe image file (%s) could not be opened correctly", argv[1]);
    exit(10);
  }

  for (SLArrayIndex_t i = 0; i < IMAGE_DIMENSION; i++) {
    fread(imageLine, sizeof(unsigned char), IMAGE_DIMENSION, fp);
    for (SLArrayIndex_t j = 0; j < IMAGE_DIMENSION; j++) {
      *(pImage0 + (i * IMAGE_DIMENSION) + j) = (SLImageData_t)imageLine[j];
    }
  }

  if ((fclose(fp)) != 0) {
    printf("\nThe image file (%s) could not be closed correctly", argv[1]);
    exit(10);
  }

  hImage =                                     // Initialize plot
      gpc_init_image(argv[1],                  // Plot title
                     IMAGE_DIMENSION,          // X-axis length
                     IMAGE_DIMENSION,          // Y-axis length
                     0,                        // Minimum signal magnitude
                     (int)PEAK_PIXEL_LEVEL,    // Maximum signal magnitude
                     GPC_MONOCHROME,           // Colour mode
                     GPC_KEY_ENABLE);          // Legend / key mode
  if (NULL == hImage) {
    printf("\nPlot creation failure.\n");
    exit(-1);
  }

  h2DPlot =                           // Initialize plot
      gpc_init_2d("Histogram",        // Plot title
                  "Level",            // X-Axis label
                  "Magnitude",        // Y-Axis label
                  GPC_AUTO_SCALE,     // Scaling mode
                  GPC_POSITIVE,       // Sign mode
                  GPC_KEY_ENABLE);    // Legend / key mode
  if (NULL == h2DPlot) {
    printf("\nPlot creation failure.\n");
    exit(-1);
  }

  gpc_plot_image(hImage,             // Graph handle
                 pImage0Start,       // Dataset
                 "Source Image");    // Dataset title
  printf("\nSource Image\nPlease hit <Carriage Return> to continue . . .");
  getchar();

  for (SLArrayIndex_t i = 0; i < IMAGE_DIMENSION; i++) {    // Set array pointers
    for (SLArrayIndex_t j = 0; j < IMAGE_DIMENSION; j++) {
      *pLineBuffer++ = (SLData_t)(*pImage0++);
    }
    pLineBuffer = pLineBufferStart;    // Reset array pointers

    SDA_Threshold(pLineBufferStart, pLineBufferStart, (THRESHOLD_LEVEL * 16), SIGLIB_SINGLE_SIDED_THOLD, IMAGE_DIMENSION);

    for (SLArrayIndex_t j = 0; j < IMAGE_DIMENSION; j++) {
      *pImage1++ = (SLImageData_t)(*pLineBuffer++);
    }
    pLineBuffer = pLineBufferStart;    // Reset array pointers
  }
  pImage0 = pImage0Start;    // Reset pointers
  pImage1 = pImage1Start;

  gpc_plot_image(hImage,                  // Graph handle
                 pImage1Start,            // Dataset
                 "Thresholded Image");    // Dataset title
  printf("\nThresholded Image\nPlease hit <Carriage Return> to continue . . .");
  getchar();

  SIM_Conv3x3(pImage0Start, pImage1Start, FilterCoeffs, IMAGE_DIMENSION, IMAGE_DIMENSION);
  gpc_plot_image(hImage,                        // Graph handle
                 pImage1Start,                  // Dataset
                 "Low-pass Filtered Image");    // Dataset title
  printf("\nLow-pass Filtered Image\nPlease hit <Carriage Return> to continue "
         ". . .");
  getchar();

  SIM_Sobel3x3(pImage0Start, pImage1Start, IMAGE_DIMENSION, IMAGE_DIMENSION);
  gpc_plot_image(hImage,                     // Graph handle
                 pImage1Start,               // Dataset
                 "Sobel Filtered Image");    // Dataset title
  printf("\nSobel Filtered Image\nPlease hit <Carriage Return> to continue . . .");
  getchar();

  SIM_Median3x3(pImage0Start, pImage1Start, IMAGE_DIMENSION, IMAGE_DIMENSION);
  gpc_plot_image(hImage,                      // Graph handle
                 pImage1Start,                // Dataset
                 "Median Filtered Image");    // Dataset title
  printf("\nMedian Filtered Image\nPlease hit <Carriage Return> to continue . "
         ". .");
  getchar();

  SIF_Histogram(pHistogram,           // Histogram array pointer
                HISTOGRAM_LENGTH);    // Histogram dataset length

  pLineBuffer = pLineBufferStart;    // Set array pointers
  for (SLArrayIndex_t i = 0; i < IMAGE_DIMENSION; i++) {
    for (SLArrayIndex_t j = 0; j < IMAGE_DIMENSION; j++) {
      *pLineBuffer++ = (SLData_t)(*pImage0++);
    }
    pLineBuffer = pLineBufferStart;    // Reset array pointers

    SDA_Histogram(pLineBufferStart,     // Input array pointer
                  pHistogram,           // Histogram array pointer
                  SIGLIB_ZERO,          // Minimum input data value
                  PEAK_PIXEL_LEVEL,     // Maximum input data value
                  IMAGE_DIMENSION,      // Array Length
                  HISTOGRAM_LENGTH);    // Number of output levels
  }
  pImage0 = pImage0Start;    // Reset pointer

  SDA_Scale(pHistogram, pHistogram, ((SLData_t)IMAGE_DIMENSION),
            PEAK_PIXEL_LEVEL);    // Scale results

  // Plot histogram
  gpc_plot_2d(h2DPlot,                          // Graph handle
              pHistogram,                       // Dataset
              HISTOGRAM_LENGTH,                 // Dataset length
              "Histogram",                      // Dataset title
              (float)SIGLIB_ZERO,               // Minimum X value
              (float)(PEAK_PIXEL_LEVEL - 1),    // Maximum X value
              "lines",                          // Graph type
              "blue",                           // Colour
              GPC_NEW);                         // New graph
  printf("\nHistogram\nPlease hit <Carriage Return> to continue . . .");
  getchar();

  // Get peak signal
  pLineBuffer = pLineBufferStart;    // Set array pointers
  HistogramMax = SIGLIB_ZERO;
  for (SLArrayIndex_t i = 0; i < IMAGE_DIMENSION; i++) {
    for (SLArrayIndex_t j = 0; j < IMAGE_DIMENSION; j++) {
      *pLineBuffer++ = (SLData_t)(*pImage0++);
    }
    pLineBuffer = pLineBufferStart;    // Reset array pointers

    Tmp = SDA_AbsMax(pLineBufferStart, IMAGE_DIMENSION);

    if (Tmp > HistogramMax) {
      HistogramMax = Tmp;
    }
  }
  pImage0 = pImage0Start;    // Reset pointers

  for (SLArrayIndex_t i = 0; i < IMAGE_DIMENSION; i++) {    // Scale histogram
    for (SLArrayIndex_t j = 0; j < IMAGE_DIMENSION; j++) {
      *pLineBuffer++ = (SLData_t)(*pImage0++);
    }
    pLineBuffer = pLineBufferStart;    // Reset array pointers

    SDA_Multiply(pLineBufferStart, ((PEAK_PIXEL_LEVEL - SIGLIB_ONE) / HistogramMax), pLineBufferStart, IMAGE_DIMENSION);

    for (SLArrayIndex_t j = 0; j < IMAGE_DIMENSION; j++) {
      *pImage1++ = (SLImageData_t)(*pLineBuffer++);
    }
    pLineBuffer = pLineBufferStart;    // Reset array pointers
  }
  pImage0 = pImage0Start;    // Reset pointers
  pImage1 = pImage1Start;

  gpc_plot_image(hImage,                          // Graph handle
                 pImage1Start,                    // Dataset
                 "Histogram Equalised Image");    // Dataset title
  printf("\nHistogram Equalised Image\nPlease hit <Carriage Return> to "
         "continue . . .");
  getchar();

  gpc_close(hImage);    // Delete the monochrome graph

  hImage =                                    // Initialize plot
      gpc_init_image("Spectrum",              // Plot title
                     IMAGE_DIMENSION,         // X-axis length
                     IMAGE_DIMENSION,         // Y-axis length
                     0,                       // Minimum signal magnitude
                     PEAK_PIXEL_LEVEL + 1,    // Maximum signal magnitude
                     GPC_COLOUR,              // Colour mode
                     GPC_KEY_ENABLE);         // Legend / key mode
  if (NULL == hImage) {
    printf("\nPlot creation failure.\n");
    exit(-1);
  }

  SIM_Fft2d(pImage0Start,                   // Source image pointer
            pImage1Start,                   // Destination image pointer
            FFTCoeffs,                      // FFT coefficients pointer
            pImagImage,                     // Pointer to FFT calculation array
            pRealBuffer,                    // Pointer to real FFT calculation array
            pImagBuffer,                    // Pointer to imag. FFT calculation array
            INVERSE_IMAGE_DIMENSION,        // 1.0 / Dimension - used for FFT scaling
            SIGLIB_NULL_ARRAY_INDEX_PTR,    // Bit reverse address table pointer
            IMAGE_DIMENSION,                // Dimension of image
            LOG_IMAGE_DIMENSION);           // Log2 of dimension of image

  // Uncomment this code if you want to save the image data to a file
  //    FILE *fimp = fopen ("image.txt", "w");          // Print the FFT results
  //    to a file for debugging for (SLArrayIndex_t i = 0; i < IMAGE_DIMENSION;
  //    i++) {
  //        for (SLArrayIndex_t j = 0; j < IMAGE_DIMENSION; j++) {
  //            fprintf (fimp, "%u, ", *(pImage1Start+(i*IMAGE_DIMENSION)+j));
  //        }
  //        fprintf (fimp, "\n");
  //    }
  //    fclose (fimp);

  printf("Max = %u\n", SIM_Max(pImage1Start, IMAGE_DIMENSION * IMAGE_DIMENSION));
  printf("Min = %u\n", SIM_Min(pImage1Start, IMAGE_DIMENSION * IMAGE_DIMENSION));

  gpc_plot_image(hImage,                  // Graph handle
                 pImage1Start,            // Dataset
                 "Spectrum Of Image");    // Dataset title
  printf("\nSpectrum Of Image\n");

  printf("\nHit <Carriage Return> to continue ....\n");
  getchar();    // Clear keyboard buffer and wait for <Carriage Return>

  gpc_close(hImage);
  gpc_close(h2DPlot);

  free(pImage0);    // Free memory
  free(pImage1);
  free(pImagImage);
  SUF_MemoryFree(pLineBuffer);
  SUF_MemoryFree(pHistogram);
  SUF_MemoryFree(pRealBuffer);
  SUF_MemoryFree(pImagBuffer);

  return (0);
}
