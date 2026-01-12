// SigLib IIR Filter Coefficients Split/Merge, to/from SigLib format to separate b and a arrays Example
// Copyright (c) 2026 Delta Numerix All rights reserved.

#include <siglib.h>

#define NUMBER_OF_BIQUADS 2
#define FILTER_ORDER 4

int main()
{
  SLData_t bqCoeffs[] = {2., 3., 4., 5., 6., 7., 8., 9., 10., 11.};
  SLData_t bq_b[] = {0., 0., 0., 0., 0., 0.};
  SLData_t bq_a[] = {0., 0., 0., 0., 0., 0.};
  SLData_t bqMerged[] = {0., 0., 0., 0., 0., 0., 0., 0., 0., 0.};
  SLData_t onCoeffs[] = {2., 3., 4., 5., 6., 7., 8., 9., 10.};
  SLData_t on_b[] = {0., 0., 0., 0., 0., 0.};
  SLData_t on_a[] = {0., 0., 0., 0., 0., 0.};
  SLData_t onMerged[] = {0., 0., 0., 0., 0., 0., 0., 0., 0., 0.};

  // Cascaded biquad filter
  printf("bqCoeffs:\n");
  SUF_PrintArray(bqCoeffs, NUMBER_OF_BIQUADS * SIGLIB_IIR_COEFFS_PER_BIQUAD);

  SDA_SplitIIRFilterCoefficients(bqCoeffs, bq_b, bq_a, NUMBER_OF_BIQUADS);
  printf("Split:\n");
  printf("bq_b:\n");
  SUF_PrintArray(bq_b, NUMBER_OF_BIQUADS * 3);    // 3 Coefficients per biquad
  printf("bq_a:\n");
  SUF_PrintArray(bq_a, NUMBER_OF_BIQUADS * 3);    // 3 Coefficients per biquad

  SDA_MergeIIRFilterCoefficients(bq_b, bq_a, bqMerged, NUMBER_OF_BIQUADS);
  printf("Merged:\n");
  printf("bqMerged:\n");
  SUF_PrintArray(bqMerged, NUMBER_OF_BIQUADS * SIGLIB_IIR_COEFFS_PER_BIQUAD);

  // Order N filter
  printf("\n\nonCoeffs:\n");
  SUF_PrintArray(onCoeffs, (FILTER_ORDER * 2) + 1);    // *2 for a and b, +1 for a[0]

  SDA_SplitIIROrderNFilterCoefficients(onCoeffs, on_b, on_a, FILTER_ORDER);
  printf("Split:\n");
  printf("on_b:\n");
  SUF_PrintArray(on_b, FILTER_ORDER + 1);
  printf("on_a:\n");
  SUF_PrintArray(on_a, FILTER_ORDER + 1);

  SDA_MergeIIROrderNFilterCoefficients(on_b, on_a, onMerged, FILTER_ORDER);
  printf("Merged:\n");
  printf("onMerged:\n");
  SUF_PrintArray(onMerged, (FILTER_ORDER * 2) + 1);    // *2 for a and b, +1 for a[0]

  return (0);
}
