// SigLib Regression Analysis Example
// Copyright (c) 2024 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>    // SigLib DSP library

// Define constants
#define DATA_LENGTH 5

// Declare global variables and arrays
static const SLData_t lin_datax[DATA_LENGTH] = {2.0, 4.0, 6.0, 8.0, 10.0};
static const SLData_t lin_datay[DATA_LENGTH] = {11.7, 21.1, 30.5, 39.9, 49.3};
static const SLData_t log_datax[DATA_LENGTH] = {1.0, 2.0, 3.0, 4.0, 5.0};
static const SLData_t log_datay[DATA_LENGTH] = {1.5, 5.38, 7.65, 9.26, 10.5};
static const SLData_t pow_datax[DATA_LENGTH] = {1.0, 3.0, 5.0, 7.0, 9.0};
static const SLData_t pow_datay[DATA_LENGTH] = {3.2, 20.71, 49.36, 87.46, 134.0};
static const SLData_t exp_datax[DATA_LENGTH] = {0.1, 0.2, 0.3, 0.4, 0.5};
static const SLData_t exp_datay[DATA_LENGTH] = {2.70, 3.64, 4.92, 6.64, 8.96};

int main(void)
{
  printf("Linear Regression analysis\n\n");
  for (SLArrayIndex_t i = 0; i < DATA_LENGTH; i++) {
    printf("X Data = %lf,\tY Data = %lf\n", lin_datax[i], lin_datay[i]);
  }
  printf("\n");

  printf("Assuming y = Mx + C\n\n");
  printf("Constant coefficient (C)    = %lf\n",
         SDA_LinraConstantCoeff(lin_datax,        // Pointer to X array
                                lin_datay,        // Pointer to Y array
                                DATA_LENGTH));    // Array length
  printf("Regression coefficient (M)  = %lf\n",
         SDA_LinraRegressionCoeff(lin_datax,        // Pointer to X array
                                  lin_datay,        // Pointer to Y array
                                  DATA_LENGTH));    // Array length
  printf("Correlation coefficient (r) = %lf\n",
         SDA_LinraCorrelationCoeff(lin_datax,        // Pointer to X array
                                   lin_datay,        // Pointer to Y array
                                   DATA_LENGTH));    // Array length
  printf("Estimated X for Y = 15.0    = %lf\n",
         SDA_LinraEstimateX(lin_datax,        // Pointer to X array
                            lin_datay,        // Pointer to Y array
                            15.0,             // Y value
                            DATA_LENGTH));    // Array length
  printf("Estimated Y for X = 5.0     = %lf\n",
         SDA_LinraEstimateY(lin_datax,        // Pointer to X array
                            lin_datay,        // Pointer to Y array
                            5.0,              // X value
                            DATA_LENGTH));    // Array length

  printf("\nPlease hit any key to continue . . .\n");
  getchar();

  printf("Logarithmic Regression analysis\n\n");
  for (SLArrayIndex_t i = 0; i < DATA_LENGTH; i++) {
    printf("X Data = %lf,\tY Data = %lf\n", log_datax[i], log_datay[i]);
  }
  printf("\n");

  printf("Assuming y = M.ln(x) + C\n\n");
  printf("Constant coefficient (C)    = %lf\n",
         SDA_LograConstantCoeff(log_datax,        // Pointer to X array
                                log_datay,        // Pointer to Y array
                                DATA_LENGTH));    // Array length
  printf("Regression coefficient (M)  = %lf\n",
         SDA_LograRegressionCoeff(log_datax,        // Pointer to X array
                                  log_datay,        // Pointer to Y array
                                  DATA_LENGTH));    // Array length
  printf("Correlation coefficient (r) = %lf\n",
         SDA_LograCorrelationCoeff(log_datax,        // Pointer to X array
                                   log_datay,        // Pointer to Y array
                                   DATA_LENGTH));    // Array length
  printf("Estimated X for Y = 6.0     = %lf\n",
         SDA_LograEstimateX(log_datax,        // Pointer to X array
                            log_datay,        // Pointer to Y array
                            6.0,              // Y value
                            DATA_LENGTH));    // Array length
  printf("Estimated Y for X = 9.0     = %lf\n",
         SDA_LograEstimateY(log_datax,        // Pointer to X array
                            log_datay,        // Pointer to Y array
                            9.0,              // X value
                            DATA_LENGTH));    // Array length

  printf("\nPlease hit any key to continue . . .\n");
  getchar();

  printf("Power Regression analysis\n\n");
  for (SLArrayIndex_t i = 0; i < DATA_LENGTH; i++) {
    printf("X Data = %lf,\tY Data = %lf\n", pow_datax[i], pow_datay[i]);
  }
  printf("\n");

  printf("Assuming y = C.exp(Mx)\n\n");
  printf("Constant coefficient (C)    = %lf\n",
         SDA_PowraConstantCoeff(pow_datax,        // Pointer to X array
                                pow_datay,        // Pointer to Y array
                                DATA_LENGTH));    // Array length
  printf("Regression coefficient (M)  = %lf\n",
         SDA_PowraRegressionCoeff(pow_datax,        // Pointer to X array
                                  pow_datay,        // Pointer to Y array
                                  DATA_LENGTH));    // Array length
  printf("Correlation coefficient (r) = %lf\n",
         SDA_PowraCorrelationCoeff(pow_datax,        // Pointer to X array
                                   pow_datay,        // Pointer to Y array
                                   DATA_LENGTH));    // Array length
  printf("Estimated X for Y = 40.0    = %lf\n",
         SDA_PowraEstimateX(pow_datax,        // Pointer to X array
                            pow_datay,        // Pointer to Y array
                            40.0,             // Y value
                            DATA_LENGTH));    // Array length
  printf("Estimated Y for X = 2.5     = %lf\n",
         SDA_PowraEstimateY(pow_datax,        // Pointer to X array
                            pow_datay,        // Pointer to Y array
                            2.5,              // X value
                            DATA_LENGTH));    // Array length

  printf("\nPlease hit any key to continue . . .\n");
  getchar();

  printf("Exponential Regression analysis\n\n");
  for (SLArrayIndex_t i = 0; i < DATA_LENGTH; i++) {
    printf("X Data = %lf,\tY Data = %lf\n", exp_datax[i], exp_datay[i]);
  }
  printf("\n");

  printf("Assuming y = C.exp(Mx)\n\n");
  printf("Constant coefficient (C)    = %lf\n",
         SDA_ExpraConstantCoeff(exp_datax,        // Pointer to X array
                                exp_datay,        // Pointer to Y array
                                DATA_LENGTH));    // Array length
  printf("Regression coefficient (M)  = %lf\n",
         SDA_ExpraRegressionCoeff(exp_datax,        // Pointer to X array
                                  exp_datay,        // Pointer to Y array
                                  DATA_LENGTH));    // Array length
  printf("Correlation coefficient (r) = %lf\n",
         SDA_ExpraCorrelationCoeff(exp_datax,        // Pointer to X array
                                   exp_datay,        // Pointer to Y array
                                   DATA_LENGTH));    // Array length
  printf("Estimated X for Y = 11.0    = %lf\n",
         SDA_ExpraEstimateX(exp_datax,        // Pointer to X array
                            exp_datay,        // Pointer to Y array
                            11.0,             // Y value
                            DATA_LENGTH));    // Array length
  printf("Estimated Y for X = 0.35    = %lf\n",
         SDA_ExpraEstimateY(exp_datax,        // Pointer to X array
                            exp_datay,        // Pointer to Y array
                            0.35,             // X value
                            DATA_LENGTH));    // Array length

  return (0);
}
