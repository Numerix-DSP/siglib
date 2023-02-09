// SigLib Modulo Incrementing / Decrementing Example
// Copyright (c) 2022 Sigma Numerix Ltd. All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>                                                 // SigLib DSP library


int main (
  void)
{
  SLFixData_t     Count = 4;

  printf ("SigLib modulo in(de)crementing Example - modulo 7\n");

  printf ("Incrementing by 1, modulo 7\n");
  printf ("Starting count = %d\n\n", Count);

  Count = SUF_ModuloIncrement (Count, 1, 7);
  printf ("|N++|7 = %d\n", Count);
  Count = SUF_ModuloIncrement (Count, 1, 7);
  printf ("|N++|7 = %d\n", Count);
  Count = SUF_ModuloIncrement (Count, 1, 7);
  printf ("|N++|7 = %d\n", Count);
  Count = SUF_ModuloIncrement (Count, 1, 7);
  printf ("|N++|7 = %d\n", Count);
  Count = SUF_ModuloIncrement (Count, 1, 7);
  printf ("|N++|7 = %d\n", Count);
  Count = SUF_ModuloIncrement (Count, 1, 7);
  printf ("|N++|7 = %d\n", Count);
  Count = SUF_ModuloIncrement (Count, 1, 7);
  printf ("|N++|7 = %d\n\n", Count);

  printf ("Decrementing by 1, modulo 7\n");
  Count = SUF_ModuloDecrement (Count, 1, 7);
  printf ("|N--|7 = %d\n", Count);
  Count = SUF_ModuloDecrement (Count, 1, 7);
  printf ("|N--|7 = %d\n", Count);
  Count = SUF_ModuloDecrement (Count, 1, 7);
  printf ("|N--|7 = %d\n", Count);
  Count = SUF_ModuloDecrement (Count, 1, 7);
  printf ("|N--|7 = %d\n", Count);
  Count = SUF_ModuloDecrement (Count, 1, 7);
  printf ("|N--|7 = %d\n", Count);
  Count = SUF_ModuloDecrement (Count, 1, 7);
  printf ("|N--|7 = %d\n", Count);
  Count = SUF_ModuloDecrement (Count, 1, 7);
  printf ("|N--|7 = %d\n\n", Count);

  printf ("Incrementing by 2, modulo 7\n");
  Count = SUF_ModuloIncrement (Count, 2, 7);
  printf ("|N+=2|7 = %d\n", Count);
  Count = SUF_ModuloIncrement (Count, 2, 7);
  printf ("|N+=2|7 = %d\n", Count);
  Count = SUF_ModuloIncrement (Count, 2, 7);
  printf ("|N+=2|7 = %d\n", Count);
  Count = SUF_ModuloIncrement (Count, 2, 7);
  printf ("|N+=2|7 = %d\n", Count);
  Count = SUF_ModuloIncrement (Count, 2, 7);
  printf ("|N+=2|7 = %d\n", Count);
  Count = SUF_ModuloIncrement (Count, 2, 7);
  printf ("|N+=2|7 = %d\n", Count);
  Count = SUF_ModuloIncrement (Count, 2, 7);
  printf ("|N+=2|7 = %d\n\n", Count);

  printf ("Decrementing by 2, modulo 7\n");
  Count = SUF_ModuloDecrement (Count, 2, 7);
  printf ("|N-=2|7 = %d\n", Count);
  Count = SUF_ModuloDecrement (Count, 2, 7);
  printf ("|N-=2|7 = %d\n", Count);
  Count = SUF_ModuloDecrement (Count, 2, 7);
  printf ("|N-=2|7 = %d\n", Count);
  Count = SUF_ModuloDecrement (Count, 2, 7);
  printf ("|N-=2|7 = %d\n", Count);
  Count = SUF_ModuloDecrement (Count, 2, 7);
  printf ("|N-=2|7 = %d\n", Count);
  Count = SUF_ModuloDecrement (Count, 2, 7);
  printf ("|N-=2|7 = %d\n", Count);
  Count = SUF_ModuloDecrement (Count, 2, 7);
  printf ("|N-=2|7 = %d\n\n", Count);

  exit (0);
}
