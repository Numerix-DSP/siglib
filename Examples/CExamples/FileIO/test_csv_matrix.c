// test_csv.c

// Include files
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <siglib.h>    // SigLib DSP library

// Define constants
#define N_ROWS 10
#define N_COLS 2

// Declare global variables and arrays
SLData_t outputData[N_ROWS][N_COLS] = {
    {0, 10},        {0.000125, 11}, {0.00025, 12},  {0.000375, 13}, {0.0005, 14},
    {0.000625, 15}, {0.00075, 16},  {0.000875, 17}, {0.001, 18},    {0.001125, 19},
};

SLData_t* pInputData;

int main(void)
{
  SLArrayIndex_t nRows, nCols;
  SLArrayIndex_t inputSampleCount;

  printf("Writing to test.csv file\n");

  inputSampleCount = SUF_CsvWriteMatrix((SLData_t*)outputData,    // Pointer to source array
                                        "test.csv",               // Output filename
                                        N_ROWS,                   // Number of rows
                                        N_COLS);                  // Number of columns
  printf("Number of words written to test.csv file: %d\n", inputSampleCount);

  printf("Reading from test.csv file\n");
  inputSampleCount = SUF_CsvReadMatrix(&pInputData,              // Pointer to destination array
                                       "test.csv",               // Input filename
                                       SIGLIB_FIRST_ROW_KEEP,    // First row keep/ignore flag
                                       &nRows,                   // Pointer to number of rows read
                                       &nCols);                  // Pointer to number of columns read
  printf("Number of words read from test.csv file:   %d\n", inputSampleCount);
  printf("Number of rows read from test.csv file:    %d\n", nRows);
  printf("Number of columns read from test.csv file: %d\n", nCols);

  printf("test.csv:\n");
  for (int i = 0; i < nRows; i++) {
    for (int j = 0; j < nCols - 1; j++) {
      printf("%lf, ", *(pInputData + (i * nCols) + j));
    }
    printf("%lf\n", *(pInputData + (i * nCols) + (nCols - 1)));
  }
  printf("\n\n");

  return (0);
}
