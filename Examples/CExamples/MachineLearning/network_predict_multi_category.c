// SigLib Neural Network Classification Prediction Program
// Copyright (c) 2024 Delta Numerix

// Include files
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <siglib.h>               // SigLib DSP library
#include <gnuplot_c.h>            // Gnuplot/C
#include <siglib_host_utils.h>    // Optionally includes conio.h and time.h subset functions

// Define constants
#ifndef NETWORK_INPUT_SAMPLE_LENGTH
#  define NETWORK_INPUT_SAMPLE_LENGTH 120
#endif
#ifndef NETWORK_HIDDEN_LAYER_NODES
#  define NETWORK_HIDDEN_LAYER_NODES 20
#endif

#ifndef DEFAULT_PREDICTION_THRESHOLD
#  define DEFAULT_PREDICTION_THRESHOLD 0.8
#endif

#ifndef INPUT_SCALING_FACTOR
#  define INPUT_SCALING_FACTOR \
    (SIGLIB_ONE / 1000.)    // Scaling factor used to avoid numerical overflow in the
                            // exponent function
#endif

#define NUM_CATEGORIES 4    // Number of output categories

// Declare global variables and arrays
static char filenameWeights[2400];
static char filenameWeightsParameter[2000];

static SLData_t predictionThresholdLevel = DEFAULT_PREDICTION_THRESHOLD;
static SLArrayIndex_t predictionCountStore[NUM_CATEGORIES];
static SLArrayIndex_t correctCategoricalValue = -1;
static SLArrayIndex_t correctClassificationCount = 0;

static SLData_t layer1Weights[NETWORK_HIDDEN_LAYER_NODES][NETWORK_INPUT_SAMPLE_LENGTH];    // Declare weights for two
                                                                                           // transition paths
static SLData_t layer2Weights[NUM_CATEGORIES][NETWORK_HIDDEN_LAYER_NODES];

#ifndef HIDDEN_LAYER_ACTIVATION_TYPE
#  define HIDDEN_LAYER_ACTIVATION_TYPE SIGLIB_ACTIVATION_TYPE_RELU    // Activation type
#  define HIDDEN_LAYER_ACTIVATION_ALPHA SIGLIB_ZERO                   // Alpha value not required in this mode
#else
#  if (HIDDEN_LAYER_ACTIVATION_TYPE == 2)
#    define HIDDEN_LAYER_ACTIVATION_ALPHA 0.01    // Leaky ReLU alpha value
#  else
#    define HIDDEN_LAYER_ACTIVATION_ALPHA SIGLIB_ZERO    // Alpha value not required in this mode
#  endif
#endif

#ifndef OUTPUT_LAYER_ACTIVATION_TYPE
#  define OUTPUT_LAYER_ACTIVATION_TYPE SIGLIB_ACTIVATION_TYPE_LOGISTIC    // Activation type
#  define OUTPUT_LAYER_ACTIVATION_ALPHA SIGLIB_ZERO                       // Alpha value not required in this mode
#else
#  if (OUTPUT_LAYER_ACTIVATION_TYPE == 2)
#    define OUTPUT_LAYER_ACTIVATION_ALPHA 0.01    // Leaky ReLU alpha value
#  else
#    define OUTPUT_LAYER_ACTIVATION_ALPHA SIGLIB_ZERO    // Alpha value not required in this mode
#  endif
#endif

SLArrayIndex_t debugFlag = 0;

void parse_command_line(int argc, char* argv[]);
void show_help(void);

#if defined(_MSC_VER)
#  pragma warning(disable : 4127)    // Conditional expression is constant has been removed
#endif

int main(int argc, char* argv[])
{
  SLData_t* pLayer1PostActivation = SUF_VectorArrayAllocate(NETWORK_HIDDEN_LAYER_NODES);    // Allocate arrays
  SLData_t* pLayer2PostActivation = SUF_VectorArrayAllocate(NUM_CATEGORIES);

  if ((NULL == pLayer1PostActivation) || (NULL == pLayer2PostActivation)) {
    printf("\n\nMemory allocation failed\n\n");
    exit(-1);
  }

  filenameWeightsParameter[0] = 0;
  parse_command_line(argc, argv);    //  Parse command line options
  if (filenameWeightsParameter[0] == 0) {
    printf("Neural network weights filename not specified\n\n");
    exit(0);
  }

  printf("\nPredicting the classification of The Data ...\n");
  printf("    Number of input layer nodes  : %d\n", NETWORK_INPUT_SAMPLE_LENGTH);
  printf("    Number of hidden layer nodes : %d\n", NETWORK_HIDDEN_LAYER_NODES);
  printf("    Hidden Layer Activation Type : ");
  if (HIDDEN_LAYER_ACTIVATION_TYPE == SIGLIB_ACTIVATION_TYPE_RELU)
    printf("ReLU\n");
  else if (HIDDEN_LAYER_ACTIVATION_TYPE == SIGLIB_ACTIVATION_TYPE_LEAKY_RELU) {
    printf("Leaky ReLU\n");
    printf("    Hidden Layer Activation Alpha: %lf", (double)HIDDEN_LAYER_ACTIVATION_ALPHA);
  } else if (HIDDEN_LAYER_ACTIVATION_TYPE == SIGLIB_ACTIVATION_TYPE_LOGISTIC)
    printf("Logistic\n");
  else if (HIDDEN_LAYER_ACTIVATION_TYPE == SIGLIB_ACTIVATION_TYPE_TANH)
    printf("TanH\n");
  printf("    Output Layer Activation Type : ");
  if (OUTPUT_LAYER_ACTIVATION_TYPE == SIGLIB_ACTIVATION_TYPE_RELU)
    printf("ReLU\n\n");
  else if (OUTPUT_LAYER_ACTIVATION_TYPE == SIGLIB_ACTIVATION_TYPE_LEAKY_RELU) {
    printf("Leaky ReLU\n");
    printf("    Output Layer Activation Alpha: %lf", (double)HIDDEN_LAYER_ACTIVATION_ALPHA);
  } else if (OUTPUT_LAYER_ACTIVATION_TYPE == SIGLIB_ACTIVATION_TYPE_LOGISTIC)
    printf("Logistic\n\n");
  else if (OUTPUT_LAYER_ACTIVATION_TYPE == SIGLIB_ACTIVATION_TYPE_TANH)
    printf("TanH\n\n");

  for (SLArrayIndex_t i = 0; i < NUM_CATEGORIES; i++) {    // Clear prediction count store
    predictionCountStore[i] = 0;
  }

  // Load Prediction data
  SLArrayIndex_t nRows, nCols;
  SLData_t* pPredictionData = NULL;    // Pointer only - memory will be allocated in SUF_CsvReadMatrix()
  SUF_CsvReadMatrix(&pPredictionData, "PredictionDataSet.csv", SIGLIB_FIRST_ROW_KEEP, &nRows, &nCols);

  if ((nRows == 0) || (NULL == pPredictionData)) {
    printf("\n\nSUF_CsvReadMatrix (PredictionDataSet.csv) failure!.\n\n");
    exit(-1);
  }

  if (nCols != NETWORK_INPUT_SAMPLE_LENGTH) {
    printf("Error - Classification prediction sequences must be %d samples long\n", NETWORK_INPUT_SAMPLE_LENGTH);
    printf("        Provided sequence was %d samples long\n", nCols);
    exit(-1);
  }

  if (debugFlag == 1) {
    printf("nCols = NETWORK_INPUT_SAMPLE_LENGTH = %d\n", NETWORK_INPUT_SAMPLE_LENGTH);

    printf("Number of prediction sequences:   %d\n", nRows);
    printf("Prediction Data\n");
    SUF_PrintMatrix(pPredictionData, nRows, NETWORK_INPUT_SAMPLE_LENGTH);
  }

  // Scale all samples to avoid overflow in exponent function
  SDA_Multiply(pPredictionData,         // Pointer to source array
               INPUT_SCALING_FACTOR,    // Scalar value
               pPredictionData,         // Pointer to destination array
               nRows * nCols);          // Array length

#if defined(_MSC_VER)    // Defined by Microsoft compilers
  sprintf(filenameWeights, "weightCoefficientsFiles\\%s", filenameWeightsParameter);
#else
  sprintf(filenameWeights, "weightCoefficientsFiles/%s", filenameWeightsParameter);
#endif

  int numItems = SUF_ReadWeightsBinaryFile(filenameWeights,              // Filename
                                           (SLData_t*)layer1Weights,     // layer1Weights
                                           (SLData_t*)layer2Weights);    // layer2Weights

  if (numItems != (NETWORK_HIDDEN_LAYER_NODES * NETWORK_INPUT_SAMPLE_LENGTH) + (NUM_CATEGORIES * NETWORK_HIDDEN_LAYER_NODES)) {
    printf("**** ERROR - INCORRECT NUMBER OF WEIGHTS READ FROM BINARY FILE: "
           "%s\n\n",
           filenameWeights);
    exit(-1);
  }

  if (debugFlag == 1) {
    printf("layer1Weights\n");
    SUF_PrintMatrix((SLData_t*)layer1Weights, NETWORK_HIDDEN_LAYER_NODES, NETWORK_INPUT_SAMPLE_LENGTH);
    printf("layer2Weights\n\n");
    SUF_PrintMatrix((SLData_t*)layer2Weights, NUM_CATEGORIES, NETWORK_HIDDEN_LAYER_NODES);

    printf("layer1Weights Min: %lf\n", SDA_Min((SLData_t*)layer1Weights,
                                               NETWORK_HIDDEN_LAYER_NODES * NETWORK_INPUT_SAMPLE_LENGTH));    // Max and min values
    printf("layer1Weights Max: %lf\n", SDA_Max((SLData_t*)layer1Weights, NETWORK_HIDDEN_LAYER_NODES * NETWORK_INPUT_SAMPLE_LENGTH));
    printf("layer2Weights Min: %lf\n", SDA_Min((SLData_t*)layer2Weights, NUM_CATEGORIES * NETWORK_HIDDEN_LAYER_NODES));
    printf("layer2Weights Max: %lf\n\n", SDA_Max((SLData_t*)layer2Weights, NUM_CATEGORIES * NETWORK_HIDDEN_LAYER_NODES));
  }

  SLArrayIndex_t totalFrameCount = SIGLIB_AI_ZERO;
  SLArrayIndex_t classifiedFrameCount = SIGLIB_AI_ZERO;

  for (SLArrayIndex_t predictionSequenceNumber = 0; predictionSequenceNumber < nRows; predictionSequenceNumber++) {
    SLNeuralNetworkPrediction_s prediction = SDA_TwoLayerNCategoryNetworkPredict(
        pPredictionData + (predictionSequenceNumber * NETWORK_INPUT_SAMPLE_LENGTH),    // Pointer to data to predict
        (SLData_t*)layer1Weights,                                                      // Pointer to layer #1 weights
        (SLData_t*)layer2Weights,                                                      // Pointer to layer #2 weights
        pLayer1PostActivation,                                                         // Pointer to post activation for hidden
                                                                                       // layer
        pLayer2PostActivation,                                                         // Pointer to post activation for output
                                                                                       // layer
        HIDDEN_LAYER_ACTIVATION_TYPE,                                                  // Hidden layer activation type
        HIDDEN_LAYER_ACTIVATION_ALPHA,                                                 // Hidden layer alpha value
        OUTPUT_LAYER_ACTIVATION_TYPE,                                                  // Output layer activation type
        OUTPUT_LAYER_ACTIVATION_ALPHA,                                                 // Output layer alpha value
        NETWORK_INPUT_SAMPLE_LENGTH,                                                   // Input sample length
        NETWORK_HIDDEN_LAYER_NODES,                                                    // Hidden layer length
        NUM_CATEGORIES);                                                               // Number of output categories

    totalFrameCount++;

    if (predictionThresholdLevel < prediction.probability) {
      // printf ("Classified  : Classification: %d, Probability: %lf\n",
      // prediction.predictedCategory, prediction.probability);
      classifiedFrameCount++;

      predictionCountStore[prediction.predictedCategory] = predictionCountStore[prediction.predictedCategory] + 1;

      if (prediction.predictedCategory == correctCategoricalValue) {
        correctClassificationCount++;
      }
    }
  }

  printf("\n\nTotal Frame Count                : %d\n", totalFrameCount);
  printf("Classified Frame Count           : %d, %d%%\n", classifiedFrameCount, classifiedFrameCount * 100 / totalFrameCount);
  printf("Unclassified Frame Count         : %d, %d%%\n\n", totalFrameCount - classifiedFrameCount,
         (totalFrameCount - classifiedFrameCount) * 100 / totalFrameCount);

  printf("Categorical Predictions:\n");
  for (SLArrayIndex_t i = 0; i < NUM_CATEGORIES; i++) {
    printf("\tCategory: %d, Count: %d\n", i, predictionCountStore[i]);
  }
  printf("\n");

  if (-1 != correctCategoricalValue) {
    printf("Correct classification count     : %d\n", correctClassificationCount);
    printf("Incorrect classification count   : %d\n", classifiedFrameCount - correctClassificationCount);
    printf("Accuracy                         : %.2lf %%\n", (SLData_t)correctClassificationCount * SIGLIB_ONE_HUNDRED / (SLData_t)totalFrameCount);
  }

  SUF_MemoryFree(pLayer1PostActivation);    // Free memory
  SUF_MemoryFree(pLayer2PostActivation);
  SUF_MemoryFree(pPredictionData);

  return (0);
}

void parse_command_line(int argc, char* argv[])
{
  for (int argNum = 1; argNum < argc; argNum++) {
    if (*(argv[argNum]) == '-') {
      switch (*(argv[argNum] + 1)) {    // Get command letter
      case 'w':
        strcpy(filenameWeightsParameter, argv[argNum + 1]);
        argNum++;
        break;

      case 'c':
        correctCategoricalValue = atoi(argv[argNum + 1]);
        argNum++;
        break;

      case 'T':
        predictionThresholdLevel = atof(argv[argNum + 1]);
        argNum++;
        break;

      case 'd':
        debugFlag = 1;
        printf("Debug information = True\n");
        break;

      case 'h':
        show_help();
        exit(0);
        break;

      default:
        printf("Invalid parameter combination\n");
        show_help();
        exit(0);
        break;
      }
    } else {
      printf("Invalid parameter combination\n");
      show_help();
      exit(0);
    }
  }
}

void show_help(void)
{
  printf("Usage:\n");
  printf("network_predict_multi_category [params]\n");
  printf("\t-w filename          Neural Network weights file name (Required)\n");
  printf("\t-c category_number   Category number being searched for "
         "(Optional). If included then classifications with this number will "
         "be recorded as correct.\n");
  printf("\t-T threshold_level   Prediction threshold level. Levels below this "
         "will be ignored.\n");
  printf("\t-d                   Display debug information\n");
  printf("\t-h                   Help\n");
}
