// SigLib Matrix Mathematics Example
// Note - in this program we will declare the matrices as multi-dimensional
// but we need to pass them by reference as arrays of type SLData_t so
// we need to cast the array pointers as follows : (SLData_t *)
// Copyright (c) 2022 Sigma Numerix Ltd. All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>                                 // SigLib DSP library

// Define constants
#define NUMBER_OF_ROWS          3       // Number of rows and columns in matrix
#define NUMBER_OF_COLUMNS       4
#define MAX_DIMENSION           4

#define NUMBER_OF_ROWS_T        4       // Number of rows and columns in transposed matrix
#define NUMBER_OF_COLUMNS_T     3

#define LARGE_ARRAY_DIMENSION   8       // Dimension of large matrix

// Declare global variables and arrays

static SLData_t SrcMatrix [MAX_DIMENSION][MAX_DIMENSION] = {
    { 1.0,  2.0,  3.0,  4.0},
    { 5.0,  6.0,  7.0,  8.0},
    { 9.0, 10.0, 11.0, 12.0},
    {13.0, 14.0, 15.0, 16.0}
};

static SLData_t DstMatrix1 [NUMBER_OF_ROWS][NUMBER_OF_COLUMNS] = {
    {0.0, 0.0, 0.0},
    {0.0, 0.0, 0.0},
    {0.0, 0.0, 0.0}
};

static SLData_t DstMatrix2 [MAX_DIMENSION+1][MAX_DIMENSION+1] = {
    {0.0, 0.0, 0.0, 0.0, 0.0},
    {0.0, 0.0, 0.0, 0.0, 0.0},
    {0.0, 0.0, 0.0, 0.0, 0.0},
    {0.0, 0.0, 0.0, 0.0, 0.0},
    {0.0, 0.0, 0.0, 0.0, 0.0}
};

static SLData_t InsertMatrix [] = {
    81.0, 82.0, 83.0, 84.0, 85.0
};

static SLData_t LargeSrcMatrix [LARGE_ARRAY_DIMENSION][LARGE_ARRAY_DIMENSION] = {
    { 1.0,  2.0,  3.0,  4.0,  5.0,  6.0,  7.0,  8.0},
    {11.0, 12.0, 13.0, 14.0, 15.0, 16.0, 17.0, 18.0},
    {21.0, 22.0, 23.0, 24.0, 25.0, 26.0, 27.0, 28.0},
    {31.0, 32.0, 33.0, 34.0, 35.0, 36.0, 37.0, 38.0},
    {41.0, 42.0, 43.0, 44.0, 45.0, 46.0, 47.0, 48.0},
    {51.0, 52.0, 53.0, 54.0, 55.0, 56.0, 57.0, 58.0},
    {61.0, 62.0, 63.0, 64.0, 65.0, 66.0, 67.0, 68.0},
    {71.0, 72.0, 73.0, 74.0, 75.0, 76.0, 77.0, 78.0}
};


int main (void)
{
    SLArrayIndex_t  i;


    printf ("Source matrix 1\n");
    SUF_PrintMatrix ((SLData_t *)SrcMatrix, NUMBER_OF_ROWS, NUMBER_OF_COLUMNS);

                    // Transpose
    printf ("Not in-place transpose\n");
    SMX_Transpose ((SLData_t *)SrcMatrix,           // Pointer to source matrix
                   (SLData_t *)DstMatrix1,          // Pointer to destination matrix
                   NUMBER_OF_ROWS,                  // Source matrix # of rows
                   NUMBER_OF_COLUMNS);              // Source matrix # cols
    SUF_PrintMatrix ((SLData_t *)DstMatrix1, NUMBER_OF_ROWS_T, NUMBER_OF_COLUMNS_T);

                    // In place
    printf ("Source matrix 2\n");
    SUF_PrintMatrix ((SLData_t *)SrcMatrix, MAX_DIMENSION, MAX_DIMENSION);
    printf ("In-place transpose\n");
    SMX_Transpose ((SLData_t *)SrcMatrix,           // Pointer to source matrix
                   (SLData_t *)SrcMatrix,           // Pointer to destination matrix
                   MAX_DIMENSION,                   // Source matrix # of rows
                   MAX_DIMENSION);                  // Source matrix # cols
    SUF_PrintMatrix ((SLData_t *)SrcMatrix, MAX_DIMENSION, MAX_DIMENSION);

                    // Re-set Source matrix
    SMX_Transpose ((SLData_t *)SrcMatrix,           // Pointer to source matrix
                   (SLData_t *)SrcMatrix,           // Pointer to destination matrix
                   MAX_DIMENSION,                   // Source matrix # of rows
                   MAX_DIMENSION);                  // Source matrix # cols

                    // Rotate clockwise
    printf ("Source matrix 1 rotated clockwise\n");
    SMX_RotateClockwise ((SLData_t *)SrcMatrix,     // Pointer to source matrix
                         (SLData_t *)DstMatrix1,    // Pointer to destination matrix
                         NUMBER_OF_ROWS,            // Number of rows in matrix
                         NUMBER_OF_COLUMNS);        // Number of columns in matrix
    SUF_PrintMatrix ((SLData_t *)DstMatrix1, NUMBER_OF_ROWS_T, NUMBER_OF_COLUMNS_T);

                    // Rotate anti-clockwise
    printf ("Source matrix 1 rotated Anti-clockwise\n");
    SMX_RotateAntiClockwise ((SLData_t *)SrcMatrix, // Pointer to source matrix
                             (SLData_t *)DstMatrix1,// Pointer to destination matrix
                             NUMBER_OF_ROWS,        // Number of rows in matrix
                             NUMBER_OF_COLUMNS);    // Number of columns in matrix
    SUF_PrintMatrix ((SLData_t *)DstMatrix1, NUMBER_OF_ROWS_T, NUMBER_OF_COLUMNS_T);

                    // Rotate clockwise
    printf ("Source matrix 2 rotated clockwise\n");
    SMX_RotateClockwise ((SLData_t *)SrcMatrix,     // Pointer to source matrix
                         (SLData_t *)DstMatrix1,    // Pointer to destination matrix
                         MAX_DIMENSION,             // Number of rows in matrix
                         MAX_DIMENSION);            // Number of columns in matrix
    SUF_PrintMatrix ((SLData_t *)DstMatrix1, MAX_DIMENSION, MAX_DIMENSION);

                    // Rotate anti-clockwise
    printf ("Source matrix 2 rotated Anti-clockwise\n");
    SMX_RotateAntiClockwise ((SLData_t *)SrcMatrix, // Pointer to source matrix
                             (SLData_t *)DstMatrix1,// Pointer to destination matrix
                             MAX_DIMENSION,         // Number of rows in matrix
                             MAX_DIMENSION);        // Number of columns in matrix
    SUF_PrintMatrix ((SLData_t *)DstMatrix1, MAX_DIMENSION, MAX_DIMENSION);

    printf ("Hit <Carriage Return> to continue . . .\n"); getchar();

    printf ("Extracted diagonal from source matrix 2\n");
    SMX_ExtractDiagonal ((SLData_t *)SrcMatrix,     // Pointer to source matrix
                         (SLData_t *)DstMatrix2,    // Pointer to destination matrix
                         MAX_DIMENSION);            // Longtst dimension (Row or column) in matrix
    for (i = 0; i < MAX_DIMENSION; i++)
    {
        printf ("%d, ", (short)DstMatrix2[0][i]);
    }
    printf ("\n\n");

    printf ("Inserted diagonal into source matrix 2\n");
    SMX_InsertDiagonal ((SLData_t *)DstMatrix2,     // Pointer to source matrix
                        (SLData_t *)InsertMatrix,   // Pointer to insertion matrix
                        (SLData_t *)DstMatrix2,     // Pointer to destination matrix
                        MAX_DIMENSION);             // Longtst dimension (Row or column) in matrix
    printf ("Modified matrix 2\n");
    SUF_PrintMatrix ((SLData_t *)DstMatrix2, MAX_DIMENSION, MAX_DIMENSION);


    printf ("Source matrix 1\n");
    SUF_PrintMatrix ((SLData_t *)SrcMatrix, NUMBER_OF_ROWS, NUMBER_OF_COLUMNS);

    printf ("Sum of columns in source matrix 1\n");
    SMX_Sum ((SLData_t *)SrcMatrix,                 // Pointer to source matrix
             (SLData_t *)DstMatrix2,                // Pointer to destination matrix
             NUMBER_OF_ROWS,                        // Number of rows in matrix
             NUMBER_OF_COLUMNS);                    // Number of columns in matrix
    for (i = 0; i < NUMBER_OF_COLUMNS; i++) {
        printf ("%d, ", (short)DstMatrix2[0][i]);
    }
    printf ("\n\n");


    printf ("Extracted row #2 from source matrix 1\n");
    SMX_ExtractRow ((SLData_t *)SrcMatrix,          // Pointer to source matrix
                    (SLData_t *)DstMatrix2,         // Pointer to destination matrix
                    2,                              // Row number being extracted
                    NUMBER_OF_COLUMNS);             // Number of columns in matrix
    for (i = 0; i < NUMBER_OF_COLUMNS; i++) {
        printf ("%d, ", (short)DstMatrix2[0][i]);
    }
    printf ("\n\n");

    printf ("Extracted column #3 from source matrix 1\n");
    SMX_ExtractColumn ((SLData_t *)SrcMatrix,       // Pointer to source matrix
                       (SLData_t *)DstMatrix2,      // Pointer to destination matrix
                       3,                           // Column to extract
                       NUMBER_OF_ROWS,              // Number of rows in matrix
                       NUMBER_OF_COLUMNS);          // Number of columns in matrix
    for (i = 0; i < NUMBER_OF_ROWS; i++) {
        printf ("%d, ", (short)DstMatrix2[0][i]);
    }
    printf ("\n\n");

    printf ("Hit <Carriage Return> to continue . . .\n"); getchar();

    printf ("Matrix 2\n");
    SUF_PrintMatrix ((SLData_t *)DstMatrix1, NUMBER_OF_ROWS_T, NUMBER_OF_COLUMNS_T);

    printf ("Inserted row #3 into source matrix 1\n");
    SMX_InsertRow ((SLData_t *)DstMatrix1,          // Pointer to source matrix
                   (SLData_t *)InsertMatrix,        // Pointer to row to insert
                   (SLData_t *)DstMatrix1,          // Pointer to destination matrix
                   3,                               // Row to insert
                   NUMBER_OF_ROWS_T,                // Number of rows in matrix
                   NUMBER_OF_COLUMNS_T);            // Number of columns in matrix
    printf ("Modified matrix 2\n");
    SUF_PrintMatrix ((SLData_t *)DstMatrix1, NUMBER_OF_ROWS_T, NUMBER_OF_COLUMNS_T);

    printf ("Inserted column #2 into source matrix 1\n");
    SMX_InsertColumn ((SLData_t *)DstMatrix1,       // Pointer to source matrix
                      (SLData_t *)InsertMatrix,     // Pointer to column to insert
                      (SLData_t *)DstMatrix1,       // Pointer to destination matrix
                      2,                            // Column to insert
                      NUMBER_OF_ROWS_T,             // Number of rows in matrix
                      NUMBER_OF_COLUMNS_T);         // Number of columns in matrix
    printf ("Modified matrix 2\n");
    SUF_PrintMatrix ((SLData_t *)DstMatrix1, NUMBER_OF_ROWS_T, NUMBER_OF_COLUMNS_T);

    printf ("Hit <Carriage Return> to continue . . .\n"); getchar();

    printf ("Original - Odd length\n");
    SUF_PrintMatrix ((SLData_t *)LargeSrcMatrix, 5, 5);

    printf ("Reflect - Odd length\n");
    SMX_Reflect ((SLData_t *)LargeSrcMatrix,        // Pointer to source matrix
                 (SLData_t *)LargeSrcMatrix,        // Pointer to destination matrix
                 5,                                 // Number of rows in matrix
                 5);                                // Number of columns in matrix
    SUF_PrintMatrix ((SLData_t *)LargeSrcMatrix, 5, 5);
    SMX_Reflect ((SLData_t *)LargeSrcMatrix,        // Pointer to source matrix
                 (SLData_t *)LargeSrcMatrix,        // Pointer to destination matrix
                 5,                                 // Number of rows in matrix
                 5);                                // Number of columns in matrix

    printf ("Flip - Odd length\n");
    SMX_Flip ((SLData_t *)LargeSrcMatrix,           // Pointer to source matrix
              (SLData_t *)LargeSrcMatrix,           // Pointer to destination matrix
              5,                                    // Number of rows in matrix
              5);                                   // Number of columns in matrix
    SUF_PrintMatrix ((SLData_t *)LargeSrcMatrix, 5, 5);
    SMX_Flip ((SLData_t *)LargeSrcMatrix,           // Pointer to source matrix
              (SLData_t *)LargeSrcMatrix,           // Pointer to destination matrix
              5,                                    // Number of rows in matrix
              5);                                   // Number of columns in matrix

    printf ("Original - Even length\n");
    SUF_PrintMatrix ((SLData_t *)LargeSrcMatrix, 6, 6);

    printf ("Reflect - Even length\n");
    SMX_Reflect ((SLData_t *)LargeSrcMatrix,        // Pointer to source matrix
                 (SLData_t *)LargeSrcMatrix,        // Pointer to destination matrix
                 6,                                 // Number of rows in matrix
                 6);                                // Number of columns in matrix
    SUF_PrintMatrix ((SLData_t *)LargeSrcMatrix, 6, 6);
    SMX_Reflect ((SLData_t *)LargeSrcMatrix,        // Pointer to source matrix
                 (SLData_t *)LargeSrcMatrix,        // Pointer to destination matrix
                 6,                                 // Number of rows in matrix
                 6);                                // Number of columns in matrix

    printf ("Flip - Even length\n");
    SMX_Flip ((SLData_t *)LargeSrcMatrix,           // Pointer to source matrix
              (SLData_t *)LargeSrcMatrix,           // Pointer to destination matrix
              6,                                    // Number of rows in matrix
              6);                                   // Number of columns in matrix
    SUF_PrintMatrix ((SLData_t *)LargeSrcMatrix, 6, 6);
    SMX_Flip ((SLData_t *)LargeSrcMatrix,           // Pointer to source matrix
              (SLData_t *)LargeSrcMatrix,           // Pointer to destination matrix
              6,                                    // Number of rows in matrix
              6);                                   // Number of columns in matrix


    printf ("Hit <Carriage Return> to continue . . .\n"); getchar();

    printf ("Region Insert / Extract\n");
    printf ("Original matrix\n");
    SUF_PrintMatrix ((SLData_t *)LargeSrcMatrix, LARGE_ARRAY_DIMENSION, LARGE_ARRAY_DIMENSION);
    printf ("Matrix to insert\n");
    SUF_PrintMatrix ((SLData_t *)DstMatrix1, NUMBER_OF_ROWS_T, NUMBER_OF_COLUMNS_T);

    printf ("Inserting modified matrix 2 into large matrix at (3,4)\n");
    SMX_InsertRegion ((SLData_t *)LargeSrcMatrix,   // Pointer to source matrix
                      (SLData_t *)DstMatrix1,       // Pointer to new region data
                      (SLData_t *)LargeSrcMatrix,   // Pointer to destination matrix
                      3,                            // Starting row to insert data
                      4,                            // Starting column to insert data
                      NUMBER_OF_ROWS_T,             // Number of rows in new data matrix
                      NUMBER_OF_COLUMNS_T,          // Number of columns in new data matrix
                      LARGE_ARRAY_DIMENSION,        // Number of rows in matrix
                      LARGE_ARRAY_DIMENSION);       // Number of columns in matrix
    SUF_PrintMatrix ((SLData_t *)LargeSrcMatrix, LARGE_ARRAY_DIMENSION, LARGE_ARRAY_DIMENSION);

    printf ("Extracting region from large matrix from (2,3)\n");
    SMX_ExtractRegion ((SLData_t *)LargeSrcMatrix,  // Pointer to source matrix
                       (SLData_t *)DstMatrix1,      // Pointer to destination matrix
                       2,                           // Starting row to extract data
                       3,                           // Starting column to extract data
                       NUMBER_OF_ROWS_T,            // Number of rows in region to extract
                       NUMBER_OF_COLUMNS_T,         // Number of columns in region to extract
                       LARGE_ARRAY_DIMENSION);      // Number of columns in matrix
    printf ("Extracted region\n");
    SUF_PrintMatrix ((SLData_t *)DstMatrix1, NUMBER_OF_ROWS_T, NUMBER_OF_COLUMNS_T);


    printf ("Hit <Carriage Return> to continue . . .\n"); getchar();


    printf ("Source matrix 1\n");
    SUF_PrintMatrix ((SLData_t *)SrcMatrix, MAX_DIMENSION, MAX_DIMENSION);

    printf ("Insert new row - 0\n");
    SMX_InsertNewRow ((SLData_t *)SrcMatrix,        // Pointer to source matrix
                      (SLData_t *)InsertMatrix,     // New data to insert into row
                      (SLData_t *)DstMatrix2,       // Pointer to destination matrix
                      0,                            // Row to insert new data into
                      MAX_DIMENSION,                // Number of rows in matrix
                      MAX_DIMENSION);               // Number of columns in matrix
    SUF_PrintMatrix ((SLData_t *)DstMatrix2, MAX_DIMENSION+1, MAX_DIMENSION);

    printf ("Delete old row - 0\n");
    SMX_DeleteOldRow ((SLData_t *)DstMatrix2,       // Pointer to source matrix
                      (SLData_t *)DstMatrix2,       // Pointer to destination matrix
                      0,                            // Row number to delete
                      MAX_DIMENSION+1,              // Number of rows in matrix
                      MAX_DIMENSION);               // Number of columns in matrix
    SUF_PrintMatrix ((SLData_t *)DstMatrix2, MAX_DIMENSION, MAX_DIMENSION);

    printf ("Insert new row - 2\n");
    SMX_InsertNewRow ((SLData_t *)SrcMatrix,        // Pointer to source matrix
                      (SLData_t *)InsertMatrix,     // New data to insert into row
                      (SLData_t *)DstMatrix2,       // Pointer to destination matrix
                      2,                            // Row to insert new data into
                      MAX_DIMENSION,                // Number of rows in matrix
                      MAX_DIMENSION);               // Number of columns in matrix
    SUF_PrintMatrix ((SLData_t *)DstMatrix2, MAX_DIMENSION+1, MAX_DIMENSION);

    printf ("Delete old row - 2\n");
    SMX_DeleteOldRow ((SLData_t *)DstMatrix2,       // Pointer to source matrix
                      (SLData_t *)DstMatrix2,       // Pointer to destination matrix
                      2,                            // Row number to delete
                      MAX_DIMENSION+1,              // Number of rows in matrix
                      MAX_DIMENSION);               // Number of columns in matrix
    SUF_PrintMatrix ((SLData_t *)DstMatrix2, MAX_DIMENSION, MAX_DIMENSION);

    printf ("Insert new row - 4\n");
    SMX_InsertNewRow ((SLData_t *)SrcMatrix,        // Pointer to source matrix
                      (SLData_t *)InsertMatrix,     // New data to insert into row
                      (SLData_t *)DstMatrix2,       // Pointer to destination matrix
                      4,                            // Row to insert new data into
                      MAX_DIMENSION,                // Number of rows in matrix
                      MAX_DIMENSION);               // Number of columns in matrix
    SUF_PrintMatrix ((SLData_t *)DstMatrix2, MAX_DIMENSION+1, MAX_DIMENSION);

    printf ("Delete old row - 4\n");
    SMX_DeleteOldRow ((SLData_t *)DstMatrix2,       // Pointer to source matrix
                      (SLData_t *)DstMatrix2,       // Pointer to destination matrix
                      4,                            // Row number to delete
                      MAX_DIMENSION+1,              // Number of rows in matrix
                      MAX_DIMENSION);               // Number of columns in matrix
    SUF_PrintMatrix ((SLData_t *)DstMatrix2, MAX_DIMENSION, MAX_DIMENSION);


    printf ("Insert new column - 0\n");
    SMX_InsertNewColumn ((SLData_t *)SrcMatrix,     // Pointer to source matrix
                         (SLData_t *)InsertMatrix,  // New data to insert into column
                         (SLData_t *)DstMatrix2,    // Pointer to destination matrix
                         0,                         // Column to insert new data into
                         MAX_DIMENSION,             // Number of rows in matrix
                         MAX_DIMENSION);            // Number of columns in matrix
    SUF_PrintMatrix ((SLData_t *)DstMatrix2, MAX_DIMENSION, MAX_DIMENSION+1);

    printf ("Delete old column - 0\n");
    SMX_DeleteOldColumn ((SLData_t *)DstMatrix2,    // Pointer to source matrix
                         (SLData_t *)DstMatrix2,    // Pointer to destination matrix
                         0,                         // Column number to delete
                         MAX_DIMENSION,             // Number of rows in matrix
                         MAX_DIMENSION+1);          // Number of columns in matrix
    SUF_PrintMatrix ((SLData_t *)DstMatrix2, MAX_DIMENSION, MAX_DIMENSION);

    printf ("Insert new column - 2\n");
    SMX_InsertNewColumn ((SLData_t *)SrcMatrix,     // Pointer to source matrix
                         (SLData_t *)InsertMatrix,  // New data to insert into column
                         (SLData_t *)DstMatrix2,    // Pointer to destination matrix
                         2,                         // Column to insert new data into
                         MAX_DIMENSION,             // Number of rows in matrix
                         MAX_DIMENSION);            // Number of columns in matrix
    SUF_PrintMatrix ((SLData_t *)DstMatrix2, MAX_DIMENSION, MAX_DIMENSION+1);

    printf ("Delete old column - 2\n");
    SMX_DeleteOldColumn ((SLData_t *)DstMatrix2,    // Pointer to source matrix
                         (SLData_t *)DstMatrix2,    // Pointer to destination matrix
                         2,                         // Column number to delete
                         MAX_DIMENSION,             // Number of rows in matrix
                         MAX_DIMENSION+1);          // Number of columns in matrix
    SUF_PrintMatrix ((SLData_t *)DstMatrix2, MAX_DIMENSION, MAX_DIMENSION);

    printf ("Insert new column - 4\n");
    SMX_InsertNewColumn ((SLData_t *)SrcMatrix,     // Pointer to source matrix
                         (SLData_t *)InsertMatrix,  // New data to insert into column
                         (SLData_t *)DstMatrix2,    // Pointer to destination matrix
                         4,                         // Column to insert new data into
                         MAX_DIMENSION,             // Number of rows in matrix
                         MAX_DIMENSION);            // Number of columns in matrix
    SUF_PrintMatrix ((SLData_t *)DstMatrix2, MAX_DIMENSION, MAX_DIMENSION+1);

    printf ("Delete old column - 4\n");
    SMX_DeleteOldColumn ((SLData_t *)DstMatrix2,    // Pointer to source matrix
                         (SLData_t *)DstMatrix2,    // Pointer to destination matrix
                         4,                         // Column number to delete
                         MAX_DIMENSION,             // Number of rows in matrix
                         MAX_DIMENSION+1);          // Number of columns in matrix
    SUF_PrintMatrix ((SLData_t *)DstMatrix2, MAX_DIMENSION, MAX_DIMENSION);


    printf ("Hit <Carriage Return> to continue . . .\n"); getchar();

    printf ("Source matrix 1\n");
    SUF_PrintMatrix ((SLData_t *)SrcMatrix, MAX_DIMENSION, MAX_DIMENSION);

    printf ("Swapping rows 1 and 3\n");
    SMX_SwapRows ((SLData_t *)SrcMatrix,            // Pointer to source matrix
                  (SLData_t *)DstMatrix1,           // Pointer to destination matrix
                  1,                                // Row number 1 to swap
                  3,                                // Row number 2 to swap
                  MAX_DIMENSION,                    // Number of rows in matrix
                  MAX_DIMENSION);                   // Number of columns in matrix
    SUF_PrintMatrix ((SLData_t *)DstMatrix1, MAX_DIMENSION, MAX_DIMENSION);

    printf ("Swapping columns 1 and 3\n");
    SMX_SwapColumns ((SLData_t *)SrcMatrix,         // Pointer to source matrix
                     (SLData_t *)DstMatrix1,        // Pointer to destination matrix
                     1,                             // Column number 1 to swap
                     3,                             // Column number 2 to swap
                     MAX_DIMENSION,                 // Number of rows in matrix
                     MAX_DIMENSION);                // Number of columns in matrix
    SUF_PrintMatrix ((SLData_t *)DstMatrix1, MAX_DIMENSION, MAX_DIMENSION);

    exit(0);
}

