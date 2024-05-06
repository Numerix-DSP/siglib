// SigLib FIR Filter Frequency Inversion
// Copyright (c) 2023 Delta Numerix All rights reserved.

// Include files
#include <gnuplot_c.h>    // Gnuplot/C
#include <siglib.h>       // SigLib DSP library
#include <stdio.h>

// Define constants
#define FILTER_LENGTH 256
#define FFT_LENGTH 256
#define LOG2_FFT_LENGTH SAI_FftLengthLog2(FFT_LENGTH)    // Log2 FFT length,

// Declare global variables and arrays

// Initialise filter coefficients
static const SLData_t filter_coeffs[FILTER_LENGTH] = {
    -1.41637511717E-0004, 6.84668558156E-0005,  3.19478601918E-0004,  7.51351390297E-0004,  1.32697375461E-0003,  1.94963778678E-0003,
    2.47385586287E-0003,  2.74135132618E-0003,  2.63156521807E-0003,  2.10985464479E-0003,  1.25436752613E-0003,  2.49949918413E-0004,
    -6.57653387054E-0004, -1.23558265916E-0003, -1.33946652481E-0003, -9.69161830252E-0004, -2.75325826578E-0004, 4.84492917768E-0004,
    1.03217422596E-0003,  1.16744925978E-0003,  8.42305261530E-0004,  1.82728659214E-0004,  -5.54781601720E-0004, -1.07833479442E-0003,
    -1.17359316534E-0003, -7.90637012987E-0004, -7.03499445665E-0005, 7.05219271925E-0004,  1.22003289638E-0003,  1.25156512712E-0003,
    7.65333003186E-0004,  -6.15944428725E-0005, -9.03149046614E-0004, -1.41080065391E-0003, -1.35660553609E-0003, -7.33693181594E-0004,
    2.28634263337E-0004,  1.14750152325E-0003,  1.63698028038E-0003,  1.46869743942E-0003,  6.75919821535E-0004,  -4.45410575848E-0004,
    -1.44516785900E-0003, -1.89814871733E-0003, -1.58341714682E-0003, -5.88386504827E-0004, 7.09619815159E-0004,  1.78463118251E-0003,
    2.17347811335E-0003,  1.67363777739E-0003,  4.43133480544E-0004,  -1.04443830436E-0003, -2.18031544530E-0003, -2.46765384874E-0003,
    -1.73680179045E-0003, -2.35262589353E-0004, 1.45153919035E-0003,  2.62648664039E-0003,  2.76669192611E-0003,  1.75340649113E-0003,
    -5.47684170648E-0005, -1.94445073108E-0003, -3.12666981053E-0003, -3.06367876657E-0003, -1.70969308851E-0003, 4.41294770069E-0004,
    2.53147610223E-0003,  3.67855531116E-0003,  3.34507119452E-0003,  1.58467846344E-0003,  -9.45787052727E-0004, -3.22679506426E-0003,
    -4.28357850114E-0003, -3.59862152304E-0003, -1.35666089222E-0003, 1.59122457830E-0003,  4.04559139532E-0003,  4.94247785185E-0003,
    3.80914941953E-0003,  9.99245248267E-0004,  -2.40563896928E-0003, -5.00614541164E-0003, -5.65470075703E-0003, -3.95429049238E-0003,
    -4.73177202780E-0004, 3.43305556834E-0003,  6.14164230439E-0003,  6.42896903681E-0003,  4.01213369792E-0003,  -2.69157785448E-0004,
    -4.73092241527E-0003, -7.49682098997E-0003, -7.27569455507E-0003, -3.94683196135E-0003, 1.30548379083E-0003,  6.39647797849E-0003,
    9.15309117147E-0003,  8.22356839940E-0003,  3.71049797944E-0003,  -2.75802308759E-0003, -8.59216884835E-0003, -1.12529219474E-0002,
    -9.32677918476E-0003, -3.21923530596E-0003, 4.85339890685E-0003,  1.16309552384E-0002,  1.40829442245E-0002,  1.07068910567E-0002,
    2.31627203757E-0003,  -8.05555556489E-0003, -1.61834981281E-0002, -1.82841770415E-0002, -1.26522216586E-0002, -6.35552816376E-0004,
    1.35170061673E-0002,  2.40052149320E-0002,  2.56320231133E-0002,  1.60206570108E-0002,  -2.94756212746E-0003, -2.51060324993E-0002,
    -4.15549946641E-0002, -4.34002952282E-0002, -2.48261438953E-0002, 1.45361201230E-0002,  6.88097551627E-0002,  1.27026433482E-0001,
    1.75972844790E-0001,  2.03892477287E-0001,  2.03892477287E-0001,  1.75972844790E-0001,  1.27026433482E-0001,  6.88097551627E-0002,
    1.45361201230E-0002,  -2.48261438953E-0002, -4.34002952282E-0002, -4.15549946641E-0002, -2.51060324993E-0002, -2.94756212746E-0003,
    1.60206570108E-0002,  2.56320231133E-0002,  2.40052149320E-0002,  1.35170061673E-0002,  -6.35552816376E-0004, -1.26522216586E-0002,
    -1.82841770415E-0002, -1.61834981281E-0002, -8.05555556489E-0003, 2.31627203757E-0003,  1.07068910567E-0002,  1.40829442245E-0002,
    1.16309552384E-0002,  4.85339890685E-0003,  -3.21923530596E-0003, -9.32677918476E-0003, -1.12529219474E-0002, -8.59216884835E-0003,
    -2.75802308759E-0003, 3.71049797944E-0003,  8.22356839940E-0003,  9.15309117147E-0003,  6.39647797849E-0003,  1.30548379083E-0003,
    -3.94683196135E-0003, -7.27569455507E-0003, -7.49682098997E-0003, -4.73092241527E-0003, -2.69157785448E-0004, 4.01213369792E-0003,
    6.42896903681E-0003,  6.14164230439E-0003,  3.43305556834E-0003,  -4.73177202780E-0004, -3.95429049238E-0003, -5.65470075703E-0003,
    -5.00614541164E-0003, -2.40563896928E-0003, 9.99245248267E-0004,  3.80914941953E-0003,  4.94247785185E-0003,  4.04559139532E-0003,
    1.59122457830E-0003,  -1.35666089222E-0003, -3.59862152304E-0003, -4.28357850114E-0003, -3.22679506426E-0003, -9.45787052727E-0004,
    1.58467846344E-0003,  3.34507119452E-0003,  3.67855531116E-0003,  2.53147610223E-0003,  4.41294770069E-0004,  -1.70969308851E-0003,
    -3.06367876657E-0003, -3.12666981053E-0003, -1.94445073108E-0003, -5.47684170648E-0005, 1.75340649113E-0003,  2.76669192611E-0003,
    2.62648664039E-0003,  1.45153919035E-0003,  -2.35262589353E-0004, -1.73680179045E-0003, -2.46765384874E-0003, -2.18031544530E-0003,
    -1.04443830436E-0003, 4.43133480544E-0004,  1.67363777739E-0003,  2.17347811335E-0003,  1.78463118251E-0003,  7.09619815159E-0004,
    -5.88386504827E-0004, -1.58341714682E-0003, -1.89814871733E-0003, -1.44516785900E-0003, -4.45410575848E-0004, 6.75919821535E-0004,
    1.46869743942E-0003,  1.63698028038E-0003,  1.14750152325E-0003,  2.28634263337E-0004,  -7.33693181594E-0004, -1.35660553609E-0003,
    -1.41080065391E-0003, -9.03149046614E-0004, -6.15944428725E-0005, 7.65333003186E-0004,  1.25156512712E-0003,  1.22003289638E-0003,
    7.05219271925E-0004,  -7.03499445665E-0005, -7.90637012987E-0004, -1.17359316534E-0003, -1.07833479442E-0003, -5.54781601720E-0004,
    1.82728659214E-0004,  8.42305261530E-0004,  1.16744925978E-0003,  1.03217422596E-0003,  4.84492917768E-0004,  -2.75325826578E-0004,
    -9.69161830252E-0004, -1.33946652481E-0003, -1.23558265916E-0003, -6.57653387054E-0004, 2.49949918413E-0004,  1.25436752613E-0003,
    2.10985464479E-0003,  2.63156521807E-0003,  2.74135132618E-0003,  2.47385586287E-0003,  1.94963778678E-0003,  1.32697375461E-0003,
    7.51351390297E-0004,  3.19478601918E-0004,  6.84668558156E-0005,  -1.41637511717E-0004};

int main(void)
{
  h_GPC_Plot* h2DPlot;    // Plot object

  // Data array
  SLData_t* pRealData = SUF_VectorArrayAllocate(FFT_LENGTH);
  SLData_t* pImagData = SUF_VectorArrayAllocate(FFT_LENGTH);
  SLData_t* pResults = SUF_VectorArrayAllocate(FFT_LENGTH);
  SLData_t* pFFTCoeffs = SUF_FftCoefficientAllocate(FFT_LENGTH);

  h2DPlot =                                            // Initialize plot
      gpc_init_2d("FIR Filter Frequency Inversion",    // Plot title
                  "Time / Frequency",                  // X-Axis label
                  "Magnitude",                         // Y-Axis label
                  GPC_AUTO_SCALE,                      // Scaling mode
                  GPC_SIGNED,                          // Sign mode
                  GPC_KEY_ENABLE);                     // Legend / key mode
  if (NULL == h2DPlot) {
    printf("\nPlot creation failure.\n");
    exit(-1);
  }

  // Initialise FFT
  SIF_Fft(pFFTCoeffs,                 // Pointer to FFT coefficients
          SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit
                                      // reverse address table
          FFT_LENGTH);                // FFT length

  SDA_Copy(filter_coeffs,     // Pointer to source array
           pRealData,         // Pointer to destination array
           FILTER_LENGTH);    // Dataset length

  gpc_plot_2d(h2DPlot,                     // Graph handle
              pRealData,                   // Dataset
              FFT_LENGTH,                  // Dataset length
              "Filter Coefficients",       // Dataset title
              SIGLIB_ZERO,                 // Minimum X value
              (double)(FFT_LENGTH - 1),    // Maximum X value
              "lines",                     // Graph type
              "blue",                      // Colour
              GPC_NEW);                    // New graph
  printf("\nFilter Coefficients\nPlease hit <Carriage Return> to continue . . .");
  getchar();

  // Perform real FFT
  SDA_Rfft(pRealData,                  // Pointer to real array
           pImagData,                  // Pointer to imaginary array
           pFFTCoeffs,                 // Pointer to FFT coefficients
           SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit
                                       // reverse address table
           FFT_LENGTH,                 // FFT length
           LOG2_FFT_LENGTH);           // log2 FFT length

  // Scale arrays for display
  SDA_ComplexScalarMultiply(pRealData,                 // Pointer to real source array 1
                            pImagData,                 // Pointer to imaginary source array 1
                            ((SLData_t)FFT_LENGTH),    // Scalar multiplier
                            pRealData,                 // Pointer to real destination array
                            pImagData,                 // Pointer to imaginary destination array
                            FFT_LENGTH);               // Dataset lengths
  // Calculate real magnitude from complex
  SDA_Magnitude(pRealData,      // Pointer to real source array
                pImagData,      // Pointer to imaginary source array
                pResults,       // Pointer to magnitude destination array
                FFT_LENGTH);    // Dataset length
                                // Calc real power fm complex
  SDA_20Log10(pResults,         // Pointer to source array
              pResults,         // Pointer to destination array
              FFT_LENGTH);      // Dataset length

  gpc_plot_2d(h2DPlot,                     // Graph handle
              pResults,                    // Dataset
              FFT_LENGTH,                  // Dataset length
              "Filter Spectrum (dB)",      // Dataset title
              SIGLIB_ZERO,                 // Minimum X value
              (double)(FFT_LENGTH - 1),    // Maximum X value
              "lines",                     // Graph type
              "blue",                      // Colour
              GPC_NEW);                    // New graph
  printf("\nFilter Spectrum (dB)\nPlease hit <Carriage Return> to continue . . .");
  getchar();

  // Calc phase from complex
  SDA_PhaseUnWrapped(pRealData,      // Pointer to real source array
                     pImagData,      // Pointer to imaginary source array
                     pResults,       // Pointer to destination array
                     FFT_LENGTH);    // Dataset length

  gpc_plot_2d(h2DPlot,                     // Graph handle
              pResults,                    // Dataset
              FFT_LENGTH,                  // Dataset length
              "Filter Phase",              // Dataset title
              SIGLIB_ZERO,                 // Minimum X value
              (double)(FFT_LENGTH - 1),    // Maximum X value
              "lines",                     // Graph type
              "blue",                      // Colour
              GPC_NEW);                    // New graph
  printf("\nFilter Phase\nPlease hit <Carriage Return> to continue . . .");
  getchar();

  SDA_Copy(filter_coeffs,     // Pointer to source array
           pRealData,         // Pointer to destination array
           FILTER_LENGTH);    // Dataset length

  // Invert spectrum
  SDA_RealSpectralInverse(pRealData,         // Pointer to source array
                          pRealData,         // Pointer to destination array
                          FILTER_LENGTH);    // Dataset lengths

  gpc_plot_2d(h2DPlot,                           // Graph handle
              pRealData,                         // Dataset
              FFT_LENGTH,                        // Dataset length
              "Inverted Filter Coefficients",    // Dataset title
              SIGLIB_ZERO,                       // Minimum X value
              (double)(FFT_LENGTH - 1),          // Maximum X value
              "lines",                           // Graph type
              "blue",                            // Colour
              GPC_NEW);                          // New graph
  printf("\nInverted Filter Coefficients\nPlease hit <Carriage Return> to "
         "continue . . .");
  getchar();

  // Perform real FFT
  SDA_Rfft(pRealData,                  // Pointer to real array
           pImagData,                  // Pointer to imaginary array
           pFFTCoeffs,                 // Pointer to FFT coefficients
           SIGLIB_BIT_REV_STANDARD,    // Bit reverse mode flag / Pointer to bit
                                       // reverse address table
           FFT_LENGTH,                 // FFT length
           LOG2_FFT_LENGTH);           // log2 FFT length

  // Scale arrays for display
  SDA_ComplexScalarMultiply(pRealData,                 // Pointer to real source array 1
                            pImagData,                 // Pointer to imaginary source array 1
                            ((SLData_t)FFT_LENGTH),    // Scalar multiplier
                            pRealData,                 // Pointer to real destination array
                            pImagData,                 // Pointer to imaginary destination array
                            FFT_LENGTH);               // Dataset lengths
  // Calculate real magnitude from complex
  SDA_Magnitude(pRealData,      // Pointer to real source array
                pImagData,      // Pointer to imaginary source array
                pResults,       // Pointer to magnitude destination array
                FFT_LENGTH);    // Dataset length
  SDA_20Log10(pResults,         // Pointer to source array
              pResults,         // Pointer to destination array
              FFT_LENGTH);      // Dataset length

  gpc_plot_2d(h2DPlot,                            // Graph handle
              pResults,                           // Dataset
              FFT_LENGTH,                         // Dataset length
              "Inverted Filter Spectrum (dB)",    // Dataset title
              SIGLIB_ZERO,                        // Minimum X value
              (double)(FFT_LENGTH - 1),           // Maximum X value
              "lines",                            // Graph type
              "blue",                             // Colour
              GPC_NEW);                           // New graph
  printf("\nInverted Filter Spectrum (dB)\nPlease hit <Carriage Return> to "
         "continue . . .");
  getchar();

  // Calc phase from complex
  SDA_PhaseUnWrapped(pRealData,      // Pointer to real source array
                     pImagData,      // Pointer to imaginary source array
                     pResults,       // Pointer to destination array
                     FFT_LENGTH);    // Dataset length

  gpc_plot_2d(h2DPlot,                     // Graph handle
              pResults,                    // Dataset
              FFT_LENGTH,                  // Dataset length
              "Inverted Filter Phase",     // Dataset title
              SIGLIB_ZERO,                 // Minimum X value
              (double)(FFT_LENGTH - 1),    // Maximum X value
              "lines",                     // Graph type
              "blue",                      // Colour
              GPC_NEW);                    // New graph
  printf("\nInverted Filter Phase\nPlease hit <Carriage Return> to continue . "
         ". .");
  getchar();

  gpc_close(h2DPlot);

  SUF_MemoryFree(pImagData);    // Free memory
  SUF_MemoryFree(pRealData);
  SUF_MemoryFree(pResults);
  SUF_MemoryFree(pFFTCoeffs);

  return (0);
}
