/**************************************************************************
File Name               : COMPLEX.C     | Author        : JOHN EDWARDS
Siglib Library Version  : 10.00         |
----------------------------------------+----------------------------------
Compiler  : Independent                 | Start Date    : 03/07/1994
Options   :                             | Latest Update : 17/11/2020
---------------------------------------------------------------------------
Support for SigLib is available via Email : support@numerix-dsp.com

This file may be modified, in any way, providing that this header remains
within the file and none of the header contents are removed or modified.

__SIGLIB_LICENSE__

Copyright (c) 2022 Sigma Numerix Ltd. All rights reserved.
---------------------------------------------------------------------------
Description : Complex number routines for SigLib DSP library.


****************************************************************************/

#define SIGLIB_SRC_FILE_COMPLEX 1                           // Defines the source file that this code is being used in

#include <siglib.h>                                         // Include SigLib header file


/**/
/********************************************************
* Function: SCV_Polar()
*
* Parameters:
*   const SLData_t IM   - Source magnitude data
*   const SLData_t IA   - Source angle data
*
* Return value:
*   Polar   - Polar data
*
* Description: Convert the real data to polar.
*
********************************************************/

SLComplexPolar_s SIGLIB_FUNC_DECL SCV_Polar (const SLData_t IM,
    const SLData_t IA)

{
    SLComplexPolar_s    Polar;

    Polar.magn = IM;
    Polar.angle = IA;

    return (Polar);

}       // End of SCV_Polar()


/**/
/********************************************************
* Function: SCV_Rectangular()
*
* Parameters:
*   const SLData_t IR   - Source real data
*   const SLData_t II   - Source imag data
*
* Return value:
*   Rect    - Rectangular data
*
* Description: Convert the real data to rectangular.
*
********************************************************/

SLComplexRect_s SIGLIB_FUNC_DECL SCV_Rectangular (const SLData_t IR,
    const SLData_t II)

{
    SLComplexRect_s Rect;

    Rect.real = IR;
    Rect.imag = II;

    return (Rect);

}       // End of SCV_Rectangular()


/**/
/********************************************************
* Function: SCV_PolarToRectangular()
*
* Parameters:
*   const Polar - Polar data
*
* Return value:
*   Rect,   - Rectangular data
*
* Description: Convert the polar data to complex.
*
********************************************************/

SLComplexRect_s SIGLIB_FUNC_DECL SCV_PolarToRectangular (const SLComplexPolar_s Polar)

{
    SLComplexRect_s Rect;

    Rect.real = Polar.magn * SDS_Cos (Polar.angle);
    Rect.imag = Polar.magn * SDS_Sin (Polar.angle);

    return (Rect);

}       // End of SCV_PolarToRectangular()


/**/
/********************************************************
* Function: SCV_RectangularToPolar()
*
* Parameters:
*   const Rect  - Rectangular data
*
* Return value:
*   Polar data
*
* Description: Convert the complex data to polar.
*
********************************************************/

SLComplexPolar_s SIGLIB_FUNC_DECL SCV_RectangularToPolar (const SLComplexRect_s Rect)

{
    SLComplexPolar_s    Polar;

    Polar.magn = SDS_Sqrt (Rect.real * Rect.real + Rect.imag * Rect.imag);

    if ((Rect.real < SIGLIB_MIN_THRESHOLD) && (Rect.real > -SIGLIB_MIN_THRESHOLD) &&
        (Rect.imag < SIGLIB_MIN_THRESHOLD) && (Rect.imag > -SIGLIB_MIN_THRESHOLD))
    {
        Polar.angle = SIGLIB_ZERO;
    }
    else
    {
        Polar.angle = SDS_Atan2 (Rect.imag, Rect.real);
    }

    return (Polar);

}       // End of SCV_RectangularToPolar()


/**/
/********************************************************
* Function: SCV_Sqrt()
*
* Parameters:
*   const Complex   IVect,  - Complex source data
*
* Return value:
*   Complex OVect,  - Complex destn. data
*
* Description: Calculate the square root of a Vector
*   using DeMoivre's Theorem, i.e. using polar
*   coordinates.
*
********************************************************/

SLComplexRect_s SIGLIB_FUNC_DECL SCV_Sqrt (const SLComplexRect_s IVect)

{
    SLComplexPolar_s    OVect;

    OVect = SCV_RectangularToPolar (IVect);

    OVect.magn = SDS_Sqrt (OVect.magn);
    OVect.angle = SIGLIB_HALF * OVect.angle;

    return (SCV_PolarToRectangular (OVect));

}       // End of SCV_Sqrt()


/**/
/********************************************************
* Function: SCV_Inverse()
*
* Parameters:
*   const Complex   IVect,  - Complex source data
*
* Return value:
*   Complex OVect,  - Complex destn. data
*
* Description: Calculate the inverse of a vector using
*   1/(a+jb) = (a-jb) / (a^2 + b^2)
*
********************************************************/

SLComplexRect_s SIGLIB_FUNC_DECL SCV_Inverse (const SLComplexRect_s IVect)

{
    SLComplexRect_s Result;
    SLData_t        Denominator = SIGLIB_ONE / (IVect.real * IVect.real + IVect.imag * IVect.imag);

    if (Denominator == SIGLIB_ZERO) {               // Check for divide by zero
        Result.real = SIGLIB_ONE;
        Result.imag = SIGLIB_ZERO;
    }

    else {
        Result.real =  IVect.real * Denominator;
        Result.imag = -IVect.imag * Denominator;
    }

    return (Result);

}       // End of SCV_Inverse()


/**/
/********************************************************
* Function: SCV_Conjugate()
*
* Parameters:
*   const Complex IVect,    - Complex source data
*
* Return value:
*   Complex OVect,  - Complex destn. data
*
* Description: Calculate the complex conjugate of a
*   Vector.
*
********************************************************/

SLComplexRect_s SIGLIB_FUNC_DECL SCV_Conjugate (const SLComplexRect_s IVect)

{
    SLComplexRect_s OVectVect;

    OVectVect = IVect;
    OVectVect.imag *= SIGLIB_MINUS_ONE;

    return (OVectVect);

}       // End of SCV_Conjugate()


/**/
/********************************************************
* Function: SCV_Magnitude()
*
* Parameters:
*   const Complex IVect,    - Complex source data
*
* Return value:
*   SLData_t        - Absolute magnitude
*
* Description: Calculate the real absolute value of a
*   Vector.
*
********************************************************/

SLData_t SIGLIB_FUNC_DECL SCV_Magnitude (const SLComplexRect_s IVect)

{
    return (SDS_Sqrt (IVect.real * IVect.real + IVect.imag * IVect.imag));

}       // End of SCV_Magnitude()


/**/
/********************************************************
* Function: SCV_MagnitudeSquared()
*
* Parameters:
*   const Complex IVect,    - Complex source data
*
* Return value:
*   SLData_t        - Absolute magnitude squared
*
* Description: Calculate the real absolute value
*   squared of a complex vector.
*
********************************************************/

SLData_t SIGLIB_FUNC_DECL SCV_MagnitudeSquared (const SLComplexRect_s IVect)

{
    return (IVect.real * IVect.real + IVect.imag * IVect.imag);

}       // End of SCV_MagnitudeSquared()


/**/
/********************************************************
* Function: SCV_Phase()
*
* Parameters:
*   const Complex IVect,    - Complex source data
*
* Return value:
*   SLData_t        - Phase
*
* Description: Calculate the phase of a complex
*   Vector.
*
********************************************************/

SLData_t SIGLIB_FUNC_DECL SCV_Phase (const SLComplexRect_s IVect)

{
    return (SDS_Atan2(IVect.imag, IVect.real));

}       // End of SCV_Phase()


/**/
/********************************************************
* Function: SCV_Multiply()
*
* Parameters:
*   const Complex IVect1,   - Complex source data
*   const Complex IVect2,   - Complex source data
*
* Return value:
*   Complex OVect,  - Complex destn. data
*
* Description: Multiply two complex vectors.
*
********************************************************/

SLComplexRect_s SIGLIB_FUNC_DECL SCV_Multiply (const SLComplexRect_s IVect1,
    const SLComplexRect_s IVect2)

{
    SLComplexRect_s OVect;

    OVect.real = IVect1.real*IVect2.real - IVect1.imag*IVect2.imag;
    OVect.imag = IVect1.real*IVect2.imag + IVect1.imag*IVect2.real;

    return (OVect);

}       // End of SCV_Multiply()


/**/
/********************************************************
* Function: SCV_Divide()
*
* Parameters:
*   const Complex Numerator,    - Complex source data
*   const Complex Divisor,      - Complex source data
*
* Return value:
*   Complex Output, - Complex output data
*
* Description: Divide complex vector 1 by vector 2 using
*   1/(a+jb) = (a-jb) / (a^2 + b^2)
*
********************************************************/

SLComplexRect_s SIGLIB_FUNC_DECL SCV_Divide (const SLComplexRect_s Numerator,
    const SLComplexRect_s Divisor)

{
    SLComplexRect_s OVect, Result;
    SLData_t        Denominator = SIGLIB_ONE / (Divisor.real * Divisor.real + Divisor.imag * Divisor.imag);

    if (Denominator == SIGLIB_ZERO) {               // Check for divide by zero
        Result.real = SIGLIB_ONE;
        Result.imag = SIGLIB_ZERO;
    }

    else {
                                                    // Negate the divisor
        OVect.real =  Divisor.real * Denominator;
        OVect.imag = -Divisor.imag * Denominator;

                                                    // Multiply numerator by divisor
        Result.real = Numerator.real * OVect.real - Numerator.imag * OVect.imag;
        Result.imag = Numerator.real * OVect.imag + Numerator.imag * OVect.real;
    }

    return (Result);

}       // End of SCV_Divide()


/**/
/********************************************************
* Function: SCV_Add()
*
* Parameters:
*   const Complex IVect1,   - Complex source data
*   const Complex IVect2,   - Complex source data
*
* Return value:
*   Complex OVect,  - Complex destn. data
*
* Description: Add two complex vectors.
*
********************************************************/

SLComplexRect_s SIGLIB_FUNC_DECL SCV_Add (const SLComplexRect_s IVect1,
    const SLComplexRect_s IVect2)

{
    SLComplexRect_s OVect;

    OVect.real = IVect1.real + IVect2.real;
    OVect.imag = IVect1.imag + IVect2.imag;

    return (OVect);

}       // End of SCV_Add()


/**/
/********************************************************
* Function: SCV_Subtract()
*
* Parameters:
*   const Complex IVect1,   - Complex source data
*   const Complex IVect2,   - Complex source data
*
* Return value:
*   Complex OVect,  - Complex destn. data
*
* Description: Subtract IVect 2 from IVect1.
*
********************************************************/

SLComplexRect_s SIGLIB_FUNC_DECL SCV_Subtract (const SLComplexRect_s IVect1,
    const SLComplexRect_s IVect2)

{
    SLComplexRect_s OVect;

    OVect.real = IVect1.real - IVect2.real;
    OVect.imag = IVect1.imag - IVect2.imag;

    return (OVect);

}       // End of SCV_Subtract()


/**/
/********************************************************
* Function: SCV_Log()
*
* Parameters:
*   const Complex IVect,    - Complex source data
*
* Return value:
*   Complex OVect,  - Complex log
*
* Description: Return the complex logrithm.
*
********************************************************/

SLComplexRect_s SIGLIB_FUNC_DECL SCV_Log (const SLComplexRect_s IVect)

{
    SLComplexRect_s OVect;

    OVect.real = SIGLIB_HALF * SDS_Log (IVect.real * IVect.real + IVect.imag * IVect.imag);
    OVect.imag = SDS_Atan2 (IVect.imag, IVect.real);

    return (OVect);

}       // End of SCV_Log()


/**/
/********************************************************
* Function: SCV_Exp()
*
* Parameters:
*   const Complex IVect,    - Complex source data
*
* Return value:
*   Complex OVect,  - Complex exponential
*
* Description: Return the complex exponential.
*
********************************************************/

SLComplexRect_s SIGLIB_FUNC_DECL SCV_Exp (const SLComplexRect_s IVect)

{
    SLComplexRect_s OVect;
    SLData_t        FTmp;

    FTmp = SDS_Exp ( IVect.real);
    OVect.imag = FTmp * SDS_Sin (IVect.imag);
    OVect.real = FTmp * SDS_Cos (IVect.imag);

    return (OVect);
}       // End of SCV_Exp()


/**/
/********************************************************
* Function: SCV_Expj()
*
* Parameters:
*   const SLData_t Theta,   - Real angle
*
* Return value:
*   Complex OVect,  - Complex exponential
*
* Description: Return the complex exponential (e^jTheta).
*
********************************************************/

SLComplexRect_s SIGLIB_FUNC_DECL SCV_Expj (const SLData_t Theta)

{

    return SCV_Rectangular (SDS_Cos (Theta), SDS_Sin (Theta));

}       // End of SCV_Expj()


/**/
/********************************************************
* Function: SCV_Pow()
*
* Parameters:
*   const Complex IVect,    - Complex source data
*   const SLData_t Power    - Real power
*
* Return value:
*   Complex OVect,  - Complex destn. data
*
* Description: Raise the complex number to a real power.
*
********************************************************/

SLComplexRect_s SIGLIB_FUNC_DECL SCV_Pow (const SLComplexRect_s IVect,
    const SLData_t Power)

{
    SLData_t        Magn, Angle;
    SLComplexRect_s OVect;

    if (((IVect.real < SIGLIB_MIN_THRESHOLD) && // Check for close to zero
        (IVect.real > -SIGLIB_MIN_THRESHOLD)) &&
        ((IVect.imag < SIGLIB_MIN_THRESHOLD) && // Check for close to zero
        (IVect.imag > -SIGLIB_MIN_THRESHOLD))) {

        OVect.real = SIGLIB_ZERO;
        OVect.imag = SIGLIB_ZERO;
        return (OVect);
    }

    Magn = SDS_Exp (Power * SDS_Log (SDS_Sqrt (IVect.real * IVect.real + IVect.imag * IVect.imag)));
    Angle = SDS_Atan2 (IVect.imag, IVect.real);
    OVect.real = Magn * SDS_Cos (Power * Angle);
    OVect.imag = Magn * SDS_Sin (Power * Angle);

    return (OVect);

}       // End of SCV_Pow()


/**/
/********************************************************
* Function: SCV_VectorAddScalar()
*
* Parameters:
*   const Complex   IVect,  - Complex source data
*   const SLData_t  Scalar  - Scalar quantity
*
* Return value:
*   Complex OVect           - Complex result
*
* Description: Adds a scalar quantity to a complex
*   vector.
*
********************************************************/

SLComplexRect_s SIGLIB_FUNC_DECL SCV_VectorAddScalar (const SLComplexRect_s IVect,
    const SLData_t Scalar)

{
    SLComplexRect_s OVect;

    OVect.real = IVect.real + Scalar;
    OVect.imag = IVect.imag;

    return (OVect);

}       // End of SCV_VectorAddScalar()


/**/
/********************************************************
* Function: SCV_VectorSubtractScalar()
*
* Parameters:
*   const Complex   IVect,  - Complex source data
*   const SLData_t  Scalar  - Scalar quantity
*
* Return value:
*   Complex OVect           - Complex result
*
* Description: Subtract a scalar quantity from a complex
*   vector.
*
********************************************************/

SLComplexRect_s SIGLIB_FUNC_DECL SCV_VectorSubtractScalar (const SLComplexRect_s IVect,
    const SLData_t Scalar)

{
    SLComplexRect_s OVect;

    OVect.real = IVect.real - Scalar;
    OVect.imag = IVect.imag;

    return (OVect);

}       // End of SCV_VectorSubtractScalar()


/**/
/********************************************************
* Function: SCV_VectorMultiplyScalar()
*
* Parameters:
*   const Complex   IVect,  - Complex source data
*   const SLData_t  Scalar  - Scalar quantity
*
* Return value:
*   Complex OVect           - Complex result
*
* Description: Multiply a complex vector by a scalar
*   quantity .
*
********************************************************/

SLComplexRect_s SIGLIB_FUNC_DECL SCV_VectorMultiplyScalar (const SLComplexRect_s IVect,
    const SLData_t Scalar)

{
    SLComplexRect_s OVect;

    OVect.real = IVect.real * Scalar;
    OVect.imag = IVect.imag * Scalar;

    return (OVect);

}       // End of SCV_VectorMultiplyScalar()


/**/
/********************************************************
* Function: SCV_VectorDivideScalar()
*
* Parameters:
*   const Complex   IVect,  - Complex source data
*   const SLData_t  Scalar  - Scalar quantity
*
* Return value:
*   Complex OVect,  - Complex result
*
* Description: Divide a complex vector by a scalar
*   quantity.
*
********************************************************/

SLComplexRect_s SIGLIB_FUNC_DECL SCV_VectorDivideScalar (const SLComplexRect_s IVect,
    const SLData_t Scalar)

{
    SLComplexRect_s OVect;

    OVect.real = IVect.real / Scalar;
    OVect.imag = IVect.imag / Scalar;

    return (OVect);

}       // End of SCV_VectorDivideScalar()


/**/
/********************************************************
* Function: SCV_ScalarSubtractVector()
*
* Parameters:
*   const SLData_t  Scalar  - Scalar quantity
*   const Complex   IVect,  - Complex source data
*
* Return value:
*   Complex OVect           - Complex result
*
* Description: Subtract a complex vector quantity from
*   a scalar.
*
********************************************************/

SLComplexRect_s SIGLIB_FUNC_DECL SCV_ScalarSubtractVector (const SLData_t Scalar,
    const SLComplexRect_s IVect)

{
    SLComplexRect_s OVect;

    OVect.real = Scalar - IVect.real;
    OVect.imag = -IVect.imag;

    return (OVect);

}       // End of SCV_ScalarSubtractVector()


/**/
/********************************************************
* Function: SCV_Roots()
*
* Parameters:
*   const SLComplexRect_s a,
*   const SLComplexRect_s b,
*   const SLComplexRect_s c,
*   SLComplexRect_s *Root1, - Pointer to root # 1
*   SLComplexRect_s *Root2  - Pointer to root # 2
*
* Return value:
*   void
*
* Description: Generates roots of ax^2 + bx + c = 0
*   Using : (-b +/- sqrt (b^2 - 4ac)) / 2a
*
********************************************************/

void SIGLIB_FUNC_DECL SCV_Roots (const SLComplexRect_s a,
    const SLComplexRect_s b,
    const SLComplexRect_s c,
    SLComplexRect_s *Root1,
    SLComplexRect_s *Root2)

{
    *Root1 = SCV_Divide (SCV_Add (SCV_VectorMultiplyScalar (b, -1.0),
        SCV_Sqrt (SCV_Subtract (SCV_Multiply (b, b), SCV_VectorMultiplyScalar (SCV_Multiply (a, c), SIGLIB_FOUR)))),
        SCV_VectorMultiplyScalar (a, SIGLIB_TWO));

    *Root2 = SCV_Divide (SCV_Subtract (SCV_VectorMultiplyScalar (b, -1.0),
        SCV_Sqrt (SCV_Subtract (SCV_Multiply (b, b), SCV_VectorMultiplyScalar (SCV_Multiply (a, c), SIGLIB_FOUR)))),
        SCV_VectorMultiplyScalar (a, SIGLIB_TWO));

}       // End of SCV_Roots()


/**/
/********************************************************
* Function: SCV_Copy()
*
* Parameters:
*   const Complex   IVect,  - Complex source data
*
* Return value:
*   Complex OVect,  - Complex destn. data
*
* Description: Copy the source vector to the destination.
*
********************************************************/

SLComplexRect_s SIGLIB_FUNC_DECL SCV_Copy (const SLComplexRect_s IVect)

{
    return (IVect);

}       // End of SCV_Copy()


/**/
/********************************************************
* Function: SCV_Compare()
*
* Parameters:
*   const Complex IVect1,   - Complex source data
*   const Complex IVect2,   - Complex source data
*
* Return value:
*   SLCompareType_t = SIGLIB_EQUAL(1) if they are the same,
*   SIGLIB_NOT_EQUAL(0) otherwise
*
* Description: Compare IVect1 to IVect2.
*
********************************************************/

enum SLCompareType_t SIGLIB_FUNC_DECL SCV_Compare (const SLComplexRect_s IVect1,
    const SLComplexRect_s IVect2)

{

    if ((IVect1.real == IVect2.real) && (IVect1.imag == IVect2.imag)) {
        return (SIGLIB_EQUAL);
    }

    return (SIGLIB_NOT_EQUAL);

}       // End of SCV_Compare()


