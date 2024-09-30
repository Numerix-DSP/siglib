// SigLib Signal Neural Network Training Program
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
#  define NETWORK_INPUT_SAMPLE_LENGTH 128
#endif
#ifndef NETWORK_HIDDEN_LAYER_NODES
#  define NETWORK_HIDDEN_LAYER_NODES 5
#endif

#ifndef INPUT_SCALING_FACTOR
#  define INPUT_SCALING_FACTOR \
    (SIGLIB_ONE / 1000.)    // Scaling factor used to avoid numerical overflow in the
                            // exponent function
#endif

#define NUM_LAYERS 2    // Number of layers

#define DEFAULT_LEARNING_RATE .1    // Learning rate can be specified on the command line with '-r' option
#define DEFAULT_EPOCH_COUNT 5       // Epoch count can be specified on the command line with '-e' option
#define DEFAULT_QUANTIZATION_BITS \
  7    // Number of quantization bits for weights can be specified on the command
       // line with '-q' option

// Declare global variables and arrays
static char filename0[512];
static char filename1[512];
static char filenameWeights[1200];

static SLData_t shuffleRowTmp[NETWORK_INPUT_SAMPLE_LENGTH + 1];    // Remember line length = NETWORK_INPUT_SAMPLE_LENGTH+1,
                                                                   // for the 'mode' column

static SLData_t layer1Weights[NETWORK_HIDDEN_LAYER_NODES][NETWORK_INPUT_SAMPLE_LENGTH];    // Weights for each layer
static SLData_t layer2Weights[NETWORK_HIDDEN_LAYER_NODES];

#ifndef HIDDEN_LAYER_ACTIVATION_TYPE
#  define HIDDEN_LAYER_ACTIVATION_TYPE SIGLIB_ACTIVATION_TYPE_LOGISTIC    // Activation type
#  define HIDDEN_LAYER_ACTIVATION_ALPHA SIGLIB_ZERO                       // Alpha value not required in this mode
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

SLArrayIndex_t epochCount = DEFAULT_EPOCH_COUNT;
SLData_t learningRate = DEFAULT_LEARNING_RATE;
SLArrayIndex_t quantizationNeuralNetworkBits = DEFAULT_QUANTIZATION_BITS;
SLArrayIndex_t debugFlag = 0;

void parse_command_line(int argc, char* argv[]);
void show_help(void);

#if defined(_MSC_VER)
#  pragma warning(disable : 4127)    // Conditional expression is constant has been removed
#endif

int main(int argc, char* argv[])
{
  SLData_t* pLayer1PreActivation = SUF_VectorArrayAllocate(NETWORK_HIDDEN_LAYER_NODES);    // Allocate arrays
  SLData_t* pLayer1PostActivation = SUF_VectorArrayAllocate(NETWORK_HIDDEN_LAYER_NODES);

  if ((NULL == pLayer1PreActivation) || (NULL == pLayer1PostActivation)) {
    printf("\n\nMemory allocation failed\n\n");
    exit(-1);
  }

  filename0[0] = 0;
  filename1[0] = 0;

  parse_command_line(argc, argv);    //  Parse command line options

  if (0 == (filename0[0]) || 0 == (filename1[0])) {
    printf("Usage: filename\n");
    show_help();
    exit(0);
  }

  printf("\nTraining The Network ...\n");
  printf("    Filename #0                  : %s\n", filename0);
  printf("    Filename #1                  : %s\n", filename1);
  printf("    Number of input layer nodes  : %d\n", NETWORK_INPUT_SAMPLE_LENGTH);
  printf("    Number of hidden layer nodes : %d\n", NETWORK_HIDDEN_LAYER_NODES);
  printf("    Learning rate                : %lf\n", learningRate);
  printf("    Epoch count                  : %d\n", epochCount);
  printf("    Number of Quantization Bits  : %d\n", quantizationNeuralNetworkBits);
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

  SDA_SignalGenerate((SLData_t*)layer1Weights,                                     // Pointer to destination array
                     SIGLIB_WHITE_NOISE,                                           // Signal type - random white noise
                     SIGLIB_ONE,                                                   // Signal peak level
                     SIGLIB_FILL,                                                  // Fill (overwrite) or add to existing array contents
                     SIGLIB_ZERO,                                                  // Signal frequency - Unused
                     SIGLIB_ZERO,                                                  // D.C. Offset
                     SIGLIB_ZERO,                                                  // Unused
                     SIGLIB_ZERO,                                                  // Signal end value - Unused
                     SIGLIB_NULL_DATA_PTR,                                         // Unused
                     SIGLIB_NULL_DATA_PTR,                                         // Unused
                     NETWORK_HIDDEN_LAYER_NODES * NETWORK_INPUT_SAMPLE_LENGTH);    // Dataset length

  SDA_SignalGenerate(layer2Weights,                  // Pointer to destination array
                     SIGLIB_WHITE_NOISE,             // Signal type - random white noise
                     SIGLIB_ONE,                     // Signal peak level
                     SIGLIB_FILL,                    // Fill (overwrite) or add to existing array contents
                     SIGLIB_ZERO,                    // Signal frequency - Unused
                     SIGLIB_ZERO,                    // D.C. Offset
                     SIGLIB_ZERO,                    // Unused
                     SIGLIB_ZERO,                    // Signal end value - Unused
                     SIGLIB_NULL_DATA_PTR,           // Unused
                     SIGLIB_NULL_DATA_PTR,           // Unused
                     NETWORK_HIDDEN_LAYER_NODES);    // Dataset length

  // Save and/or restore random initial weight values to ensure multiple runs
  // are consistent
  //    SUF_BinWriteFile ((SLData_t *)layer1Weights, "rand_data1.bin",
  //    SIGLIB_LITTLE_ENDIAN,
  //    NETWORK_HIDDEN_LAYER_NODES*NETWORK_INPUT_SAMPLE_LENGTH); SUF_BinReadFile
  //    ((SLData_t *)layer1Weights, "rand_data1.bin", SIGLIB_LITTLE_ENDIAN,
  //    NETWORK_HIDDEN_LAYER_NODES*NETWORK_INPUT_SAMPLE_LENGTH);
  //    SUF_BinWriteFile ((SLData_t *)layer2Weights, "rand_data2.bin",
  //    SIGLIB_LITTLE_ENDIAN, NETWORK_HIDDEN_LAYER_NODES); SUF_BinReadFile
  //    ((SLData_t *)layer2Weights, "rand_data2.bin", SIGLIB_LITTLE_ENDIAN,
  //    NETWORK_HIDDEN_LAYER_NODES);

  if (debugFlag == 1) {
    printf("Initial layer1Weights Min: %lf\n", SDA_Min((SLData_t*)layer1Weights,
                                                       NETWORK_HIDDEN_LAYER_NODES * NETWORK_INPUT_SAMPLE_LENGTH));    // Max and min values
    printf("Initial layer1Weights Max: %lf\n", SDA_Max((SLData_t*)layer1Weights, NETWORK_HIDDEN_LAYER_NODES * NETWORK_INPUT_SAMPLE_LENGTH));
    printf("Initial layer2Weights Min: %lf\n", SDA_Min(layer2Weights, NETWORK_HIDDEN_LAYER_NODES));
    printf("Initial layer2Weights Max: %lf\n\n", SDA_Max(layer2Weights, NETWORK_HIDDEN_LAYER_NODES));
  }

  SDA_Fill(pLayer1PreActivation, SIGLIB_ZERO,
           NETWORK_HIDDEN_LAYER_NODES);    // Fill with zeros
  SDA_Fill(pLayer1PostActivation, SIGLIB_ZERO, NETWORK_HIDDEN_LAYER_NODES);

  // Load training data
  SLArrayIndex_t nRows, nCols;
  SLData_t* pTrainingData = NULL;    // Pointer only - memory will be allocated in SUF_CsvReadMatrix()
  SUF_CsvReadMatrix(&pTrainingData, "TrainingDataSet.csv", SIGLIB_FIRST_ROW_KEEP, &nRows, &nCols);

  if ((nRows == 0) || (NULL == pTrainingData)) {
    printf("\n\nSUF_CsvReadMatrix (TrainingDataSet.csv) failure!.\n\n");
    exit(-1);
  }

  if ((nCols - 1) != NETWORK_INPUT_SAMPLE_LENGTH) {
    printf("Error - Training sequences must be %d samples long\n", NETWORK_INPUT_SAMPLE_LENGTH);
    printf("        Provided sequence was %d samples long\n", nCols);
    exit(-1);
  }

  if (debugFlag == 1) {
    printf("nCols-1 = NETWORK_INPUT_SAMPLE_LENGTH = %d\n", NETWORK_INPUT_SAMPLE_LENGTH);

    printf("Number of training sequences:   %d\n", nRows);
    printf("Training Data\n");
    SUF_PrintMatrix(pTrainingData, nRows, NETWORK_INPUT_SAMPLE_LENGTH);
  }

  SLArrayIndex_t* pCategoricalValue = SUF_IndexArrayAllocate(nRows);           // Allocate the categorical value array
  SLData_t* pShuffledTrainingData = SUF_VectorArrayAllocate(nRows * nCols);    // Allocate the shuffled data array
  if ((NULL == pCategoricalValue) || (NULL == pShuffledTrainingData)) {
    printf("\n\nMemory allocation failed (pCategoricalValue and "
           "pShuffledTrainingData)\n\n");
    exit(-1);
  }

  for (SLArrayIndex_t e = 0; e < epochCount; e++) {
    // Randomly shuffle the rows in the training data frame
    SMX_ShuffleRows(pTrainingData, pShuffledTrainingData, shuffleRowTmp, nRows, nCols);

    SMX_ExtractCategoricalColumn(pShuffledTrainingData,    // Pointer to source matrix
                                 pCategoricalValue,        // Pointer to destination array
                                 nRows,                    // Number of rows in matrix
                                 nCols);                   // Number of columns in matrix
    SMX_DeleteOldColumn(pShuffledTrainingData,             // Pointer to source matrix
                        pShuffledTrainingData,             // Pointer to destination matrix
                        nCols - 1,                         // Column number to delete
                        nRows,                             // Number of rows in matrix
                        nCols);                            // Number of columns in matrix

    SLArrayIndex_t nInputNodes = nCols - 1;    // We have removed the categorical value column

    // Scale all samples to avoid overflow in exponent function
    SDA_Multiply(pShuffledTrainingData,    // Pointer to source array
                 INPUT_SCALING_FACTOR,     // Scalar value
                 pShuffledTrainingData,    // Pointer to destination array
                 nRows * nInputNodes);     // Array length

    // Train the network
    SDA_TwoLayer2CategoryNetworkFit(pShuffledTrainingData,            // Pointer to training data
                                    pCategoricalValue,                // Pointer to training signal type
                                    (SLData_t*)layer1Weights,         // Pointer to layer #1 weights
                                    layer2Weights,                    // Pointer to layer #2 weights
                                    pLayer1PreActivation,             // Pointer to pre activation for hidden layer
                                    pLayer1PostActivation,            // Pointer to post activation for hidden layer
                                    HIDDEN_LAYER_ACTIVATION_TYPE,     // Hidden layer activation type
                                    HIDDEN_LAYER_ACTIVATION_ALPHA,    // Hidden layer alpha value
                                    OUTPUT_LAYER_ACTIVATION_TYPE,     // Output layer activation type
                                    OUTPUT_LAYER_ACTIVATION_ALPHA,    // Output layer alpha value
                                    learningRate,                     // Learning rate
                                    nRows,                            // Number of training sequences
                                    NETWORK_INPUT_SAMPLE_LENGTH,      // Input sample length
                                    NETWORK_HIDDEN_LAYER_NODES);      // Hidden layer length
  }

  if (debugFlag == 1) {
    printf("\nweightCoefficientsFiles/weights%s_%s.dat\n", filename0, filename1);
    printf("layer1Weights\n");
    SUF_PrintMatrix((SLData_t*)layer1Weights, NETWORK_HIDDEN_LAYER_NODES, NETWORK_INPUT_SAMPLE_LENGTH);
    printf("layer2Weights\n\n");
    SUF_PrintMatrix((SLData_t*)layer2Weights, 1, NETWORK_HIDDEN_LAYER_NODES);
  }

  printf("layer1Weights Min: %lf\n", SDA_Min((SLData_t*)layer1Weights,
                                             NETWORK_HIDDEN_LAYER_NODES * NETWORK_INPUT_SAMPLE_LENGTH));    // Max and min values
  printf("layer1Weights Max: %lf\n", SDA_Max((SLData_t*)layer1Weights, NETWORK_HIDDEN_LAYER_NODES * NETWORK_INPUT_SAMPLE_LENGTH));
  printf("layer2Weights Min: %lf\n", SDA_Min(layer2Weights, NETWORK_HIDDEN_LAYER_NODES));
  printf("layer2Weights Max: %lf\n\n", SDA_Max(layer2Weights, NETWORK_HIDDEN_LAYER_NODES));

// Write weights to file : Binary plus C header : float and 8 bit words
#if defined(_MSC_VER)    // Defined by Microsoft compilers
  sprintf(filenameWeights, "weightCoefficientsFiles\\weights%s_%s.h", filename0, filename1);
#else
  sprintf(filenameWeights, "weightCoefficientsFiles/weights%s_%s.h", filename0, filename1);
#endif
  int numItems = SUF_WriteWeightsIntegerCFile(filenameWeights,                                             // Filename
                                              (SLData_t*)layer1Weights,                                    // layer1Weights
                                              (SLData_t*)layer2Weights,                                    // layer2Weights
                                              NETWORK_HIDDEN_LAYER_NODES * NETWORK_INPUT_SAMPLE_LENGTH,    // layer1Weights array length
                                              NETWORK_HIDDEN_LAYER_NODES,                                  // layer2Weights array length
                                              NUM_LAYERS);                                                 // Number of layers
  if (numItems != (NETWORK_HIDDEN_LAYER_NODES * NETWORK_INPUT_SAMPLE_LENGTH) + (NETWORK_HIDDEN_LAYER_NODES)) {
    printf("**** ERROR - INCORRECT NUMBER OF WEIGHTS WRITTEN TO INTEGER C "
           "HEADER FILE: %s\n\n",
           filenameWeights);
    printf("**** ERROR - NUMBER OF WEIGHTS WRITTEN: %d\n\n", numItems);
    exit(-1);
  }

#if defined(_MSC_VER)    // Defined by Microsoft compilers
  sprintf(filenameWeights, "weightCoefficientsFiles\\weightsFloat%s_%s.h", filename0, filename1);
#else
  sprintf(filenameWeights, "weightCoefficientsFiles/weightsFloat%s_%s.h", filename0, filename1);
#endif
  numItems = SUF_WriteWeightsFloatCFile(filenameWeights,                                             // Filename
                                        (SLData_t*)layer1Weights,                                    // layer1Weights
                                        (SLData_t*)layer2Weights,                                    // layer2Weights
                                        NETWORK_HIDDEN_LAYER_NODES * NETWORK_INPUT_SAMPLE_LENGTH,    // layer1Weights array length
                                        NETWORK_HIDDEN_LAYER_NODES,                                  // layer2Weights array length
                                        NUM_LAYERS);                                                 // Number of layers
  if (numItems != (NETWORK_HIDDEN_LAYER_NODES * NETWORK_INPUT_SAMPLE_LENGTH) + (NETWORK_HIDDEN_LAYER_NODES)) {
    printf("**** ERROR - INCORRECT NUMBER OF WEIGHTS WRITTEN TO FLOATING POINT "
           "C HEADER FILE: %s\n\n",
           filenameWeights);
    printf("**** ERROR - NUMBER OF WEIGHTS WRITTEN: %d\n\n", numItems);
    exit(-1);
  }

#if defined(_MSC_VER)    // Defined by Microsoft compilers
  sprintf(filenameWeights, "weightCoefficientsFiles\\weights%s_%s.dat", filename0, filename1);
#else
  sprintf(filenameWeights, "weightCoefficientsFiles/weights%s_%s.dat", filename0, filename1);
#endif

  numItems = SUF_WriteWeightsBinaryFile(filenameWeights,                                             // Filename
                                        (SLData_t*)layer1Weights,                                    // layer1Weights
                                        (SLData_t*)layer2Weights,                                    // layer2Weights
                                        NETWORK_HIDDEN_LAYER_NODES * NETWORK_INPUT_SAMPLE_LENGTH,    // layer1Weights array length
                                        NETWORK_HIDDEN_LAYER_NODES,                                  // layer2Weights array length
                                        NUM_LAYERS,                                                  // Number of layers
                                        quantizationNeuralNetworkBits);                              // Number of quantization bits
  if (numItems != (NETWORK_HIDDEN_LAYER_NODES * NETWORK_INPUT_SAMPLE_LENGTH) + (NETWORK_HIDDEN_LAYER_NODES)) {
    printf("**** ERROR - INCORRECT NUMBER OF WEIGHTS WRITTEN TO BINARY FILE: "
           "%s\n\n",
           filenameWeights);
    printf("**** ERROR - NUMBER OF WEIGHTS WRITTEN: %d\n\n", numItems);
    exit(-1);
  }

  SUF_MemoryFree(pLayer1PreActivation);    // Free memory
  SUF_MemoryFree(pLayer1PostActivation);
  SUF_MemoryFree(pTrainingData);
  SUF_MemoryFree(pShuffledTrainingData);
  SUF_MemoryFree(pCategoricalValue);

  return (0);
}

void parse_command_line(int argc, char* argv[])
{
  for (int argNum = 1; argNum < argc; argNum++) {
    if (*(argv[argNum]) == '-') {
      switch (*(argv[argNum] + 1)) {    // Get command letter
      case 'e':
        epochCount = atoi(argv[argNum + 1]);
        argNum++;
        break;

      case 'r':
        learningRate = atof(argv[argNum + 1]);
        argNum++;
        break;

      case 'q':
        quantizationNeuralNetworkBits = atoi(argv[argNum + 1]);
        argNum++;
        break;

      case '0':
        strcpy(filename0, argv[argNum + 1]);
        argNum++;
        break;

      case '1':
        strcpy(filename1, argv[argNum + 1]);
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
  printf("network_train_single_category [params]\n");
  printf("\t-0 filename          Filename #0 (Required)\n");
  printf("\t-1 filename          Filename #1 (Required)\n");
  printf("\t-r Rate              Learning Rate\n");
  printf("\t-q #bits             Number of Quantization Bits\n");
  printf("\t-e Count             Epoch Count\n");
  printf("\t-d                   Display debug information\n");
  printf("\t-h                   Help\n");
}
