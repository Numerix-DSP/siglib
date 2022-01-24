// SigLib frequency domain plot header file
// This file must be included after siglib.h
// Frequency Domain Plots :
// These functions support separate lengths for the source array and the DFT
// so that short arrays can be zero padded to longer (power of 2) lengths
// If the source dataset length is longer than the DFT length then the
// DFT only operates on the DFT length, not the entire array
// Time Domain Plots :
// These functions generate simple time domain plots.

#include "string.h"
#include <gnuplot_c.h>                              // Gnuplot/C

SLError_t plot_frequency_domain (SLData_t *pSrc,
    const enum SLWindow_t WindowType,
    char *pLabelString,
    SLArrayIndex_t SampleLength,
    SLArrayIndex_t DFTLength);

SLError_t plot_complex_frequency_domain (SLData_t *pSrcReal,
    SLData_t *pSrcImag,
    const enum SLWindow_t WindowType,
    char *pLabelString,
    SLArrayIndex_t SampleLength,
    SLArrayIndex_t DFTLength);

SLError_t plot_frequency_magnitude (SLData_t *,
    SLData_t *,
    char *,
    SLArrayIndex_t);

SLError_t plot_time_domain (SLData_t *pSrcReal,
    char *pLabelString,
    SLArrayIndex_t SequenceLength);

SLError_t plot_complex_time_domain (SLData_t *pSrcReal,
    SLData_t *pSrcImag,
    char *pLabelString,
    SLArrayIndex_t SequenceLength);



SLError_t plot_frequency_domain (SLData_t *pSrc,
    const enum SLWindow_t WindowType,
    char *pLabelString,
    SLArrayIndex_t SampleLength,
    SLArrayIndex_t DFTLength)

{
    static h_GPC_Plot   *h2DPlot;                       // Plot object
    static int          FirstTimeFlag = 1;

    SLData_t        Max;
    SLData_t        *pFDPSrcReal, *pFDPRealData, *pFDPImagData, *pFDPResults, *pWindowCoeffs;
    SLArrayIndex_t  MaxIndex;

    char            PrintString [100];
    strcpy (PrintString, pLabelString);
    strcat (PrintString, " (dB)");

    pFDPSrcReal = SUF_VectorArrayAllocate (DFTLength);          // Real source data array
    pFDPRealData = SUF_VectorArrayAllocate (DFTLength);         // Real data array
    pFDPImagData = SUF_VectorArrayAllocate (DFTLength);         // Imaginary data array
    pFDPResults = SUF_VectorArrayAllocate (DFTLength);          // Results data array
    pWindowCoeffs = SUF_VectorArrayAllocate (DFTLength);        // Window coeffs data array

    if ((NULL == pFDPSrcReal) || (NULL == pFDPRealData) || (NULL == pFDPImagData) || (NULL == pFDPResults) ||
        (NULL == pWindowCoeffs)) {
        printf ("Memory allocation error ...\n");
        return (SIGLIB_MEM_ALLOC_ERROR);
    }

    if (FirstTimeFlag == 1) {                       // If this is the first time then create the graph
        h2DPlot =                                   // Initialize plot
            gpc_init_2d ("Frequency Domain",        // Plot title
                         "Frequency",               // X-Axis
                         "Magnitude (dB)",          // Y-Axis
                         GPC_AUTO_SCALE,            // Scaling mode
                         GPC_SIGNED,                // Sign mode
                         GPC_KEY_DISABLE);          // Legend / key mode
      FirstTimeFlag = 0;
    }

    if (NULL == h2DPlot) {
        printf ("Graph creation error ...\n");
        return (SIGLIB_ERROR);                      // Graph creation failed
    }

                                                    // Generate window table
    SIF_Window (pWindowCoeffs,                      // Window coefficients pointer
                WindowType,                         // Window type
                SIGLIB_ZERO,                        // Window coefficient
                DFTLength);                         // Window length

                                                    // Clear the FFT array to zero pad the data
    SDA_Clear (pFDPSrcReal,                         // Pointer to source array
               DFTLength);                          // Dataset length

                                                    // Copy the input data to preserve it
    if (SampleLength <= DFTLength) {
        SDA_Copy (pSrc,                             // Pointer to source array
                  pFDPSrcReal,                      // Pointer to destination array
                  SampleLength);                    // Dataset length
    }
    else {
        SDA_Copy (pSrc,                             // Pointer to source array
                  pFDPSrcReal,                      // Pointer to destination array
                  DFTLength);                       // Dataset length
    }

                                                    // Apply window to real data
    SDA_Window (pFDPSrcReal,                        // Source array pointer
                pFDPSrcReal,                        // Destination array pointer
                pWindowCoeffs,                      // Window array pointer
                DFTLength);                         // Window size

                                                    // Perform DFT
    SDA_Rdft (pFDPSrcReal,                          // Real source array pointer
              pFDPRealData,                         // Real destination array pointer
              pFDPImagData,                         // Imaginary destination array pointer
              DFTLength);                           // DFT length

                                                    // Calc real power fm complex
    SDA_LogMagnitude (pFDPRealData,                 // Pointer to real source array
                      pFDPImagData,                 // Pointer to imaginary source array
                      pFDPResults,                  // Pointer to log magnitude destination array
                      DFTLength);                   // Dataset length

// printf ("\nInputMaxPos = %d\n", SDA_AbsMaxIndex (pFDPSrcReal, SampleLength));
// printf ("\npFDPRealDataMaxPos = %d\n", SDA_AbsMaxIndex (pFDPRealData, SampleLength));
// printf ("\npFDPImagDataMaxPos = %d\n", SDA_AbsMaxIndex (pFDPImagData, SampleLength));
// printf ("\npFDPResultsMaxPos = %d\n", SDA_AbsMaxIndex (pFDPResults, SampleLength));

// printf ("\nInputMax = %lf\n", SDA_AbsMax (pFDPSrcReal, SampleLength));
// printf ("\npFDPRealDataMax = %lf\n", SDA_Max (pFDPRealData, DFTLength));
// printf ("\npFDPImagDataMax = %lf\n", SDA_Max (pFDPImagData, DFTLength));
// printf ("\nResultMax = %lf\n", SDA_AbsMax (pFDPResults, DFTLength));
// printf ("\nMaxIndex = %d\n", SDA_DetectFirstPeakOverThreshold (pFDPResults, -20.0, DFTLength/2));

// SUF_PrintArray (pFDPSrcReal, SampleLength);
// SUF_PrintArray (pFDPRealData, SampleLength);
// SUF_PrintArray (pFDPImagData, SampleLength);
// SUF_PrintArray (pFDPResults, SampleLength);

    Max =
        SDA_AbsMax (pFDPResults,                    // Pointer to source array
                    DFTLength);                     // Dataset length

    SDA_Add (pFDPResults,                           // Pointer to source array
             -Max,                                  // Offset
             pFDPResults,                           // Pointer to destination array
             DFTLength);                            // Dataset length

    MaxIndex =
        SDA_DetectFirstPeakOverThreshold (pFDPResults,  // Pointer to source array
                                          -20.0,        // Threshold over which peak will be detected
                                          DFTLength/2); // Dataset length

    if (MaxIndex  == ((DFTLength/2) - 1)) {         // If there is not a peak above the threshold then detect the highest peak
        MaxIndex =
            SDA_MaxIndex (pFDPResults,              // Pointer to source array
                          DFTLength/2);             // Dataset length
    }


    gpc_plot_2d (h2DPlot,                           // Graph handle
                 pFDPResults,                       // Dataset
                 (int)(DFTLength/2),                // Dataset length
                 PrintString,                       // Dataset title
                 SIGLIB_ZERO,                       // Minimum X value
                 (double)((DFTLength/2) - 1),       // Maximum X value
                 "lines",                           // Graph type
                 "blue",                            // Colour
                 GPC_NEW);                          // New graph
    printf ("\nFrequency Domain Plot\nPeak location = %d\n", MaxIndex);
  //  printf ("Please hit <Carriage Return> to continue . . .\n"); getchar();

    free (pFDPSrcReal);                             // Free memory
    free (pFDPRealData);
    free (pFDPImagData);
    free (pFDPResults);
    free (pWindowCoeffs);

    return (SIGLIB_NO_ERROR);                       // Return success code

}       // End of plot_frequency_domain()


SLError_t plot_complex_frequency_domain (SLData_t *pSrcReal,
    SLData_t *pSrcImag,
    const enum SLWindow_t WindowType,
    char *pLabelString,
    SLArrayIndex_t SampleLength,
    SLArrayIndex_t DFTLength)

{
    static h_GPC_Plot   *h2DPlot;                       // Plot object
    static int          FirstTimeFlag = 1;

    SLData_t        Max;
    SLData_t        *pFDPSrcReal, *pFDPSrcImag, *pFDPRealData, *pFDPImagData, *pFDPResults, *pWindowCoeffs;
    SLArrayIndex_t  MaxIndex;

    char            PrintString [100];
    strcpy (PrintString, pLabelString);
    strcat (PrintString, " (dB)");

    pFDPSrcReal = SUF_VectorArrayAllocate (DFTLength);          // Real source data array
    pFDPSrcImag = SUF_VectorArrayAllocate (DFTLength);          // Imaginary source data array
    pFDPRealData = SUF_VectorArrayAllocate (DFTLength);         // Real data array
    pFDPImagData = SUF_VectorArrayAllocate (DFTLength);         // Imaginary data array
    pFDPResults = SUF_VectorArrayAllocate (DFTLength);          // Results data array
    pWindowCoeffs = SUF_VectorArrayAllocate (DFTLength);        // Window coeffs data array

    if ((NULL == pFDPSrcReal) || (NULL == pFDPSrcImag) || (NULL == pFDPRealData) || (NULL == pFDPImagData) || (NULL == pFDPResults) ||
        (NULL == pWindowCoeffs)) {
        printf ("Memory allocation error ...\n");
        return (SIGLIB_MEM_ALLOC_ERROR);
    }

    if (FirstTimeFlag == 1) {                       // If this is the first time then create the graph
      h2DPlot =                                     // Initialize plot
          gpc_init_2d ("Complex Frequency Domain",  // Plot title
                       "Frequency",                 // X-Axis
                       "Magnitude",                 // Y-Axis
                       GPC_AUTO_SCALE,              // Scaling mode
                       GPC_SIGNED,                  // Sign mode
                       GPC_KEY_DISABLE);            // Legend / key mode
      FirstTimeFlag = 0;
    }

    if (NULL == h2DPlot) {
        printf ("Graph creation error ...\n");
        return (SIGLIB_ERROR);                      // Graph creation failed
    }

                                                    // Generate window table
    SIF_Window (pWindowCoeffs,                      // Window coefficients pointer
                WindowType,                         // Window type
                SIGLIB_ZERO,                        // Window coefficient
                DFTLength);                         // Window length

                                                    // Clear the FFT arrays to zero pad the data
    SDA_Clear (pFDPRealData,                        // Pointer to source array
               DFTLength);                          // Dataset length
    SDA_Clear (pFDPImagData,                        // Pointer to source array
               DFTLength);                          // Dataset length

                                                    // Copy the input data to preserve it
    if (SampleLength <= DFTLength) {
        SDA_Copy (pSrcReal,                         // Pointer to source array
                  pFDPSrcReal,                      // Pointer to destination array
                  SampleLength);                    // Dataset length
        SDA_Copy (pSrcImag,                         // Pointer to source array
                  pFDPSrcImag,                      // Pointer to destination array
                  SampleLength);                    // Dataset length
    }
    else {
        SDA_Copy (pSrcReal,                         // Pointer to source array
                  pFDPSrcReal,                      // Pointer to destination array
                  DFTLength);                       // Dataset length
        SDA_Copy (pSrcImag,                         // Pointer to source array
                  pFDPSrcImag,                      // Pointer to destination array
                  DFTLength);                       // Dataset length
    }

                                                    // Apply window to real data
    SDA_ComplexWindow (pFDPSrcReal,                 // Source array pointer
                       pFDPSrcImag,                 // Pointer to imaginary array
                       pFDPSrcReal,                 // Destination array pointer
                       pFDPSrcImag,                 // Pointer to imaginary array
                       pWindowCoeffs,               // Window array pointer
                       pWindowCoeffs,               // Window array pointer
                       DFTLength);                  // Window size

                                                    // Perform DFT
    SDA_Cdft (pFDPSrcReal,                          // Real source array pointer
              pFDPSrcImag,                          // Imaginary source array pointer
              pFDPRealData,                         // Real destination array pointer
              pFDPImagData,                         // Imaginary destination array pointer
              DFTLength);                           // DFT length

                                                    // Calc real power fm complex
    SDA_LogMagnitude (pFDPRealData,                 // Pointer to real source array
                      pFDPImagData,                 // Pointer to imaginary source array
                      pFDPResults,                  // Pointer to log magnitude destination array
                      DFTLength);                   // Dataset length

    Max =
        SDA_AbsMax (pFDPResults,                    // Pointer to source array
                    DFTLength);                     // Dataset length

    SDA_Add (pFDPResults,                           // Pointer to source array
             -Max,                                  // Offset
             pFDPResults,                           // Pointer to destination array
             DFTLength);                            // Dataset length

    MaxIndex =
        SDA_DetectFirstPeakOverThreshold (pFDPResults,  // Pointer to source array
                                          -20.0,        // Threshold over which peak will be detected
                                          DFTLength/2); // Dataset length

    if (MaxIndex  == ((DFTLength/2) - 1)) {         // If there is not a peak above the threshold then detect the highest peak
        MaxIndex =
            SDA_MaxIndex (pFDPResults,              // Pointer to source array
                          DFTLength/2);             // Dataset length
    }

    gpc_plot_2d (h2DPlot,                           // Graph handle
                 pFDPResults,                       // Dataset
                 (int)(DFTLength/2),                // Dataset length
                 PrintString,                       // Dataset title
                 SIGLIB_ZERO,                       // Minimum X value
                 (double)((DFTLength/2) - 1),       // Maximum X value
                 "lines",                           // Graph type
                 "blue",                            // Colour
                 GPC_NEW);                          // New graph
    printf ("\nFrequency Domain Plot\nPeak location = %d\n", MaxIndex);
  //  printf ("Please hit <Carriage Return> to continue . . .\n"); getchar();

    free (pFDPSrcReal);                             // Free memory
    free (pFDPSrcImag);
    free (pFDPRealData);
    free (pFDPImagData);
    free (pFDPResults);
    free (pWindowCoeffs);

    return (SIGLIB_NO_ERROR);                       // Return success code

}       // End of plot_complex_frequency_domain()


SLError_t plot_frequency_magnitude (SLData_t *pSrcReal,
    SLData_t *pSrcImag,
    char *pLabelString,
    SLArrayIndex_t DFTLength)

{
    static h_GPC_Plot   *h2DPlot;                   // Plot object
    static int          FirstTimeFlag = 1;

    SLData_t        *pFDPResults;

    pFDPResults = SUF_VectorArrayAllocate (DFTLength);  // Results data array

    if (NULL == pFDPResults) {
        printf ("Memory allocation error ...\n");
        return (SIGLIB_MEM_ALLOC_ERROR);
    }

    if (FirstTimeFlag == 1) {                       // If this is the first time then create the graph
        h2DPlot =                                   // Initialize plot
            gpc_init_2d ("Frequency Domain - Magnitude",   // Plot title
                         "Frequency",               // X-Axis
                         "Magnitude",               // Y-Axis
                         GPC_AUTO_SCALE,            // Scaling mode
                         GPC_SIGNED,                // Sign mode
                         GPC_KEY_DISABLE);          // Legend / key mode
        FirstTimeFlag = 0;
    }

    if (NULL == h2DPlot) {
        printf ("Graph creation error ...\n");
        return (SIGLIB_ERROR);                      // Graph creation failed
    }

                                                    // Calc real power fm complex
    SDA_LogMagnitude (pSrcReal,                     // Pointer to real source array
                      pSrcImag,                     // Pointer to imaginary source array
                      pFDPResults,                  // Pointer to log magnitude destination array
                      DFTLength);                   // Dataset length

    gpc_plot_2d (h2DPlot,                           // Graph handle
                 pFDPResults,                       // Dataset
                 DFTLength,                         // Dataset length
                 pLabelString,                      // Dataset title
                 SIGLIB_ZERO,                       // Minimum X value
                 (double)(DFTLength - 1),           // Maximum X value
                 "lines",                           // Graph type
                 "blue",                            // Colour
                 GPC_NEW);                          // New graph
    printf ("\nFrequency Domain Plot\n");
//  printf ("Please hit <Carriage Return> to continue . . .\n"); getchar();

    free (pFDPResults);                             // Free memory

    return (SIGLIB_NO_ERROR);                       // Return success code

}       // End of plot_frequency_magnitude()


SLError_t plot_time_domain (SLData_t *pSrcReal,
    char *pLabelString,
    SLArrayIndex_t SequenceLength)

{
    static h_GPC_Plot   *h2DPlot;                   // Plot object
    static int          FirstTimeFlag = 1;

    if (FirstTimeFlag == 1) {                       // If this is the first time then create the graph
        h2DPlot =                                   // Initialize plot
            gpc_init_2d ("Time Domain",             // Plot title
                         "Time",                    // X-Axis
                         "Magnitude",               // Y-Axis
                         GPC_AUTO_SCALE,            // Scaling mode
                         GPC_SIGNED,                // Sign mode
                         GPC_KEY_DISABLE);          // Legend / key mode
        FirstTimeFlag = 0;
    }

    if (NULL == h2DPlot) {
        printf ("Graph creation error ...\n");
        return (SIGLIB_ERROR);                      // Graph creation failed
    }

    gpc_plot_2d (h2DPlot,                           // Graph handle
                 pSrcReal,                          // Dataset
                 SequenceLength,                    // Dataset length
                 pLabelString,                      // Dataset title
                 SIGLIB_ZERO,                       // Minimum X value
                 (double)(SequenceLength - 1),      // Maximum X value
                 "lines",                           // Graph type
                 "blue",                            // Colour
                 GPC_NEW);                          // New graph
    printf ("\nTime Domain Plot\n");
//  printf ("Please hit <Carriage Return> to continue . . .\n"); getchar();

    return (SIGLIB_NO_ERROR);                       // Return success code

}       // End of plot_frequency_magnitude()


SLError_t plot_complex_time_domain (SLData_t *pSrcReal,
    SLData_t *pSrcImag,
    char *pLabelString,
    SLArrayIndex_t SequenceLength)

{
    static h_GPC_Plot   *h2DPlot;                   // Plot object
    static int          FirstTimeFlag = 1;

    char            PrintString [100];

    if (FirstTimeFlag == 1) {                       // If this is the first time then create the graph
        h2DPlot =                                   // Initialize plot
            gpc_init_2d ("Complex Time Domain",     // Plot title
                         pLabelString,              // X-Axis
                         "Magnitude",               // Y-Axis
                         GPC_AUTO_SCALE,            // Scaling mode
                         GPC_SIGNED,                // Sign mode
                         GPC_KEY_DISABLE);          // Legend / key mode
          FirstTimeFlag = 0;
    }

    if (NULL == h2DPlot) {
        printf ("Graph creation error ...\n");
        return (SIGLIB_ERROR);                      // Graph creation failed
    }

    strcpy (PrintString, pLabelString);
    strcat (PrintString, " - real");

    gpc_plot_2d (h2DPlot,                           // Graph handle
                 pSrcReal,                          // Dataset
                 SequenceLength,                    // Dataset length
                 PrintString,                       // Dataset title
                 SIGLIB_ZERO,                       // Minimum X value
                 (double)(SequenceLength - 1),      // Maximum X value
                 "lines",                           // Graph type
                 "blue",                            // Colour
                 GPC_NEW);                          // New graph

    strcpy (PrintString, pLabelString);
    strcat (PrintString, " - imaginary");

    gpc_plot_2d (h2DPlot,                           // Graph handle
                 pSrcImag,                          // Dataset
                 SequenceLength,                    // Dataset length
                 PrintString,                       // Dataset title
                 SIGLIB_ZERO,                       // Minimum X value
                 (double)(SequenceLength - 1),      // Maximum X value
                 "lines",                           // Graph type
                 "red",                             // Colour
                 GPC_ADD);                          // New graph
    printf ("\n%s\n", pLabelString);

    return (SIGLIB_NO_ERROR);                       // Return success code

}       // End of plot_frequency_magnitude()

