// SigLib Fill array from disk hex file test program
// Copyright (c) 2022 Sigma Numerix Ltd. All rights reserved.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <siglib.h>                                         // SigLib DSP library
#include <gnuplot_c.h>                                      // Gnuplot/C
#include "plot_fd.h"

// Declare constants
#define SAMPLE_LENGTH     512

double  *pDisplay;
double  *pInput;
FILE    *pInputFile;


int main( int argc, char **argv )
{
    h_GPC_Plot  *h2DPlot;                                   // Plot object

    long    numread;

    if (argc != 2) {
        printf ("\nUsage error  :\nsigplot filename (no extension)\n\n");
        exit(-1);                                           // Exit - usage error
    }

    pInput = (double *)malloc (SAMPLE_LENGTH * sizeof (double));
    pDisplay = (double *)malloc (SAMPLE_LENGTH * sizeof (double));

    if ((pInputFile = fopen(argv[1], "rb")) == NULL) {
        printf ("Error opening input file %s\n", argv[1]);
        exit(0);
    }

    numread = SUF_SigReadData (pInput, pInputFile, SAMPLE_LENGTH);

    fclose (pInputFile);

    if (!numread) {
        printf ("Can not read from .sig file %s\n", argv[1]);
        free (pDisplay);                                    // Free memory
        free (pInput);
        exit(0);
    }

    h2DPlot =                                               // Initialize plot
        gpc_init_2d ("Plot File",                           // Plot title
                     "Time",                                // X-Axis label
                     "Magnitude",                           // Y-Axis label
                     GPC_AUTO_SCALE,                        // Scaling mode
                     GPC_SIGNED,                            // Sign mode
                     GPC_KEY_ENABLE);                       // Legend / key mode
    if (NULL == h2DPlot) {
        printf ("\nPlot creation failure.\n");
        exit(-1);
    }

    gpc_plot_2d (h2DPlot,                                   // Graph handle
                 pInput,                                    // Dataset
                 numread,                                   // Dataset length
                 "Input Signal",                            // Dataset title
                 (double)SIGLIB_ZERO,                       // Minimum X value
                 (double)(numread - 1),                     // Maximum X value
                 "lines",                                   // Graph type
                 "magenta",                                 // Colour
                 GPC_NEW);                                  // New graph

    plot_frequency_domain (pInput, SIGLIB_FLAT_TOP, "Frequency response", numread, 1024);

    getchar();

    free (pDisplay);                                        // Free memory
    free (pInput);

    exit(0);
}
