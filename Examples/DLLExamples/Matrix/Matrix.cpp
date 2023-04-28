// Matrix.cpp : Console application to test the SigLib matrix processing functionality
// This program required the Numerix Host Library, which is available from :
// https://www.numerix-dsp.com/files
// This project was generated automatically in Visual Studio it is a command line applicaiton
// Copyright (c) 2023 Alpha Numerix All rights reserved.

// Include files
#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <siglib.h>                                                 // SigLib DSP library

// Define constants
#define MATRIXA_ROWS    3                                           /* Declare matrix A */
#define MATRIXA_COLS    2
#define MATRIXA_SIZE    (MATRIXA_ROWS * MATRIXA_COLS)

#define MATRIXAT_ROWS   MATRIXA_COLS                                /* Declare matrix A Transpose */
#define MATRIXAT_COLS   MATRIXA_ROWS

#define MATRIXB_ROWS    2                                           /* Declare matrix B */
#define MATRIXB_COLS    4
#define MATRIXB_SIZE    (MATRIXB_ROWS * MATRIXB_COLS)

#define MATRIXC_ROWS    3                                           /* Declare matrix C */
#define MATRIXC_COLS    4
#define MATRIXC_SIZE    (MATRIXC_ROWS * MATRIXC_COLS)

#define MATRIXD_ROWS    3                                           /* Declare matrix D */
#define MATRIXD_COLS    3
#define MATRIXD_SIZE    (MATRIXD_ROWS * MATRIXD_COLS)

#define MATRIXD_INV_ROWS    MATRIXD_ROWS                            /* Declare matrix D inverse */
#define MATRIXD_INV_COLS    MATRIXD_COLS
#define MATRIXD_INV_SIZE    (MATRIXD_INV_ROWS * MATRIXD_INV_COLS)

#define MATRIXI_ROWS    3                                           /* Declare identity matrix (I) */
#define MATRIXI_COLS    3
#define MATRIXI_SIZE    (MATRIXI_ROWS * MATRIXI_COLS)

/* Declare global variables and arrays */
SLData_t       *pMatrixa, *pMatrixb, *pMatrixc, *pMatrixd, *pMatrixd_inv, *pMatrixi;
SLData_t       *pTempSourceArray, *pSubstituteIndex, *pScalingFactor;
SLFixData_t    *pRowInterchangeIndex;


int main (
  int argc,
  SLChar_t * argv[])
{


  SLData_t        Determinant;
  SLFixData_t     ErrorCode;


/*
   Allocate matrices
 */
  pMatrixa = SUF_VectorArrayAllocate (MATRIXA_SIZE);
  pMatrixb = SUF_VectorArrayAllocate (MATRIXB_SIZE);
  pMatrixc = SUF_VectorArrayAllocate (MATRIXC_SIZE);
  pMatrixd = SUF_VectorArrayAllocate (MATRIXD_SIZE);
  pMatrixd_inv = SUF_VectorArrayAllocate (MATRIXD_INV_SIZE);
  pMatrixi = SUF_VectorArrayAllocate (MATRIXI_SIZE);

  pTempSourceArray = SUF_VectorArrayAllocate (MATRIXD_SIZE);
  pSubstituteIndex = SUF_VectorArrayAllocate (MATRIXD_ROWS);
  pRowInterchangeIndex = SUF_IndexArrayAllocate (MATRIXD_ROWS);
  pScalingFactor = SUF_VectorArrayAllocate (MATRIXD_ROWS);

/*
   Fill matrix A
 */
  *pMatrixa++ = 1;
  *pMatrixa++ = 2;
  *pMatrixa++ = 3;
  *pMatrixa++ = 4;
  *pMatrixa++ = 5;
  *pMatrixa++ = 6;
  pMatrixa -= MATRIXA_SIZE;

/*
   Print matrix A contents
 */
  printf ("Source matrix A :\n");
  SUF_PrintMatrix (pMatrixa, MATRIXA_ROWS, MATRIXA_COLS);

/*
   Copy matrix A to matrix C for temporary backup
 */
  SMX_Copy (pMatrixa, pMatrixc, MATRIXA_ROWS, MATRIXA_COLS);

/*
   MultpImagDataly matrix A by a scalar
 */
  SMX_ScalarMultiply (pMatrixa, 3.0, pMatrixa, MATRIXA_ROWS, MATRIXA_COLS);
  printf ("\nMatrix A multiplied by 3.0 :\n");
  SUF_PrintMatrix (pMatrixa, MATRIXA_ROWS, MATRIXA_COLS);

/*
   Restore matrix A and transpose
 */
  SMX_Copy (pMatrixc, pMatrixa, MATRIXA_ROWS, MATRIXA_COLS);
  SMX_Transpose (pMatrixa, pMatrixb, MATRIXA_ROWS, MATRIXA_COLS);
  printf ("\nMatrix A transposed :\n");
  SUF_PrintMatrix (pMatrixb, MATRIXAT_ROWS, MATRIXAT_COLS);

/*
   Fill matrix B
 */
  *pMatrixb++ = 1;
  *pMatrixb++ = 2;
  *pMatrixb++ = 3;
  *pMatrixb++ = 4;
  *pMatrixb++ = 5;
  *pMatrixb++ = 6;
  *pMatrixb++ = 7;
  *pMatrixb++ = 8;
  pMatrixb -= MATRIXB_SIZE;

/*
   Print matrix B contents with matrix A dimensions
 */
  printf ("\nSource matrix B :\n");
  SUF_PrintMatrix (pMatrixb, MATRIXA_ROWS, MATRIXA_COLS);

/*
   Restore matrix A and add to matrix B
 */
  SMX_Copy (pMatrixc, pMatrixa, MATRIXA_ROWS, MATRIXA_COLS);
  SMX_Add (pMatrixa, pMatrixb, pMatrixc, MATRIXA_ROWS, MATRIXA_COLS);
  printf ("\nA + B matrix :\n");
  SUF_PrintMatrix (pMatrixc, MATRIXA_ROWS, MATRIXA_COLS);

/*
   Restore matrix A and subtract from matrix B
 */
  SMX_Copy (pMatrixc, pMatrixa, MATRIXA_ROWS, MATRIXA_COLS);
  SMX_Subtract (pMatrixa, pMatrixb, pMatrixc, MATRIXA_ROWS, MATRIXA_COLS);
  SMX_Copy (pMatrixc, pMatrixa, MATRIXA_ROWS, MATRIXA_COLS);

  printf ("\nPlease hit any key to continue . . .\n");
  getchar ();

/*
   Print matrix B contents with correct dimensions
 */
  printf ("\nNew source matrix B :\n");
  SUF_PrintMatrix (pMatrixb, MATRIXB_ROWS, MATRIXB_COLS);

/*
   MultpImagDataly matrix A by matrix B
 */
  SMX_Multiply (pMatrixa, pMatrixb, pMatrixc, MATRIXA_ROWS, MATRIXA_COLS, MATRIXB_COLS);
  printf ("\nA * B matrix :\n");
  SUF_PrintMatrix (pMatrixc, MATRIXC_ROWS, MATRIXC_COLS);

  printf ("\nPlease hit any key to continue . . .\n");
  getchar ();

/*
   Fill matrix D
 */
  *pMatrixd++ = 3;
  *pMatrixd++ = 2;
  *pMatrixd++ = 4;
  *pMatrixd++ = 1;
  *pMatrixd++ = 2;
  *pMatrixd++ = 5;
  *pMatrixd++ = 7;
  *pMatrixd++ = 1;
  *pMatrixd++ = 4;
  pMatrixd -= MATRIXD_SIZE;

/*
   Print matrix D
 */
  printf ("\nSource matrix D :\n");
  SUF_PrintMatrix (pMatrixd, MATRIXD_ROWS, MATRIXD_COLS);


/*
   Invert matrix D
 */
  ErrorCode = SMX_Inverse (pMatrixd, pMatrixd_inv, pTempSourceArray, pSubstituteIndex, pRowInterchangeIndex, pScalingFactor, MATRIXD_ROWS);

  if (ErrorCode == SIGLIB_ERROR) {
    printf ("Matrix inverse failure - The matrix was singular.\n\n");
  }
  else if (ErrorCode == SIGLIB_MEM_ALLOC_ERROR) {
    printf ("Memory allocation failure in matrix inverse function.\n\n");
  }

/*
   Print inverse of matrix D
 */
  printf ("\nInverse of matrix D :\n");
  SUF_PrintMatrix (pMatrixd_inv, MATRIXD_INV_ROWS, MATRIXD_INV_COLS);

  printf ("Inverse should be :\n");
  printf ("    0.1579   -0.2105    0.1053\n");
  printf ("    1.6316   -0.8421   -0.5789\n");
  printf ("   -0.6842    0.5789    0.2105\n\n");

/*
   MultpImagDataly matrix D by the inverse of matrix D
 */
  SMX_Multiply (pMatrixd, pMatrixd_inv, pMatrixi, MATRIXD_ROWS, MATRIXD_COLS, MATRIXD_INV_COLS);

/*
   Threshold Results to zero out values close to zero
 */
  SDA_Threshold (pMatrixi, pMatrixi, SIGLIB_MIN_THRESHOLD, SIGLIB_DOUBLE_SIDED_THOLD, MATRIXD_SIZE);
  printf ("\nD * D^-1 matrix :\n");
  SUF_PrintMatrix (pMatrixi, MATRIXD_ROWS, MATRIXD_COLS);

/*
   Print the determinant of matrix D
 */
  Determinant = SMX_Determinant (pMatrixd, pTempSourceArray, pRowInterchangeIndex, pScalingFactor, MATRIXD_ROWS);
  printf ("\nThe determinant of matrix D is : %lf\n", Determinant);

  printf ("\nThe determinant of matrix D should be : 19\n", Determinant);

/*
   Create identity matrix (I)
 */
  SMX_CreateIdentity (pMatrixi, MATRIXI_ROWS);

/*
   Print identity matrix (I)
 */
  printf ("\nIdentity matrix (I) :\n");
  SUF_PrintMatrix (pMatrixi, MATRIXI_ROWS, MATRIXI_COLS);


  SUF_MemoryFree (pMatrixa);                                        /* Free memory */
  SUF_MemoryFree (pMatrixb);
  SUF_MemoryFree (pMatrixc);
  SUF_MemoryFree (pMatrixd);
  SUF_MemoryFree (pMatrixd_inv);
  SUF_MemoryFree (pMatrixi);

  return 0;
}
