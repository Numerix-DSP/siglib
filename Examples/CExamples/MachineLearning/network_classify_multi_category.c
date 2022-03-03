// SigLib Neural Network Verification Program
// Copyright (c) 2022 Sigma Numerix Ltd.

// Include files
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <siglib_host_utils.h>                              // Optionally includes conio.h and time.h subset functions
#include <siglib.h>                                         // SigLib DSP library
#include <gnuplot_c.h>                                      // Gnuplot/C


// Define constants
#ifndef NETWORK_INPUT_SAMPLE_LENGTH
    #define NETWORK_INPUT_SAMPLE_LENGTH     128
#endif
#ifndef NETWORK_HIDDEN_LAYER_NODES
    #define NETWORK_HIDDEN_LAYER_NODES      5
#endif

#ifndef PREDICTION_THRESHOLD
    #define PREDICTION_THRESHOLD            0.8
#endif

#ifndef INPUT_SCALING_FACTOR
    #define INPUT_SCALING_FACTOR            (SIGLIB_ONE/1000.)  // Scaling factor used to avoid numerical overflow in the exponent function
#endif

#define NUM_CATEGORIES                      4               // Number of output categories

// Declare global variables and arrays
static SLData_t         *pLayer1PostActivation, *pLayer2PostActivation;
static SLArrayIndex_t   *pCategoricalValue;

static char             filenameWeights[2400];

static SLData_t layer1Weights[NETWORK_HIDDEN_LAYER_NODES][NETWORK_INPUT_SAMPLE_LENGTH]; // Declare weights for two transition paths
static SLData_t layer2Weights[NUM_CATEGORIES][NETWORK_HIDDEN_LAYER_NODES];

#ifndef HIDDEN_LAYER_ACTIVATION_TYPE
    #define HIDDEN_LAYER_ACTIVATION_TYPE            SIGLIB_ACTIVATION_TYPE_LOGISTIC // Activation type
    #define HIDDEN_LAYER_ACTIVATION_ALPHA           SIGLIB_ZERO     // Alpha value not required in this mode
#else
    #if (HIDDEN_LAYER_ACTIVATION_TYPE == 2)
        #define HIDDEN_LAYER_ACTIVATION_ALPHA       0.01            // Leaky ReLU alpha value
    #else
        #define HIDDEN_LAYER_ACTIVATION_ALPHA       SIGLIB_ZERO     // Alpha value not required in this mode
    #endif
#endif

#ifndef OUTPUT_LAYER_ACTIVATION_TYPE
    #define OUTPUT_LAYER_ACTIVATION_TYPE            SIGLIB_ACTIVATION_TYPE_LOGISTIC // Activation type
    #define OUTPUT_LAYER_ACTIVATION_ALPHA           SIGLIB_ZERO     // Alpha value not required in this mode
#else
    #if (OUTPUT_LAYER_ACTIVATION_TYPE == 2)
        #define OUTPUT_LAYER_ACTIVATION_ALPHA       0.01            // Leaky ReLU alpha value
    #else
        #define OUTPUT_LAYER_ACTIVATION_ALPHA       SIGLIB_ZERO     // Alpha value not required in this mode
    #endif
#endif


SLArrayIndex_t  debugFlag = 0;

void parse_command_line (int argc, char *argv[]);
void show_help (void);


#if defined(_MSC_VER)
    #pragma warning(disable : 4127) /* conditional expression is constant has been removed */
#endif



int main (int argc, char *argv[])
{
    pLayer1PostActivation = SUF_VectorArrayAllocate (NETWORK_HIDDEN_LAYER_NODES);   // Allocate arrays
    pLayer2PostActivation = SUF_VectorArrayAllocate (NUM_CATEGORIES);

    if ((NULL == pLayer1PostActivation) || (NULL == pLayer2PostActivation)) {
        printf ("\n\nMemory allocation failed\n\n");
        exit(-1);
    }

    parse_command_line (argc, argv);                        //  Parse command line options

    printf ("\nClassifying The Data ...\n");
    printf ("    Number of input layer nodes  : %d\n", NETWORK_INPUT_SAMPLE_LENGTH);
    printf ("    Number of hidden layer nodes : %d\n", NETWORK_HIDDEN_LAYER_NODES);
    printf ("    Hidden Layer Activation Type : ");
    if (HIDDEN_LAYER_ACTIVATION_TYPE == SIGLIB_ACTIVATION_TYPE_RELU)
        printf ("ReLU\n");
    else if (HIDDEN_LAYER_ACTIVATION_TYPE == SIGLIB_ACTIVATION_TYPE_LEAKY_RELU)
        { printf ("Leaky ReLU\n"); printf ("    Hidden Layer Activation Alpha: %lf", (double)HIDDEN_LAYER_ACTIVATION_ALPHA); }
    else if (HIDDEN_LAYER_ACTIVATION_TYPE == SIGLIB_ACTIVATION_TYPE_LOGISTIC)
        printf ("Logistic\n");
    else if (HIDDEN_LAYER_ACTIVATION_TYPE == SIGLIB_ACTIVATION_TYPE_TANH)
        printf ("TanH\n");
    printf ("    Output Layer Activation Type : ");
    if (OUTPUT_LAYER_ACTIVATION_TYPE == SIGLIB_ACTIVATION_TYPE_RELU)
        printf ("ReLU\n\n");
    else if (OUTPUT_LAYER_ACTIVATION_TYPE == SIGLIB_ACTIVATION_TYPE_LEAKY_RELU)
        { printf ("Leaky ReLU\n"); printf ("    Output Layer Activation Alpha: %lf", (double)HIDDEN_LAYER_ACTIVATION_ALPHA); }
    else if (OUTPUT_LAYER_ACTIVATION_TYPE == SIGLIB_ACTIVATION_TYPE_LOGISTIC)
        printf ("Logistic\n\n");
    else if (OUTPUT_LAYER_ACTIVATION_TYPE == SIGLIB_ACTIVATION_TYPE_TANH)
        printf ("TanH\n\n");


    // Load classification data
    SLArrayIndex_t nRows, nCols;
    SLData_t *pClassificationData = NULL;                   // Pointer only - memory will be allocated in SUF_CsvReadMatrix()
    SUF_CsvReadMatrix (&pClassificationData, "TrainingDataSet.csv", SIGLIB_FIRST_ROW_KEEP, &nRows, &nCols);

    if ((nRows == 0) || (NULL == pClassificationData)) {
        printf ("\n\nSUF_CsvReadMatrix (TrainingDataSet.csv) failure!.\n\n");
        exit(-1);
    }

    if ((nCols-1) != NETWORK_INPUT_SAMPLE_LENGTH) {
        printf ("Error - Classification sequences must be %d samples long\n", NETWORK_INPUT_SAMPLE_LENGTH);
        printf ("        Provided sequence was %d samples long\n", nCols);
        exit(-1);
    }

    if (debugFlag == 1) {
        printf ("nCols-1 = NETWORK_INPUT_SAMPLE_LENGTH = %d\n", NETWORK_INPUT_SAMPLE_LENGTH);

        printf ("Number of classification sequences:   %d\n", nRows);
        printf ("Classification Data\n");
        SUF_PrintMatrix (pClassificationData, nRows, NETWORK_INPUT_SAMPLE_LENGTH);
    }

    pCategoricalValue = SUF_IndexArrayAllocate(nRows);      // Allocate the categorical value array
    if (NULL == pCategoricalValue) {
        printf ("\n\nMemory allocation failed (pCategoricalValue)\n\n");
        exit(-1);
    }

    SMX_ExtractCategoricalColumn (pClassificationData,      // Pointer to source matrix
                                  pCategoricalValue,        // Pointer to destination array
                                  nRows,                    // Number of rows in matrix
                                  nCols);                   // Number of columns in matrix
    SMX_DeleteOldColumn (pClassificationData,               // Pointer to source matrix
                         pClassificationData,               // Pointer to destination matrix
                         nCols-1,                           // Column number to delete
                         nRows,                             // Number of rows in matrix
                         nCols);                            // Number of columns in matrix

    nCols--;                                                // We have removed the categorical value column

                                                            // Scale all samples to avoid overflow in exponent function
    SDA_Multiply (pClassificationData,                      // Pointer to source array
                  INPUT_SCALING_FACTOR,                     // Scalar value
                  pClassificationData,                      // Pointer to destination array
                  nRows*nCols);                             // Array length


#if defined (_MSC_VER)                                      // Defined by Microsoft compilers
    sprintf (filenameWeights, "weightCoefficientsFiles\\weightsMachine1_Machine2_Machine3_Machine4.dat");
#else
    sprintf (filenameWeights, "weightCoefficientsFiles/weightsMachine1_Machine2_Machine3_Machine4.dat");
#endif

    int numItems =
        SUF_ReadWeightsBinaryFile (filenameWeights,             // Filename
                                   (SLData_t *)layer1Weights,   // layer1Weights
                                   (SLData_t *)layer2Weights);  // layer2Weights

    if (numItems != (NETWORK_HIDDEN_LAYER_NODES*NETWORK_INPUT_SAMPLE_LENGTH)+(NUM_CATEGORIES*NETWORK_HIDDEN_LAYER_NODES)) {
        printf ("**** ERROR - INCORRECT NUMBER OF WEIGHTS READ FROM BINARY FILE: %s\n\n", filenameWeights);
        exit(-1);
    }

    if (debugFlag == 1) {
        printf ("layer1Weights\n"); SUF_PrintMatrix((SLData_t *)layer1Weights, NETWORK_HIDDEN_LAYER_NODES, NETWORK_INPUT_SAMPLE_LENGTH);
        printf ("layer2Weights\n\n"); SUF_PrintMatrix((SLData_t *)layer2Weights, NUM_CATEGORIES, NETWORK_HIDDEN_LAYER_NODES);

        printf ("layer1Weights Min: %lf\n", SDA_Min((SLData_t *)layer1Weights, NETWORK_HIDDEN_LAYER_NODES*NETWORK_INPUT_SAMPLE_LENGTH));    // Max and min values
        printf ("layer1Weights Max: %lf\n", SDA_Max((SLData_t *)layer1Weights, NETWORK_HIDDEN_LAYER_NODES*NETWORK_INPUT_SAMPLE_LENGTH));
        printf ("layer2Weights Min: %lf\n", SDA_Min((SLData_t *)layer2Weights, NUM_CATEGORIES*NETWORK_HIDDEN_LAYER_NODES));
        printf ("layer2Weights Max: %lf\n\n", SDA_Max((SLData_t *)layer2Weights, NUM_CATEGORIES*NETWORK_HIDDEN_LAYER_NODES));
    }

    SLArrayIndex_t totalFrameCount = SIGLIB_AI_ZERO;
    SLArrayIndex_t classifiedFrameCount = SIGLIB_AI_ZERO;
    SLArrayIndex_t unclassifiedFrameCount = SIGLIB_AI_ZERO;

    for (SLArrayIndex_t classificationSequenceNumber = 0; classificationSequenceNumber < nRows; classificationSequenceNumber++) {
        SLNeuralNetworkPrediction_s prediction =
            SDA_TwoLayerNCategoryNetworkPredict (pClassificationData+(classificationSequenceNumber*NETWORK_INPUT_SAMPLE_LENGTH),    // Pointer to data to classify
                                                 (SLData_t *)layer1Weights,                                                 // Pointer to layer #1 weights
                                                 (SLData_t *)layer2Weights,                                                 // Pointer to layer #2 weights
                                                 pLayer1PostActivation,                                                     // Pointer to post activation for hidden layer
                                                 pLayer2PostActivation,                                                     // Pointer to post activation for output layer
                                                 HIDDEN_LAYER_ACTIVATION_TYPE,                                              // Hidden layer activation type
                                                 HIDDEN_LAYER_ACTIVATION_ALPHA,                                             // Hidden layer alpha value
                                                 OUTPUT_LAYER_ACTIVATION_TYPE,                                              // Output layer activation type
                                                 OUTPUT_LAYER_ACTIVATION_ALPHA,                                             // Output layer alpha value
                                                 NETWORK_INPUT_SAMPLE_LENGTH,                                               // Input sample length
                                                 NETWORK_HIDDEN_LAYER_NODES,                                                // Hidden layer length
                                                 NUM_CATEGORIES);                                                           // Number of output categories

        totalFrameCount++;

        if (PREDICTION_THRESHOLD < prediction.probability) {
            printf ("Classified  : Classification: %d, Probability: %lf\n", prediction.predictedCategory, prediction.probability);
            classifiedFrameCount++;
        }
        else {
            printf ("Unclassified: Probability: %lf\n", prediction.probability);
            unclassifiedFrameCount++;
        }
    }

    printf ("\n\nTotal Frame Count        : %d\n", totalFrameCount);
    printf ("Classified Frame Count   : %d, %d%%\n", classifiedFrameCount, classifiedFrameCount*100/totalFrameCount);
    printf ("Unclassified Frame Count : %d, %d%%\n\n", unclassifiedFrameCount, unclassifiedFrameCount*100/totalFrameCount);

    SUF_MemoryFree (pLayer1PostActivation);                 // Free memory
    SUF_MemoryFree (pLayer2PostActivation);
    SUF_MemoryFree (pClassificationData);
    SUF_MemoryFree (pCategoricalValue);

    exit(0);
}


void parse_command_line (int argc, char *argv[])

{
    for (int argNum = 1; argNum < argc; argNum++) {
        if (*(argv[argNum]) == '-') {
            switch (*(argv[argNum]+1)) {                    // Get command letter
                case 'd':
                    debugFlag = 1;
                    printf ("Debug information = True\n");
                    break;

                case 'h':
                    show_help();
                    exit(0);
                    break;

                default:
                    printf ("Invalid parameter combination\n");
                    show_help();
                    exit(0);
                    break;
            }
        }
        else {
            printf ("Invalid parameter combination\n");
            show_help();
            exit(0);
        }
    }
}


void show_help (void)

{
    printf ("Usage:\n");
    printf ("network_classify_multi_category [params]\n");
    printf ("\t-d                   Display debug information\n");
    printf ("\t-h                   Help\n");
}

