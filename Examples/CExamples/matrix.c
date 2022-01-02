// SigLib Matrix Processing Example
// Copyright (c) 2022 Sigma Numerix Ltd. All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>                         // SigLib DSP library

// Define constants
#define MATRIXA_ROWS    3                   // Declare matrix A
#define MATRIXA_COLS    2
#define MATRIXA_SIZE    (MATRIXA_ROWS * MATRIXA_COLS)

#define MATRIXAT_ROWS   MATRIXA_COLS        // Declare matrix A Transpose
#define MATRIXAT_COLS   MATRIXA_ROWS

#define MATRIXB_ROWS    2                   // Declare matrix B
#define MATRIXB_COLS    4
#define MATRIXB_SIZE    (MATRIXB_ROWS * MATRIXB_COLS)

#define MATRIXC_ROWS    3                   // Declare matrix C
#define MATRIXC_COLS    4
#define MATRIXC_SIZE    (MATRIXC_ROWS * MATRIXC_COLS)

#define MATRIXD_ROWS    3                   // Declare matrix D
#define MATRIXD_COLS    3
#define MATRIXD_SIZE    (MATRIXD_ROWS * MATRIXD_COLS)

#define MATRIXD_INV_ROWS    MATRIXD_ROWS    // Declare matrix D inverse
#define MATRIXD_INV_COLS    MATRIXD_COLS
#define MATRIXD_INV_SIZE    (MATRIXD_INV_ROWS * MATRIXD_INV_COLS)

#define MATRIXI_ROWS    3                   // Declare identity matrix (I)
#define MATRIXI_COLS    3
#define MATRIXI_SIZE    (MATRIXI_ROWS * MATRIXI_COLS)

// Declare global variables and arrays
static SLData_t         *pMatrixa, * pMatrixb, *pMatrixc, *pMatrixd, *pMatrixd_inv, *pMatrixi;

static SLData_t         *pTempSourceArray, *pSubstituteIndex, *pScalingFactor;
static SLArrayIndex_t   *pRowInterchangeIndex;

static const SLData_t   SrcReal2x2 [] = {3.0, 1.0, 4.0, 2.0};
static SLData_t         DstReal2x2 [4];

static const SLComplexRect_s SrcComplex2x2 [] = {
//  {3.0, 0.0},                         // Use these values to test the real component of the matrix inverse function
//  {1.0, 0.0},
//  {4.0, 0.0},
//  {2.0, 0.0},
    {0.0, 3.0},                         // Use these values to test the imaginary component of the matrix inverse function
    {0.0, 1.0},
    {0.0, 4.0},
    {0.0, 2.0},
};
static SLComplexRect_s  DstComplex2x2 [4];


int main (void)
{
    SLData_t    Determinant;
    SLError_t   SigLibErrorCode;

                    // Allocate matrices
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

                    // Fill matrix A
    *pMatrixa++ = 1; *pMatrixa++ = 2;
    *pMatrixa++ = 3; *pMatrixa++ = 4;
    *pMatrixa++ = 5; *pMatrixa++ = 6;
    pMatrixa -= MATRIXA_SIZE;

                    // Print matrix A contents
    printf("Source matrix A :\n");
    SUF_PrintMatrix (pMatrixa, MATRIXA_ROWS, MATRIXA_COLS);

                    // Copy matrix A to matrix C for temporary backup
    SMX_Copy (pMatrixa,                             // Pointer to source matrix
              pMatrixc,                             // Pointer to destination matrix
              MATRIXA_ROWS,                         // Number of rows
              MATRIXA_COLS);                        // Number of columns

                    // MultpImagDataly matrix A by a scalar
    SMX_ScalarMultiply (pMatrixa,                   // Pointer to source matrix
                        3.0,                        // Scalar multiplier
                        pMatrixa,                   // Pointer to destination matrix
                        MATRIXA_ROWS,               // Number of rows
                        MATRIXA_COLS);              // Number of columns
    printf("\nMatrix A multiplied by 3.0 :\n");
    SUF_PrintMatrix (pMatrixa, MATRIXA_ROWS, MATRIXA_COLS);

                    // Restore matrix A and transpose
    SMX_Copy (pMatrixc,                             // Pointer to source matrix
              pMatrixa,                             // Pointer to destination matrix
              MATRIXA_ROWS,                         // Number of rows
              MATRIXA_COLS);                        // Number of columns
    SMX_Transpose (pMatrixa,                        // Pointer to source matrix
                   pMatrixb,                        // Pointer to destination matrix
                   MATRIXA_ROWS,                    // Number of rows
                   MATRIXA_COLS);                   // Number of columns
    printf("\nMatrix A transposed :\n");
    SUF_PrintMatrix (pMatrixb, MATRIXAT_ROWS, MATRIXAT_COLS);

                    // Fill matrix B
    *pMatrixb++ = 1; *pMatrixb++ = 2; *pMatrixb++ = 3; *pMatrixb++ = 4;
    *pMatrixb++ = 5; *pMatrixb++ = 6; *pMatrixb++ = 7; *pMatrixb++ = 8;
    pMatrixb -= MATRIXB_SIZE;

                    // Print matrix B contents with matrix A dimensions
    printf("\nSource matrix B :\n");
    SUF_PrintMatrix (pMatrixb, MATRIXA_ROWS, MATRIXA_COLS);

                    // Restore matrix A and add to matrix B
    SMX_Copy (pMatrixc,                             // Pointer to source matrix
              pMatrixa,                             // Pointer to destination matrix
              MATRIXA_ROWS,                         // Number of rows
              MATRIXA_COLS);                        // Number of columns
    SMX_Add (pMatrixa,                              // Pointer to source matrix 1
             pMatrixb,                              // Pointer to source matrix 2
             pMatrixc,                              // Pointer to destination matrix
             MATRIXA_ROWS,                          // Number of rows
             MATRIXA_COLS);                         // Number of columns
    printf("\nA + B matrix :\n");
    SUF_PrintMatrix (pMatrixc, MATRIXA_ROWS, MATRIXA_COLS);

                    // Restore matrix A and subtract from matrix B
    SMX_Copy (pMatrixc,                             // Pointer to source matrix
              pMatrixa,                             // Pointer to destination matrix
              MATRIXA_ROWS,                         // Number of rows
              MATRIXA_COLS);                        // Number of columns
    SMX_Subtract (pMatrixa,                         // Pointer to source matrix 1
                  pMatrixb,                         // Pointer to source matrix 2
                  pMatrixc,                         // Pointer to destination matrix
                  MATRIXA_ROWS,                     // Number of rows
                  MATRIXA_COLS);                    // Number of columns
    SMX_Copy (pMatrixc,                             // Pointer to source matrix
              pMatrixa,                             // Pointer to destination matrix
              MATRIXA_ROWS,                         // Number of rows
              MATRIXA_COLS);                        // Number of columns

    printf("\nPlease hit any key to continue . . .\n");
    getchar();

                    // Print matrix B contents with correct dimensions
    printf("\nNew source matrix B :\n");
    SUF_PrintMatrix (pMatrixb, MATRIXB_ROWS, MATRIXB_COLS);

                    // MultpImagDataly matrix A by matrix B
    SMX_Multiply (pMatrixa,                         // Pointer to source matrix 1
                  pMatrixb,                         // Pointer to source matrix 2
                  pMatrixc,                         // Pointer to destination matrix
                  MATRIXA_ROWS,                     // Number of rows matrix 1
                  MATRIXA_COLS,                     // Number of columns matrix 1
                  MATRIXB_COLS);                    // Number of columns matrix 2
    printf("\nA * B matrix :\n");
    SUF_PrintMatrix (pMatrixc, MATRIXC_ROWS, MATRIXC_COLS);

    printf("\nPlease hit any key to continue . . .\n");
    getchar();

                    // Fill matrix D
    *pMatrixd++ = 3; *pMatrixd++ = 2; *pMatrixd++ = 4;
    *pMatrixd++ = 1; *pMatrixd++ = 2; *pMatrixd++ = 5;
    *pMatrixd++ = 7; *pMatrixd++ = 1; *pMatrixd++ = 4;
    pMatrixd -= MATRIXD_SIZE;

                    // Print matrix D
    printf("\nSource matrix D :\n");
    SUF_PrintMatrix (pMatrixd, MATRIXD_ROWS, MATRIXD_COLS);


                    // Invert matrix D
    SigLibErrorCode =
        SMX_Inverse (pMatrixd,                      // Pointer to source matrix
                     pMatrixd_inv,                  // Pointer to destination matrix
                     pTempSourceArray,              // Temporary array for source
                     pSubstituteIndex,              // Index substitution array
                     pRowInterchangeIndex,          // Row interchange indices
                     pScalingFactor,                // Scaling factor array
                     MATRIXD_ROWS);                 // Number of rows and columns in matrix

    if (SigLibErrorCode == SIGLIB_ERROR) {
        printf ("Matrix inverse failure - The matrix was singular.\n\n");
    }
    else if (SigLibErrorCode != SIGLIB_NO_ERROR) {
        printf ("SigLib Error Message :%s\n", SUF_StrError (SigLibErrorCode));
    }

                    // Print inverse of matrix D
    printf("\nInverse of matrix D :\n");
    SUF_PrintMatrix (pMatrixd_inv, MATRIXD_INV_ROWS, MATRIXD_INV_COLS);

    printf ("Inverse should be :\n");
    printf ("    0.1579   -0.2105    0.1053\n");
    printf ("    1.6316   -0.8421   -0.5789\n");
    printf ("   -0.6842    0.5789    0.2105\n\n");

                    // MultpImagDataly matrix D by the inverse of matrix D
    SMX_Multiply (pMatrixd,                         // Pointer to source matrix 1
                  pMatrixd_inv,                     // Pointer to source matrix 2
                  pMatrixi,                         // Pointer to destination matrix
                  MATRIXD_ROWS,                     // Number of rows matrix 1
                  MATRIXD_COLS,                     // Number of columns matrix 1
                  MATRIXD_INV_COLS);                // Number of columns matrix 2

                    // Threshold results to zero out values close to zero
    SDA_Threshold (pMatrixi,                        // Pointer to source matrix
                   pMatrixi,                        // Pointer to destination matrix
                   SIGLIB_MIN_THRESHOLD,            // Threshold level
                   SIGLIB_DOUBLE_SIDED_THOLD,       // Threshold type
                   MATRIXD_SIZE);                   // Matrix size - Number of rows x number of columns
    printf("\nD * D^-1 matrix :\n");
    SUF_PrintMatrix (pMatrixi, MATRIXD_ROWS, MATRIXD_COLS);

                    // Print the determinant of matrix D
    Determinant =
        SMX_Determinant (pMatrixd,                  // Pointer to source matrix
                         pTempSourceArray,          // Temporary array for source
                         pRowInterchangeIndex,      // Row interchange indices
                         pScalingFactor,            // Scaling factor array
                         MATRIXD_ROWS);             // Number of rows and columns in square matrix
    printf ("\nThe determinant of matrix D is : %lf\n", Determinant);

    printf ("\nThe determinant of matrix D should be : 19\n");

                    // Create identity matrix (I)
    SMX_CreateIdentity (pMatrixi,                   // Pointer to destination matrix
                        MATRIXI_ROWS);              // Number of rows and columns in square matrix

                    // Print identity matrix (I)
    printf("\nIdentity matrix (I) :\n");
    SUF_PrintMatrix (pMatrixi, MATRIXI_ROWS, MATRIXI_COLS);


    printf ("\nSource matrix = \n\t%lf, %lf\n\t%lf, %lf\n", SrcReal2x2[0], SrcReal2x2[1], SrcReal2x2[2], SrcReal2x2[3]);
    SMX_Inverse2x2 (SrcReal2x2, DstReal2x2);
    printf ("Destination matrix = \n\t%lf, %lf\n\t%lf, %lf\n", DstReal2x2[0], DstReal2x2[1], DstReal2x2[2], DstReal2x2[3]);

    printf ("Source matrix = \n\t%lf + j%lf, %lf + j%lf\n\t%lf + j%lf, %lf + j%lf\n",
            SrcComplex2x2[0].real, SrcComplex2x2[0].imag, SrcComplex2x2[1].real, SrcComplex2x2[1].imag,
            SrcComplex2x2[2].real, SrcComplex2x2[2].imag, SrcComplex2x2[3].real, SrcComplex2x2[3].imag);
    SMX_ComplexInverse2x2 (SrcComplex2x2, DstComplex2x2);
    printf ("Destination matrix = \n\t%lf + j%lf, %lf + j%lf\n\t%lf + j%lf, %lf + j%lf\n",
            DstComplex2x2[0].real, DstComplex2x2[0].imag, DstComplex2x2[1].real, DstComplex2x2[1].imag,
            DstComplex2x2[2].real, DstComplex2x2[2].imag, DstComplex2x2[3].real, DstComplex2x2[3].imag);


    SUF_MemoryFree (pMatrixa);                      // Free memory
    SUF_MemoryFree (pMatrixb);
    SUF_MemoryFree (pMatrixc);
    SUF_MemoryFree (pMatrixd);
    SUF_MemoryFree (pMatrixd_inv);
    SUF_MemoryFree (pMatrixi);

    exit(0);
}
