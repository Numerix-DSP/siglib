// Image display example

// Include files
#include <stdio.h>
#include <stdlib.h>
#include <gnuplot_c.h>                              // Gnuplot/C

// Define constants
#define IMAGE_X_DIMENSION           256
#define IMAGE_Y_DIMENSION           256
#define IMAGE_SIZE                  (IMAGE_X_DIMENSION * IMAGE_Y_DIMENSION)

// Declare global variables and arrays
unsigned char imageLine [IMAGE_X_DIMENSION];        // Array for reading line of unsigned chars in from file
unsigned int image [IMAGE_SIZE];

int main(void)
{
    FILE            *fp;
    h_GPC_Plot      *hImage;                        // Plot object
    int             i;

    hImage =                                        // Initialize plot
        gpc_init_image ("Image",                    // Plot title
                        IMAGE_X_DIMENSION,          // X-axis length
                        IMAGE_Y_DIMENSION,          // Y-axis length
                        0,                          // Minimum Z value
                        256,                        // Maximum Z value
                        GPC_MONOCHROME,             // Colour mode
                        GPC_KEY_ENABLE);            // Legend / key mode

    if (NULL == hImage) {                           // Graph creation failed - e.g is server running ?
        printf ("\nGraph creation failure\n");
        exit(-1);
    }

    if((fp = fopen ("lena.dat", "rb")) == NULL ) {
        printf ("\nThe image file (%s) could not be opened correctly", "lena.dat");
        exit(10);
    }

    for (i = 0; i < IMAGE_Y_DIMENSION; i++) {       // Read image and convert from unsigned char to unsigned int
        fread (imageLine, sizeof (unsigned char), IMAGE_X_DIMENSION, fp);
        for (int j = 0; j < IMAGE_X_DIMENSION; j++)
            image[(i * IMAGE_X_DIMENSION) + j] = (unsigned int)imageLine[j];
    }

    if ((fclose (fp)) != 0 ) {
        printf ("\nThe image file (%s) could not be closed correctly", "lena.dat");
        exit(10);
    }

    printf ("Image display\n");

    gpc_plot_image (hImage,                         // Graph handle
                    image,                          // Data array
                    "Lena");                        // Dataset title

    printf ("\nHit <Carriage Return> to continue ....\n"); getchar();   // Clear keyboard buffer and wait for <Carriage Return>

    gpc_close (hImage);                             // Close the plot

    return (0);
}

