// SigLib - Multiplex / Demultiplex Example
// Copyright (c) 2022 Sigma Numerix Ltd. All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>                                 // SigLib DSP library
#include <gnuplot_c.h>                              // Gnuplot/C

// Define constants
#define SAMPLE_LENGTH           512
#define NUMBER_OF_CHANNELS      2

// Declare global variables and arrays
static SLData_t     *pData1, *pData2;               // Dataset pointers
static SLData_t     *pMuxedData;

static SLData_t     SinePhase, CosinePhase;

static SLData_t muxd[] = {1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12.};
static SLData_t demuxd[4];
static SLData_t stream[4] = {11., 12., 13., 14.};


int main(void)
{
    h_GPC_Plot  *h2DPlot;                           // Plot object

    pData1 = SUF_VectorArrayAllocate (SAMPLE_LENGTH);
    pData2 = SUF_VectorArrayAllocate (SAMPLE_LENGTH);
    pMuxedData = SUF_VectorArrayAllocate (SAMPLE_LENGTH * NUMBER_OF_CHANNELS);

    h2DPlot =                                       // Initialize plot
        gpc_init_2d ("Multiplex / Demultiplex",     // Plot title
                     "Time",                        // X-Axis label
                     "Magnitude",                   // Y-Axis label
                     GPC_AUTO_SCALE,                // Scaling mode
                     GPC_SIGNED,                    // Sign mode
                     GPC_KEY_ENABLE);               // Legend / key mode
    if (NULL == h2DPlot) {
        printf ("\nPlot creation failure.\n");
        exit(-1);
    }

    SinePhase = SIGLIB_ZERO;
    CosinePhase = SIGLIB_ZERO;

    SDA_SignalGenerate (pData1,                     // Pointer to destination array
                        SIGLIB_SINE_WAVE,           // Signal type - Sine wave
                        0.9,                        // Signal peak level
                        SIGLIB_FILL,                // Fill (overwrite) or add to existing array contents
                        0.005,                      // Signal frequency
                        SIGLIB_ZERO,                // D.C. Offset
                        SIGLIB_ZERO,                // Unused
                        SIGLIB_ZERO,                // Signal end value - Unused
                        &SinePhase,                 // Signal phase - maintained across array boundaries
                        SIGLIB_NULL_DATA_PTR,       // Unused
                        SAMPLE_LENGTH);             // Output dataset length

    SDA_SignalGenerate (pData2,                     // Pointer to destination array
                        SIGLIB_COSINE_WAVE,         // Signal type - Cosine wave
                        SIGLIB_ONE,                 // Signal peak level
                        SIGLIB_FILL,                // Fill (overwrite) or add to existing array contents
                        0.01,                       // Signal frequency
                        SIGLIB_ZERO,                // D.C. Offset
                        SIGLIB_ZERO,                // Unused
                        SIGLIB_ZERO,                // Signal end value - Unused
                        &CosinePhase,               // Signal phase - maintained across array boundaries
                        SIGLIB_NULL_DATA_PTR,       // Unused
                        SAMPLE_LENGTH);             // Output dataset length

    gpc_plot_2d (h2DPlot,                           // Graph handle
                 pData1,                            // Dataset
                 SAMPLE_LENGTH,                     // Dataset length
                 "Sine Wave",                       // Dataset title
                 SIGLIB_ZERO,                       // Minimum X value
                 (double)(SAMPLE_LENGTH - 1),       // Maximum X value
                 "lines",                           // Graph type
                 "blue",                            // Colour
                 GPC_NEW);                          // New graph
    printf ("\nSine Wave\nPlease hit <Carriage Return> to continue . . ."); getchar();

    gpc_plot_2d (h2DPlot,                           // Graph handle
                 pData2,                            // Dataset
                 SAMPLE_LENGTH,                     // Dataset length
                 "Cosine Wave",                     // Dataset title
                 SIGLIB_ZERO,                       // Minimum X value
                 (double)(SAMPLE_LENGTH - 1),       // Maximum X value
                 "lines",                           // Graph type
                 "blue",                            // Colour
                 GPC_NEW);                          // New graph
    printf ("\nCosine Wave\nPlease hit <Carriage Return> to continue . . ."); getchar();

    SDA_Mux2 (pData1,                               // Pointer to source array 1
              pData2,                               // Pointer to source array 2
              pMuxedData,                           // Pointer to destination array
              SAMPLE_LENGTH);                       // Input Dataset length

    gpc_plot_2d (h2DPlot,                           // Graph handle
                 pMuxedData,                        // Dataset
                 2 * SAMPLE_LENGTH,                 // Dataset length
                 "Multiplexed Waveforms",           // Dataset title
                 SIGLIB_ZERO,                       // Minimum X value
                 (double)(SAMPLE_LENGTH - 1),       // Maximum X value
                 "lines",                           // Graph type
                 "blue",                            // Colour
                 GPC_NEW);                          // New graph
    printf ("\nMultiplexed Waveforms\nPlease hit <Carriage Return> to continue . . ."); getchar();

    SDA_Demux2 (pMuxedData,                         // Pointer to source array
                pData1,                             // Pointer to destination array 1
                pData2,                             // Pointer to destination array 2
                SAMPLE_LENGTH);                     // Input Dataset length

    gpc_plot_2d (h2DPlot,                           // Graph handle
                 pData1,                            // Dataset
                 SAMPLE_LENGTH,                     // Dataset length
                 "Demultiplexed Sine Wave",         // Dataset title
                 SIGLIB_ZERO,                       // Minimum X value
                 (double)(SAMPLE_LENGTH - 1),       // Maximum X value
                 "lines",                           // Graph type
                 "blue",                            // Colour
                 GPC_NEW);                          // New graph
    printf ("\nDemultiplexed Sine Wave\nPlease hit <Carriage Return> to continue . . ."); getchar();

    gpc_plot_2d (h2DPlot,                           // Graph handle
                 pData2,                            // Dataset
                 SAMPLE_LENGTH,                     // Dataset length
                 "Demultiplexed Cosine Wave",       // Dataset title
                 SIGLIB_ZERO,                       // Minimum X value
                 (double)(SAMPLE_LENGTH - 1),       // Maximum X value
                 "lines",                           // Graph type
                 "blue",                            // Colour
                 GPC_NEW);                          // New graph
    printf ("\nDemultiplexed Cosine Wave\n");

    printf ("\nHit <Carriage Return> to continue ....\n"); getchar(); // Wait for <Carriage Return>
    gpc_close (h2DPlot);

    printf ("Multiplexed array :\n");
    SUF_PrintArray (muxd, 12);

    SDA_Demultiplex (muxd,                          // Pointer to source multiplexed array
                     demuxd,                        // Pointer to destination array
                     2,                             // Frame sample index to extract
                     4,                             // Number of frames in array
                     3);                            // Number of samples in frame
    printf ("Demultiplexed array, frame sample index 1:\n");
    SUF_PrintArray (demuxd, 4);

    SDA_Multiplex (muxd,                            // Pointer to source multiplexed array
                   stream,                          // Input data for frame sample index
                   muxd,                            // Pointer to destination multiplexed array
                   1,                               // Frame sample index to insert data
                   4,                               // Number of frames in array
                   3);                              // Number of samples in frame

    printf ("Multiplexed array :\n");
    SUF_PrintArray (muxd, 12);

    SUF_MemoryFree (pData1);                        // Free memory
    SUF_MemoryFree (pData2);
    SUF_MemoryFree (pMuxedData);

    exit(0);
}

