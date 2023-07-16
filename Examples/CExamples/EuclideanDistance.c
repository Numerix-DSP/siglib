// SigLib example for calculating the speed of sound with change in temperature
// Copyright (c) 2023 Delta Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <math.h>
#include <siglib.h>                                                 // SigLib DSP library

#define SRC_LEN     3

SLData_t        src1x[] = { 1., 2., 3. };
SLData_t        src1y[] = { 4., 5., -6. };
SLData_t        src2x[] = { 2., 1., -1. };
SLData_t        src2y[] = { -5., -3., -2. };

SLData_t        dst[SRC_LEN];

SLComplexRect_s srcVector1[] = { { 1., 2. }, { 3., 4. }, { 5., -6. } };
SLComplexRect_s srcVector2[] = { { 2., 1. }, { -1., -5. }, { -3., -2. } };


int main (
  void)
{

  printf ("SCV_EuclideanDistance (Sample): %.2lf\n\n", SCV_EuclideanDistance (srcVector1[0], srcVector2[0]));

  SCA_EuclideanDistance (srcVector1, srcVector2, dst, SRC_LEN);
  printf ("SCA_EuclideanDistance (Array): %.2lf, %.2lf, %.2lf\n\n", dst[0], dst[1], dst[2]);

  printf ("SCV_EuclideanDistanceSquared (Sample): %.2lf\n\n", SCV_EuclideanDistanceSquared (srcVector1[0], srcVector2[0]));

  SCA_EuclideanDistanceSquared (srcVector1, srcVector2, dst, SRC_LEN);
  printf ("SCA_EuclideanDistanceSquared (Array): %.2lf, %.2lf, %.2lf\n\n", dst[0], dst[1], dst[2]);


  printf ("SDS_EuclideanDistance (Sample): %.2lf\n\n", SDS_EuclideanDistance (src1x[0], src1y[0], src2x[0], src2y[0]));

  SDA_EuclideanDistance (src1x, src1y, src2x, src2y, dst, SRC_LEN);
  printf ("SDA_EuclideanDistance (Array) : %.2lf, %.2lf, %.2lf\n\n", dst[0], dst[1], dst[2]);

  printf ("SDS_EuclideanDistanceSquared (Sample): %.2lf\n\n", SDS_EuclideanDistanceSquared (src1x[0], src1y[0], src2x[0], src2y[0]));

  SDA_EuclideanDistanceSquared (src1x, src1y, src2x, src2y, dst, SRC_LEN);
  printf ("SDA_EuclideanDistanceSquared (Array) : %.2lf, %.2lf, %.2lf\n\n", dst[0], dst[1], dst[2]);

  exit (0);
}
