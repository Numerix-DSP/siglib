
/******************************************************************************
File Name               : machinelearning.c | Author        : JOHN EDWARDS
Siglib Library Version  : 10.50             |
--------------------------------------------+----------------------------------
Compiler  : Independent                     | Start Date    : 25/05/2022
Options   :                                 | Latest Update : 07/12/2022
-------------------------------------------------------------------------------
Support for SigLib is available via Email: support@numerix-dsp.com

This file may be modified, in any way, providing that this header remains
within the file and none of the header contents are removed or modified.

SigLib is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License V2, as
published by the Free Software Foundation.

SigLib is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1335, USA

This sofware is also available with a commercial license, for use in
proprietary, research, government or commercial applications.
Please contact Delta Numerix for further details :
https://www.numerix-dsp.com
support@.numerix-dsp.com

Copyright (c) 2023 Delta Numerix All rights reserved.
---------------------------------------------------------------------------
Description: DSP basic math functions, for SigLib DSP library.

****************************************************************************/

#define SIGLIB_SRC_FILE_MACHINELEARNING 1    // Defines the source file that code is being used in

#include <siglib.h>    // Include SigLib header file

#if __GNUC__
#  pragma GCC diagnostic ignored "-Wunused-result"
#endif

/**/

/********************************************************
 * Function: SDA_TwoLayer2CategoryNetworkFit
 *
 * Parameters:
 *   const SLData_t *,               - pTrainingData
 *   const SLArrayIndex_t *,         - pCategoricalValue
 *   SLData_t *,                     - pLayer1Weights
 *   SLData_t *,                     - pLayer2Weights
 *   SLData_t *,                     - pLayer1PreActivation
 *   SLData_t *,                     - pLayer1PostActivation
 *   const enum SLActivationType_t   - layer1ActivationType
 *   const SLData_t,                 - layer1ActivationAlpha
 *   const enum SLActivationType_t   - layer2ActivationType
 *   const SLData_t,                 - layer2ActivationAlpha
 *   const SLData_t,                 - learningRate
 *   const SLArrayIndex_t,           - numberOfTrainingSequences
 *   const SLArrayIndex_t,           - numberOfInputNodes
 *   const SLArrayIndex_t)           - numberOfLayer1Nodes
 *
 * Return value:
 *   void
 *
 * Description:
 *   Fit the 2 category model to the data
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_TwoLayer2CategoryNetworkFit(const SLData_t* pTrainingData, const SLArrayIndex_t* pCategoricalValue,
                                                      SLData_t* pLayer1Weights, SLData_t* pLayer2Weights, SLData_t* pLayer1PreActivation,
                                                      SLData_t* pLayer1PostActivation, const enum SLActivationType_t layer1ActivationType,
                                                      const SLData_t layer1ActivationAlpha, const enum SLActivationType_t layer2ActivationType,
                                                      const SLData_t layer2ActivationAlpha, const SLData_t learningRate,
                                                      const SLArrayIndex_t numberOfTrainingSequences, const SLArrayIndex_t numberOfInputNodes,
                                                      const SLArrayIndex_t numberOfLayer1Nodes)
{
  // Forward propagation
  for (SLArrayIndex_t trainingSequenceNumber = 0; trainingSequenceNumber < numberOfTrainingSequences; trainingSequenceNumber++) {
    for (SLArrayIndex_t layer1Node = 0; layer1Node < numberOfLayer1Nodes; layer1Node++) {
      pLayer1PreActivation[layer1Node] =
          SDA_RealDotProduct(pTrainingData + (trainingSequenceNumber * numberOfInputNodes),    // Source vector 1 pointer
                             pLayer1Weights + (layer1Node * numberOfInputNodes),               // Source vector 2 pointer
                             numberOfInputNodes);                                              // Vector lengths

      switch (layer1ActivationType) {    // Choose required activation function
      case SIGLIB_ACTIVATION_TYPE_LEAKY_RELU:
        *(pLayer1PostActivation + layer1Node) = SDS_ActivationLeakyReLU(pLayer1PreActivation[layer1Node], layer1ActivationAlpha);
        break;
      case SIGLIB_ACTIVATION_TYPE_LOGISTIC:
        *(pLayer1PostActivation + layer1Node) = SDS_ActivationLogistic(pLayer1PreActivation[layer1Node]);
        break;
      case SIGLIB_ACTIVATION_TYPE_TANH:
        *(pLayer1PostActivation + layer1Node) = SDS_ActivationTanH(pLayer1PreActivation[layer1Node]);
        break;
      case SIGLIB_ACTIVATION_TYPE_RELU:
      default:
        *(pLayer1PostActivation + layer1Node) = SDS_ActivationReLU(pLayer1PreActivation[layer1Node]);
        break;
      }
    }

    SLData_t layer2PreActivation = SDA_RealDotProduct(pLayer1PostActivation,    // Source vector 1 pointer
                                                      pLayer2Weights,           // Source vector 2 pointer
                                                      numberOfLayer1Nodes);     // Vector lengths

    SLData_t layer2PostActivation;
    switch (layer2ActivationType) {    // Choose required activation function
    case SIGLIB_ACTIVATION_TYPE_LEAKY_RELU:
      layer2PostActivation = SDS_ActivationLeakyReLU(layer2PreActivation, layer2ActivationAlpha);
      break;
    case SIGLIB_ACTIVATION_TYPE_LOGISTIC:
      layer2PostActivation = SDS_ActivationLogistic(layer2PreActivation);
      break;
    case SIGLIB_ACTIVATION_TYPE_TANH:
      layer2PostActivation = SDS_ActivationTanH(layer2PreActivation);
      break;
    case SIGLIB_ACTIVATION_TYPE_RELU:
    default:
      layer2PostActivation = SDS_ActivationReLU(layer2PreActivation);
      break;
    }

    SLData_t layer2Error = (SLData_t)pCategoricalValue[trainingSequenceNumber] - layer2PostActivation;

    // Backward propagation
    for (SLArrayIndex_t layer1Node = 0; layer1Node < numberOfLayer1Nodes; layer1Node++) {
      SLData_t layer1Error;
      switch (layer2ActivationType) {    // Choose required activation derivative
                                         // function
      case SIGLIB_ACTIVATION_TYPE_LEAKY_RELU:
        layer1Error = layer2Error * SDS_ActivationLeakyReLUDerivative(layer2PreActivation, layer2ActivationAlpha);
        break;
      case SIGLIB_ACTIVATION_TYPE_LOGISTIC:
        layer1Error = layer2Error * SDS_ActivationLogisticDerivative(layer2PreActivation);
        break;
      case SIGLIB_ACTIVATION_TYPE_TANH:
        layer1Error = layer2Error * SDS_ActivationTanHDerivative(layer2PreActivation);
        break;
      case SIGLIB_ACTIVATION_TYPE_RELU:
      default:
        layer1Error = layer2Error * SDS_ActivationReLUDerivative(layer2PreActivation);
        break;
      }

      SLData_t layer2Gradient = layer1Error * *(pLayer1PostActivation + layer1Node);

      for (SLArrayIndex_t inputLayerNode = 0; inputLayerNode < numberOfInputNodes; inputLayerNode++) {
        SLData_t trainingDataSample = pTrainingData[(trainingSequenceNumber * numberOfInputNodes) + inputLayerNode];
        SLData_t layer1Gradient;
        switch (layer1ActivationType) {    // Choose required activation derivative
                                           // function
        case SIGLIB_ACTIVATION_TYPE_LEAKY_RELU:
          layer1Gradient = layer1Error * *(pLayer2Weights + layer1Node) * trainingDataSample *
                           SDS_ActivationLeakyReLUDerivative(pLayer1PreActivation[layer1Node], layer1ActivationAlpha);
          break;
        case SIGLIB_ACTIVATION_TYPE_LOGISTIC:
          layer1Gradient = layer1Error * *(pLayer2Weights + layer1Node) * trainingDataSample *
                           SDS_ActivationLogisticDerivative(pLayer1PreActivation[layer1Node]);
          break;
        case SIGLIB_ACTIVATION_TYPE_TANH:
          layer1Gradient =
              layer1Error * *(pLayer2Weights + layer1Node) * trainingDataSample * SDS_ActivationTanHDerivative(pLayer1PreActivation[layer1Node]);
          break;
        case SIGLIB_ACTIVATION_TYPE_RELU:
        default:
          layer1Gradient =
              layer1Error * *(pLayer2Weights + layer1Node) * trainingDataSample * SDS_ActivationReLUDerivative(pLayer1PreActivation[layer1Node]);
          break;
        }

        *(pLayer1Weights + (layer1Node * numberOfInputNodes) + inputLayerNode) += learningRate * layer1Gradient;
      }
      *(pLayer2Weights + layer1Node) += learningRate * layer2Gradient;
    }
  }
}    // End of SDA_TwoLayer2CategoryNetworkFit()

/**/

/********************************************************
 * Function: SDA_TwoLayer2CategoryNetworkPredict
 *
 * Parameters:
 *   const SLData_t *,               - pData
 *   SLData_t *,                     - pLayer1Weights
 *   SLData_t *,                     - pLayer2Weights
 *   SLData_t *,                     - pLayer1PostActivation
 *   SLData_t *,                     - pLayer2Activation
 *   const enum SLActivationType_t   - layer1ActivationType
 *   const SLData_t,                 - layer1ActivationAlpha
 *   const enum SLActivationType_t   - layer2ActivationType
 *   const SLData_t,                 - layer2ActivationAlpha
 *   const SLData_t,                 - classificationThreshold
 *   const SLArrayIndex_t,           - numberOfInputNodes
 *   const SLArrayIndex_t)           - numberOfLayer1Nodes
 *
 * Return value:
 *   Predicted category and associated probability
 *
 * Description:
 *   Use the model to predict the category of the data
 *
 ********************************************************/

SLNeuralNetworkPrediction_s SIGLIB_FUNC_DECL SDA_TwoLayer2CategoryNetworkPredict(
    const SLData_t* pData, const SLData_t* pLayer1Weights, const SLData_t* pLayer2Weights, SLData_t* pLayer1PostActivation,
    SLData_t* pLayer2Activation, const enum SLActivationType_t layer1ActivationType, const SLData_t layer1ActivationAlpha,
    const enum SLActivationType_t layer2ActivationType, const SLData_t layer2ActivationAlpha, const SLData_t classificationThreshold,
    const SLArrayIndex_t numberOfInputNodes, const SLArrayIndex_t numberOfLayer1Nodes)
{
  // Forward propagation
  for (SLArrayIndex_t layer1Node = 0; layer1Node < numberOfLayer1Nodes; layer1Node++) {
    SLData_t layer1PreActivation = SDA_RealDotProduct(pData,                                                 // Source vector 1 pointer
                                                      pLayer1Weights + (layer1Node * numberOfInputNodes),    // Source vector 2 pointer
                                                      numberOfInputNodes);                                   // Vector lengths

    switch (layer1ActivationType) {    // Choose required activation function
    case SIGLIB_ACTIVATION_TYPE_LEAKY_RELU:
      *(pLayer1PostActivation + layer1Node) = SDS_ActivationLeakyReLU(layer1PreActivation, layer1ActivationAlpha);
      break;
    case SIGLIB_ACTIVATION_TYPE_LOGISTIC:
      *(pLayer1PostActivation + layer1Node) = SDS_ActivationLogistic(layer1PreActivation);
      break;
    case SIGLIB_ACTIVATION_TYPE_TANH:
      *(pLayer1PostActivation + layer1Node) = SDS_ActivationTanH(layer1PreActivation);
      break;
    case SIGLIB_ACTIVATION_TYPE_RELU:
    default:
      *(pLayer1PostActivation + layer1Node) = SDS_ActivationReLU(layer1PreActivation);
      break;
    }
  }

  SLData_t layer2PreActivation = SDA_RealDotProduct(pLayer1PostActivation,    // Source vector 1 pointer
                                                    pLayer2Weights,           // Source vector 2 pointer
                                                    numberOfLayer1Nodes);     // Vector lengths

  SLData_t layer2PostActivation;
  switch (layer2ActivationType) {    // Choose required activation function
  case SIGLIB_ACTIVATION_TYPE_LEAKY_RELU:
    layer2PostActivation = SDS_ActivationLeakyReLU(layer2PreActivation, layer2ActivationAlpha);
    break;
  case SIGLIB_ACTIVATION_TYPE_LOGISTIC:
    layer2PostActivation = SDS_ActivationLogistic(layer2PreActivation);
    break;
  case SIGLIB_ACTIVATION_TYPE_TANH:
    layer2PostActivation = SDS_ActivationTanH(layer2PreActivation);
    break;
  case SIGLIB_ACTIVATION_TYPE_RELU:
  default:
    layer2PostActivation = SDS_ActivationReLU(layer2PreActivation);
    break;
  }

  SLNeuralNetworkPrediction_s prediction;
  if (layer2PostActivation > classificationThreshold) {
    prediction.predictedCategory = SIGLIB_AI_ONE;
  } else {
    prediction.predictedCategory = SIGLIB_AI_ZERO;
  }

  // Search for category with max activation level
  prediction.probability = layer2PostActivation;

  *pLayer2Activation = layer2PostActivation;    // Used for visualization

  return (prediction);
}    // End of SDA_TwoLayer2CategoryNetworkPredict()

/**/

/********************************************************
 * Function: SDA_TwoLayerNCategoryNetworkFit
 *
 * Parameters:
 *   const SLData_t *,               - pTrainingData
 *   const SLArrayIndex_t *,         - pCategoricalValue
 *   SLData_t *,                     - pLayer1Weights
 *   SLData_t *,                     - pLayer2Weights
 *   SLData_t *,                     - pLayer1PreActivation
 *   SLData_t *,                     - pLayer1PostActivation
 *   const enum SLActivationType_t   - layer1ActivationType
 *   const SLData_t,                 - layer1ActivationAlpha
 *   const enum SLActivationType_t   - layer2ActivationType
 *   const SLData_t,                 - layer2ActivationAlpha
 *   const SLData_t,                 - learningRate
 *   const SLArrayIndex_t,           - numberOfTrainingSequences
 *   const SLArrayIndex_t,           - numberOfInputNodes
 *   const SLArrayIndex_t)           - numberOfLayer1Nodes
 *
 * Return value:
 *   void
 *
 * Description:
 *   Fit the N category model to the data
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_TwoLayerNCategoryNetworkFit(const SLData_t* pTrainingData, const SLArrayIndex_t* pCategoricalValues,
                                                      SLData_t* pLayer1Weights, SLData_t* pLayer2Weights, SLData_t* pLayer1PreActivation,
                                                      SLData_t* pLayer1PostActivation, SLData_t* pLayer2PostActivation,
                                                      const enum SLActivationType_t layer1ActivationType, const SLData_t layer1ActivationAlpha,
                                                      const enum SLActivationType_t layer2ActivationType, const SLData_t layer2ActivationAlpha,
                                                      const SLData_t learningRate, const SLArrayIndex_t numberOfTrainingSequences,
                                                      const SLArrayIndex_t numberOfInputNodes, const SLArrayIndex_t numberOfLayer1Nodes,
                                                      const SLArrayIndex_t numberOfCategories)
{
  // Forward propagation
  for (SLArrayIndex_t trainingSequenceNumber = 0; trainingSequenceNumber < numberOfTrainingSequences; trainingSequenceNumber++) {
    SLData_t layer2PreActivation;

    for (SLArrayIndex_t layer1Node = 0; layer1Node < numberOfLayer1Nodes; layer1Node++) {
      pLayer1PreActivation[layer1Node] =
          SDA_RealDotProduct(pTrainingData + (trainingSequenceNumber * numberOfInputNodes),    // Source vector 1 pointer
                             pLayer1Weights + (layer1Node * numberOfInputNodes),               // Source vector 2 pointer
                             numberOfInputNodes);                                              // Vector lengths

      switch (layer1ActivationType) {    // Choose required activation function
      case SIGLIB_ACTIVATION_TYPE_LEAKY_RELU:
        *(pLayer1PostActivation + layer1Node) = SDS_ActivationLeakyReLU(pLayer1PreActivation[layer1Node], layer1ActivationAlpha);
        break;
      case SIGLIB_ACTIVATION_TYPE_LOGISTIC:
        *(pLayer1PostActivation + layer1Node) = SDS_ActivationLogistic(pLayer1PreActivation[layer1Node]);
        break;
      case SIGLIB_ACTIVATION_TYPE_TANH:
        *(pLayer1PostActivation + layer1Node) = SDS_ActivationTanH(pLayer1PreActivation[layer1Node]);
        break;
      case SIGLIB_ACTIVATION_TYPE_RELU:
      default:
        *(pLayer1PostActivation + layer1Node) = SDS_ActivationReLU(pLayer1PreActivation[layer1Node]);
        break;
      }
    }

    for (SLArrayIndex_t layer2Node = 0; layer2Node < numberOfCategories; layer2Node++) {
      layer2PreActivation = SDA_RealDotProduct(pLayer1PostActivation,                                  // Source vector 1 pointer
                                               pLayer2Weights + (layer2Node * numberOfLayer1Nodes),    // Source vector 2 pointer
                                               numberOfLayer1Nodes);                                   // Vector lengths

      switch (layer2ActivationType) {    // Choose required activation function
      case SIGLIB_ACTIVATION_TYPE_LEAKY_RELU:
        *(pLayer2PostActivation + layer2Node) = SDS_ActivationLeakyReLU(layer2PreActivation, layer2ActivationAlpha);
        break;
      case SIGLIB_ACTIVATION_TYPE_LOGISTIC:
        *(pLayer2PostActivation + layer2Node) = SDS_ActivationLogistic(layer2PreActivation);
        break;
      case SIGLIB_ACTIVATION_TYPE_TANH:
        *(pLayer2PostActivation + layer2Node) = SDS_ActivationTanH(layer2PreActivation);
        break;
      case SIGLIB_ACTIVATION_TYPE_RELU:
      default:
        *(pLayer2PostActivation + layer2Node) = SDS_ActivationReLU(layer2PreActivation);
        break;
      }
    }

    SLData_t layer2Error;
    // Backward propagation
    for (SLArrayIndex_t layer2Node = 0; layer2Node < numberOfCategories; layer2Node++) {
      if (layer2Node == pCategoricalValues[trainingSequenceNumber]) {
        layer2Error = SIGLIB_ONE - *(pLayer2PostActivation + layer2Node);
      } else {
        layer2Error = -*(pLayer2PostActivation + layer2Node);
      }

      for (SLArrayIndex_t layer1Node = 0; layer1Node < numberOfLayer1Nodes; layer1Node++) {
        SLData_t layer1Error;
        switch (layer2ActivationType) {    // Choose required activation derivative
                                           // function
        case SIGLIB_ACTIVATION_TYPE_LEAKY_RELU:
          layer1Error = layer2Error * SDS_ActivationLeakyReLUDerivative(layer2PreActivation, layer2ActivationAlpha);
          break;
        case SIGLIB_ACTIVATION_TYPE_LOGISTIC:
          layer1Error = layer2Error * SDS_ActivationLogisticDerivative(layer2PreActivation);
          break;
        case SIGLIB_ACTIVATION_TYPE_TANH:
          layer1Error = layer2Error * SDS_ActivationTanHDerivative(layer2PreActivation);
          break;
        case SIGLIB_ACTIVATION_TYPE_RELU:
        default:
          layer1Error = layer2Error * SDS_ActivationReLUDerivative(layer2PreActivation);
          break;
        }

        SLData_t layer2Gradient = layer1Error * *(pLayer1PostActivation + layer1Node);

        for (SLArrayIndex_t inputLayerNode = 0; inputLayerNode < numberOfInputNodes; inputLayerNode++) {
          SLData_t trainingDataSample = pTrainingData[(trainingSequenceNumber * numberOfInputNodes) + inputLayerNode];
          SLData_t layer1Gradient;
          switch (layer1ActivationType) {    // Choose required activation
                                             // derivative function
          case SIGLIB_ACTIVATION_TYPE_LEAKY_RELU:
            layer1Gradient = layer1Error * pLayer2Weights[(layer2Node * numberOfLayer1Nodes) + layer1Node] * trainingDataSample *
                             SDS_ActivationLeakyReLUDerivative(pLayer1PreActivation[layer1Node], layer1ActivationAlpha);
            break;
          case SIGLIB_ACTIVATION_TYPE_LOGISTIC:
            layer1Gradient = layer1Error * pLayer2Weights[(layer2Node * numberOfLayer1Nodes) + layer1Node] * trainingDataSample *
                             SDS_ActivationLogisticDerivative(pLayer1PreActivation[layer1Node]);
            break;
          case SIGLIB_ACTIVATION_TYPE_TANH:
            layer1Gradient = layer1Error * pLayer2Weights[(layer2Node * numberOfLayer1Nodes) + layer1Node] * trainingDataSample *
                             SDS_ActivationTanHDerivative(pLayer1PreActivation[layer1Node]);
            break;
          case SIGLIB_ACTIVATION_TYPE_RELU:
          default:
            layer1Gradient = layer1Error * pLayer2Weights[(layer2Node * numberOfLayer1Nodes) + layer1Node] * trainingDataSample *
                             SDS_ActivationReLUDerivative(pLayer1PreActivation[layer1Node]);
            break;
          }

          *(pLayer1Weights + (layer1Node * numberOfInputNodes) + inputLayerNode) += learningRate * layer1Gradient;
        }
        *(pLayer2Weights + (layer2Node * numberOfLayer1Nodes) + layer1Node) += learningRate * layer2Gradient;
      }
    }
  }
}    // End of SDA_TwoLayerNCategoryNetworkFit()

/**/

/********************************************************
 * Function: SDA_TwoLayerNCategoryNetworkPredict
 *
 * Parameters:
 *   const SLData_t *,               - pData
 *   SLData_t *,                     - pLayer1Weights
 *   SLData_t *,                     - pLayer2Weights
 *   SLData_t *,                     - pLayer1PostActivation
 *   SLData_t *,                     - pLayer2PostActivation
 *   const enum SLActivationType_t   - layer1ActivationType
 *   const SLData_t,                 - layer1ActivationAlpha
 *   const enum SLActivationType_t   - layer2ActivationType
 *   const SLData_t,                 - layer2ActivationAlpha
 *   const SLArrayIndex_t,           - numberOfInputNodes
 *   const SLArrayIndex_t,           - numberOfLayer1Nodes
 *   const SLArrayIndex_t)           - numberOfCategories
 *
 * Return value:
 *   Predicted category and associated probability
 *
 * Description:
 *   Use the model to predict the category of the data
 *
 ********************************************************/

SLNeuralNetworkPrediction_s SIGLIB_FUNC_DECL SDA_TwoLayerNCategoryNetworkPredict(
    const SLData_t* pData, const SLData_t* pLayer1Weights, const SLData_t* pLayer2Weights, SLData_t* pLayer1PostActivation,
    SLData_t* pLayer2PostActivation, const enum SLActivationType_t layer1ActivationType, const SLData_t layer1ActivationAlpha,
    const enum SLActivationType_t layer2ActivationType, const SLData_t layer2ActivationAlpha, const SLArrayIndex_t numberOfInputNodes,
    const SLArrayIndex_t numberOfLayer1Nodes, const SLArrayIndex_t numberOfCategories)
{
  // Forward propagation
  // First layer
  for (SLArrayIndex_t layer1Node = 0; layer1Node < numberOfLayer1Nodes; layer1Node++) {
    SLData_t layer1PreActivation = SDA_RealDotProduct(pData,                                                 // Source vector 1 pointer
                                                      pLayer1Weights + (layer1Node * numberOfInputNodes),    // Source vector 2 pointer
                                                      numberOfInputNodes);                                   // Vector lengths

    switch (layer1ActivationType) {    // Choose required activation function
    case SIGLIB_ACTIVATION_TYPE_LEAKY_RELU:
      *(pLayer1PostActivation + layer1Node) = SDS_ActivationLeakyReLU(layer1PreActivation, layer1ActivationAlpha);
      break;
    case SIGLIB_ACTIVATION_TYPE_LOGISTIC:
      *(pLayer1PostActivation + layer1Node) = SDS_ActivationLogistic(layer1PreActivation);
      break;
    case SIGLIB_ACTIVATION_TYPE_TANH:
      *(pLayer1PostActivation + layer1Node) = SDS_ActivationTanH(layer1PreActivation);
      break;
    case SIGLIB_ACTIVATION_TYPE_RELU:
    default:
      *(pLayer1PostActivation + layer1Node) = SDS_ActivationReLU(layer1PreActivation);
      break;
    }
  }

  // Second layer
  for (SLArrayIndex_t layer2Node = 0; layer2Node < numberOfCategories; layer2Node++) {
    SLData_t layer2PreActivation = SDA_RealDotProduct(pLayer1PostActivation,                                  // Source vector 1 pointer
                                                      pLayer2Weights + (layer2Node * numberOfLayer1Nodes),    // Source vector 2 pointer
                                                      numberOfLayer1Nodes);                                   // Vector lengths

    switch (layer2ActivationType) {    // Choose required activation function
    case SIGLIB_ACTIVATION_TYPE_LEAKY_RELU:
      *(pLayer2PostActivation + layer2Node) = SDS_ActivationLeakyReLU(layer2PreActivation, layer2ActivationAlpha);
      break;
    case SIGLIB_ACTIVATION_TYPE_LOGISTIC:
      *(pLayer2PostActivation + layer2Node) = SDS_ActivationLogistic(layer2PreActivation);
      break;
    case SIGLIB_ACTIVATION_TYPE_TANH:
      *(pLayer2PostActivation + layer2Node) = SDS_ActivationTanH(layer2PreActivation);
      break;
    case SIGLIB_ACTIVATION_TYPE_RELU:
    default:
      *(pLayer2PostActivation + layer2Node) = SDS_ActivationReLU(layer2PreActivation);
      break;
    }
  }

  SLNeuralNetworkPrediction_s prediction;
  // Search for category with max activation level
  prediction.predictedCategory = SDA_MaxIndex(pLayer2PostActivation, numberOfCategories);
  prediction.probability = pLayer2PostActivation[prediction.predictedCategory];

  return (prediction);    // Category encoding: 0 to N-1
}    // End of SDA_TwoLayerNCategoryNetworkPredict()

/**/

/********************************************************
 * Function: SDA_TwoLayer2CategoryWithBiasesNetworkFit
 *
 * Parameters:
 *   const SLData_t *,               - pTrainingData
 *   const SLArrayIndex_t *,         - pCategoricalValue
 *   SLData_t *,                     - pLayer1Weights
 *   SLData_t *,                     - pLayer1Biases
 *   SLData_t *,                     - pLayer2Weights
 *   SLData_t *,                     - pLayer2Biases
 *   SLData_t *,                     - pLayer1PreActivation
 *   SLData_t *,                     - pLayer1BiasPreActivation
 *   SLData_t *,                     - pLayer1PostActivation
 *   const enum SLActivationType_t   - layer1ActivationType
 *   const SLData_t,                 - layer1ActivationAlpha
 *   const enum SLActivationType_t   - layer2ActivationType
 *   const SLData_t,                 - layer2ActivationAlpha
 *   const SLData_t,                 - learningRate
 *   const SLArrayIndex_t,           - numberOfTrainingSequences
 *   const SLArrayIndex_t,           - numberOfInputNodes
 *   const SLArrayIndex_t)           - numberOfLayer1Nodes
 *
 * Return value:
 *   void
 *
 * Description:
 *   Fit the 2 category model to the data
 *   This network uses both weights and biases in all nodes.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_TwoLayer2CategoryWithBiasesNetworkFit(
    const SLData_t* pTrainingData, const SLArrayIndex_t* pCategoricalValue, SLData_t* pLayer1Weights, SLData_t* pLayer1Biases,
    SLData_t* pLayer2Weights, SLData_t* pLayer2Biases, SLData_t* pLayer1PreActivation, SLData_t* pLayer1PostActivation,
    const enum SLActivationType_t layer1ActivationType, const SLData_t layer1ActivationAlpha, const enum SLActivationType_t layer2ActivationType,
    const SLData_t layer2ActivationAlpha, const SLData_t learningRate, const SLArrayIndex_t numberOfTrainingSequences,
    const SLArrayIndex_t numberOfInputNodes, const SLArrayIndex_t numberOfLayer1Nodes)
{
  // Forward propagation
  for (SLArrayIndex_t trainingSequenceNumber = 0; trainingSequenceNumber < numberOfTrainingSequences; trainingSequenceNumber++) {
    for (SLArrayIndex_t layer1Node = 0; layer1Node < numberOfLayer1Nodes; layer1Node++) {
      pLayer1PreActivation[layer1Node] =
          SDA_RealDotProduct(pTrainingData + (trainingSequenceNumber * numberOfInputNodes),    // Source vector 1 pointer
                             pLayer1Weights + (layer1Node * numberOfInputNodes),               // Source vector 2 pointer
                             numberOfInputNodes) +                                             // Vector lengths
          pLayer1Biases[layer1Node];

      switch (layer1ActivationType) {    // Choose required activation function
      case SIGLIB_ACTIVATION_TYPE_LEAKY_RELU:
        *(pLayer1PostActivation + layer1Node) = SDS_ActivationLeakyReLU(pLayer1PreActivation[layer1Node], layer1ActivationAlpha);
        break;
      case SIGLIB_ACTIVATION_TYPE_LOGISTIC:
        *(pLayer1PostActivation + layer1Node) = SDS_ActivationLogistic(pLayer1PreActivation[layer1Node]);
        break;
      case SIGLIB_ACTIVATION_TYPE_TANH:
        *(pLayer1PostActivation + layer1Node) = SDS_ActivationTanH(pLayer1PreActivation[layer1Node]);
        break;
      case SIGLIB_ACTIVATION_TYPE_RELU:
      default:
        *(pLayer1PostActivation + layer1Node) = SDS_ActivationReLU(pLayer1PreActivation[layer1Node]);
        break;
      }
    }

    SLData_t layer2PreActivation = SDA_RealDotProduct(pLayer1PostActivation,    // Source vector 1 pointer
                                                      pLayer2Weights,           // Source vector 2 pointer
                                                      numberOfLayer1Nodes) +    // Vector lengths
                                   pLayer2Biases[0];

    SLData_t layer2PostActivation;
    switch (layer2ActivationType) {    // Choose required activation function
    case SIGLIB_ACTIVATION_TYPE_LEAKY_RELU:
      layer2PostActivation = SDS_ActivationLeakyReLU(layer2PreActivation, layer2ActivationAlpha);
      break;
    case SIGLIB_ACTIVATION_TYPE_LOGISTIC:
      layer2PostActivation = SDS_ActivationLogistic(layer2PreActivation);
      break;
    case SIGLIB_ACTIVATION_TYPE_TANH:
      layer2PostActivation = SDS_ActivationTanH(layer2PreActivation);
      break;
    case SIGLIB_ACTIVATION_TYPE_RELU:
    default:
      layer2PostActivation = SDS_ActivationReLU(layer2PreActivation);
      break;
    }

    SLData_t layer2Error = (SLData_t)pCategoricalValue[trainingSequenceNumber] - layer2PostActivation;

    // Backward propagation
    for (SLArrayIndex_t layer1Node = 0; layer1Node < numberOfLayer1Nodes; layer1Node++) {
      SLData_t layer1Error;
      switch (layer2ActivationType) {    // Choose required activation derivative
                                         // function
      case SIGLIB_ACTIVATION_TYPE_LEAKY_RELU:
        layer1Error = layer2Error * SDS_ActivationLeakyReLUDerivative(layer2PreActivation, layer2ActivationAlpha);
        break;
      case SIGLIB_ACTIVATION_TYPE_LOGISTIC:
        layer1Error = layer2Error * SDS_ActivationLogisticDerivative(layer2PreActivation);
        break;
      case SIGLIB_ACTIVATION_TYPE_TANH:
        layer1Error = layer2Error * SDS_ActivationTanHDerivative(layer2PreActivation);
        break;
      case SIGLIB_ACTIVATION_TYPE_RELU:
      default:
        layer1Error = layer2Error * SDS_ActivationReLUDerivative(layer2PreActivation);
        break;
      }

      SLData_t layer2Gradient = layer1Error * *(pLayer1PostActivation + layer1Node);

      for (SLArrayIndex_t inputLayerNode = 0; inputLayerNode < numberOfInputNodes; inputLayerNode++) {
        SLData_t trainingDataSample = pTrainingData[(trainingSequenceNumber * numberOfInputNodes) + inputLayerNode];
        SLData_t layer1Gradient;
        switch (layer1ActivationType) {    // Choose required activation derivative
                                           // function
        case SIGLIB_ACTIVATION_TYPE_LEAKY_RELU:
          layer1Gradient = layer1Error * *(pLayer2Weights + layer1Node) * trainingDataSample *
                           SDS_ActivationLeakyReLUDerivative(pLayer1PreActivation[layer1Node], layer1ActivationAlpha);
          break;
        case SIGLIB_ACTIVATION_TYPE_LOGISTIC:
          layer1Gradient = layer1Error * *(pLayer2Weights + layer1Node) * trainingDataSample *
                           SDS_ActivationLogisticDerivative(pLayer1PreActivation[layer1Node]);
          break;
        case SIGLIB_ACTIVATION_TYPE_TANH:
          layer1Gradient =
              layer1Error * *(pLayer2Weights + layer1Node) * trainingDataSample * SDS_ActivationTanHDerivative(pLayer1PreActivation[layer1Node]);
          break;
        case SIGLIB_ACTIVATION_TYPE_RELU:
        default:
          layer1Gradient =
              layer1Error * *(pLayer2Weights + layer1Node) * trainingDataSample * SDS_ActivationReLUDerivative(pLayer1PreActivation[layer1Node]);
          break;
        }

        *(pLayer1Weights + (layer1Node * numberOfInputNodes) + inputLayerNode) += learningRate * layer1Gradient;
      }
      //##
      SLData_t layer1Gradient;
      switch (layer1ActivationType) {    // Choose required activation derivative
                                         // function
      case SIGLIB_ACTIVATION_TYPE_LEAKY_RELU:
        layer1Gradient =
            layer1Error * pLayer2Weights[layer1Node] * SDS_ActivationLeakyReLUDerivative(pLayer1PreActivation[layer1Node], layer1ActivationAlpha);
        break;
      case SIGLIB_ACTIVATION_TYPE_LOGISTIC:
        layer1Gradient = layer1Error * pLayer2Weights[layer1Node] * SDS_ActivationLogisticDerivative(pLayer1PreActivation[layer1Node]);
        break;
      case SIGLIB_ACTIVATION_TYPE_TANH:
        layer1Gradient = layer1Error * pLayer2Weights[layer1Node] * SDS_ActivationTanHDerivative(pLayer1PreActivation[layer1Node]);
        break;
      case SIGLIB_ACTIVATION_TYPE_RELU:
      default:
        layer1Gradient = layer1Error * pLayer2Weights[layer1Node] * SDS_ActivationReLUDerivative(pLayer1PreActivation[layer1Node]);
        break;
      }
      *(pLayer1Biases + layer1Node) += learningRate * layer1Gradient;
      *(pLayer2Weights + layer1Node) += learningRate * layer2Gradient;
    }
    //##
    SLData_t layer2Gradient;
    switch (layer2ActivationType) {    // Choose required activation derivative
                                       // function
    case SIGLIB_ACTIVATION_TYPE_LEAKY_RELU:
      layer2Gradient = layer2Error * SDS_ActivationLeakyReLUDerivative(layer2PreActivation, layer2ActivationAlpha);
      break;
    case SIGLIB_ACTIVATION_TYPE_LOGISTIC:
      layer2Gradient = layer2Error * SDS_ActivationLogisticDerivative(layer2PreActivation);
      break;
    case SIGLIB_ACTIVATION_TYPE_TANH:
      layer2Gradient = layer2Error * SDS_ActivationTanHDerivative(layer2PreActivation);
      break;
    case SIGLIB_ACTIVATION_TYPE_RELU:
    default:
      layer2Gradient = layer2Error * SDS_ActivationReLUDerivative(layer2PreActivation);
      break;
    }

    *pLayer1Biases += learningRate * layer2Gradient;
  }
}    // End of SDA_TwoLayer2CategoryWithBiasesNetworkFit()

/**/

/********************************************************
 * Function: SDA_TwoLayer2CategoryWithBiasesNetworkPredict
 *
 * Parameters:
 *   const SLData_t *,               - pData
 *   const SLData_t *,               - pLayer1Weights
 *   const SLData_t *,               - pLayer1Biases
 *   const SLData_t *,               - pLayer2Weights
 *   const SLData_t *,               - pLayer2Biases
 *   SLData_t *,                     - pLayer1PostActivation
 *   SLData_t *,                     - pLayer2Activation
 *   const enum SLActivationType_t   - layer1ActivationType
 *   const SLData_t,                 - layer1ActivationAlpha
 *   const enum SLActivationType_t   - layer2ActivationType
 *   const SLData_t,                 - layer2ActivationAlpha
 *   const SLData_t,                 - classificationThreshold
 *   const SLArrayIndex_t,           - numberOfInputNodes
 *   const SLArrayIndex_t)           - numberOfLayer1Nodes
 *
 * Return value:
 *   Predicted category and associated probability
 *
 * Description:
 *   Use the model to predict the category of the data
 *   This network uses both weights and biases in all nodes.
 *
 ********************************************************/

SLNeuralNetworkPrediction_s SIGLIB_FUNC_DECL SDA_TwoLayer2CategoryWithBiasesNetworkPredict(
    const SLData_t* pData, const SLData_t* pLayer1Weights, const SLData_t* pLayer1Biases, const SLData_t* pLayer2Weights,
    const SLData_t* pLayer2Biases, SLData_t* pLayer1PostActivation, SLData_t* pLayer2Activation, const enum SLActivationType_t layer1ActivationType,
    const SLData_t layer1ActivationAlpha, const enum SLActivationType_t layer2ActivationType, const SLData_t layer2ActivationAlpha,
    const SLData_t classificationThreshold, const SLArrayIndex_t numberOfInputNodes, const SLArrayIndex_t numberOfLayer1Nodes)
{
  // Forward propagation
  for (SLArrayIndex_t layer1Node = 0; layer1Node < numberOfLayer1Nodes; layer1Node++) {
    SLData_t layer1PreActivation = SDA_RealDotProduct(pData,                                                 // Source vector 1 pointer
                                                      pLayer1Weights + (layer1Node * numberOfInputNodes),    // Source vector 2 pointer
                                                      numberOfInputNodes) +                                  // Vector lengths
                                   pLayer1Biases[layer1Node];

    switch (layer1ActivationType) {    // Choose required activation function
    case SIGLIB_ACTIVATION_TYPE_LEAKY_RELU:
      *(pLayer1PostActivation + layer1Node) = SDS_ActivationLeakyReLU(layer1PreActivation, layer1ActivationAlpha);
      break;
    case SIGLIB_ACTIVATION_TYPE_LOGISTIC:
      *(pLayer1PostActivation + layer1Node) = SDS_ActivationLogistic(layer1PreActivation);
      break;
    case SIGLIB_ACTIVATION_TYPE_TANH:
      *(pLayer1PostActivation + layer1Node) = SDS_ActivationTanH(layer1PreActivation);
      break;
    case SIGLIB_ACTIVATION_TYPE_RELU:
    default:
      *(pLayer1PostActivation + layer1Node) = SDS_ActivationReLU(layer1PreActivation);
      break;
    }
  }

  SLData_t layer2PreActivation = SDA_RealDotProduct(pLayer1PostActivation,    // Source vector 1 pointer
                                                    pLayer2Weights,           // Source vector 2 pointer
                                                    numberOfLayer1Nodes) +    // Vector lengths
                                 pLayer2Biases[0];

  SLData_t layer2PostActivation;
  switch (layer2ActivationType) {    // Choose required activation function
  case SIGLIB_ACTIVATION_TYPE_LEAKY_RELU:
    layer2PostActivation = SDS_ActivationLeakyReLU(layer2PreActivation, layer2ActivationAlpha);
    break;
  case SIGLIB_ACTIVATION_TYPE_LOGISTIC:
    layer2PostActivation = SDS_ActivationLogistic(layer2PreActivation);
    break;
  case SIGLIB_ACTIVATION_TYPE_TANH:
    layer2PostActivation = SDS_ActivationTanH(layer2PreActivation);
    break;
  case SIGLIB_ACTIVATION_TYPE_RELU:
  default:
    layer2PostActivation = SDS_ActivationReLU(layer2PreActivation);
    break;
  }

  SLNeuralNetworkPrediction_s prediction;
  if (layer2PostActivation > classificationThreshold) {
    prediction.predictedCategory = SIGLIB_AI_ONE;
  } else {
    prediction.predictedCategory = SIGLIB_AI_ZERO;
  }

  // Search for category with max activation level
  prediction.probability = layer2PostActivation;

  *pLayer2Activation = layer2PostActivation;    // Used for visualization

  return (prediction);
}    // End of SDA_TwoLayer2CategoryWithBiasesNetworkPredict()

/**/

/********************************************************
 * Function: SDA_TwoLayerNCategoryWithBiasesNetworkFit
 *
 * Parameters:
 *   const SLData_t *,               - pTrainingData
 *   const SLArrayIndex_t *,         - pCategoricalValue
 *   SLData_t *,                     - pLayer1Weights
 *   SLData_t *,                     - pLayer1Biases
 *   SLData_t *,                     - pLayer2Weights
 *   SLData_t *,                     - pLayer2Biases
 *   SLData_t *,                     - pLayer1PreActivation
 *   SLData_t *,                     - pLayer1PostActivation
 *   const enum SLActivationType_t   - layer1ActivationType
 *   const SLData_t,                 - layer1ActivationAlpha
 *   const enum SLActivationType_t   - layer2ActivationType
 *   const SLData_t,                 - layer2ActivationAlpha
 *   const SLData_t,                 - learningRate
 *   const SLArrayIndex_t,           - numberOfTrainingSequences
 *   const SLArrayIndex_t,           - numberOfInputNodes
 *   const SLArrayIndex_t)           - numberOfLayer1Nodes
 *
 * Return value:
 *   void
 *
 * Description:
 *   Fit the N category model to the data
 *   This network uses both weights and biases in all nodes.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_TwoLayerNCategoryWithBiasesNetworkFit(
    const SLData_t* pTrainingData, const SLArrayIndex_t* pCategoricalValues, SLData_t* pLayer1Weights, SLData_t* pLayer1Biases,
    SLData_t* pLayer2Weights, SLData_t* pLayer2Biases, SLData_t* pLayer1PreActivation, SLData_t* pLayer1PostActivation,
    SLData_t* pLayer2PostActivation, const enum SLActivationType_t layer1ActivationType, const SLData_t layer1ActivationAlpha,
    const enum SLActivationType_t layer2ActivationType, const SLData_t layer2ActivationAlpha, const SLData_t learningRate,
    const SLArrayIndex_t numberOfTrainingSequences, const SLArrayIndex_t numberOfInputNodes, const SLArrayIndex_t numberOfLayer1Nodes,
    const SLArrayIndex_t numberOfCategories)
{
  // Forward propagation
  for (SLArrayIndex_t trainingSequenceNumber = 0; trainingSequenceNumber < numberOfTrainingSequences; trainingSequenceNumber++) {
    SLData_t layer2PreActivation;

    for (SLArrayIndex_t layer1Node = 0; layer1Node < numberOfLayer1Nodes; layer1Node++) {
      pLayer1PreActivation[layer1Node] =
          SDA_RealDotProduct(pTrainingData + (trainingSequenceNumber * numberOfInputNodes),    // Source vector 1 pointer
                             pLayer1Weights + (layer1Node * numberOfInputNodes),               // Source vector 2 pointer
                             numberOfInputNodes) +                                             // Vector lengths
          pLayer1Biases[layer1Node];

      switch (layer1ActivationType) {    // Choose required activation function
      case SIGLIB_ACTIVATION_TYPE_LEAKY_RELU:
        *(pLayer1PostActivation + layer1Node) = SDS_ActivationLeakyReLU(pLayer1PreActivation[layer1Node], layer1ActivationAlpha);
        break;
      case SIGLIB_ACTIVATION_TYPE_LOGISTIC:
        *(pLayer1PostActivation + layer1Node) = SDS_ActivationLogistic(pLayer1PreActivation[layer1Node]);
        break;
      case SIGLIB_ACTIVATION_TYPE_TANH:
        *(pLayer1PostActivation + layer1Node) = SDS_ActivationTanH(pLayer1PreActivation[layer1Node]);
        break;
      case SIGLIB_ACTIVATION_TYPE_RELU:
      default:
        *(pLayer1PostActivation + layer1Node) = SDS_ActivationReLU(pLayer1PreActivation[layer1Node]);
        break;
      }
    }

    for (SLArrayIndex_t layer2Node = 0; layer2Node < numberOfCategories; layer2Node++) {
      layer2PreActivation = SDA_RealDotProduct(pLayer1PostActivation,                                  // Source vector 1 pointer
                                               pLayer2Weights + (layer2Node * numberOfLayer1Nodes),    // Source vector 2 pointer
                                               numberOfLayer1Nodes) +                                  // Vector lengths
                            pLayer2Biases[layer2Node];

      switch (layer2ActivationType) {    // Choose required activation function
      case SIGLIB_ACTIVATION_TYPE_LEAKY_RELU:
        *(pLayer2PostActivation + layer2Node) = SDS_ActivationLeakyReLU(layer2PreActivation, layer2ActivationAlpha);
        break;
      case SIGLIB_ACTIVATION_TYPE_LOGISTIC:
        *(pLayer2PostActivation + layer2Node) = SDS_ActivationLogistic(layer2PreActivation);
        break;
      case SIGLIB_ACTIVATION_TYPE_TANH:
        *(pLayer2PostActivation + layer2Node) = SDS_ActivationTanH(layer2PreActivation);
        break;
      case SIGLIB_ACTIVATION_TYPE_RELU:
      default:
        *(pLayer2PostActivation + layer2Node) = SDS_ActivationReLU(layer2PreActivation);
        break;
      }
    }

    SLData_t layer2Error;
    // Backward propagation
    for (SLArrayIndex_t layer2Node = 0; layer2Node < numberOfCategories; layer2Node++) {
      if (layer2Node == pCategoricalValues[trainingSequenceNumber]) {
        layer2Error = SIGLIB_ONE - *(pLayer2PostActivation + layer2Node);
      } else {
        layer2Error = -*(pLayer2PostActivation + layer2Node);
      }

      for (SLArrayIndex_t layer1Node = 0; layer1Node < numberOfLayer1Nodes; layer1Node++) {
        SLData_t layer1Error;
        switch (layer2ActivationType) {    // Choose required activation derivative
                                           // function
        case SIGLIB_ACTIVATION_TYPE_LEAKY_RELU:
          layer1Error = layer2Error * SDS_ActivationLeakyReLUDerivative(layer2PreActivation, layer2ActivationAlpha);
          break;
        case SIGLIB_ACTIVATION_TYPE_LOGISTIC:
          layer1Error = layer2Error * SDS_ActivationLogisticDerivative(layer2PreActivation);
          break;
        case SIGLIB_ACTIVATION_TYPE_TANH:
          layer1Error = layer2Error * SDS_ActivationTanHDerivative(layer2PreActivation);
          break;
        case SIGLIB_ACTIVATION_TYPE_RELU:
        default:
          layer1Error = layer2Error * SDS_ActivationReLUDerivative(layer2PreActivation);
          break;
        }

        SLData_t layer2Gradient = layer1Error * *(pLayer1PostActivation + layer1Node);

        for (SLArrayIndex_t inputLayerNode = 0; inputLayerNode < numberOfInputNodes; inputLayerNode++) {
          SLData_t trainingDataSample = pTrainingData[(trainingSequenceNumber * numberOfInputNodes) + inputLayerNode];
          SLData_t layer1Gradient;
          switch (layer1ActivationType) {    // Choose required activation
                                             // derivative function
          case SIGLIB_ACTIVATION_TYPE_LEAKY_RELU:
            layer1Gradient = layer1Error * pLayer2Weights[(layer2Node * numberOfLayer1Nodes) + layer1Node] * trainingDataSample *
                             SDS_ActivationLeakyReLUDerivative(pLayer1PreActivation[layer1Node], layer1ActivationAlpha);
            break;
          case SIGLIB_ACTIVATION_TYPE_LOGISTIC:
            layer1Gradient = layer1Error * pLayer2Weights[(layer2Node * numberOfLayer1Nodes) + layer1Node] * trainingDataSample *
                             SDS_ActivationLogisticDerivative(pLayer1PreActivation[layer1Node]);
            break;
          case SIGLIB_ACTIVATION_TYPE_TANH:
            layer1Gradient = layer1Error * pLayer2Weights[(layer2Node * numberOfLayer1Nodes) + layer1Node] * trainingDataSample *
                             SDS_ActivationTanHDerivative(pLayer1PreActivation[layer1Node]);
            break;
          case SIGLIB_ACTIVATION_TYPE_RELU:
          default:
            layer1Gradient = layer1Error * pLayer2Weights[(layer2Node * numberOfLayer1Nodes) + layer1Node] * trainingDataSample *
                             SDS_ActivationReLUDerivative(pLayer1PreActivation[layer1Node]);
            break;
          }

          *(pLayer1Weights + (layer1Node * numberOfInputNodes) + inputLayerNode) += learningRate * layer1Gradient;
        }

        SLData_t layer1Gradient;
        switch (layer1ActivationType) {    // Choose required activation derivative
                                           // function
        case SIGLIB_ACTIVATION_TYPE_LEAKY_RELU:
          layer1Gradient = layer1Error * pLayer2Weights[(layer2Node * numberOfLayer1Nodes) + layer1Node] *
                           SDS_ActivationLeakyReLUDerivative(pLayer1PreActivation[layer1Node], layer1ActivationAlpha);
          break;
        case SIGLIB_ACTIVATION_TYPE_LOGISTIC:
          layer1Gradient = layer1Error * pLayer2Weights[(layer2Node * numberOfLayer1Nodes) + layer1Node] *
                           SDS_ActivationLogisticDerivative(pLayer1PreActivation[layer1Node]);
          break;
        case SIGLIB_ACTIVATION_TYPE_TANH:
          layer1Gradient = layer1Error * pLayer2Weights[(layer2Node * numberOfLayer1Nodes) + layer1Node] *
                           SDS_ActivationTanHDerivative(pLayer1PreActivation[layer1Node]);
          break;
        case SIGLIB_ACTIVATION_TYPE_RELU:
        default:
          layer1Gradient = layer1Error * pLayer2Weights[(layer2Node * numberOfLayer1Nodes) + layer1Node] *
                           SDS_ActivationReLUDerivative(pLayer1PreActivation[layer1Node]);
          break;
        }
        *(pLayer1Biases + layer1Node) += learningRate * layer1Gradient;

        *(pLayer2Weights + (layer2Node * numberOfLayer1Nodes) + layer1Node) += learningRate * layer2Gradient;
      }

      SLData_t layer2Gradient;
      switch (layer2ActivationType) {    // Choose required activation derivative
                                         // function
      case SIGLIB_ACTIVATION_TYPE_LEAKY_RELU:
        layer2Gradient = layer2Error * SDS_ActivationLeakyReLUDerivative(layer2PreActivation, layer2ActivationAlpha);
        break;
      case SIGLIB_ACTIVATION_TYPE_LOGISTIC:
        layer2Gradient = layer2Error * SDS_ActivationLogisticDerivative(layer2PreActivation);
        break;
      case SIGLIB_ACTIVATION_TYPE_TANH:
        layer2Gradient = layer2Error * SDS_ActivationTanHDerivative(layer2PreActivation);
        break;
      case SIGLIB_ACTIVATION_TYPE_RELU:
      default:
        layer2Gradient = layer2Error * SDS_ActivationReLUDerivative(layer2PreActivation);
        break;
      }

      *pLayer1Biases += learningRate * layer2Gradient;
    }
  }
}    // End of SDA_TwoLayerNCategoryWithBiasesNetworkFit()

/**/

/********************************************************
 * Function: SDA_TwoLayerNCategoryWithBiasesNetworkPredict
 *
 * Parameters:
 *   const SLData_t *,               - pData
 *   const SLData_t *,               - pLayer1Weights
 *   const SLData_t *,               - pLayer1Biases
 *   const SLData_t *,               - pLayer2Weights
 *   const SLData_t *,               - pLayer2Biases
 *   SLData_t *,                     - pLayer1PostActivation
 *   SLData_t *,                     - pLayer2PostActivation
 *   const enum SLActivationType_t   - layer1ActivationType
 *   const SLData_t,                 - layer1ActivationAlpha
 *   const enum SLActivationType_t   - layer2ActivationType
 *   const SLData_t,                 - layer2ActivationAlpha
 *   const SLArrayIndex_t,           - numberOfInputNodes
 *   const SLArrayIndex_t,           - numberOfLayer1Nodes
 *   const SLArrayIndex_t)           - numberOfCategories
 *
 * Return value:
 *   Predicted category and associated probability
 *
 * Description:
 *   Use the model to predict the category of the data
 *   This network uses both weights and biases in all nodes.
 *
 ********************************************************/

SLNeuralNetworkPrediction_s SIGLIB_FUNC_DECL SDA_TwoLayerNCategoryWithBiasesNetworkPredict(
    const SLData_t* pData, const SLData_t* pLayer1Weights, const SLData_t* pLayer1Biases, const SLData_t* pLayer2Weights,
    const SLData_t* pLayer2Biases, SLData_t* pLayer1PostActivation, SLData_t* pLayer2PostActivation,
    const enum SLActivationType_t layer1ActivationType, const SLData_t layer1ActivationAlpha, const enum SLActivationType_t layer2ActivationType,
    const SLData_t layer2ActivationAlpha, const SLArrayIndex_t numberOfInputNodes, const SLArrayIndex_t numberOfLayer1Nodes,
    const SLArrayIndex_t numberOfCategories)
{
  // Forward propagation
  // First layer
  for (SLArrayIndex_t layer1Node = 0; layer1Node < numberOfLayer1Nodes; layer1Node++) {
    SLData_t layer1PreActivation = SDA_RealDotProduct(pData,                                                 // Source vector 1 pointer
                                                      pLayer1Weights + (layer1Node * numberOfInputNodes),    // Source vector 2 pointer
                                                      numberOfInputNodes) +                                  // Vector lengths
                                   pLayer1Biases[layer1Node];
    switch (layer1ActivationType) {    // Choose required activation function
    case SIGLIB_ACTIVATION_TYPE_LEAKY_RELU:
      *(pLayer1PostActivation + layer1Node) = SDS_ActivationLeakyReLU(layer1PreActivation, layer1ActivationAlpha);
      break;
    case SIGLIB_ACTIVATION_TYPE_LOGISTIC:
      *(pLayer1PostActivation + layer1Node) = SDS_ActivationLogistic(layer1PreActivation);
      break;
    case SIGLIB_ACTIVATION_TYPE_TANH:
      *(pLayer1PostActivation + layer1Node) = SDS_ActivationTanH(layer1PreActivation);
      break;
    case SIGLIB_ACTIVATION_TYPE_RELU:
    default:
      *(pLayer1PostActivation + layer1Node) = SDS_ActivationReLU(layer1PreActivation);
      break;
    }
  }

  // Second layer
  for (SLArrayIndex_t layer2Node = 0; layer2Node < numberOfCategories; layer2Node++) {
    SLData_t layer2PreActivation = SDA_RealDotProduct(pLayer1PostActivation,                                  // Source vector 1 pointer
                                                      pLayer2Weights + (layer2Node * numberOfLayer1Nodes),    // Source vector 2 pointer
                                                      numberOfLayer1Nodes) +                                  // Vector lengths
                                   pLayer2Biases[layer2Node];

    switch (layer2ActivationType) {    // Choose required activation function
    case SIGLIB_ACTIVATION_TYPE_LEAKY_RELU:
      *(pLayer2PostActivation + layer2Node) = SDS_ActivationLeakyReLU(layer2PreActivation, layer2ActivationAlpha);
      break;
    case SIGLIB_ACTIVATION_TYPE_LOGISTIC:
      *(pLayer2PostActivation + layer2Node) = SDS_ActivationLogistic(layer2PreActivation);
      break;
    case SIGLIB_ACTIVATION_TYPE_TANH:
      *(pLayer2PostActivation + layer2Node) = SDS_ActivationTanH(layer2PreActivation);
      break;
    case SIGLIB_ACTIVATION_TYPE_RELU:
    default:
      *(pLayer2PostActivation + layer2Node) = SDS_ActivationReLU(layer2PreActivation);
      break;
    }
  }

  SLNeuralNetworkPrediction_s prediction;
  // Search for category with max activation level
  prediction.predictedCategory = SDA_MaxIndex(pLayer2PostActivation, numberOfCategories);
  prediction.probability = pLayer2PostActivation[prediction.predictedCategory];

  return (prediction);    // Category encoding: 0 to N-1
}    // End of SDA_TwoLayerNCategoryWithBiasesNetworkPredict()

/**/

/********************************************************
 * Function: SDS_ActivationReLU
 *
 * Parameters:
 *   const SLData_t          - Source sample
 *
 * Return value:
 *   Result sample
 *
 * Description: Perform ReLU activation on a sample.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_ActivationReLU(const SLData_t x)
{
  if (x > SIGLIB_ZERO) {
    return x;
  }

  return SIGLIB_ZERO;
}    // End of SDS_ActivationReLU()

/**/

/********************************************************
 * Function: SDA_ActivationReLU
 *
 * Parameters:
 *   const SLData_t *        - Pointer to source array
 *   SLData_t *              - Pointer to destination array
 *   const SLArrayIndex_t    - Array lengths
 *
 * Return value:
 *   void
 *
 * Description: Perform ReLU activation on an array of
 *   samples.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_ActivationReLU(const SLData_t* pSrc, SLData_t* pDst, const SLArrayIndex_t arrayLength)
{
  for (SLArrayIndex_t i = 0; i < arrayLength; i++) {
    if (*pSrc > SIGLIB_ZERO) {
      *pDst++ = *pSrc;
    } else {
      *pDst++ = SIGLIB_ZERO;
    }
    pSrc++;
  }
}    // End of SDA_ActivationReLU()

/**/

/********************************************************
 * Function: SDS_ActivationReLUDerivative
 *
 * Parameters:
 *   const SLData_t          - Source sample
 *
 * Return value:
 *   Result sample
 *
 * Description: Perform ReLU derivative on a sample.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_ActivationReLUDerivative(const SLData_t x)
{
  if (x > SIGLIB_ZERO) {
    return SIGLIB_ONE;
  }

  return SIGLIB_ZERO;
}    // End of SDS_ActivationReLUDerivative()

/**/

/********************************************************
 * Function: SDA_ActivationReLUDerivative
 *
 * Parameters:
 *   const SLData_t *        - Pointer to source array
 *   SLData_t *              - Pointer to destination array
 *   const SLArrayIndex_t    - Array lengths
 *
 * Return value:
 *   void
 *
 * Description: Perform ReLU derivative on an array of
 *   samples.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_ActivationReLUDerivative(const SLData_t* pSrc, SLData_t* pDst, const SLArrayIndex_t arrayLength)
{
  for (SLArrayIndex_t i = 0; i < arrayLength; i++) {
    if (*pSrc > SIGLIB_ZERO) {
      *pDst++ = SIGLIB_ONE;
    } else {
      *pDst++ = SIGLIB_ZERO;
    }
    pSrc++;
  }
}    // End of SDA_ActivationReLUDerivative()

/**/

/********************************************************
 * Function: SDS_ActivationLeakyReLU
 *
 * Parameters:
 *   const SLData_t          - Source sample
 *
 * Return value:
 *   Result sample
 *
 * Description: Perform leaky ReLU activation on a sample.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_ActivationLeakyReLU(const SLData_t x, const SLData_t alpha)
{
  if (x > SIGLIB_ZERO) {
    return x;
  }

  return (x * alpha);
}    // End of SDS_ActivationLeakyReLU()

/**/

/********************************************************
 * Function: SDA_ActivationLeakyReLU
 *
 * Parameters:
 *   const SLData_t *        - Pointer to source array
 *   SLData_t *              - Pointer to destination array
 *   const SLArrayIndex_t    - Array lengths
 *
 * Return value:
 *   void
 *
 * Description: Perform leaky ReLU activation on an array of
 *   samples.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_ActivationLeakyReLU(const SLData_t* pSrc, const SLData_t alpha, SLData_t* pDst, const SLArrayIndex_t arrayLength)
{
  for (SLArrayIndex_t i = 0; i < arrayLength; i++) {
    if (*pSrc > SIGLIB_ZERO) {
      *pDst++ = *pSrc;
    } else {
      *pDst++ = *pSrc * alpha;
    }
    pSrc++;
  }
}    // End of SDA_ActivationLeakyReLU()

/**/

/********************************************************
 * Function: SDS_ActivationLeakyReLUDerivative
 *
 * Parameters:
 *   const SLData_t          - Source sample
 *
 * Return value:
 *   Result sample
 *
 * Description: Perform leaky ReLU derivative on a sample.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_ActivationLeakyReLUDerivative(const SLData_t x, const SLData_t alpha)
{
  if (x > SIGLIB_ZERO) {
    return SIGLIB_ONE;
  }

  return alpha;
}    // End of SDS_ActivationLeakyReLUDerivative()

/**/

/********************************************************
 * Function: SDA_ActivationLeakyReLUDerivative
 *
 * Parameters:
 *   const SLData_t *        - Pointer to source array
 *   SLData_t *              - Pointer to destination array
 *   const SLArrayIndex_t    - Array lengths
 *
 * Return value:
 *   void
 *
 * Description: Perform leaky ReLU derivative on an array of
 *   samples.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_ActivationLeakyReLUDerivative(const SLData_t* pSrc, const SLData_t alpha, SLData_t* pDst,
                                                        const SLArrayIndex_t arrayLength)
{
  for (SLArrayIndex_t i = 0; i < arrayLength; i++) {
    if (*pSrc > SIGLIB_ZERO) {
      *pDst++ = SIGLIB_ONE;
    } else {
      *pDst++ = alpha;
    }
    pSrc++;
  }
}    // End of SDA_ActivationLeakyReLUDerivative()

/**/

/********************************************************
 * Function: SDS_ActivationLogistic
 *
 * Parameters:
 *   const SLData_t          - Source sample
 *
 * Return value:
 *   Result sample
 *
 * Description: Perform logistic activation on a sample.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_ActivationLogistic(const SLData_t x)
{
  return (SIGLIB_ONE / (SIGLIB_ONE + SDS_Exp(-x)));
}    // End of SDS_ActivationLogistic()

/**/

/********************************************************
 * Function: SDA_ActivationLogistic
 *
 * Parameters:
 *   const SLData_t *        - Pointer to source array
 *   SLData_t *              - Pointer to destination array
 *   const SLArrayIndex_t    - Array lengths
 *
 * Return value:
 *   void
 *
 * Description: Perform logistic activation on an array of
 *   samples.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_ActivationLogistic(const SLData_t* pSrc, SLData_t* pDst, const SLArrayIndex_t arrayLength)
{
  for (SLArrayIndex_t i = 0; i < arrayLength; i++) {
    *pDst++ = SIGLIB_ONE / (SIGLIB_ONE + SDS_Exp(-*pSrc));
    pSrc++;
  }
}    // End of SDA_ActivationLogistic()

/**/

/********************************************************
 * Function: SDS_ActivationLogisticDerivative
 *
 * Parameters:
 *   const SLData_t          - Source sample
 *
 * Return value:
 *   Result sample
 *
 * Description: Perform logistic derivative on a sample.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_ActivationLogisticDerivative(const SLData_t x)
{
  return (SIGLIB_ONE / (SIGLIB_ONE + SDS_Exp(-x))) * (SIGLIB_ONE - (SIGLIB_ONE / (SIGLIB_ONE + SDS_Exp(-x))));
}    // End of SDS_ActivationLogisticDerivative()

/**/

/********************************************************
 * Function: SDA_ActivationLogisticDerivative
 *
 * Parameters:
 *   const SLData_t *        - Pointer to source array
 *   SLData_t *              - Pointer to destination array
 *   const SLArrayIndex_t    - Array lengths
 *
 * Return value:
 *   void
 *
 * Description: Perform logistic derivative on an array of
 *   samples.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_ActivationLogisticDerivative(const SLData_t* pSrc, SLData_t* pDst, const SLArrayIndex_t arrayLength)
{
  for (SLArrayIndex_t i = 0; i < arrayLength; i++) {
    *pDst++ = (SIGLIB_ONE / (SIGLIB_ONE + SDS_Exp(-*pSrc))) * (SIGLIB_ONE - (SIGLIB_ONE / (SIGLIB_ONE + SDS_Exp(-*pSrc))));
    pSrc++;
  }
}    // End of SDA_ActivationLogisticDerivative()

/**/

/********************************************************
 * Function: SDS_ActivationTanH
 *
 * Parameters:
 *   const SLData_t          - Source sample
 *
 * Return value:
 *   Result sample
 *
 * Description: Perform hyperbolic tangent activation
 *   on a sample.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_ActivationTanH(const SLData_t x)
{
  return (SIGLIB_TWO / (SIGLIB_ONE + SDS_Exp(-SIGLIB_TWO * x))) - SIGLIB_ONE;
}    // End of SDS_ActivationTanH()

/**/

/********************************************************
 * Function: SDA_ActivationTanH
 *
 * Parameters:
 *   const SLData_t *        - Pointer to source array
 *   SLData_t *              - Pointer to destination array
 *   const SLArrayIndex_t    - Array lengths
 *
 * Return value:
 *   void
 *
 * Description: Perform hyperbolic tangent activation
 *   on an array of samples.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_ActivationTanH(const SLData_t* pSrc, SLData_t* pDst, const SLArrayIndex_t arrayLength)
{
  for (SLArrayIndex_t i = 0; i < arrayLength; i++) {
    *pDst++ = (SIGLIB_TWO / (SIGLIB_ONE + SDS_Exp(-SIGLIB_TWO * *pSrc))) - SIGLIB_ONE;
    pSrc++;
  }
}    // End of SDA_ActivationTanH()

/**/

/********************************************************
 * Function: SDS_ActivationTanHDerivative
 *
 * Parameters:
 *   const SLData_t          - Source sample
 *
 * Return value:
 *   Result sample
 *
 * Description: Perform hyperbolic tangent derivative
 *   on a sample.
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDS_ActivationTanHDerivative(const SLData_t x)
{
  SLData_t fx = (SIGLIB_TWO / (SIGLIB_ONE + SDS_Exp(-SIGLIB_TWO * x))) - SIGLIB_ONE;
  return (SIGLIB_ONE / (fx * fx));
}    // End of SDS_ActivationTanHDerivative()

/**/

/********************************************************
 * Function: SDA_ActivationTanHDerivative
 *
 * Parameters:
 *   const SLData_t *        - Pointer to source array
 *   SLData_t *              - Pointer to destination array
 *   const SLArrayIndex_t    - Array lengths
 *
 * Return value:
 *   void
 *
 * Description: Perform hyperbolic tangent derivative
 *   on an array of samples.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_ActivationTanHDerivative(const SLData_t* pSrc, SLData_t* pDst, const SLArrayIndex_t arrayLength)
{
  for (SLArrayIndex_t i = 0; i < arrayLength; i++) {
    SLData_t fx = (SIGLIB_TWO / (SIGLIB_ONE + SDS_Exp(-SIGLIB_TWO * *pSrc))) - SIGLIB_ONE;
    pSrc++;
    *pDst++ = SIGLIB_ONE / (fx * fx);
  }
}    // End of SDA_ActivationTanHDerivative()
