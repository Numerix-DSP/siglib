// SigLib - Move Peak Towards Dead Band Example
// Copyright (c) 2022 Sigma Numerix Ltd. All rights reserved.

// Include files
#include <stdio.h>
#include <stdlib.h>
#include <siglib.h>                                 // SigLib DSP library

// Define constants
#define SAMPLE_LENGTH           10
#define DEAD_BAND_LOW_POINT     4
#define DEAD_BAND_HIGH_POINT    6

// Declare global variables and arrays
static const SLData_t    pSrc1[] = {
    1, 7, 0, -3, 2, 5, 6, 3, -3, -4
};
static const SLData_t    pSrc2[] = {
    1, 7, 0, -3, 2, 5, 6, 3, -9, -4
};
static SLData_t     pDst[10];


int main (void)

{
    SLFixData_t i;

    printf ("\nSource Array 1\n");
    for (i = 0; i < SAMPLE_LENGTH; i++) {printf ("%1.1lf, ", *(pSrc1+i)); } printf ("\n");
    SDA_MovePeakTowardsDeadBand (pSrc1,                 // Pointer to source array
                                 pDst,                  // Pointer to destination array
                                 DEAD_BAND_LOW_POINT,   // Dead-band low-point
                                 DEAD_BAND_HIGH_POINT,  // Dead-band high-point
                                 SAMPLE_LENGTH);        // Array length
    for (i = 0; i < SAMPLE_LENGTH; i++) {printf ("%1.1lf, ", *(pDst+i)); } printf ("\n");
    SDA_MovePeakTowardsDeadBand (pDst,                  // Pointer to source array
                                 pDst,                  // Pointer to destination array
                                 DEAD_BAND_LOW_POINT,   // Dead-band low-point
                                 DEAD_BAND_HIGH_POINT,  // Dead-band high-point
                                 SAMPLE_LENGTH);        // Array length
    for (i = 0; i < SAMPLE_LENGTH; i++) {printf ("%1.1lf, ", *(pDst+i)); } printf ("\n");
    SDA_MovePeakTowardsDeadBand (pDst,                  // Pointer to source array
                                 pDst,                  // Pointer to destination array
                                 DEAD_BAND_LOW_POINT,   // Dead-band low-point
                                 DEAD_BAND_HIGH_POINT,  // Dead-band high-point
                                 SAMPLE_LENGTH);        // Array length
    for (i = 0; i < SAMPLE_LENGTH; i++) {printf ("%1.1lf, ", *(pDst+i)); } printf ("\n");
    SDA_MovePeakTowardsDeadBand (pDst,                  // Pointer to source array
                                 pDst,                  // Pointer to destination array
                                 DEAD_BAND_LOW_POINT,   // Dead-band low-point
                                 DEAD_BAND_HIGH_POINT,  // Dead-band high-point
                                 SAMPLE_LENGTH);        // Array length
    for (i = 0; i < SAMPLE_LENGTH; i++) {printf ("%1.1lf, ", *(pDst+i)); } printf ("\n");

    printf ("\nSource Array 2\n");
    for (i = 0; i < SAMPLE_LENGTH; i++) {printf ("%1.1lf, ", *(pSrc2+i)); } printf ("\n");
    SDA_MovePeakTowardsDeadBand (pSrc2,                 // Pointer to source array
                                 pDst,                  // Pointer to destination array
                                 DEAD_BAND_LOW_POINT,   // Dead-band low-point
                                 DEAD_BAND_HIGH_POINT,  // Dead-band high-point
                                 SAMPLE_LENGTH);        // Array length
    for (i = 0; i < SAMPLE_LENGTH; i++) {printf ("%1.1lf, ", *(pDst+i)); } printf ("\n");
    SDA_MovePeakTowardsDeadBand (pDst,                  // Pointer to source array
                                 pDst,                  // Pointer to destination array
                                 DEAD_BAND_LOW_POINT,   // Dead-band low-point
                                 DEAD_BAND_HIGH_POINT,  // Dead-band high-point
                                 SAMPLE_LENGTH);        // Array length
    for (i = 0; i < SAMPLE_LENGTH; i++) {printf ("%1.1lf, ", *(pDst+i)); } printf ("\n");
    SDA_MovePeakTowardsDeadBand (pDst,                  // Pointer to source array
                                 pDst,                  // Pointer to destination array
                                 DEAD_BAND_LOW_POINT,   // Dead-band low-point
                                 DEAD_BAND_HIGH_POINT,  // Dead-band high-point
                                 SAMPLE_LENGTH);        // Array length
    for (i = 0; i < SAMPLE_LENGTH; i++) {printf ("%1.1lf, ", *(pDst+i)); } printf ("\n");
    SDA_MovePeakTowardsDeadBand (pDst,                  // Pointer to source array
                                 pDst,                  // Pointer to destination array
                                 DEAD_BAND_LOW_POINT,   // Dead-band low-point
                                 DEAD_BAND_HIGH_POINT,  // Dead-band high-point
                                 SAMPLE_LENGTH);        // Array length
    for (i = 0; i < SAMPLE_LENGTH; i++) {printf ("%1.1lf, ", *(pDst+i)); } printf ("\n");

    exit(0);
}

