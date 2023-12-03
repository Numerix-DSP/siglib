// SigLib Multi-Rate Filter Example
// Copyright (c) 2023 Delta Numerix All rights reserved.

// This demonstration shows how to translate a set of FIR filter
// coefficients into a poly-phase filter bank and use the
// polyphase filter system for sample rate changing.
// Polyphase filters ignore unrequired inputs or outputs and
//  redundant multiplications by 0.
//
// This example gives a sample rate increase by a factor of 3/2, for
// example from 32 KHz to 48 KHz, a common ratio required in an audio
// application.
//
// The complete system is best described by the following block
// diagram:
//
//                    +---------+
//                    |         |
//               +--->| Phase 1 |------+
//               |    | Filter  |      |
//               |    |         |      |
//               |    +---------+      |
//               |       ^   ^         |
//               |       I   O Clks    |
//               |                     v
//               |    +---------+    +----+
//         Input |    |         |    | __ |  Output
//         ------+--->| Phase 2 |--->| \  |------>
//               |    | Filter  |    | /_ |
//               |    |         |    |    |
//               |    +---------+    +----+
//               |       ^   ^         ^
//               |       I   O Clks    |
//               |                     |
//               |    +---------+      |
//               |    |         |      |
//               +--->| Phase 3 |------+
//                    | Filter  |
//                    |         |
//                    +---------+
//                       ^   ^
//                       I   O Clks
//
// The polyphase filter coefficients are generated in identical manner
// to those used in a standard decimation / interpolation filter, however
// each of the three filters shown contain a subset of the standard
// coefficients.
//
// For the given example, the interpolation filter used is a low pass
// filter, with a cut off frequency of 0.133, normalised to a sampling
// rate of 1.0.
//
// The input stream in this example is limited to 512 samples, this gives
// 768 (512 * 3/2) samples in the output stream, this is truncated to 512
// samples for performing the FFT. Looking closely at the FFTs of the
// input and output arrays shows that the spectral peak has moved from
// bin 51 to bin 34, a ratio change of 2/3, as would be expected.

// Include files
#include <stdio.h>
#include <siglib.h>                                                 // SigLib DSP library
#include <gnuplot_c.h>                                              // Gnuplot/C

// Define constants
#define POLY_PHASE_FILTER_LENGTH    64
#define NUMBER_OF_POLY_PHASES       3

#define INPUT_SAMPLE_LENGTH         512
#define OUTPUT_SAMPLE_LENGTH        768                             // 512 * 3/2

#define FFT_LENGTH                  512
#define LOG2_FFT_LENGTH             SAI_FftLengthLog2(FFT_LENGTH)   // Log2 FFT length,
#define WINDOW_LENGTH               FFT_LENGTH

// Declare global variables and arrays

            // Initialise filter coefficients - Generated using Digital Filter Plus
static const SLData_t FilterCoeffs[POLY_PHASE_FILTER_LENGTH] = {
  5.77631805109E-0003, 1.92889745312E-0002, 4.35002244116E-0002, 7.58729382999E-0002, 1.07925782844E-0001, 1.27031786293E-0001,
  1.21895670658E-0001, 8.93421308882E-0002, 3.84198486934E-0002, -1.16365819878E-0002, -3.99157169663E-0002, -3.49296728171E-0002,
  -1.86816770676E-0003, 3.91598494350E-0002, 6.40472139114E-0002, 5.89385630297E-0002, 2.90200008567E-0002, -4.35613858428E-0003,
  -1.72794177132E-0002, 6.47499091583E-0004, 3.85084795481E-0002, 7.05951550651E-0002, 7.39455524871E-0002, 4.56071348553E-0002,
  7.41108634041E-0003, -6.48060519202E-0003, 2.74707508663E-0002, 1.03347844358E-0001, 1.84313276136E-0001, 2.21332756002E-0001,
  1.81844214432E-0001, 7.13171986326E-0002, -7.13171986326E-0002, -1.81844214432E-0001, -2.21332756002E-0001, -1.84313276136E-0001,
  -1.03347844358E-0001, -2.74707508663E-0002, 6.48060519202E-0003, -7.41108634041E-0003, -4.56071348553E-0002, -7.39455524871E-0002,
  -7.05951550651E-0002, -3.85084795481E-0002, -6.47499091583E-0004, 1.72794177132E-0002, 4.35613858428E-0003, -2.90200008567E-0002,
  -5.89385630297E-0002, -6.40472139114E-0002, -3.91598494350E-0002, 1.86816770676E-0003, 3.49296728171E-0002, 3.99157169663E-0002,
  1.16365819878E-0002, -3.84198486934E-0002, -8.93421308882E-0002, -1.21895670658E-0001, -1.27031786293E-0001, -1.07925782844E-0001,
  -7.58729382999E-0002, -4.35002244116E-0002, -1.92889745312E-0002, -5.77631805109E-0003
};

static SLData_t FilterPolyPhaseCoeffs[POLY_PHASE_FILTER_LENGTH];
static SLData_t *pPolyPhaseFilterCoeffPointers[POLY_PHASE_FILTER_LENGTH];
static SLArrayIndex_t PolyPhaseFilterLengths[NUMBER_OF_POLY_PHASES];


int main (
  void)
{
  h_GPC_Plot     *h2DPlot;                                          // Plot object

  SLFixData_t     i, phase_clock;

  SLArrayIndex_t  PPFilter0Index, PPFilter1Index, Filter2Index;

  SLData_t       *pInput = SUF_VectorArrayAllocate (INPUT_SAMPLE_LENGTH);
  SLData_t       *pConverted = SUF_VectorArrayAllocate (OUTPUT_SAMPLE_LENGTH);
  SLData_t       *pRealData = SUF_VectorArrayAllocate (FFT_LENGTH);
  SLData_t       *pImagData = SUF_VectorArrayAllocate (FFT_LENGTH);
  SLData_t       *pResults = SUF_VectorArrayAllocate (FFT_LENGTH);
  SLData_t       *pFFTCoeffs = SUF_FftCoefficientAllocate (FFT_LENGTH);

  h2DPlot =                                                         // Initialize plot
    gpc_init_2d ("Multi-rate Filter Design",                        // Plot title
                 "Time / Frequency",                                // X-Axis label
                 "Magnitude",                                       // Y-Axis label
                 GPC_AUTO_SCALE,                                    // Scaling mode
                 GPC_SIGNED,                                        // Sign mode
                 GPC_KEY_ENABLE);                                   // Legend / key mode
  if (NULL == h2DPlot) {
    printf ("\nPlot creation failure.\n");
    exit (-1);
  }


  SIF_FirPolyPhaseGenerate (FilterCoeffs,                           // FIR filter coefficients
                            FilterPolyPhaseCoeffs,                  // Poly-phase filter coefficients
                            pPolyPhaseFilterCoeffPointers,          // Poly-phase filter coefficient pointers
                            PolyPhaseFilterLengths,                 // Poly-phase filter lengths
                            NUMBER_OF_POLY_PHASES,                  // Number of phases
                            POLY_PHASE_FILTER_LENGTH);              // FIR filter length

// Uncomment this little lot, if you want to print the filter lengths and coefficients
// printf ("Filter length 0 = %d\n", PolyPhaseFilterLengths[0]);
// SUF_PrintArray (pPolyPhaseFilterCoeffPointers[0], PolyPhaseFilterLengths[0]);
// printf ("\nFilter length 1 = %d\n", PolyPhaseFilterLengths[1]);
// SUF_PrintArray (pPolyPhaseFilterCoeffPointers[1], PolyPhaseFilterLengths[1]);
// printf ("\nFilter length 2 = %d\n", PolyPhaseFilterLengths[2]);
// SUF_PrintArray (pPolyPhaseFilterCoeffPointers[2], PolyPhaseFilterLengths[2]);
// printf ("\n");

  SLData_t       *pPPFilter0StateArray = SUF_VectorArrayAllocate (PolyPhaseFilterLengths[0]);
  SLData_t       *pPPFilter1StateArray = SUF_VectorArrayAllocate (PolyPhaseFilterLengths[1]);
  SLData_t       *pPPFilter2StateArray = SUF_VectorArrayAllocate (PolyPhaseFilterLengths[2]);

  SIF_Fir (pPPFilter0StateArray,                                    // Pointer to filter state array
           &PPFilter0Index,                                         // Pointer to filter index register
           PolyPhaseFilterLengths[0]);                              // Filter length
  SIF_Fir (pPPFilter1StateArray,                                    // Pointer to filter state array
           &PPFilter1Index,                                         // Pointer to filter index register
           PolyPhaseFilterLengths[1]);                              // Filter length
  SIF_Fir (pPPFilter2StateArray,                                    // Pointer to filter state array
           &Filter2Index,                                           // Pointer to filter index register
           PolyPhaseFilterLengths[2]);                              // Filter length

// Initialise FFT
  SIF_Fft (pFFTCoeffs,                                              // Pointer to FFT coefficients
           SIGLIB_BIT_REV_STANDARD,                                 // Bit reverse mode flag / Pointer to bit reverse address table
           FFT_LENGTH);                                             // FFT length

  SLData_t        sinePhase = SIGLIB_ZERO;                          // Generate a sinewave
  SDA_SignalGenerate (pInput,                                       // Pointer to destination array
                      SIGLIB_SINE_WAVE,                             // Signal type - Sine wave
                      0.7,                                          // Signal peak level
                      SIGLIB_FILL,                                  // Fill (overwrite) or add to existing array contents
                      0.1,                                          // Signal frequency
                      SIGLIB_ZERO,                                  // D.C. Offset
                      SIGLIB_ZERO,                                  // Unused
                      SIGLIB_ZERO,                                  // Signal end value - Unused
                      &sinePhase,                                   // Signal phase - maintained across array boundaries
                      SIGLIB_NULL_DATA_PTR,                         // Unused
                      INPUT_SAMPLE_LENGTH);                         // Output dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pInput,                                              // Dataset
               INPUT_SAMPLE_LENGTH,                                 // Dataset length
               "Source Signal",                                     // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (INPUT_SAMPLE_LENGTH - 1),                  // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nSource Signal\nPlease hit <Carriage Return> to continue . . .");
  getchar ();

  SDA_Copy (pInput,                                                 // Pointer to source array
            pRealData,                                              // Pointer to destination array
            FFT_LENGTH);                                            // Dataset length

// Perform real FFT
  SDA_Rfft (pRealData,                                              // Pointer to real array
            pImagData,                                              // Pointer to imaginary array
            pFFTCoeffs,                                             // Pointer to FFT coefficients
            SIGLIB_BIT_REV_STANDARD,                                // Bit reverse mode flag / Pointer to bit reverse address table
            FFT_LENGTH,                                             // FFT length
            LOG2_FFT_LENGTH);                                       // log2 FFT length

// Calculate real magnitude from complex
  SDA_Magnitude (pRealData,                                         // Pointer to real source array
                 pImagData,                                         // Pointer to imaginary source array
                 pResults,                                          // Pointer to magnitude destination array
                 FFT_LENGTH);                                       // Dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pResults,                                            // Dataset
               FFT_LENGTH,                                          // Dataset length
               "Source Signal Spectrum",                            // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (FFT_LENGTH - 1),                           // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nSource Signal Spectrum\nPlease hit <Carriage Return> to continue . . .");
  getchar ();

  for (i = 0, phase_clock = 0; i < INPUT_SAMPLE_LENGTH; i++) {
    if (phase_clock == 0) {
      *pConverted++ = SDS_Fir (*pInput,                             // Input data sample to be filtered
                               pPPFilter0StateArray,                // Pointer to filter state array
                               pPolyPhaseFilterCoeffPointers[0],    // Pointer to filter coefficients
                               &PPFilter0Index,                     // Pointer to filter index register
                               PolyPhaseFilterLengths[0]);          // Filter length

      SDS_FirAddSample (*pInput,                                    // Input sample to add to delay line
                        pPPFilter1StateArray,                       // Pointer to filter data state array
                        &PPFilter1Index,                            // Pointer to filter index register
                        PolyPhaseFilterLengths[1]);                 // Filter length

      *pConverted++ = SDS_Fir (*pInput++,                           // Input data sample to be filtered
                               pPPFilter2StateArray,                // Pointer to filter state array
                               pPolyPhaseFilterCoeffPointers[2],    // Pointer to filter coefficients
                               &Filter2Index,                       // Pointer to filter index register
                               PolyPhaseFilterLengths[2]);          // Filter length

      phase_clock = 1;                                              // Increment the phase control clock
    }

    else {
      SDS_FirAddSample (*pInput,                                    // Input sample to add to delay line
                        pPPFilter0StateArray,                       // Pointer to filter data state array
                        &PPFilter0Index,                            // Pointer to filter index register
                        PolyPhaseFilterLengths[0]);                 // Filter length

      *pConverted++ = SDS_Fir (*pInput,                             // Input data sample to be filtered
                               pPPFilter1StateArray,                // Pointer to filter state array
                               pPolyPhaseFilterCoeffPointers[1],    // Pointer to filter coefficients
                               &PPFilter1Index,                     // Pointer to filter index register
                               PolyPhaseFilterLengths[1]);          // Filter length

      SDS_FirAddSample (*pInput++,                                  // Input sample to add to delay line
                        pPPFilter2StateArray,                       // Pointer to filter data state array
                        &Filter2Index,                              // Pointer to filter index register
                        PolyPhaseFilterLengths[2]);                 // Filter length

      phase_clock = 0;                                              // Increment the phase control clock
    }
  }

  pConverted -= OUTPUT_SAMPLE_LENGTH;
  pInput -= INPUT_SAMPLE_LENGTH;

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pConverted,                                          // Dataset
               OUTPUT_SAMPLE_LENGTH,                                // Dataset length
               "Converted Signal",                                  // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (OUTPUT_SAMPLE_LENGTH - 1),                 // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nConverted Signal\nPlease hit <Carriage Return> to continue . . .");
  getchar ();

// Perform real FFT
  SDA_Rfft (pConverted,                                             // Pointer to real array
            pImagData,                                              // Pointer to imaginary array
            pFFTCoeffs,                                             // Pointer to FFT coefficients
            SIGLIB_BIT_REV_STANDARD,                                // Bit reverse mode flag / Pointer to bit reverse address table
            FFT_LENGTH,                                             // FFT length
            LOG2_FFT_LENGTH);                                       // log2 FFT length

// Calculate real magnitude from complex
  SDA_Magnitude (pConverted,                                        // Pointer to real source array
                 pImagData,                                         // Pointer to imaginary source array
                 pResults,                                          // Pointer to magnitude destination array
                 FFT_LENGTH);                                       // Dataset length

  gpc_plot_2d (h2DPlot,                                             // Graph handle
               pResults,                                            // Dataset
               FFT_LENGTH,                                          // Dataset length
               "Converted Signal Spectrum",                         // Dataset title
               SIGLIB_ZERO,                                         // Minimum X value
               (double) (FFT_LENGTH - 1),                           // Maximum X value
               "lines",                                             // Graph type
               "blue",                                              // Colour
               GPC_NEW);                                            // New graph
  printf ("\nConverted Signal Spectrum\n");

  printf ("\nHit <Carriage Return> to continue ....\n");
  getchar ();                                                       // Wait for <Carriage Return>
  gpc_close (h2DPlot);

  SUF_MemoryFree (pInput);                                          // Free memory
  SUF_MemoryFree (pConverted);
  SUF_MemoryFree (pRealData);
  SUF_MemoryFree (pImagData);
  SUF_MemoryFree (pResults);
  SUF_MemoryFree (pFFTCoeffs);
  SUF_MemoryFree (pPPFilter0StateArray);
  SUF_MemoryFree (pPPFilter1StateArray);
  SUF_MemoryFree (pPPFilter2StateArray);

  return (0);
}
