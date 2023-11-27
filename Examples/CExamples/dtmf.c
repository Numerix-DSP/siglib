// SigLib - DTMF Generation And Detection Example
// Copyright (c) 2023 Delta Numerix All rights reserved.

// Freq. (Hz) 1209 1336 1477 1633
//     697     1     2    3    A
//     770     4     5    6    B
//     852     7     8    9    C
//     941     *     0    #    D

// Include files
#include <stdio.h>
#include <siglib_host_utils.h>                                      // Optionally includes conio.h and time.h subset functions
#include <siglib.h>                                                 // SigLib DSP library
#include <gnuplot_c.h>                                              // Gnuplot/C

// Define constants
#define VIEW_TIME_DOMAIN        0                                   // Set to '1' to view the time domain plot
#define VIEW_FREQ_DOMAIN        1                                   // Set to '1' to view the frequency domain plot

#define SAMPLE_LENGTH           512
#define FFT_LENGTH              512
#define LOG2_FFT_LENGTH         SAI_FftLengthLog2(FFT_LENGTH)       // Log2 FFT length,
#define DTMF_DETECT_LENGTH      102
#define SAMPLE_RATE_HZ          8000.
#define SIGNAL_MAGNITUDE        SIGLIB_HALF                         // Note - the final output magnitude will be twice this magnitude

// Declare global variables and arrays
static SLData_t *pRealData, *pImagData, *pResults, *pFFTCoeffs;     // Dataset pointers
static SLData_t *pDTMFGenCoeffs;                                    // DTMF generator frequency look up table


int main (
  void)
{
  h_GPC_Plot     *h2DPlot;                                          // Plot object

#if VIEW_TIME_DOMAIN
  SLData_t        TimeIndex = SIGLIB_ZERO;
#endif

  SLFixData_t     Key;
  SLStatus_t      KeyCode;

  pRealData = SUF_VectorArrayAllocate (SAMPLE_LENGTH);
  pImagData = SUF_VectorArrayAllocate (FFT_LENGTH);
  pResults = SUF_VectorArrayAllocate (FFT_LENGTH);
  pDTMFGenCoeffs = SUF_VectorArrayAllocate (SIGLIB_DTMF_FTABLE_LENGTH);
  pFFTCoeffs = SUF_FftCoefficientAllocate (FFT_LENGTH);

  printf ("\n\n\nHit standard DTMF keys 0 - 9, A - D, * and #. <Esc> to exit\n");
  printf ("The key displayed on the screen is decoded from the DTMF.\n\n");
  printf ("Hit any key to start . . .");
  _getch ();

#if VIEW_TIME_DOMAIN
  h2DPlot =                                                         // Initialize plot
    gpc_init_2d ("DTMF Generation And Detection",                   // Plot title
                 "Time / Frequency",                                // X-Axis label
                 "Magnitude",                                       // Y-Axis label
                 GPC_AUTO_SCALE,                                    // Scaling mode
                 GPC_SIGNED,                                        // Sign mode
                 GPC_KEY_ENABLE);                                   // Legend / key mode
  if (NULL == h2DPlot) {
    printf ("\nPlot creation failure.\n");
    exit (-1);
  }
#endif


#if VIEW_FREQ_DOMAIN
  h2DPlot =                                                         // Initialize plot
    gpc_init_2d ("DTMF Generation And Detection",                   // Plot title
                 "Frequency (Hz)",                                  // X-Axis label
                 "Magnitude (dB)",                                  // Y-Axis label
                 GPC_AUTO_SCALE,                                    // Scaling mode
                 GPC_SIGNED,                                        // Sign mode
                 GPC_KEY_ENABLE);                                   // Legend / key mode
  if (NULL == h2DPlot) {
    printf ("\nPlot creation failure.\n");
    exit (-1);
  }
#endif

  SIF_DtmfGenerate (pDTMFGenCoeffs,                                 // Generator coefficient look up table pointer
                    SAMPLE_RATE_HZ);                                // Sample rate
  SIF_DtmfDetect (SAMPLE_RATE_HZ,                                   // Sample rate
                  SAMPLE_LENGTH);                                   // Dataset length

// Initialise FFT
  SIF_Fft (pFFTCoeffs,                                              // Pointer to FFT coefficients
           SIGLIB_BIT_REV_STANDARD,                                 // Bit reverse mode flag / Pointer to bit reverse address table
           FFT_LENGTH);                                             // FFT length

  while ((Key = (SLFixData_t) _getch ()) != 0x1b) {
    if ((KeyCode = SUF_AsciiToKeyCode (Key)) != SIGLIB_NO_DTMF_KEY) {
//          printf ("\nKeyCode = %d\n", (int)KeyCode);

      SDA_DtmfGenerate (pRealData,                                  // Destination array pointer
                        KeyCode,                                    // Key code
                        SIGNAL_MAGNITUDE,                           // Signal magnitude
                        pDTMFGenCoeffs,                             // Generator coefficient look up table pointer
                        SAMPLE_LENGTH);                             // Dataset length

      KeyCode = SDA_DtmfDetect (pRealData,                          // Source array pointer
                                DTMF_DETECT_LENGTH);                // Dataset length

      Key = SUF_KeyCodeToAscii (KeyCode);
      printf ("\nDTMF signal detected: %c\n", (char) Key);

#if VIEW_TIME_DOMAIN
      gpc_plot_2d (h2DPlot,                                         // Graph handle
                   pRealData,                                       // Dataset
                   SAMPLE_LENGTH,                                   // Dataset length
                   "Time domain signal",                            // Dataset title
                   TimeIndex,                                       // Minimum X value
                   TimeIndex + ((double) (SAMPLE_LENGTH - 1) / SAMPLE_RATE_HZ), // Maximum X value
                   "lines",                                         // Graph type
                   "blue",                                          // Colour
                   GPC_NEW);                                        // New graph
      TimeIndex += (SLData_t) SAMPLE_LENGTH / SAMPLE_RATE_HZ;
      printf ("Please hit <Carriage Return> to continue . . .");
#endif

#if VIEW_FREQ_DOMAIN
// Perform real FFT
      SDA_Rfft (pRealData,                                          // Pointer to real array
                pImagData,                                          // Pointer to imaginary array
                pFFTCoeffs,                                         // Pointer to FFT coefficients
                SIGLIB_BIT_REV_STANDARD,                            // Bit reverse mode flag / Pointer to bit reverse address table
                FFT_LENGTH,                                         // FFT length
                LOG2_FFT_LENGTH);                                   // log2 FFT length
// Calculate real magnitude from complex
      SDA_Magnitude (pRealData,                                     // Pointer to real source array
                     pImagData,                                     // Pointer to imaginary source array
                     pResults,                                      // Pointer to magnitude destination array
                     FFT_LENGTH);                                   // Dataset length

      gpc_plot_2d (h2DPlot,                                         // Graph handle
                   pResults,                                        // Dataset
                   FFT_LENGTH >> 1,                                 // Dataset length
                   "Frequency domain signal",                       // Dataset title
                   SIGLIB_ZERO,                                     // Minimum X value
                   (((double) ((SAMPLE_RATE_HZ / 2) * (FFT_LENGTH - 1))) / ((double) FFT_LENGTH)),  // Maximum X value
                   "lines",                                         // Graph type
                   "blue",                                          // Colour
                   GPC_NEW);                                        // New graph
      printf ("Please hit <Carriage Return> to continue . . .");
#endif
    }
  }

#if (VIEW_TIME_DOMAIN) || (VIEW_FREQ_DOMAIN)
  gpc_close (h2DPlot);
#endif


  SUF_MemoryFree (pRealData);                                       // Free memory
  SUF_MemoryFree (pImagData);
  SUF_MemoryFree (pResults);
  SUF_MemoryFree (pDTMFGenCoeffs);
  SUF_MemoryFree (pFFTCoeffs);

  exit (0);
}
