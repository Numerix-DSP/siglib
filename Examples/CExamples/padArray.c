// Siglib padding example program
// Copyright (c) 2024 Delta Numerix All rights reserved.

#include <stdio.h>
#include <stdlib.h>
#include <printf.h>
#include <siglib.h>

int main()
{
  // SLData_t a[] = {1., 2., 3., 4., 5., 6., 7., 2., 1.};
  SLData_t a[] = {1., 2., 3., 4., 5., 6., 7.};
  SLData_t a_ext[20];

  printf("Not in-place:\n");
  SDA_Zeros(a_ext, 20);
  SDA_Pad(a, a_ext, SIGLIB_ARRAY_PAD_MODE_EVEN, 2, SAI_NumberOfElements(a));
  printf("Even extension:\n");
  SUF_PrintArray(a_ext, 20);

  SDA_Zeros(a_ext, 20);
  SDA_Pad(a, a_ext, SIGLIB_ARRAY_PAD_MODE_ODD, 2, SAI_NumberOfElements(a));
  printf("Odd extension:\n");
  SUF_PrintArray(a_ext, 20);

  SDA_Zeros(a_ext, 20);
  SDA_Pad(a, a_ext, SIGLIB_ARRAY_PAD_MODE_CONSTANT, 2, SAI_NumberOfElements(a));
  printf("Constant extension:\n");
  SUF_PrintArray(a_ext, 20);

  printf("In-place:\n");
  SDA_Zeros(a_ext, 20);
  SDA_Copy(a, a_ext, SAI_NumberOfElements(a));
  SDA_Pad(a_ext, a_ext, SIGLIB_ARRAY_PAD_MODE_EVEN, 2, SAI_NumberOfElements(a));
  printf("Even extension:\n");
  SUF_PrintArray(a_ext, 20);

  SDA_Zeros(a_ext, 20);
  SDA_Copy(a, a_ext, SAI_NumberOfElements(a));
  SDA_Pad(a_ext, a_ext, SIGLIB_ARRAY_PAD_MODE_ODD, 2, SAI_NumberOfElements(a));
  printf("Odd extension:\n");
  SUF_PrintArray(a_ext, 20);

  SDA_Zeros(a_ext, 20);
  SDA_Copy(a, a_ext, SAI_NumberOfElements(a));
  SDA_Pad(a_ext, a_ext, SIGLIB_ARRAY_PAD_MODE_CONSTANT, 2, SAI_NumberOfElements(a));
  printf("Constant extension:\n");
  SUF_PrintArray(a_ext, 20);

  return 0;
}
