// SigLib Array and Matrix Shuffling Example
// Copyright (c) 2026 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>    // SigLib DSP library

// Define constants
#define ARRAY_LENGTH 9    // Declare array length

#define MATRIX_ROWS 5    // Declare matrix geometry
#define MATRIX_COLS 5
#define MATRIX_SIZE (MATRIX_ROWS * MATRIX_COLS)

// Declare global variables and arrays
static SLData_t src[] = {1., 2., 3., 4., 5., 6., 7., 8., 9.};

static SLData_t dst[ARRAY_LENGTH];

static SLData_t Msrc[MATRIX_ROWS][MATRIX_COLS] = {
    {11., 12., 13., 14., 15.}, {21., 22., 23., 24., 25.}, {31., 32., 33., 34., 35.}, {41., 42., 43., 44., 45.}, {51., 52., 53., 54., 55.}};

int main(void)
{
  SLData_t* pMsrc = (SLData_t*)Msrc;    // Setup source matrix pointer
  SLData_t* pMdst = SUF_VectorArrayAllocate(MATRIX_SIZE);
  SLData_t* pMcolTmp1 = SUF_VectorArrayAllocate(MATRIX_ROWS);
  SLData_t* pMcolTmp2 = SUF_VectorArrayAllocate(MATRIX_ROWS);
  SLData_t* pMrowTmp = SUF_VectorArrayAllocate(MATRIX_COLS);

  printf("Source Array:\n");
  SUF_PrintArray(src, ARRAY_LENGTH);
  SDA_Shuffle(src, dst, ARRAY_LENGTH);
  printf("\nShuffled Array:\n");
  SUF_PrintArray(dst, ARRAY_LENGTH);

  printf("\nSource matrix:\n");
  SUF_PrintMatrix(pMsrc, MATRIX_ROWS, MATRIX_COLS);
  SMX_ShuffleColumns(pMsrc, pMdst, pMcolTmp1, pMcolTmp2, MATRIX_ROWS, MATRIX_COLS);
  printf("\nShuffled columns matrix:\n");
  SUF_PrintMatrix(pMdst, MATRIX_ROWS, MATRIX_COLS);

  printf("\nSource matrix:\n");
  SUF_PrintMatrix(pMsrc, MATRIX_ROWS, MATRIX_COLS);
  SMX_ShuffleRows(pMsrc, pMdst, pMrowTmp, MATRIX_ROWS, MATRIX_COLS);
  printf("\nShuffled rows matrix:\n");
  SUF_PrintMatrix(pMdst, MATRIX_ROWS, MATRIX_COLS);

  SUF_MemoryFree(pMdst);    // Free memory
  SUF_MemoryFree(pMcolTmp1);
  SUF_MemoryFree(pMcolTmp2);
  SUF_MemoryFree(pMrowTmp);

  return (0);
}
