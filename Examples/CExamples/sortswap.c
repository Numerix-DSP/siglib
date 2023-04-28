// SigLib Sort / Swap Example
// Copyright (c) 2023 Alpha Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>                                                 // SigLib DSP library


int main (
  void)
{
  SLData_t        a = 1.;
  SLData_t        b = 2.;
  SLData_t        c = 3.;

  printf ("\n\na = %lf\n", a);
  printf ("b = %lf\n", b);

  SDS_Swap (a, b);

  printf ("\n\na = %lf\n", a);
  printf ("b = %lf\n", b);

  a = 3.;
  b = 2.;
  c = 1.;

  printf ("\n\na = %lf\n", a);
  printf ("b = %lf\n", b);
  printf ("c = %lf\n", c);

  SDS_Sort3 (a, b, c);

  printf ("\n\na = %lf\n", a);
  printf ("b = %lf\n", b);
  printf ("c = %lf\n", c);

  exit (0);
}
