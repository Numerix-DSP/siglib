// SigLib image coding example
// Copyright (c) 2023 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>
#include <gnuplot_c.h>                                              // Gnuplot/C

// Define constants
#define DC_SOURCE           0                                       // Define test image pattern
#define POINT_SOURCE        0
#define FACE_SOURCE         1

#define ENABLE_QUANTISATION 0                                       // Set to 1 to enable, 0 to disable
#define QUANTISATION_BITS   5                                       // Change to vary quantisation levels

#define DCT_SIZE            8
#define IMAGE_SIZE          64

#define NUMBER_OF_RAW_DATA_CODES        15.
#define NUMBER_OF_DCT_DATA_CODES        64.

// Declare global variables and arrays
#if POINT_SOURCE
static SLData_t source_data[] = {
  1., 1., 1., 1., 1., 1., 1., 1.,
  1., 15., 1., 1., 1., 1., 1., 1.,
  1., 1., 1., 1., 1., 1., 1., 1.,
  1., 1., 1., 1., 1., 1., 1., 1.,
  1., 1., 1., 1., 1., 1., 1., 1.,
  1., 1., 1., 1., 1., 1., 1., 1.,
  1., 1., 1., 1., 1., 1., 1., 1.,
  1., 1., 1., 1., 1., 1., 1., 1.
};
#endif

#if DC_SOURCE
static SLData_t source_data[] = {
  3., 3., 3., 3., 3., 3., 3., 3.,
  3., 3., 3., 3., 3., 3., 3., 3.,
  3., 3., 3., 3., 3., 3., 3., 3.,
  3., 3., 3., 3., 3., 3., 3., 3.,
  3., 3., 3., 3., 3., 3., 3., 3.,
  3., 3., 3., 3., 3., 3., 3., 3.,
  3., 3., 3., 3., 3., 3., 3., 3.,
  3., 3., 3., 3., 3., 3., 3., 3.
};
#endif

#if FACE_SOURCE
static SLData_t source_data[] = {
  1., 1., 0., 0., 0., 0., 1., 1.,
  1., 0., 7., 7., 7., 7., 0., 1.,
  9., 7., 6., 7., 7., 6., 7., 9.,
  9., 7., 7., 7., 7., 7., 7., 9.,
  10., 7., 12., 7., 7., 12., 7., 10.,
  10., 7., 7., 12., 12., 7., 7., 10.,
  2., 10., 7., 7., 7., 7., 10., 2.,
  2., 2., 10., 7., 7., 10., 2., 2.
};
#endif

static SLImageData_t pImageArray[64];                               // Used to plot the results


int main (
  void)
{
  h_GPC_Plot     *hImage;                                           // Declare image object

  SLData_t       *pDctResults = SUF_VectorArrayAllocate (IMAGE_SIZE);
  SLData_t       *pZZSResults = SUF_VectorArrayAllocate (IMAGE_SIZE);
  SLData_t       *pQuantizeResults = SUF_VectorArrayAllocate (IMAGE_SIZE);

  SIF_Dct8x8 ();

  hImage =                                                          // Initialize plot
    gpc_init_image ("Image Coding",                                 // Plot title
                    DCT_SIZE,                                       // X-axis length
                    DCT_SIZE,                                       // Y-axis length
                    GPC_IMG_AUTO_SCALE,                             // Minimum signal magnitude
                    GPC_IMG_AUTO_SCALE,                             // Maximum signal magnitude
                    GPC_MONOCHROME,                                 // Colour mode
                    GPC_KEY_ENABLE);                                // Legend / key mode
  if (NULL == hImage) {
    printf ("\nPlot creation failure.\n");
    exit (-1);
  }

  SDA_Scale (source_data, source_data, NUMBER_OF_DCT_DATA_CODES, IMAGE_SIZE);
  for (SLArrayIndex_t i = 0; i < IMAGE_SIZE; i++) {
    pImageArray[i] = (SLImageData_t) source_data[i];
  }
  gpc_plot_image (hImage,                                           // Graph handle
                  pImageArray,                                      // Dataset
                  "Scaled Source Image");                           // Dataset title
  printf ("\nScaled Source Image\nPlease hit <Carriage Return> to continue . . .");
  getchar ();


  SIM_Dct8x8 (source_data,                                          // Pointer to source sub image
              pDctResults);                                         // Pointer to destination sub image

  SIM_ZigZagScan (pDctResults,                                      // Pointer to source array
                  pZZSResults,                                      // Pointer to destination array
                  DCT_SIZE);                                        // Square image line length


  SDA_Scale (pZZSResults, pZZSResults, NUMBER_OF_DCT_DATA_CODES, IMAGE_SIZE);
  for (SLArrayIndex_t i = 0; i < IMAGE_SIZE; i++) {
    pImageArray[i] = (SLImageData_t) pZZSResults[i];
  }
  gpc_plot_image (hImage,                                           // Graph handle
                  pImageArray,                                      // Dataset
                  "DCT Image");                                     // Dataset title
  printf ("\nDCT Image\nPlease hit <Carriage Return> to continue . . .");
  getchar ();

#if ENABLE_QUANTISATION
  SDA_Quantize (pZZSResults,                                        // Pointer to source array
                pQuantizeResults,                                   // Pointer to destination array
                QUANTISATION_BITS,                                  // Quantisation
                NUMBER_OF_DCT_DATA_CODES,                           // Peak value
                IMAGE_SIZE);                                        // Source array size
#else
  SDA_Copy (pZZSResults,                                            // Pointer to source array
            pQuantizeResults,                                       // Pointer to destination array
            IMAGE_SIZE);                                            // Dataset length
#endif

  for (SLArrayIndex_t i = 0; i < IMAGE_SIZE; i++) {
    pImageArray[i] = (SLImageData_t) pQuantizeResults[i];
  }

  gpc_plot_image (hImage,                                           // Graph handle
                  pImageArray,                                      // Dataset
                  "Quantized DCT Image");                           // Dataset title
  printf ("\nQuantized DCT Image\nPlease hit <Carriage Return> to continue . . .");
  getchar ();

  SIM_ZigZagDescan (pQuantizeResults,                               // Pointer to source array
                    pZZSResults,                                    // Pointer to destination array
                    DCT_SIZE);                                      // Square image line length

  SIM_Idct8x8 (pZZSResults,                                         // Pointer to source sub image
               source_data);                                        // Pointer to destination sub image


  for (SLArrayIndex_t i = 0; i < IMAGE_SIZE; i++) {
    pImageArray[i] = (SLImageData_t) source_data[i];
  }
  gpc_plot_image (hImage,                                           // Graph handle
                  pImageArray,                                      // Dataset
                  "Inverse DCT Image");                             // Dataset title
  printf ("\nInverse DCT Image\n");

  printf ("\nHit <Carriage Return> to continue ....\n");
  getchar ();                                                       // Clear keyboard buffer and wait for <Carriage Return>

  gpc_close (hImage);

  SUF_MemoryFree (pDctResults);                                     // Free memory
  SUF_MemoryFree (pZZSResults);
  SUF_MemoryFree (pQuantizeResults);

  return (0);
}
