/*
Remez DLL functions based on the code by : John G. Zweizig
Source :
http://www.ldas-sw.ligo.caltech.edu/cgi-bin/cvsweb.cgi/gds/SignalProcessing/?cvsroot=GDS#dirlist
*/

#if defined(_MSC_VER)  /* Defined by Microsoft compilers */
#  include <windows.h> /* Required for Windows applications */
#endif
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#if __APPLE__
#  include <malloc/malloc.h>
#else
#  include <malloc.h>
#endif

// #ifdef DEBUG_LOG_FILE
// #undef DEBUG_LOG_FILE
// #define DEBUG_LOG_FILE 1 // Set to 1 to log progress to "debug.log" file
// #endif

#if defined(_MSC_VER)                // Defined by Microsoft compilers
#  pragma warning(disable : 4001)    // Remove // warning from plain C
#  pragma warning(disable : 4100)    // Remove unreferenced formal parameter warning
#  pragma warning(disable : 4101)    // Remove unreferenced local variable warning
#endif

/* Table of constant values */

#define BANDPASS 1
#define DIFFERENTIATOR 2
#define HILBERT 3

#define NEGATIVE 0
#define POSITIVE 1

#define GRID_DENSITY 16
#define MAXITERATIONS 40

static const double fsh = 1e-6;

#define PI (3.14159265358979323846264338327950288419716939937510) /* Pi */
#define TWO_PI (2.0 * PI)                                         /* 2.0 * Pi */

// -----------------------------------------------------------------------
//      FIR LINEAR PHASE FILTER DESIGN PROGRAM
//
// Author:  John G. Zweizig
//
//      Based on public domain software by J.H. McClellan,
//      T.W. Parks and L. R. Rabiner
//
// Inputs:
//  NFILT-- Filter length
//  JTYPE-- Type of filter
//      1 = Multiple passband/stopband
//      2 = Differentiator
//      3 = Hilbert transform
//  NBANDS-- Number of bands specified
//
//  EDGE(2*NBANDS)-- Band edge array: Lower and upper edges of each band.
//           Frequencies are specified in units of the Fsample and
//           should be constrained to the interval [0, 0.5]
//
//  FX(NBANDS)-- Filter gain for each band (or slope if filter is a
//         differentiator)
//
//  WTX(NBANDS)-- Weight for each band.  For a differentiator, the weight
//          function is proportional to 1/Frequency.
//
//  Outputs:
//  H(NFILT)--      Impulse response
//  DEVIAT(NBANDS)--  Deviations.
//  EXTRM(NFILT)--    Extremal frequencies.
//
//  Returns:
//     1   Invalid NFILT
//     2   Invalid JTYPE
//     3   Calculation didn't converge
//
// ----------------------------------------------------------------------- */

/*  External function definitions */
long remez(long nfilt, long jtype, long nbands, const double* edge, const double* fx, const double* wtx, double* h, double* deviat, double* extrm);

/*  Internal function definitions */
static double RemezMagnitudeResponse(double freq, double fx, double wtx, long jtype);
static double RemezWeightFunction(double freq, double fx, double wtx, long jtype);
static long RemezImplementation(double* ad, double* dev, double* grid, double* des, double* wt, double* alpha, long* iext, long nfcns, long ngrid,
                                long FilterLength);
static double RemezLagrangeInterpolation(long k, long n, long m, double* x);
static double RemezFreqResponse(long k, long n, double* ad, double* x, double* y, double* grid);

/*  Driving function */
long remez(long FilterLength, long jtype, long nbands, const double* edge, const double* fx, const double* wtx, double* h, double* deviat,
           double* extrm)
{
  /* System generated locals */
  long ret_val;

  /* Local variables */
  double delf;
  long nodd;
  long j, l, lband;
  long ngrid, nfcns;    //, nfmax;
  double change, wtt;
  long nz, nm1, neg;
  double dev, fup;

  /*  THE PROGRAM IS SET UP FOR A MAXIMUM LENGTH OF 128, BUT */
  /*  THIS UPPER LIMIT CAN BE CHANGED BY REDIMENSIONING THE */
  /*  ARRAYS IEXT, AD, ALPHA, X, Y, H TO BE NFMAX/2 + 2. */
  //    long iext[66];
  //    double ad[66], alpha[66];

  /*  THE ARRAYS DES, GRID, AND WT MUST DIMENSIONED */
  /*  GRID_DENSITY(NFMAX/2 + 2). */
  //    double  des[1045], wt[1045], grid[1045];

  long* iext = (long*)malloc(((FilterLength / 2) + 2 + 1) * sizeof(long));
  double* ad = (double*)malloc(((FilterLength / 2) + 2 + 1) * sizeof(double));
  double* alpha = (double*)malloc(((FilterLength / 2) + 2 + 1) * sizeof(double));

  double* des = (double*)malloc(((FilterLength / 2) + 2 + 1) * GRID_DENSITY * sizeof(double));
  double* wt = (double*)malloc(((FilterLength / 2) + 2 + 1) * GRID_DENSITY * sizeof(double));
  double* grid = (double*)malloc(((FilterLength / 2) + 2 + 1) * GRID_DENSITY * sizeof(double));

#if DEBUG_LOG_FILE
  SUF_Debugfprintf("Got here : remez\n");
#endif

  //    nfmax = 128;

  /* Parameter adjustments */
  --h;
  --fx;
  --edge;

  /* Function Body */
  ret_val = 0;

  /*--------------------------------------  Check arguments */

  //    if (FilterLength > nfmax || FilterLength <= 3)
  //    {
  //    free (iext);
  //    free (ad);
  //    free (alpha);
  //
  //    free (des);
  //    free (wt);
  //    free (grid);
  //
  //    return 1;
  //    }

  if (nbands <= 0)
    nbands = 1;

  /*  GRID DENSITY IS ASSUMED TO BE 16 UNLESS SPECIFIED */
  /*  OTHERWISE */
  //    if (lgrid <= 0) lgrid = GRID_DENSITY;

  if (jtype <= 0 || jtype > 3) {
    free(iext);
    free(ad);
    free(alpha);

    free(des);
    free(wt);
    free(grid);

    return 2;
  }

  if (jtype == 1)
    neg = 0;
  else
    neg = 1;

  nodd = FilterLength % 2;

  nfcns = FilterLength / 2;
  if (nodd == 1 && neg == 0)
    ++nfcns;

  /*  SET UP THE DENSE GRID.  THE NUMBER OF POINTS IN THE GRID */
  /*  IS (FILTER LENGTH + 1)*GRID DENSITY/2 */
  /*  CALCULATE THE DESIRED MAGNITUDE RESPONSE AND THE WEIGHT */
  /*  FUNCTION ON THE GRID */
  delf = 0.5 / (double)(GRID_DENSITY * nfcns);
  j = 0;
  for (lband = 1; lband <= nbands; lband++) {
    if (wtx)
      wtt = wtx[lband - 1];
    else
      wtt = 1.0;
    l = lband * 2 - 1;
    grid[j] = edge[l];
    if ((j == 0) && (neg != 0) && (edge[1] < delf))
      grid[0] = delf;
    fup = edge[l + 1];
    while (grid[j] <= fup) {
      des[j] = RemezMagnitudeResponse(grid[j], fx[lband], wtt, jtype);
      wt[j] = RemezWeightFunction(grid[j], fx[lband], wtt, jtype);
      grid[j + 1] = grid[j] + delf;
      j++;
    }
    grid[j - 1] = fup;
    des[j - 1] = RemezMagnitudeResponse(fup, fx[lband], wtt, jtype);
    wt[j - 1] = RemezWeightFunction(fup, fx[lband], wtt, jtype);
  }
  ngrid = j;
  if (neg == nodd) {
    if (grid[ngrid - 1] > 0.5 - delf) {
      --ngrid;
    }
  }

  /*  SET UP A NEW APPROXIMATION PROBLEM WHICH IS EQUIVALENT */
  /*  TO THE ORIGINAL PROBLEM */
  if (neg == 0) {
    if (nodd != 1) {
      for (j = 0; j < ngrid; ++j) {
        change = cos(PI * grid[j]);
        des[j] /= change;
        wt[j] *= change;
      }
    }
  } else {
    if (nodd == 1) {
      for (j = 0; j < ngrid; ++j) {
        change = sin(TWO_PI * grid[j]);
        des[j] /= change;
        wt[j] *= change;
      }
    } else {
      for (j = 0; j < ngrid; ++j) {
        change = sin(PI * grid[j]);
        des[j] /= change;
        wt[j] *= change;
      }
    }
  }

  /*  INITIAL GUESS FOR THE EXTREMAL FREQUENCIES--EQUALLY */
  /*  SPACED ALONG THE GRID */
  for (j = 0; j < nfcns; ++j)
    iext[j] = (j * ngrid - j) / nfcns + 1;
  iext[nfcns] = ngrid;
  nm1 = nfcns - 1;
  nz = nfcns + 1;

  /*  CALL THE REMEZ EXCHANGE ALGORITHM TO DO THE APPROXIMATION */
  /*  PROBLEM */
  ret_val = RemezImplementation(ad, &dev, grid, des, wt, alpha, iext, nfcns, ngrid, FilterLength);

  // RemezImplementation can return code of 3 !!!
#if DEBUG_LOG_FILE
  SUF_Debugfprintf("RemezImplementation returned : %d\n", ret_val);
  {
    long c;
    for (c = 0; c < ((FilterLength / 2) + 2); c++) {
      SUF_Debugfprintf("RemezImplementation returned Alpha [%d]: %lf\n", c, alpha[c]);
    }
    //    double *alpha = (double *) malloc (((FilterLength / 2) + 2) * sizeof
    //    (double));
  }
#endif

  /*  CALCULATE THE IMPULSE RESPONSE. */

  if (neg == 0) {
    if (nodd == 0) {
      h[1] = alpha[nfcns - 1] * 0.25;
      for (j = 2; j <= nm1; ++j) {
        h[j] = (alpha[nz - j - 1] + alpha[nfcns + 1 - j]) * 0.25;
      }
      h[nfcns] = alpha[0] * 0.5 + alpha[1] * 0.25;
    } else {
      for (j = 1; j <= nm1; ++j) {
        h[j] = alpha[nz - j - 1] * 0.5;
      }
      h[nfcns] = alpha[0];
    }
  } else {
    if (nodd == 0) {
      h[1] = alpha[nfcns - 1] * 0.25;
      for (j = 2; j <= nm1; ++j) {
        h[j] = (alpha[nz - j - 1] - alpha[nfcns + 1 - j]) * 0.25;
      }
      h[nfcns] = alpha[0] * 0.5 - alpha[1] * 0.25;
    } else {
      h[1] = alpha[nfcns - 1] * 0.25;
      h[2] = alpha[nm1 - 1] * 0.25;
      for (j = 3; j <= nm1; ++j) {
        h[j] = (alpha[nz - j - 1] - alpha[nfcns + 2 - j]) * 0.25;
      }
      h[nfcns] = alpha[0] * 0.5 - alpha[2] * 0.25;
      h[nz] = 0.0;
    }
  }

#if DEBUG_LOG_FILE
  SUF_Debugfprintf("remez : Impulse response calculated\n");
  SUF_Debugfprintf("remez : nfcns = %d, FilterLength = %d, jtype = %d\n", nfcns, FilterLength, jtype);
  {
    long c;
    SUF_Debugfprintf("h[F] = \n");
    for (c = 1; c <= FilterLength; c++) {
      if ((c == 10) || (c == 20) || (c == 30) || (c == 40) || (c == 50) || (c == 60) || (c == 70))
        SUF_Debugfprintf("\n");
      SUF_Debugfprintf("%lf, ", h[c]);
    }
    SUF_Debugfprintf("\n");
  }
#endif

  for (j = nfcns; j < FilterLength; j++) {
    h[j + 1] = h[FilterLength - j];
    if (jtype == 2)    // Differentiator
    {
      h[j + 1] = -h[j + 1];
    } else if (jtype == 3)    // Hilbert transform
    {
      h[FilterLength - j] = -h[FilterLength - j];
    }
  }

#if DEBUG_LOG_FILE
  {
    long c;
    SUF_Debugfprintf("h[F] = \n");
    for (c = 1; c <= FilterLength; c++) {
      if ((c == 10) || (c == 20) || (c == 30) || (c == 40) || (c == 50) || (c == 60) || (c == 70))
        SUF_Debugfprintf("\n");
      SUF_Debugfprintf("%lf, ", h[c]);
    }
    SUF_Debugfprintf("\n");
  }
#endif

  free(iext);
  free(ad);
  free(alpha);

  free(des);
  free(wt);
  free(grid);

#if DEBUG_LOG_FILE
  SUF_Debugfprintf("Got here : remez - End\n");
#endif

  return ret_val;

} /* End of remez () */

/* ----------------------------------------------------------------------- */
/* FUNCTION: RemezMagnitudeResponse */
/*   FUNCTION TO CALCULATE THE DESIRED MAGNITUDE RESPONSE */
/*   AS A FUNCTION OF FREQUENCY. */
/*   AN ARBITRARY FUNCTION OF FREQUENCY CAN BE */
/*   APPROXIMATED IF THE USER REPLACES THIS FUNCTION */
/*   WITH THE APPROPRIATE CODE TO EVALUATE THE IDEAL */
/*   MAGNITUDE.  NOTE THAT THE PARAMETER FREQ IS THE */
/*   VALUE OF NORMALIZED FREQUENCY NEEDED FOR EVALUATION. */
/* ----------------------------------------------------------------------- */

static double RemezMagnitudeResponse(double freq, double fx, double wtx, long jtype)
{
#if DEBUG_LOG_FILE1
  SUF_Debugfprintf("Got here : RemezMagnitudeResponse\n");
#endif
  if (jtype != 2)
    return fx;
  else
    return fx * freq;
} /* End of RemezMagnitudeResponse () */

/* ----------------------------------------------------------------------- */
/* FUNCTION: RemezWeightFunction */
/*   FUNCTION TO CALCULATE THE WEIGHT FUNCTION AS A FUNCTION */
/*   OF FREQUENCY.  SIMILAR TO THE FUNCTION EFF, THIS FUNCTION CAN */
/*   BE REPLACED BY A USER-WRITTEN ROUTINE TO CALCULATE ANY */
/*   DESIRED WEIGHTING FUNCTION. */
/* ----------------------------------------------------------------------- */

static double RemezWeightFunction(double freq, double fx, double wtx, long jtype)
{
#if DEBUG_LOG_FILE1
  SUF_Debugfprintf("Got here : RemezWeightFunction\n");
#endif
  if (jtype != 2)
    return wtx;
  else if (fx >= 1e-4)
    return wtx / freq;
  else
    return wtx;
} /* End of RemezWeightFunction () */

/* ----------------------------------------------------------------------- */
/* SUBROUTINE: RemezImplementation */
/*   THIS SUBROUTINE IMPLEMENTS THE REMEZ EXCHANGE ALGORITH */
/*   FOR THE WEIGHTED CHEBYSHEV APPROXIMATION OF A CONTINUOUS */
/*   FUNCTION WITH A SUM OF COSINES.  INPUTS TO THE SUBROUTINE */
/*   ARE A DENSE GRID WHICH REPLACES THE FREQUENCY AXIS, THE */
/*   DESIRED FUNCTION ON THIS GRID, THE WEIGHT FUNCTION ON THE */
/*   GRID, THE NUMBER OF COSINES, AND AN INITIAL GUESS OF THE */
/*   EXTREMAL FREQUENCIES.  THE PROGRAM MINIMIZES THE CHEBYSHEV */
/*   ERROR BY DETERMINING THE BEST LOCATION OF THE EXTREMAL */
/*   FREQUENCIES (POINTS OF MAXIMUM ERROR) AND THEN CALCULATES */
/*   THE COEFFICIENTS OF THE BEST APPROXIMATION. */
/* ----------------------------------------------------------------------- */

static long RemezImplementation(double* ad, double* dev, double* grid, double* des, double* wt, double* alpha, long* iext, long nfcns, long ngrid,
                                long FilterLength)
{
  /* System generated locals */
  long ret_val;

  /* Local variables */
  double dden, dnum, delf, devl, comp;
  long luck, klow;
  long j, k, l;
  double gtemp;
  long niter, k1;
  double y1, aa, bb;
  double cn;
  long kn;
  double ft;
  long jchnge, nu, nz;
  double xt;
  long itrmax, nm1;
  double xt1;
  long kkk, jet;
  double err;
  long kup, knz;
  double ynz, ksign, nut, nut1;
  long nzz;
  double dtemp;

  //    double a[66], x[66], y[66];
  //    double p[65], q[65];

  double* a = (double*)malloc(((FilterLength / 2) + 2 + 1) * sizeof(double));
  double* x = (double*)malloc(((FilterLength / 2) + 2 + 1) * sizeof(double));
  double* y = (double*)malloc(((FilterLength / 2) + 2 + 1) * sizeof(double));
  double* p = (double*)malloc(((FilterLength / 2) + 1 + 1) * sizeof(double));
  double* q = (double*)malloc(((FilterLength / 2) + 1 + 1) * sizeof(double));

#if DEBUG_LOG_FILE1
  SUF_Debugfprintf("Got here : RemezImplementation\n");
#endif

  /* Parameter adjustments */
  --iext;
  --alpha;
  --wt;
  --des;
  --grid;
  --ad;

  /* Function Body */

  /*  THE PROGRAM ALLOWS A MAXIMUM NUMBER OF ITERATIONS OF 25 */

  ret_val = 0;
  itrmax = 25;
  devl = -1.0;
  nz = nfcns + 1;
  nzz = nfcns + 2;
  niter = 0;
  luck = 0;
  comp = 0.0;
  y1 = 0.0;
  aa = 0.0;
  bb = 0.0;
  ynz = 0.0;
  nut1 = 1.0;

L100:
  iext[nzz] = ngrid + 1;
  ++niter;
  if (niter > itrmax) {
    goto L400;
  }
  for (j = 1; j <= nz; ++j) {
    x[j - 1] = cos(grid[iext[j]] * TWO_PI);
  }
  jet = (nfcns - 1) / 15 + 1;
  for (j = 1; j <= nz; ++j) {
    ad[j] = RemezLagrangeInterpolation(j, nz, jet, x);
  }
  dnum = 0.;
  dden = 0.;
  ksign = 1.0;
  for (j = 1; j <= nz; ++j) {
    l = iext[j];
    dnum += ad[j] * des[l];
    dden += ksign * ad[j] / wt[l];
    ksign = -ksign;
  }
  *dev = dnum / dden;
  if (*dev > 0.) {
    nu = -1;
  } else {
    nu = 1;
    *dev = -*dev;
  }
  ksign = nu;
  for (j = 1; j <= nz; ++j) {
    l = iext[j];
    y[j - 1] = des[l] + (ksign * *dev / wt[l]);
    ksign = -ksign;
  }
  if (*dev <= devl) {
    ret_val = 3;
    goto L400;
  }
  devl = *dev;
  jchnge = 0;
  k1 = iext[1];
  knz = iext[nz];
  klow = 0;
  nut = -nu;
  j = 1;

  /*  SEARCH FOR THE EXTREMAL FREQUENCIES OF THE BEST */
  /*  APPROXIMATION */

L200:
  if (j == nzz)
    ynz = comp;
  if (j >= nzz)
    goto L300;
  kup = iext[j + 1];
  l = iext[j] + 1;
  nut = -nut;
  if (j == 2)
    y1 = comp;
  comp = *dev;
  if (l >= kup)
    goto L220;
  err = RemezFreqResponse(l, nz, &ad[1], x, y, &grid[1]);
  err = (err - des[l]) * wt[l];
  if (nut * err <= comp)
    goto L220;
  comp = nut * err;
L210:
  for (++l; l < kup; ++l) {
    err = RemezFreqResponse(l, nz, &ad[1], x, y, &grid[1]);
    err = (err - des[l]) * wt[l];
    if (nut * err <= comp)
      break;
    comp = nut * err;
  }
L215:
  iext[j] = l - 1;
  ++j;
  klow = l - 1;
  ++jchnge;
  goto L200;
L220:
  --l;
L225:
  --l;
  if (l <= klow) {
    goto L250;
  }
  err = RemezFreqResponse(l, nz, &ad[1], x, y, &grid[1]);
  err = (err - des[l]) * wt[l];
  if (nut * err > comp) {
    goto L230;
  }
  if (jchnge == 0) {
    goto L225;
  }
  klow = iext[j];
  ++j;
  goto L200;
L230:
  comp = nut * err;
L235:
  for (--l; l > klow; --l) {
    err = (RemezFreqResponse(l, nz, &ad[1], x, y, &grid[1]) - des[l]) * wt[l];
    if (nut * err <= comp)
      break;
    comp = nut * err;
  }

  klow = iext[j];
  iext[j] = l + 1;
  ++j;
  ++jchnge;
  goto L200;
L250:
  l = iext[j] + 1;
  if (jchnge > 0) {
    goto L215;
  }
L255:
  ++l;
  if (l >= kup) {
    goto L260;
  }
  err = RemezFreqResponse(l, nz, &ad[1], x, y, &grid[1]);
  err = (err - des[l]) * wt[l];
  if (nut * err <= comp) {
    goto L255;
  }
  comp = nut * err;
  goto L210;
L260:
  klow = iext[j];
  ++j;
  goto L200;
L300:
  if (j > nzz) {
    goto L320;
  }
  if (k1 > iext[1]) {
    k1 = iext[1];
  }
  if (knz < iext[nz]) {
    knz = iext[nz];
  }
  nut1 = nut;
  nut = -nu;
  l = 0;
  kup = k1;
  comp = ynz * (double)1.00001;
  luck = 1;
L310:
  ++l;
  if (l >= kup) {
    goto L315;
  }
  err = RemezFreqResponse(l, nz, &ad[1], x, y, &grid[1]);
  err = (err - des[l]) * wt[l];
  if (nut * err <= comp) {
    goto L310;
  }
  comp = nut * err;
  j = nzz;
  goto L210;
L315:
  luck = 6;
  goto L325;
L320:
  if (luck > 9)
    goto L350;
  if (comp > y1)
    y1 = comp;
  k1 = iext[nzz];
L325:
  l = ngrid + 1;
  klow = knz;
  nut = -nut1;
  comp = y1 * 1.00001;
L330:
  --l;
  if (l <= klow) {
    goto L340;
  }
  err = RemezFreqResponse(l, nz, &ad[1], x, y, &grid[1]);
  err = (err - des[l]) * wt[l];
  if (nut * err <= comp) {
    goto L330;
  }
  j = nzz;
  comp = nut * err;
  luck += 10;
  goto L235;
L340:
  if (luck == 6) {
    goto L370;
  }
  for (j = 1; j <= nfcns; ++j) {
    iext[nzz - j] = iext[nz - j];
  }
  iext[1] = k1;
  goto L100;
L350:
  kn = iext[nzz];
  for (j = 1; j <= nfcns; ++j) {
    iext[j] = iext[j + 1];
  }
  iext[nz] = kn;
  goto L100;
L370:
  if (jchnge > 0) {
    goto L100;
  }

  /*  CALCULATION OF THE COEFFICIENTS OF THE BEST APPROXIMATION */
  /*  USING THE INVERSE DISCRETE FOURIER TRANSFORM */

L400:
  gtemp = grid[1];
  x[nzz - 1] = -2.0;
  cn = (double)(2 * nfcns - 1);
  delf = 1.0 / cn;
  l = 1;
  if ((grid[1] < 0.01 && grid[ngrid] > 0.49) || nfcns <= 3) {
    kkk = 1;
  } else {
    kkk = 0;
    dtemp = cos(TWO_PI * grid[1]);
    dnum = cos(TWO_PI * grid[ngrid]);
    aa = 2.0 / (dtemp - dnum);
    bb = -(dtemp + dnum) / (dtemp - dnum);
  }
  for (j = 0; j < nfcns; ++j) {
    ft = delf * (double)(j);
    xt = cos(TWO_PI * ft);
    if (kkk != 1) {
      xt = (xt - bb) / aa;
      xt1 = sqrt(1.0 - xt * xt);
      ft = atan2(xt1, xt) / TWO_PI;
    }
    while ((xt <= x[l - 1]) && (x[l - 1] - xt >= fsh))
      l++;
    if (xt - x[l - 1] < fsh) {
      a[j] = y[l - 1];
    } else {
      grid[1] = ft;
      a[j] = RemezFreqResponse(1, nz, &ad[1], x, y, &grid[1]);
    }
    if (l > 1)
      --l;
  }
  grid[1] = gtemp;
  dden = TWO_PI / cn;
  for (j = 1; j <= nfcns; ++j) {
    dnum = dden * (double)(j - 1);
    dtemp = 0.0;
    for (k = 1; k < nfcns; ++k) {
      dtemp += a[k] * cos(dnum * (double)k);
    }
    dtemp = dtemp * 2.0 + a[0];
    alpha[j] = dtemp;
  }
  for (j = 2; j <= nfcns; ++j) {
    alpha[j] *= 2.0 / cn;
  }
  alpha[1] /= cn;
  if (kkk == 0) {
    nm1 = nfcns - 1;
    p[0] = alpha[nfcns] * 2.0 * bb + alpha[nm1];
    p[1] = aa * 2.0 * alpha[nfcns];
    q[0] = alpha[nfcns - 2] - alpha[nfcns];
    for (j = 2; j <= nm1; ++j) {
      if (j == nm1) {
        aa *= 0.5;
        bb *= 0.5;
      }
      p[j] = 0.0;
      for (k = 0; k < j; ++k) {
        a[k] = p[k];
        p[k] *= 2.0 * bb;
      }
      p[1] += a[0] * 2.0 * aa;
      for (k = 1; k < j; ++k) {
        p[k - 1] += q[k - 1] + aa * a[k];
      }
      for (k = 2; k <= j; ++k) {
        p[k] += aa * a[k - 1];
      }
      if (j != nm1) {
        for (k = 0; k < j; ++k) {
          q[k] = -a[k];
        }
        q[0] += alpha[nm1 - j];
      }
    }
    for (j = 1; j <= nfcns; ++j) {
      alpha[j] = p[j - 1];
    }
  }
  if (nfcns <= 3) {
    alpha[nfcns + 1] = 0.;
    alpha[nfcns + 2] = 0.;
  }

  free(a);
  free(x);
  free(y);
  free(p);
  free(q);

#if DEBUG_LOG_FILE1
  SUF_Debugfprintf("Got here : RemezImplementation - End\n");
#endif

  return ret_val;
} /* End of RemezImplementation () */

/* ----------------------------------------------------------------------- */
/* FUNCTION: RemezLagrangeInterpolation */
/*   FUNCTION TO CALCULATE THE LAGRANGE INTERPOLATION */
/*   COEFFICIENTS FOR USE IN THE FUNCTION GEE. */
/* ----------------------------------------------------------------------- */

static double RemezLagrangeInterpolation(long k, long n, long m, double* x)
{

  /* Local variables */
  double cf;
  long j, l;
  double q;

#if DEBUG_LOG_FILE1
  SUF_Debugfprintf("Got here : RemezLagrangeInterpolation\n");
#endif

  /* Parameter adjustments */
  --x;

  /* Function Body */
  cf = 1.0;
  q = x[k];
  for (l = 1; l <= m; ++l) {
    for (j = l; j <= n; j += m) {
      if (j != k)
        cf *= 2.0 * (q - x[j]);
    }
  }

#if DEBUG_LOG_FILE1
  SUF_Debugfprintf("Got here : RemezLagrangeInterpolation - End\n");
#endif

  return 1.0 / cf;

} /* End of RemezLagrangeInterpolation () */

/* ----------------------------------------------------------------------- */
/* FUNCTION: RemezFreqResponse */
/*   FUNCTION TO EVALUATE THE FREQUENCY RESPONSE USING THE */
/*   LAGRANGE INTERPOLATION FORMULA IN THE BARYCENTRIC FORM */
/* ----------------------------------------------------------------------- */

static double RemezFreqResponse(long k, long n, double* ad, double* x, double* y, double* grid)
{

  /* Local variables */
  double c, d;
  long j;
  double p, xf;

#if DEBUG_LOG_FILE1
  SUF_Debugfprintf("Got here : RemezFreqResponse\n");
#endif

  /* Parameter adjustments */
  --grid;

  /* Function Body */
  p = 0.0;
  xf = cos(TWO_PI * grid[k]);
  d = 0.0;
  for (j = 0; j < n; ++j) {
    c = ad[j] / (xf - x[j]);
    d += c;
    p += c * y[j];
  }

#if DEBUG_LOG_FILE1
  SUF_Debugfprintf("Got here : RemezFreqResponse - End\n");
#endif

  return p / d;
} /* End of RemezFreqResponse () */
