/*
  Generates s-plane specification for design of Butterworth,
  Chebyschev, Inverse Chebyschev & Elliptic IIR digital filters

  Modified by John Edwards to SigLib complex inverse and square root functions

  Translated from a f77 program by C. S. Burrus - author unknown
  T.W. Parkes & C.S. Burrus, "Digital Filter Design", Wiley 1987.
*/

#if defined(_MSC_VER)  /* Defined by Microsoft compilers */
#  include <windows.h> /* Required for Windows applications */
#endif
#include <math.h>
#include <stdio.h>
#if __APPLE__
#  include <malloc/malloc.h>
#else
#  include <malloc.h>
#endif

#include <siglib.h>    // SigLib DSP library

// #ifdef DEBUG_LOG_FILE
// #undef DEBUG_LOG_FILE
// #define DEBUG_LOG_FILE 1 // Set to 1 to log progress to "debug.log" file
// #endif

typedef struct /* Filter specification structure */
{
  long Order;              /* Filter order */
  long BandSpec;           /* Filter type (lp, hp, bp, bs) */
  long DesignAlgo;         /* Approximation (bw, ch, ic, el) */
  double Ripple_dB;        /* Ripple in dB (+ve number) */
  double Attenuation_dB;   /* Attenuation in dB (+ve number) */
  double OhmegaZero;       /* W0 */
  double LowerBandEdge;    /* Lower frequency band edge */
  double UpperBandEdge;    /* Upper frequency band edge */
  SLComplexRect_s* pPoles; /* Dynamic allocated array of poles */
  SLComplexRect_s* pZeros; /* Dynamic allocated array of zeros */
} filter_t;

enum    // IIR filter band type
{
  IIR_LPF,
  IIR_HPF,
  IIR_BPF,
  IIR_BSF
};

enum    // IIR bilinear transform filter design type
{
  IIR_BUTTERWORTH,
  IIR_CHEBYSCHEV,
  IIR_INVERSE_CHEBYSCHEV,
  IIR_ELLIPTIC,
  IIR_BESSEL
};

/* constants */
#define MX_STR 1024
#define E_MXITER 50    /* mx iters for elliptic approx fns */
#define E_DELTA 1e-15  /* convergence crit for elliptic fns */
#define HUGEFLOAT 1e16 /* (HUGE) */
#define BIGFLOAT 1e15  /* smaller than HUGEFLOAT */

#define PI (3.14159265358979323846264338327950288419716939937510) /* Pi */
#define TWO_PI (2.0 * PI)                                         /* 2.0 * Pi */
#define HALF_PI (PI / 2.0)                                        /* Pi / 2.0 */

/* useful macros */
#define odd(n) ((n) & 1) /* returns 1 if n odd, 0 otherwise */
#define asinh(x) (log((x) + sqrt((x) * (x) + 1.0)))
#define dB2abs(a) (sqrt(pow(10.0, 0.1 * (a)) - 1.0))

/*  External function definitions */
void elliptic_filter_roots(filter_t* FilterSpecification);
void non_elliptic_filter_roots(filter_t* FilterSpecification);
void high_pass_transform(SLComplexRect_s* x, long n);
void bandpass_bandstop_transform(SLComplexRect_s* x, long n, long ftype, double w0);

/*************** FILTER DESIGN ROUTINES ***************/
/* complete elliptic integral */
double complete_elliptic_integral(double kc)

{
  double a, b, at;
  long i;

  a = 1.0;
  b = kc;
  for (i = E_MXITER; i--;) {
    at = (a + b) / 2.0;
    b = sqrt(a * b);
    a = at;
    if (((a - b) / a) < E_DELTA)
      return (HALF_PI / a);
  }

  printf("complete_elliptic_integral(%g) failed to converge\n", kc);

  return (HALF_PI / a);
}

/* elliptic functions */
void elliptic_function(double x, double kc, double* sn, double* cn, double* dn)

{
  double aa[E_MXITER], bb[E_MXITER];
  double a, b, c, d, e;
  double at;
  long i;

  if (x == 0.0) {
    *sn = 0.0;
    *cn = *dn = 1.0;
    return;
  }
  a = 1.0;
  b = kc;
  for (i = 0; i < E_MXITER; i++) {
    aa[i] = a;
    bb[i] = b;
    at = (a + b) / 2;
    b = sqrt(a * b);
    a = at;
    if (((a - b) / a) < E_DELTA) {
      /* converged */
      c = a / tan(x * a);
      d = 1.0;
      do {
        e = c * c / a;
        c = c * d;
        a = aa[i];
        d = (e + bb[i]) / (e + a);
      } while (i-- > 0);

      *sn = 1.0 / sqrt(1.0 + c * c);
      *cn = *sn * c;
      *dn = d;
      return;
    }
  }

  printf("elliptic_function(%g,  %g) failed to converge\n", x, kc);
}

/* modulus from ratio of k/k' */
double modulus_function(double u)

{
  double a, b, c, d;
  double q;
  long i;

  d = q = exp(-PI * u);
  a = b = c = 1.0;
  for (i = E_MXITER; i--;) {
    a += 2.0 * c * d;
    c = c * d * d;
    b += c;
    d *= q;
    if (c < E_DELTA)
      return (4.0 * sqrt(q) * pow(b / a, 2.0));
  }

  printf("modulus_function(%g) failed to converge\n", u);

  return (4.0 * sqrt(q) * pow(b / a, 2.0));
}

/* arc elliptic tangent */
double arc_elliptic_tan(double u, double kc)

{
  double y;
  double a, b;
  double bt;
  long k, j;

  a = 1.0;
  b = kc;
  y = 1.0 / u;
  for (k = 0, j = E_MXITER; j--;) {
    bt = a * b;
    a += b;
    b = 2.0 * sqrt(bt);
    y -= bt / y;
    if (y == 0.0)
      y = sqrt(bt) * 1e-10;
    if (fabs(a - b) < (a * E_DELTA)) {
      if (y < 0.0)
        k++;
      return (atan(a / y) + PI * ((double)k)) / a;
    }
    k *= 2;
    if (y < 0.0)
      k++;
  }

  printf("arc_elliptic_tan(%g, %g) failed to converge", u, kc);

  return (atan(a / y) + PI * ((double)k)) / a;
}

/* calculate poles and zeros for elliptic filters */
void elliptic_filter_roots(filter_t* FilterSpecification)

{
  double e;
  double k, kc, k1, k1c;
  double kk, kkc, kk1, kk1c;
  double arg;
  double xn;
  double v0;
  double sm, cm, dm;
  double sn, cn, dn;
  long i;
  long j;
  long ff;

#if DEBUG_LOG_FILE
  SUF_Debugfprintf("elliptic_filter_roots ()\n");

  SUF_Debugfprintf("FilterSpecification->Order = %d\n", FilterSpecification->Order);
  SUF_Debugfprintf("FilterSpecification->BandSpec = %d\n", FilterSpecification->BandSpec);
  SUF_Debugfprintf("FilterSpecification->DesignAlgo = %d\n", FilterSpecification->DesignAlgo);
  SUF_Debugfprintf("FilterSpecification->Ripple_dB = %lf\n", FilterSpecification->Ripple_dB);
  SUF_Debugfprintf("FilterSpecification->Attenuation_dB = %lf\n", FilterSpecification->Attenuation_dB);
  SUF_Debugfprintf("FilterSpecification->OhmegaZero = %lf\n", FilterSpecification->OhmegaZero);
  SUF_Debugfprintf("FilterSpecification->LowerBandEdge = %lf\n", FilterSpecification->LowerBandEdge);
  SUF_Debugfprintf("FilterSpecification->UpperBandEdge = %lf\n", FilterSpecification->UpperBandEdge);
#endif

  e = dB2abs(FilterSpecification->Ripple_dB);
  k = FilterSpecification->LowerBandEdge / FilterSpecification->UpperBandEdge;
  kc = sqrt(1.0 - k * k);
  k1 = e / dB2abs(FilterSpecification->Attenuation_dB);
  k1c = sqrt(1.0 - k1 * k1);
  kk = complete_elliptic_integral(kc);
  kkc = complete_elliptic_integral(k);
  kk1 = complete_elliptic_integral(k1c);
  kk1c = complete_elliptic_integral(k1);
  xn = kk * kk1c / kk1 / kkc;

  FilterSpecification->Order = (xn + 1.0);

  k1 = modulus_function(FilterSpecification->Order * kkc / kk);
  k1c = sqrt(1.0 - k1 * k1);
  kk1 = complete_elliptic_integral(k1c);

  v0 = (kk / kk1 / FilterSpecification->Order) * arc_elliptic_tan(1.0 / e, k1);

  elliptic_function(v0, k, &sm, &cm, &dm);

  j = odd(FilterSpecification->Order) ? 0 : 1;

  for (ff = 1, i = 0; i < FilterSpecification->Order; i++, ff = -ff) {
    arg = kk * j / FilterSpecification->Order;
    elliptic_function(arg, kc, &sn, &cn, &dn);
    FilterSpecification->pZeros[i].real = 0.0;
    FilterSpecification->pZeros[i].imag = j ? (ff * FilterSpecification->UpperBandEdge / sn) : HUGEFLOAT;
    FilterSpecification->pPoles[i].real = -FilterSpecification->LowerBandEdge * sm * cm * cn * dn / (1.0 - (dn * sm * dn * sm));
    FilterSpecification->pPoles[i].imag = ff * FilterSpecification->LowerBandEdge * dm * sn / (1.0 - (dn * sm * dn * sm));

#if DEBUG_LOG_FILE
    SUF_Debugfprintf("EFR : P = %lf, %lf; Z = %lf, %lf\n", FilterSpecification->pPoles[i].real, FilterSpecification->pPoles[i].imag,
                     FilterSpecification->pZeros[i].real, FilterSpecification->pZeros[i].imag);
#endif

    /*
    ** after we have generated the conjugate pairs
    ** we incr l -- this is at different times for
    ** even and odd values of n
    */
    if ((!odd(i) && odd(FilterSpecification->Order)) || (odd(i) && !odd(FilterSpecification->Order)))
      j += 2;
  }
}

/* calculate poles and zeros for BWORTH, CHEBY and ICHEBY filters */

void non_elliptic_filter_roots(filter_t* FilterSpecification)

{
  double err;
  double sm, cm;
  double tr, ti;
  double x;
  long i;
  long j;
  long ff;

#if DEBUG_LOG_FILE
  SUF_Debugfprintf("elliptic_filter_roots ()\n");
  SUF_Debugfprintf("FilterSpecification->Order = %d\n", FilterSpecification->Order);
  SUF_Debugfprintf("FilterSpecification->BandSpec = %d\n", FilterSpecification->BandSpec);
  SUF_Debugfprintf("FilterSpecification->DesignAlgo = %d\n", FilterSpecification->DesignAlgo);
  SUF_Debugfprintf("FilterSpecification->Ripple_dB = %lf\n", FilterSpecification->Ripple_dB);
  SUF_Debugfprintf("FilterSpecification->Attenuation_dB = %lf\n", FilterSpecification->Attenuation_dB);
  SUF_Debugfprintf("FilterSpecification->OhmegaZero = %lf\n", FilterSpecification->OhmegaZero);
  SUF_Debugfprintf("FilterSpecification->LowerBandEdge = %lf\n", FilterSpecification->LowerBandEdge);
  SUF_Debugfprintf("FilterSpecification->UpperBandEdge = %lf\n", FilterSpecification->UpperBandEdge);
  SUF_Debugfprintf("FilterSpecification->pPoles = %d\n", FilterSpecification->pPoles);
  SUF_Debugfprintf("FilterSpecification->pZeros = %d\n", FilterSpecification->pZeros);
#endif

  if (FilterSpecification->DesignAlgo == IIR_BUTTERWORTH) {
    sm = 1.0;
    cm = 1.0;
  } else {
    err = dB2abs(FilterSpecification->Ripple_dB); /* Change from dB to absolute */
    if (FilterSpecification->DesignAlgo == IIR_INVERSE_CHEBYSCHEV)
      err = 1.0 / err;
    x = asinh(1.0 / err) / FilterSpecification->Order;
    sm = sinh(x);
    cm = cosh(x);
  }

  /* j = 1 if filter order is even, 0 if odd */
  j = odd(FilterSpecification->Order) ? 0 : 1;

  for (i = 0, ff = 1; i < FilterSpecification->Order; i++, ff = -ff) {
    /* Filter specification from Digital Filters P145 */
    tr = -sm * cos(HALF_PI * ((double)j) / ((double)FilterSpecification->Order));
    ti = cm * sin(HALF_PI * ((double)j) / ((double)FilterSpecification->Order));

    FilterSpecification->pZeros[i].real = 0.0;
    FilterSpecification->pZeros[i].imag = HUGEFLOAT;

    if (FilterSpecification->DesignAlgo == IIR_INVERSE_CHEBYSCHEV) {
      if (j != 0)
        FilterSpecification->pZeros[i].imag = FilterSpecification->UpperBandEdge / sin(HALF_PI * ((double)j) / FilterSpecification->Order) * ff;

      x = (tr * tr + ti * ti);
      FilterSpecification->pPoles[i].real = FilterSpecification->UpperBandEdge * tr / x;
      FilterSpecification->pPoles[i].imag = FilterSpecification->UpperBandEdge * ti / x * ff;
    } else {
      FilterSpecification->pPoles[i].real = FilterSpecification->LowerBandEdge * tr;
      FilterSpecification->pPoles[i].imag = FilterSpecification->LowerBandEdge * ti * ff;
    }

    /*
    ** after we have generated the conjugate pairs
    ** we incr l -- this is at different times for
    ** even and odd values of n
    */
    if ((!odd(i) && odd(FilterSpecification->Order)) || (odd(i) && !odd(FilterSpecification->Order))) {
      j += 2;
    }
  }
}

/* S-domain high pass transform */
void high_pass_transform(SLComplexRect_s* x, long n)

{
  SLComplexRect_s TempRect;
  long i;

  for (i = 0; i < n; i++) {
    if (x[i].imag >= BIGFLOAT) {
      x[i].real = 0.0;
      x[i].imag = 0.0;
    } else {
      TempRect = SCV_Inverse(x[i]);

      x[i].real = -fabs(TempRect.real);
      x[i].imag = TempRect.imag;
    }
  }
}

/* S-domain bandpass or bandstop transform */
void bandpass_bandstop_transform(SLComplexRect_s* x, long n, long ftype, double w0)

{
  SLComplexRect_s pc, sc, t1c, t2c;
  long nt = 2 * n - 1;
  long i;

  for (i = 0; i < n; i++) {
    if (x[i].imag >= BIGFLOAT) {
      x[i].real = 0.0;
      x[nt - i].real = 0.0;
      if (ftype == IIR_BSF) {
        x[i].imag = w0;
        x[nt - i].imag = -w0;
      } else {
        x[i].imag = HUGEFLOAT;
        x[nt - i].imag = 0.0;
      }
    } else {
      if (ftype == IIR_BSF)
        pc = SCV_Inverse(x[i]);

      else
        pc = x[i];

      /* sc = (pc - csdssqrt (pc * pc - 4 * w0 * w0)) / 2.0 */
      //      t1c = ComplexMultiply (pc,  pc);
      t1c.real = pc.real * pc.real - pc.imag * pc.imag;
      t1c.imag = pc.real * pc.imag + pc.imag * pc.real;
      t1c.real -= 4.0 * w0 * w0;

      t2c = SCV_Sqrt(t1c);
      //      sc = ComplexSubtract (pc, t2c);

      sc.real = pc.real - t2c.real;
      sc.imag = pc.imag - t2c.imag;

      sc.real /= 2.0;
      sc.imag /= 2.0;

      x[i].real = -fabs(sc.real);
      x[i].imag = sc.imag;

      /* sc = (pc + csdssqrt (pc * pc - 4 * w0 * w0)) / 2.0 */
      //      sc = Add (pc, t2c);
      sc.real = pc.real + t2c.real;
      sc.imag = pc.imag + t2c.imag;

      sc.real /= 2.0;
      sc.imag /= 2.0;
      x[nt - i].real = -fabs(sc.real);
      x[nt - i].imag = sc.imag;
    }
  }
}
