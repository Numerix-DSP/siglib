// SigLib insert and delete example program

#include <stdio.h>
#include <siglib.h>

#define SOURCE1_ARRAY_LENGTH 10
#define SOURCE2_ARRAY_LENGTH 3

SLData_t src1[] = {1., 2., 3., 4., 5., 6., 7., 8., 9., 10.};
SLData_t src2[] = {37., 38., 39.};

SLData_t dst[SOURCE1_ARRAY_LENGTH];
SLData_t extracted[SOURCE2_ARRAY_LENGTH];

int main(void)
{
  SDA_Zeros(dst, SOURCE1_ARRAY_LENGTH);

  SDA_InsertSample(src1,                     // Pointer to source array
                   23.,                      // New sample
                   dst,                      // Pointer to destination array
                   3,                        // New sample location
                   SOURCE1_ARRAY_LENGTH);    // Source array length

  printf("\nSource Array:\n");
  SUF_PrintArray(src1, SOURCE1_ARRAY_LENGTH);

  printf("\nDestination Array:\n");
  SUF_PrintArray(dst, SOURCE1_ARRAY_LENGTH);

  SDA_Zeros(dst, SOURCE1_ARRAY_LENGTH);

  SDA_InsertArray(src1,                     // Pointer to source array
                  src2,                     // New sample array pointer
                  dst,                      // Pointer to destination array
                  3,                        // New sample location
                  SOURCE2_ARRAY_LENGTH,     // New sample array length
                  SOURCE1_ARRAY_LENGTH);    // Source array length

  printf("\nSource Array:\n");
  SUF_PrintArray(src1, SOURCE1_ARRAY_LENGTH);

  printf("\nDestination Array:\n");
  SUF_PrintArray(dst, SOURCE1_ARRAY_LENGTH);

  SDA_Zeros(dst, SOURCE1_ARRAY_LENGTH);

  SLData_t extractedSample = SDA_ExtractSample(src1,                     // Pointer to source array
                                               dst,                      // Pointer to destination array
                                               3,                        // Extracted sample location
                                               SOURCE1_ARRAY_LENGTH);    // Source array length

  printf("\nSource Array:\n");
  SUF_PrintArray(src1, SOURCE1_ARRAY_LENGTH);

  printf("\nExtracted sample: %lf\n", extractedSample);

  printf("\nDestination Array:\n");
  SUF_PrintArray(dst, SOURCE1_ARRAY_LENGTH - 1);

  SDA_Zeros(dst, SOURCE1_ARRAY_LENGTH);

  SDA_ExtractArray(src1,                     // Pointer to source array
                   dst,                      // Pointer to destination array
                   extracted,                // Extracted sample array pointer
                   3,                        // Extracted sample location
                   SOURCE2_ARRAY_LENGTH,     // Extracted sample array length
                   SOURCE1_ARRAY_LENGTH);    // Source array length

  printf("\nSource Array:\n");
  SUF_PrintArray(src1, SOURCE1_ARRAY_LENGTH);

  printf("\nExtracted Array:\n");
  SUF_PrintArray(extracted, SOURCE2_ARRAY_LENGTH);

  printf("\nDestination Array:\n");
  SUF_PrintArray(dst, SOURCE1_ARRAY_LENGTH - SOURCE2_ARRAY_LENGTH);

  return (0);
}
