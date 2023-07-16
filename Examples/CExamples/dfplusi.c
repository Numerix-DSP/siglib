// SigLib z-Domain Coefficients From Digital Filter Plus Example
// Copyright (c) 2023 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>                                                 // SigLib DSP library

// Define constants
#define FILTER_ORDER    4

// Declare global variables and arrays
// Sample rate 10K, LPF 1K
static const SLData_t zDomainCoefficientArray[] = {
  5.55522277758905139200e-01, 1.49373148199684147200e+01,
  9.99999999999999795200e-01, 1.79999999999999974400e+02,
  5.55522277758905139200e-01, -1.49373148199684147200e+01,
  9.99999999999999795200e-01, 1.79999999999999974400e+02,
  8.00558623510172467200e-01, 3.27838219623744665600e+01,
  9.99999999999999795200e-01, 1.79999999999999974400e+02,
  8.00558623510172467200e-01, -3.27838219623744665600e+01,
  9.99999999999999795200e-01, 1.79999999999999974400e+02
};


int main (
  void)
{
  SLComplexRect_s pZPlanePoles[FILTER_ORDER];
  SLComplexRect_s pZPlaneZeros[FILTER_ORDER];

  SDA_ZDomainCoefficientReorg (zDomainCoefficientArray,             // Pointer to source z-domain coefficient array
                               pZPlanePoles,                        // Pointer to z-domain poles array
                               pZPlaneZeros,                        // Pointer to z-domain zeros array
                               FILTER_ORDER);                       // Filter order

  for (SLArrayIndex_t i = 0; i < FILTER_ORDER; i++) {
    printf ("P[%d] = %le + j%le\n", i, pZPlanePoles[i].real, pZPlanePoles[i].imag);
    printf ("Z[%d] = %le + j%le\n\n", i, pZPlaneZeros[i].real, pZPlaneZeros[i].imag);
  }

  exit (0);
}
