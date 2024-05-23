// IIR Bessel digital filter functions
// This software is provided under the terms of the GNU General Public License
// V2, as published by the Free Software Foundation. Copyright (c) 1999-2024
// Delta Numerix, All rights reserved.

/* Even order analog Bessel filter pole locations
Only include positive conjugate pair values
Frequency normalized - Provides 3dB attenuation at 1 radian per second.

Look-up table reference :
  www.crbond.com/papers/bsf2.pdf
Translation reference :
  http://www.ece.unh.edu/courses/ece707_3/Docs/DSP/IIR/IIr.htm
*/

#include <siglib.h>    // SigLib DSP library

#define DELAY_NORMAL 0    // Set to '1' for delay normalized poles

#if DELAY_NORMAL
SLComplexRect_s SPlaneBesselPoles1[] = {-1.0, 0.0};

SLComplexRect_s SPlaneBesselPoles2[] = {-1.5, 0.8660254038};

SLComplexRect_s SPlaneBesselPoles3[] = {

    {-1.8389073227, 1.7543809598}, {-2.3221853546, 0.0000000000}};

SLComplexRect_s SPlaneBesselPoles4[] = {{-2.1037893972, 2.6574180419}, {-2.8962106028, 0.8672341289}};

SLComplexRect_s SPlaneBesselPoles5[] = {{-2.3246743032, 3.5710229203}, {-3.3519563992, 1.7426614162}, {-3.6467385953, 0.0000000000}};

SLComplexRect_s SPlaneBesselPoles6[] = {{-2.5159322478, 4.4926729537}, {-3.7357083563, 2.6262723114}, {-4.2483593959, 0.8675096732}};

SLComplexRect_s SPlaneBesselPoles7[] = {
    {-2.6856768789, 5.4206941307}, {-4.0701391636, 3.5171740477}, {-4.7582905282, 1.7392860611}, {-4.9717868585, 0.0000000000}};

SLComplexRect_s SPlaneBesselPoles8[] = {
    {-2.8389839489, 6.3539112986}, {-4.3682892172, 4.4144425005}, {-5.2048407906, 2.6161751526}, {-5.5878860433, 0.8676144454}};

SLComplexRect_s SPlaneBesselPoles9[] = {{-2.9792607982, 7.2914636883},
                                        {-4.6384398872, 5.3172716754},
                                        {-5.6044218195, 3.4981569179},
                                        {-6.1293679043, 1.7378483835},
                                        {-6.2970191817, 0.0000000000}};

SLComplexRect_s SPlaneBesselPoles10[] = {{-3.1089162336, 8.2326994591},
                                         {-4.8862195669, 6.2249854825},
                                         {-5.9675283286, 4.3849471889},
                                         {-6.6152909655, 2.6115679208},
                                         {-6.9220449054, 0.8676651955}};

#else    // Frequency normalized

SLComplexRect_s SPlaneBesselPoles1[] = {{-1.0000000000, 0.0000000000}};

SLComplexRect_s SPlaneBesselPoles2[] = {{-1.10160133059, 0.636009824757}};

SLComplexRect_s SPlaneBesselPoles3[] = {{-1.0474091610, 0.9992644363}, {-1.3226757999, 0.0000000000}};

SLComplexRect_s SPlaneBesselPoles4[] = {{-0.995208764350, 1.25710573945}, {-1.37006783055, 0.410249717494}};

SLComplexRect_s SPlaneBesselPoles5[] = {{-0.9576765486, 1.4711243207}, {-1.3808773259, 0.7179095876}, {-1.5023162714, 0.0000000000}};

SLComplexRect_s SPlaneBesselPoles6[] = {{-0.930656522947, 1.66186326894}, {-1.38185809760, 0.971471890712}, {-1.57149040362, 0.320896374221}};

SLComplexRect_s SPlaneBesselPoles7[] = {
    {-0.9098677806, 1.8364513530}, {-1.3789032168, 1.1915667778}, {-1.6120387662, 0.5892445069}, {-1.6843681793, 0.0000000000}};

SLComplexRect_s SPlaneBesselPoles8[] = {
    {-0.892869718847, 1.99832584364}, {-1.37384121764, 1.38835657588}, {-1.63693941813, 0.822795625139}, {-1.75740840040, 0.272867575103}};

SLComplexRect_s SPlaneBesselPoles9[] = {{-0.8783992762, 2.1498005243},
                                        {-1.3675883098, 1.5677337122},
                                        {-1.6523964846, 1.0313895670},
                                        {-1.8071705350, 0.5123837306},
                                        {-1.8566005012, 0.0000000000}};

SLComplexRect_s SPlaneBesselPoles10[] = {{-0.865756901707, 2.29260483098},
                                         {-1.36069227838, 1.73350574267},
                                         {-1.66181024140, 1.22110021857},
                                         {-1.84219624443, 0.727257597722},
                                         {-1.92761969145, 0.241623471082}};
#endif

void BesselFilter(filter_t* FilterSpecification)

{

  /*
            d0
    H(s) = -----
           Bn(s)

    Where n is the filter order and :
          (2*n)!
    d0 = --------
         2^n * n!

             n
    Bn(s) = Sum (d[k] * s^k),   k = 0, ..., n
            k=0

                ((2*n)-k)!
    d[k] = -----------------------
           (2^(n-k)) * k! * (n-k)!

    Filter poles are scaled by d0^(1/n)

  */

  SLArrayIndex_t i;

  //  char ttString [80];
  //  sprintf (ttString, "NumberOfBiquads = %d", NumberOfBiquads);
  //  wxMessageBox (ttString, "BesselFilter", wxOK | wxICON_EXCLAMATION, NULL);

  //  wxMessageBox ("Got Here", "BesselFilter", wxOK | wxICON_EXCLAMATION,
  //  NULL);

#if COMMENT
  SLData_t n, k;
  SLData_t d0, dk;

  for (n = 1; n <= 10; n++) {
    //    printf ("Factorial %d = %lf\n", (short) n, SDS_Factorial (n));
    //    printf ("2^n %d = %lf\n", (short) n, SDS_Pow (SIGLIB_TWO, n));

    d0 = SDS_Factorial(SIGLIB_TWO * n) / (SDS_Pow(SIGLIB_TWO, n) * SDS_Factorial(n));
    //    printf ("d0 %d = %lf\n", (short) n, d0);

    for (k = 0; k <= n; k++) {
      dk = SDS_Factorial((SIGLIB_TWO * n) - k) / (SDS_Pow(SIGLIB_TWO, (n - k)) * SDS_Factorial(k) * SDS_Factorial(n - k));
      //      printf ("d[%d] = %lf\n", (short) k, dk);
    }

    //    printf ("\n");
  }
#endif

  if (FilterSpecification->Order == 1) {
    for (i = 0; i < ((FilterSpecification->Order + 1) >> 1); i++) {
      FilterSpecification->pPoles[2 * i] = SPlaneBesselPoles1[i];
      FilterSpecification->pPoles[2 * i + 1] = SCV_Conjugate(SPlaneBesselPoles1[i]);
      FilterSpecification->pZeros[2 * i] = SCV_Rectangular(SIGLIB_ZERO, SIGLIB_MAX);
      FilterSpecification->pZeros[2 * i + 1] = SCV_Rectangular(SIGLIB_ZERO, SIGLIB_MAX);
    }
  }

  else if (FilterSpecification->Order == 2) {
    for (i = 0; i < ((FilterSpecification->Order + 1) >> 1); i++) {
      FilterSpecification->pPoles[2 * i] = SPlaneBesselPoles2[i];
      FilterSpecification->pPoles[2 * i + 1] = SCV_Conjugate(SPlaneBesselPoles2[i]);
      FilterSpecification->pZeros[2 * i] = SCV_Rectangular(SIGLIB_ZERO, SIGLIB_MAX);
      FilterSpecification->pZeros[2 * i + 1] = SCV_Rectangular(SIGLIB_ZERO, SIGLIB_MAX);
    }
  }

  else if (FilterSpecification->Order == 3) {
    for (i = 0; i < ((FilterSpecification->Order + 1) >> 1); i++) {
      FilterSpecification->pPoles[2 * i] = SPlaneBesselPoles3[i];
      FilterSpecification->pPoles[2 * i + 1] = SCV_Conjugate(SPlaneBesselPoles3[i]);
      FilterSpecification->pZeros[2 * i] = SCV_Rectangular(SIGLIB_ZERO, SIGLIB_MAX);
      FilterSpecification->pZeros[2 * i + 1] = SCV_Rectangular(SIGLIB_ZERO, SIGLIB_MAX);
    }
  }

  else if (FilterSpecification->Order == 4) {
    for (i = 0; i < ((FilterSpecification->Order + 1) >> 1); i++) {
      FilterSpecification->pPoles[2 * i] = SPlaneBesselPoles4[i];
      FilterSpecification->pPoles[2 * i + 1] = SCV_Conjugate(SPlaneBesselPoles4[i]);
      FilterSpecification->pZeros[2 * i] = SCV_Rectangular(SIGLIB_ZERO, SIGLIB_MAX);
      FilterSpecification->pZeros[2 * i + 1] = SCV_Rectangular(SIGLIB_ZERO, SIGLIB_MAX);
    }
  }

  else if (FilterSpecification->Order == 5) {
    for (i = 0; i < ((FilterSpecification->Order + 1) >> 1); i++) {
      FilterSpecification->pPoles[2 * i] = SPlaneBesselPoles5[i];
      FilterSpecification->pPoles[2 * i + 1] = SCV_Conjugate(SPlaneBesselPoles5[i]);
      FilterSpecification->pZeros[2 * i] = SCV_Rectangular(SIGLIB_ZERO, SIGLIB_MAX);
      FilterSpecification->pZeros[2 * i + 1] = SCV_Rectangular(SIGLIB_ZERO, SIGLIB_MAX);
    }
  }

  else if (FilterSpecification->Order == 6) {
    for (i = 0; i < ((FilterSpecification->Order + 1) >> 1); i++) {
      FilterSpecification->pPoles[2 * i] = SPlaneBesselPoles6[i];
      FilterSpecification->pPoles[2 * i + 1] = SCV_Conjugate(SPlaneBesselPoles6[i]);
      FilterSpecification->pZeros[2 * i] = SCV_Rectangular(SIGLIB_ZERO, SIGLIB_MAX);
      FilterSpecification->pZeros[2 * i + 1] = SCV_Rectangular(SIGLIB_ZERO, SIGLIB_MAX);
    }
  }

  else if (FilterSpecification->Order == 7) {
    for (i = 0; i < ((FilterSpecification->Order + 1) >> 1); i++) {
      FilterSpecification->pPoles[2 * i] = SPlaneBesselPoles7[i];
      FilterSpecification->pPoles[2 * i + 1] = SCV_Conjugate(SPlaneBesselPoles7[i]);
      FilterSpecification->pZeros[2 * i] = SCV_Rectangular(SIGLIB_ZERO, SIGLIB_MAX);
      FilterSpecification->pZeros[2 * i + 1] = SCV_Rectangular(SIGLIB_ZERO, SIGLIB_MAX);
    }
  }

  else if (FilterSpecification->Order == 8) {
    for (i = 0; i < ((FilterSpecification->Order + 1) >> 1); i++) {
      FilterSpecification->pPoles[2 * i] = SPlaneBesselPoles8[i];
      FilterSpecification->pPoles[2 * i + 1] = SCV_Conjugate(SPlaneBesselPoles8[i]);
      FilterSpecification->pZeros[2 * i] = SCV_Rectangular(SIGLIB_ZERO, SIGLIB_MAX);
      FilterSpecification->pZeros[2 * i + 1] = SCV_Rectangular(SIGLIB_ZERO, SIGLIB_MAX);
    }
  }

  else if (FilterSpecification->Order == 9) {
    for (i = 0; i < ((FilterSpecification->Order + 1) >> 1); i++) {
      FilterSpecification->pPoles[2 * i] = SPlaneBesselPoles9[i];
      FilterSpecification->pPoles[2 * i + 1] = SCV_Conjugate(SPlaneBesselPoles9[i]);
      FilterSpecification->pZeros[2 * i] = SCV_Rectangular(SIGLIB_ZERO, SIGLIB_MAX);
      FilterSpecification->pZeros[2 * i + 1] = SCV_Rectangular(SIGLIB_ZERO, SIGLIB_MAX);
    }
  }

  else if (FilterSpecification->Order == 10) {
    for (i = 0; i < ((FilterSpecification->Order + 1) >> 1); i++) {
      FilterSpecification->pPoles[2 * i] = SPlaneBesselPoles10[i];
      FilterSpecification->pPoles[2 * i + 1] = SCV_Conjugate(SPlaneBesselPoles10[i]);
      FilterSpecification->pZeros[2 * i] = SCV_Rectangular(SIGLIB_ZERO, SIGLIB_MAX);
      FilterSpecification->pZeros[2 * i + 1] = SCV_Rectangular(SIGLIB_ZERO, SIGLIB_MAX);
    }
  }

  //  wxMessageBox ("Got Here", "BesselFilter", wxOK | wxICON_EXCLAMATION,
  //  NULL);
}
