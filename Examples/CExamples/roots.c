// SigLib Example for SDS_Roots and SCV_Roots
// This program calculates the real factors of the polynomial: ax^2 + bx +c = 0
// The values of a, b and c can be supplied on the command line alternatively,
// if they are no then the program uses some pre-calculated values. Copyright
// (c) 2023 Delta Numerix All rights reserved.

// Include files
#include <siglib.h>    // SigLib DSP library
#include <stdio.h>

int main(int argc, char** argv)
{
  SLData_t Root1, Root2;
  SLComplexRect_s CRoot1, CRoot2;
  SLError_t SigLibErrorCode;

  if (argc == 4) {
    printf("Bi-quadratic equation: %lfx^2 + %lfx + %lf = 0.0\n", atof(argv[1]), atof(argv[2]), atof(argv[3]));
    SigLibErrorCode = SDS_Roots(atof(argv[1]),    // a
                                atof(argv[2]),    // b
                                atof(argv[3]),    // c
                                &Root1,           // Pointer to root # 1
                                &Root2);          // Pointer to root # 2
    if (SigLibErrorCode != SIGLIB_NO_ERROR) {
      printf("SigLib Error Message:%s\n", SUF_StrError(SigLibErrorCode));
      printf("There are no real factors for the polynomial provided!\n");
    } else {
      printf("The factors for the polynomial are: %lf and %lf\n", Root1, Root2);
      printf("The factorized equation is: ");
      if (-Root1 >= SIGLIB_ZERO) {
        printf("(x + %lf).", -Root1);
      } else {
        printf("(x - %lf).", Root1);
      }
      if (-Root2 >= SIGLIB_ZERO) {
        printf("(x + %lf)", -Root2);
      } else {
        printf("(x - %lf)", Root2);
      }
      printf(" = 0.0\n\n");
    }
  }

  else {
    printf("x^2 - 5x + 6 = 0\n");
    SDS_Roots(1.0,        // a
              -5.0,       // b
              6.0,        // c
              &Root1,     // Pointer to root # 1
              &Root2);    // Pointer to root # 2
    printf("Roots = %lf, %lf\n\n", Root1, Root2);

    printf("x^2 + 19/15x - 6/15 = 0\n");
    SDS_Roots(1.0,            // a
              19.0 / 15.0,    // b
              -6.0 / 15.0,    // c
              &Root1,         // Pointer to root # 1
              &Root2);        // Pointer to root # 2
    printf("Roots = %lf, %lf\n\n", Root1, Root2);

    printf("15x^2 + 19x - 6 = 0\n");
    SDS_Roots(15.0,       // a
              19.0,       // b
              -6.0,       // c
              &Root1,     // Pointer to root # 1
              &Root2);    // Pointer to root # 2
    printf("Roots = %lf, %lf\n\n", Root1, Root2);

    printf("x^2 + (1-j7)x + (-22 +j7) = 0\n");
    SCV_Roots(SCV_Rectangular(1.0, 0.0),      // a
              SCV_Rectangular(1.0, -7.0),     // b
              SCV_Rectangular(-22.0, 7.0),    // c
              &CRoot1,                        // Pointer to root # 1
              &CRoot2);                       // Pointer to root # 2
    printf("Roots = %lf + j%lf, %lf + j%lf\n\n", CRoot1.real, CRoot1.imag, CRoot2.real, CRoot2.imag);

    printf("3x^2 + (3-j21)x + (-66 +j21) = 0\n");
    SCV_Roots(SCV_Rectangular(3.0, 0.0),       // a
              SCV_Rectangular(3.0, -21.0),     // b
              SCV_Rectangular(-66.0, 21.0),    // c
              &CRoot1,                         // Pointer to root # 1
              &CRoot2);                        // Pointer to root # 2
    printf("Roots = %lf + j%lf, %lf + j%lf\n\n", CRoot1.real, CRoot1.imag, CRoot2.real, CRoot2.imag);
  }

  return (0);
}
