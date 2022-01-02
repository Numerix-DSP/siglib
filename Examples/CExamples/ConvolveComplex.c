// SigLib Complex Convolution Examples
// Copyright (c) 2022 Sigma Numerix Ltd. All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>                                         // SigLib DSP library
#include <gnuplot_c.h>                                      // Gnuplot/C

// Define constants
#define DATA_SET_1              2                           // Select '1' or '0' to choose between test sequences

#define TEST_MODE_REAL          1                           // Select which test mode to use '1' to enable, '0' to disable
#define TEST_MODE_IMAG          0
#define TEST_MODE_COMPLEX       0

#if DATA_SET_1
#define INPUT_LENGTH            20                          // Input dataset length
#define IMPULSE_LENGTH          20                          // Impulse response dataset length
#define PARTIAL_IMPULSE_LENGTH  10                          // Partial response dataset length
#if (TEST_MODE_REAL)
static const SLData_t   input_re[] = {                      // Input data
    0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
    };
static const SLData_t   input_im[] = {
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
    };

static const SLData_t   impulse_re[] = {                    // Impulse response data
    1.0, 1.0, 1.0, 1.0, 1.0, 0.5, 0.5, 0.5, 0.5, 0.5,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
    };
static const SLData_t   impulse_im[] = {
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
    };
#elif (TEST_MODE_IMAG)
static const SLData_t   input_re[] = {                      // Input data
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
    };
static const SLData_t   input_im[] = {
    0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
    };

static const SLData_t   impulse_re[] = {                    // Impulse response data
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
    };
static const SLData_t   impulse_im[] = {
    1.0, 1.0, 1.0, 1.0, 1.0, 0.5, 0.5, 0.5, 0.5, 0.5,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
    };
#elif (TEST_MODE_COMPLEX)
static const SLData_t   input_re[] = {                      // Input data
    0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
    };
static const SLData_t   input_im[] = {
    0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
    };

static const SLData_t   impulse_re[] = {                    // Impulse response data
    1.0, 1.0, 1.0, 1.0, 1.0, 0.5, 0.5, 0.5, 0.5, 0.5,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
    };
static const SLData_t   impulse_im[] = {
    1.0, 1.0, 1.0, 1.0, 1.0, 0.5, 0.5, 0.5, 0.5, 0.5,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
    };
#endif

#else
#define INPUT_LENGTH            5                           // Input dataset length
#define IMPULSE_LENGTH          3                           // Impulse response dataset length
#define PARTIAL_IMPULSE_LENGTH  3                           // Partial response dataset length
#if (TEST_MODE_REAL)
static const SLData_t   input_re[] = {                      // Input data
    1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
    };
static const SLData_t   input_im[] = {
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
    };

static const SLData_t   impulse_re[] = {                    // Impulse response data
    4.0, 5.0, 6.0, 7.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
    };
static const SLData_t   impulse_im[] = {
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
    };
#elif (TEST_MODE_IMAG)
static const SLData_t   input_re[] = {                      // Input data
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
    };
static const SLData_t   input_im[] = {
    1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
    };

static const SLData_t   impulse_re[] = {                    // Impulse response data
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
    };
static const SLData_t   impulse_im[] = {
    4.0, 5.0, 6.0, 7.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
    };
#elif (TEST_MODE_COMPLEX)
static const SLData_t   input_re[] = {                      // Input data
    1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
    };
static const SLData_t   input_im[] = {
    1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
    };

static const SLData_t   impulse_re[] = {                    // Impulse response data
    4.0, 5.0, 6.0, 7.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
    };
static const SLData_t   impulse_im[] = {
    4.0, 5.0, 6.0, 7.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
    };
#endif

#endif

#define RESULT_LENGTH   (INPUT_LENGTH + IMPULSE_LENGTH - 1) // Result dataset length

// Declare global variables and arrays
static SLData_t         dest_re[RESULT_LENGTH];
static SLData_t         dest_im[RESULT_LENGTH];


int main(void)
{
    h_GPC_Plot  *h2DPlot;                                   // Plot object

    h2DPlot =                                               // Initialize plot
        gpc_init_2d ("Convolution",                         // Plot title
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
                 input_re,                                  // Dataset
                 INPUT_LENGTH,                              // Dataset length
                 "Source Signal (Real)",                    // Dataset title
                 SIGLIB_ZERO,                               // Minimum X value
                 (double)(INPUT_LENGTH - 1),                // Maximum X value
                 "lines",                                   // Graph type
                 "blue",                                    // Colour
                 GPC_NEW);                                  // New graph
    gpc_plot_2d (h2DPlot,                                   // Graph handle
                 input_im,                                  // Dataset
                 INPUT_LENGTH,                              // Dataset length
                 "Source Signal (Imag)",                    // Dataset title
                 SIGLIB_ZERO,                               // Minimum X value
                 (double)(INPUT_LENGTH - 1),                // Maximum X value
                 "lines",                                   // Graph type
                 "red",                                     // Colour
                 GPC_ADD);                                  // New graph
    printf ("\nSource Signal\nPlease hit <Carriage Return> to continue . . ."); getchar();

    gpc_plot_2d (h2DPlot,                                   // Graph handle
                 impulse_re,                                // Dataset
                 IMPULSE_LENGTH,                            // Dataset length
                 "Impulse Response (Real)",                 // Dataset title
                 SIGLIB_ZERO,                               // Minimum X value
                 (double)(IMPULSE_LENGTH - 1),              // Maximum X value
                 "lines",                                   // Graph type
                 "blue",                                    // Colour
                 GPC_NEW);                                  // New graph
    gpc_plot_2d (h2DPlot,                                   // Graph handle
                 impulse_im,                                // Dataset
                 IMPULSE_LENGTH,                            // Dataset length
                 "Impulse Response (Imag)",                 // Dataset title
                 SIGLIB_ZERO,                               // Minimum X value
                 (double)(IMPULSE_LENGTH - 1),              // Maximum X value
                 "lines",                                   // Graph type
                 "red",                                     // Colour
                 GPC_ADD);                                  // New graph
    printf ("\nImpulse Response\nPlease hit <Carriage Return> to continue . . ."); getchar();


    SDA_ConvolveLinearComplex (input_re,                    // Pointer to real input array
                               input_im,                    // Pointer to imag input array
                               impulse_re,                  // Pointer to real impulse response data
                               impulse_im,                  // Pointer to imag impulse response data
                               dest_re,                     // Pointer to real destination array
                               dest_im,                     // Pointer to imag destination array
                               INPUT_LENGTH,                // Input data length
                               IMPULSE_LENGTH);             // Impulse response length

    printf ("\ny(t) = x(t)*h(t)\n");
    gpc_plot_2d (h2DPlot,                                   // Graph handle
                 dest_re,                                   // Dataset
                 RESULT_LENGTH,                             // Dataset length
                 "Convolved Data (Real)",                   // Dataset title
                 SIGLIB_ZERO,                               // Minimum X value
                 (double)(RESULT_LENGTH - 1),               // Maximum X value
                 "lines",                                   // Graph type
                 "blue",                                    // Colour
                 GPC_NEW);                                  // New graph
    gpc_plot_2d (h2DPlot,                                   // Graph handle
                 dest_im,                                   // Dataset
                 RESULT_LENGTH,                             // Dataset length
                 "Convolved Data (Imag)",                   // Dataset title
                 SIGLIB_ZERO,                               // Minimum X value
                 (double)(RESULT_LENGTH - 1),               // Maximum X value
                 "lines",                                   // Graph type
                 "red",                                     // Colour
                 GPC_ADD);                                  // New graph
    printf ("Convolved Data\nPlease hit <Carriage Return> to continue . . ."); getchar();

    SDA_ConvolvePartialComplex (input_re,                   // Pointer to real input array
                                input_im,                   // Pointer to imag input array
                                impulse_re,                 // Pointer to real impulse response data
                                impulse_im,                 // Pointer to imag impulse response data
                                dest_re,                    // Pointer to real destination array
                                dest_im,                    // Pointer to imag destination array
                                INPUT_LENGTH,               // Input data length
                                PARTIAL_IMPULSE_LENGTH);    // Impulse response length

    printf ("\ny(t) = x(t)*h(t)\n");
    gpc_plot_2d (h2DPlot,                                           // Graph handle
                 dest_re,                                           // Dataset
                 INPUT_LENGTH - PARTIAL_IMPULSE_LENGTH + 1,         // Dataset length
                 "Partially Convolved Data (Real)",                 // Dataset title
                 SIGLIB_ZERO,                                       // Minimum X value
                 (double)(INPUT_LENGTH - PARTIAL_IMPULSE_LENGTH),   // Maximum X value
                 "lines",                                           // Graph type
                 "blue",                                            // Colour
                 GPC_NEW);                                          // New graph
    gpc_plot_2d (h2DPlot,                                           // Graph handle
                 dest_im,                                           // Dataset
                 INPUT_LENGTH - PARTIAL_IMPULSE_LENGTH + 1,         // Dataset length
                 "Partially Convolved Data (Imag)",                 // Dataset title
                 SIGLIB_ZERO,                                       // Minimum X value
                 (double)(INPUT_LENGTH - PARTIAL_IMPULSE_LENGTH),   // Maximum X value
                 "lines",                                           // Graph type
                 "red",                                             // Colour
                 GPC_ADD);                                          // New graph
    printf ("Partially Convolved Data\nPlease hit <Carriage Return> to continue . . ."); getchar();

    SDA_ConvolveCircularComplex (input_re,                  // Pointer to real input array
                                 input_im,                  // Pointer to imag input array
                                 impulse_re,                // Pointer to real impulse response data
                                 impulse_im,                // Pointer to imag impulse response data
                                 dest_re,                   // Pointer to real destination array
                                 dest_im,                   // Pointer to imag destination array
                                 INPUT_LENGTH);             // Length of input arrays

    printf ("\ny(t) = x(t)*h(t)\n");
    gpc_plot_2d (h2DPlot,                                   // Graph handle
                 dest_re,                                   // Dataset
                 INPUT_LENGTH,                              // Dataset length
                 "Circularly Convolved Data (Real)",        // Dataset title
                 SIGLIB_ZERO,                               // Minimum X value
                 (double)(INPUT_LENGTH - 1),                // Maximum X value
                 "lines",                                   // Graph type
                 "blue",                                    // Colour
                 GPC_NEW);                                  // New graph
    gpc_plot_2d (h2DPlot,                                   // Graph handle
                 dest_im,                                   // Dataset
                 INPUT_LENGTH,                              // Dataset length
                 "Circularly Convolved Data (Imag)",        // Dataset title
                 SIGLIB_ZERO,                               // Minimum X value
                 (double)(INPUT_LENGTH - 1),                // Maximum X value
                 "lines",                                   // Graph type
                 "red",                                     // Colour
                 GPC_ADD);                                  // New graph
    printf ("Circularly Convolved Data\n");

    printf ("\nHit <Carriage Return> to continue ....\n"); getchar(); // Wait for <Carriage Return>
    gpc_close (h2DPlot);

    exit(0);
}


