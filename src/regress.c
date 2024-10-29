
/**************************************************************************
File Name               : REGRESS.C     | Author        : JOHN EDWARDS
Siglib Library Version  : 10.50         |
----------------------------------------+----------------------------------
Compiler  : Independent                 | Start Date    : 29/12/1993
Options   :                             | Latest Update : 17/11/2020
---------------------------------------------------------------------------
Support for SigLib is available via Email: support@numerix-dsp.com

This file may be modified, in any way, providing that this header remains
within the file and none of the header contents are removed or modified.

SigLib is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License V2, as
published by the Free Software Foundation.

SigLib is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1335, USA

This sofware is also available with a commercial license, for use in
proprietary, research, government or commercial applications.
Please contact Delta Numerix for further details :
https://www.numerix-dsp.com
support@.numerix-dsp.com

Copyright (c) 2024 Delta Numerix All rights reserved.
---------------------------------------------------------------------------
Description: SigLib DSP library regression analysis routines.

****************************************************************************/

#define SIGLIB_SRC_FILE_REGRESS 1    // Defines the source file that this code is being used in

#include <siglib.h>    // Include SigLib header file

/********************************************************
 * Function: SDA_LinraConstantCoeff
 *
 * Parameters:
 *      const SLData_t * SIGLIB_PTR_DECL pSrcX - Buffer X data pointer
 *      const SLData_t * SIGLIB_PTR_DECL pSrcY - Buffer Y data pointer
 *      const SLArrayIndex_t sampleLength - Buffer length
 *
 * Return value: Constant coefficient.
 *
 * Description: The constant coefficient for the
 *  supplied data.
 *
 *  Assuming the data can be modeled according to:
 *
 *      y = Mx + C
 *
 *  Gives:
 *
 *      sum(y) - M.sum(x)
 *  C = -----------------
 *             n
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_LinraConstantCoeff(const SLData_t* SIGLIB_PTR_DECL pSrcX, const SLData_t* SIGLIB_PTR_DECL pSrcY,
                                                 const SLArrayIndex_t sampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef _TMS320C6700               // Defined by TI compiler
  _nassert((int)pSrcX % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pSrcY % 8 == 0);
#  endif
#endif

  SLData_t SumX = *pSrcX;
  SLData_t SumY = *pSrcY;
  pSrcX++;
  pSrcY++;

  for (SLArrayIndex_t i = 1; i < sampleLength; i++) {
    SumX += *pSrcX;
    SumY += *pSrcY;
    pSrcX++;
    pSrcY++;
  }

  pSrcX -= sampleLength;
  pSrcY -= sampleLength;

  return ((SumY - (SDA_LinraRegressionCoeff(pSrcX, pSrcY, sampleLength) * SumX)) / sampleLength);
}    // End of SDA_LinraConstantCoeff()

/********************************************************
 * Function: SDA_LinraRegressionCoeff
 *
 * Parameters:
 *      const SLData_t * SIGLIB_PTR_DECL pSrc1 - Buffer X data pointer
 *      const SLData_t * SIGLIB_PTR_DECL pSrc2 - Buffer Y data pointer
 *      const SLArrayIndex_t sampleLength - Buffer length
 *
 * Return value: Regression coefficient.
 *
 * Description: The regression coefficient for the
 *  supplied data.
 *
 *  Assuming the data can be modeled according to:
 *
 *      y = Mx + C
 *
 *  Gives:
 *
 *      n.sum(x.y) - sum(x) . sum(y)
 *  M = ----------------------------
 *        n.sum(x^2) - (sum(x))^2
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_LinraRegressionCoeff(const SLData_t* SIGLIB_PTR_DECL pSrcX, const SLData_t* SIGLIB_PTR_DECL pSrcY,
                                                   const SLArrayIndex_t sampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef _TMS320C6700               // Defined by TI compiler
  _nassert((int)pSrcX % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pSrcY % 8 == 0);
#  endif
#endif

  SLData_t SumXY = (*pSrcX) * (*pSrcY);
  SLData_t SumX = *pSrcX;
  SLData_t SumY = *pSrcY;
  SLData_t SumXSquared = (*pSrcX) * (*pSrcX);
  pSrcX++;
  pSrcY++;

  for (SLArrayIndex_t i = 1; i < sampleLength; i++) {
    SumXY += (*pSrcX) * (*pSrcY);
    SumX += *pSrcX;
    SumY += *pSrcY;
    SumXSquared += (*pSrcX) * (*pSrcX);
    pSrcX++;
    pSrcY++;
  }

  return (((sampleLength * SumXY) - (SumX * SumY)) / ((sampleLength * SumXSquared) - (SumX * SumX)));
}    // End of SDA_LinraRegressionCoeff()

/********************************************************
 * Function: SDA_LinraCorrelationCoeff
 *
 * Parameters:
 *      const SLData_t * SIGLIB_PTR_DECL pSrc1 - Buffer X data pointer
 *      const SLData_t * SIGLIB_PTR_DECL pSrc2 - Buffer Y data pointer
 *      const SLArrayIndex_t sampleLength - Buffer length
 *
 * Return value: Correlation coefficient.
 *
 * Description: The correlation coefficient for the
 *  supplied data.
 *
 *  Assuming the data can be modeled according to:
 *
 *      y = Mx + C
 *
 *  Gives:
 *
 *                      n.sum(x.y) - sum(x) . sum(y)
 *  r = ----------------------------------------------------------
 *      sqrt ((n.sum(x^2) - (sum(x))^2).(n.sum(y^2) - (sum(y))^2))
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_LinraCorrelationCoeff(const SLData_t* SIGLIB_PTR_DECL pSrcX, const SLData_t* SIGLIB_PTR_DECL pSrcY,
                                                    const SLArrayIndex_t sampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef _TMS320C6700               // Defined by TI compiler
  _nassert((int)pSrcX % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pSrcY % 8 == 0);
#  endif
#endif

  SLData_t SumXY = (*pSrcX) * (*pSrcY);
  SLData_t SumX = *pSrcX;
  SLData_t SumY = *pSrcY;
  SLData_t SumXSquared = (*pSrcX) * (*pSrcX);
  SLData_t SumYSquared = (*pSrcY) * (*pSrcY);
  pSrcX++;
  pSrcY++;

  for (SLArrayIndex_t i = 1; i < sampleLength; i++) {
    SumXY += (*pSrcX) * (*pSrcY);
    SumX += *pSrcX;
    SumY += *pSrcY;
    SumXSquared += (*pSrcX) * (*pSrcX);
    SumYSquared += (*pSrcY) * (*pSrcY);
    pSrcX++;
    pSrcY++;
  }

  return (((sampleLength * SumXY) - (SumX * SumY)) /
          SDS_Sqrt(((sampleLength * SumXSquared) - (SumX * SumX)) * ((sampleLength * SumYSquared) - (SumY * SumY))));
}    // End of SDA_LinraRegressionCoeff()

/********************************************************
 * Function: SDA_LinraEstimateX
 *
 * Parameters:
 *      const SLData_t * SIGLIB_PTR_DECL pSrc1 - Buffer X data pointer
 *      const SLData_t * SIGLIB_PTR_DECL pSrc2 - Buffer Y data pointer
 *      const SLData_t YVal  - Y value
 *      const SLArrayIndex_t sampleLength - Buffer length
 *
 * Return value: Estimate X
 *
 * Description: Estimate a value for X, given a value
 *  for Y.
 *
 *  Assuming the data can be modeled according to:
 *
 *      y = Mx + C
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_LinraEstimateX(const SLData_t* SIGLIB_PTR_DECL pSrcX, const SLData_t* SIGLIB_PTR_DECL pSrcY, const SLData_t YVal,
                                             const SLArrayIndex_t sampleLength)
{
  return ((YVal - SDA_LinraConstantCoeff(pSrcX, pSrcY, sampleLength)) / SDA_LinraRegressionCoeff(pSrcX, pSrcY, sampleLength));
}    // End of SDA_LinraEstimateX()

/********************************************************
 * Function: SDA_LinraEstimateY
 *
 * Parameters:
 *      const SLData_t * SIGLIB_PTR_DECL pSrc1 - Buffer X data pointer
 *      const SLData_t * SIGLIB_PTR_DECL pSrc2 - Buffer Y data pointer
 *      const SLData_t YVal  - Y value
 *      const SLArrayIndex_t sampleLength - Buffer length
 *
 * Return value: Estimate Y
 *
 * Description: Estimate a value for Y, given a value
 *  for X.
 *
 *  Assuming the data can be modeled according to:
 *
 *      y = Mx + C
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_LinraEstimateY(const SLData_t* SIGLIB_PTR_DECL pSrcX, const SLData_t* SIGLIB_PTR_DECL pSrcY, const SLData_t XVal,
                                             const SLArrayIndex_t sampleLength)
{
  return ((XVal * SDA_LinraRegressionCoeff(pSrcX, pSrcY, sampleLength)) + SDA_LinraConstantCoeff(pSrcX, pSrcY, sampleLength));
}    // End of SDA_LinraEstimateY()

/********************************************************
 * Function: SDA_LograConstantCoeff
 *
 * Parameters:
 *      const SLData_t * SIGLIB_PTR_DECL pSrcX - Buffer X data pointer
 *      const SLData_t * SIGLIB_PTR_DECL pSrcY - Buffer Y data pointer
 *      const SLArrayIndex_t sampleLength - Buffer length
 *
 * Return value: Constant coefficient.
 *
 * Description: The constant coefficient for the
 *  supplied data.
 *
 *  Assuming the data can be modeled according to:
 *
 *      y = M.ln(x) + C
 *
 *  Gives:
 *
 *      sum(y) - M.sum(ln(x))
 *  C = ---------------------
 *              n
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_LograConstantCoeff(const SLData_t* SIGLIB_PTR_DECL pSrcX, const SLData_t* SIGLIB_PTR_DECL pSrcY,
                                                 const SLArrayIndex_t sampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__              // Defined by TI compiler
  _nassert((int)pSrcX % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pSrcY % 8 == 0);
#  endif
#endif

  SLData_t SumX = SDS_Log(*pSrcX);
  SLData_t SumY = *pSrcY;
  pSrcX++;
  pSrcY++;

  for (SLArrayIndex_t i = 1; i < sampleLength; i++) {
    SumX += SDS_Log(*pSrcX);
    SumY += *pSrcY;
    pSrcX++;
    pSrcY++;
  }

  pSrcX -= sampleLength;
  pSrcY -= sampleLength;

  return ((SumY - (SDA_LograRegressionCoeff(pSrcX, pSrcY, sampleLength) * SumX)) / sampleLength);
}    // End of SDA_LograConstantCoeff()

/********************************************************
 * Function: SDA_LograRegressionCoeff
 *
 * Parameters:
 *      const SLData_t * SIGLIB_PTR_DECL pSrc1 - Buffer X data pointer
 *      const SLData_t * SIGLIB_PTR_DECL pSrc2 - Buffer Y data pointer
 *      const SLArrayIndex_t sampleLength - Buffer length
 *
 * Return value: Regression coefficient.
 *
 * Description: The regression coefficient for the
 *  supplied data.
 *
 *  Assuming the data can be modeled according to:
 *
 *      y = M.ln(x) + C
 *
 *  Gives:
 *
 *      n.sum(ln(x).y) - sum(ln(x)) . sum(y)
 *  M = ------------------------------------
 *        n.sum(ln(x)^2) - (sum(ln(x)))^2
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_LograRegressionCoeff(const SLData_t* SIGLIB_PTR_DECL pSrcX, const SLData_t* SIGLIB_PTR_DECL pSrcY,
                                                   const SLArrayIndex_t sampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__              // Defined by TI compiler
  _nassert((int)pSrcX % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pSrcY % 8 == 0);
#  endif
#endif

  SLData_t SumXY = SDS_Log(*pSrcX) * (*pSrcY);
  SLData_t SumX = SDS_Log(*pSrcX);
  SLData_t SumY = *pSrcY;
  SLData_t SumXSquared = SDS_Log(*pSrcX) * SDS_Log(*pSrcX);
  pSrcX++;
  pSrcY++;

  for (SLArrayIndex_t i = 1; i < sampleLength; i++) {
    SumXY += SDS_Log(*pSrcX) * (*pSrcY);
    SumX += SDS_Log(*pSrcX);
    SumY += *pSrcY;
    SumXSquared += SDS_Log(*pSrcX) * SDS_Log(*pSrcX);
    pSrcX++;
    pSrcY++;
  }

  return (((sampleLength * SumXY) - (SumX * SumY)) / ((sampleLength * SumXSquared) - (SumX * SumX)));
}    // End of SDA_LograRegressionCoeff()

/********************************************************
 * Function: SDA_LograCorrelationCoeff
 *
 * Parameters:
 *      const SLData_t * SIGLIB_PTR_DECL pSrc1 - Buffer X data pointer
 *      const SLData_t * SIGLIB_PTR_DECL pSrc2 - Buffer Y data pointer
 *      const SLArrayIndex_t sampleLength - Buffer length
 *
 * Return value: Correlation coefficient.
 *
 * Description: The correlation coefficient for the
 *  supplied data.
 *
 *  Assuming the data can be modeled according to:
 *
 *      y = M.ln(x) + C
 *
 *  Gives:
 *
 *                      n.sum(ln(x).y) - sum(ln(x)) . sum(y)
 *  r = ------------------------------------------------------------------
 *      sqrt ((n.sum(ln(x)^2) - (sum(ln(x)))^2).(n.sum(y^2) - (sum(y))^2))
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_LograCorrelationCoeff(const SLData_t* SIGLIB_PTR_DECL pSrcX, const SLData_t* SIGLIB_PTR_DECL pSrcY,
                                                    const SLArrayIndex_t sampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__              // Defined by TI compiler
  _nassert((int)pSrcX % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pSrcY % 8 == 0);
#  endif
#endif

  SLData_t SumXY = SDS_Log(*pSrcX) * (*pSrcY);
  SLData_t SumX = SDS_Log(*pSrcX);
  SLData_t SumY = *pSrcY;
  SLData_t SumXSquared = SDS_Log(*pSrcX) * SDS_Log(*pSrcX);
  SLData_t SumYSquared = (*pSrcY) * (*pSrcY);
  pSrcX++;
  pSrcY++;

  for (SLArrayIndex_t i = 1; i < sampleLength; i++) {
    SumXY += SDS_Log(*pSrcX) * (*pSrcY);
    SumX += SDS_Log(*pSrcX);
    SumY += *pSrcY;
    SumXSquared += SDS_Log(*pSrcX) * SDS_Log(*pSrcX);
    SumYSquared += (*pSrcY) * (*pSrcY);
    pSrcX++;
    pSrcY++;
  }

  return (((sampleLength * SumXY) - (SumX * SumY)) /
          SDS_Sqrt(((sampleLength * SumXSquared) - (SumX * SumX)) * ((sampleLength * SumYSquared) - (SumY * SumY))));
}    // End of SDA_LograRegressionCoeff()

/********************************************************
 * Function: SDA_LograEstimateX
 *
 * Parameters:
 *      const SLData_t * SIGLIB_PTR_DECL pSrc1 - Buffer X data pointer
 *      const SLData_t * SIGLIB_PTR_DECL pSrc2 - Buffer Y data pointer
 *      const SLData_t YVal  - Y value
 *      const SLArrayIndex_t sampleLength - Buffer length
 *
 * Return value: Estimate X
 *
 * Description: Estimate a value for X, given a value
 *  for Y.
 *
 *  Assuming the data can be modeled according to:
 *
 *      y = M.ln(x) + C
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_LograEstimateX(const SLData_t* SIGLIB_PTR_DECL pSrcX, const SLData_t* SIGLIB_PTR_DECL pSrcY, const SLData_t YVal,
                                             const SLArrayIndex_t sampleLength)
{
  return (SDS_Exp((YVal - SDA_LograConstantCoeff(pSrcX, pSrcY, sampleLength)) / SDA_LograRegressionCoeff(pSrcX, pSrcY, sampleLength)));
}    // End of SDA_LograEstimateX()

/********************************************************
 * Function: SDA_LograEstimateY
 *
 * Parameters:
 *      const SLData_t * SIGLIB_PTR_DECL pSrc1 - Buffer X data pointer
 *      const SLData_t * SIGLIB_PTR_DECL pSrc2 - Buffer Y data pointer
 *      const SLData_t YVal  - Y value
 *      const SLArrayIndex_t sampleLength - Buffer length
 *
 * Return value: Estimate Y
 *
 * Description: Estimate a value for Y, given a value
 *  for X.
 *
 *  Assuming the data can be modeled according to:
 *
 *      y = M.ln(x) + C
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_LograEstimateY(const SLData_t* SIGLIB_PTR_DECL pSrcX, const SLData_t* SIGLIB_PTR_DECL pSrcY, const SLData_t XVal,
                                             const SLArrayIndex_t sampleLength)
{
  return ((SDS_Log(XVal) * SDA_LograRegressionCoeff(pSrcX, pSrcY, sampleLength)) + SDA_LograConstantCoeff(pSrcX, pSrcY, sampleLength));
}    // End of SDA_LograEstimateY()

/********************************************************
 * Function: SDA_ExpraConstantCoeff
 *
 * Parameters:
 *      const SLData_t * SIGLIB_PTR_DECL pSrcX - Buffer X data pointer
 *      const SLData_t * SIGLIB_PTR_DECL pSrcY - Buffer Y data pointer
 *      const SLArrayIndex_t sampleLength - Buffer length
 *
 * Return value: Constant coefficient.
 *
 * Description: The constant coefficient for the
 *  supplied data.
 *
 *  Assuming the data can be modeled according to:
 *
 *      y = C.exp(Mx)
 *
 *  Gives:
 *
 *      sum(ln(y)) - M.sum(x)
 *  C = ---------------------
 *              n
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_ExpraConstantCoeff(const SLData_t* SIGLIB_PTR_DECL pSrcX, const SLData_t* SIGLIB_PTR_DECL pSrcY,
                                                 const SLArrayIndex_t sampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__              // Defined by TI compiler
  _nassert((int)pSrcX % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pSrcY % 8 == 0);
#  endif
#endif

  SLData_t SumX = *pSrcX;
  SLData_t SumY = SDS_Log(*pSrcY);
  pSrcX++;
  pSrcY++;

  for (SLArrayIndex_t i = 1; i < sampleLength; i++) {
    SumX += *pSrcX;
    SumY += SDS_Log(*pSrcY);
    pSrcX++;
    pSrcY++;
  }

  pSrcX -= sampleLength;
  pSrcY -= sampleLength;

  return ((SumY - (SDA_ExpraRegressionCoeff(pSrcX, pSrcY, sampleLength) * SumX)) / sampleLength);
}    // End of SDA_ExpraConstantCoeff()

/********************************************************
 * Function: SDA_ExpraRegressionCoeff
 *
 * Parameters:
 *      const SLData_t * SIGLIB_PTR_DECL pSrc1 - Buffer X data pointer
 *      const SLData_t * SIGLIB_PTR_DECL pSrc2 - Buffer Y data pointer
 *      const SLArrayIndex_t sampleLength - Buffer length
 *
 * Return value: Regression coefficient.
 *
 * Description: The regression coefficient for the
 *  supplied data.
 *
 *  Assuming the data can be modeled according to:
 *
 *      y = C.exp(Mx)
 *
 *  Gives:
 *
 *      n.sum(x.ln(y)) - sum(x) . sum(ln(y))
 *  M = ------------------------------------
 *          n.sum(x^2) - (sum(x))^2
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_ExpraRegressionCoeff(const SLData_t* SIGLIB_PTR_DECL pSrcX, const SLData_t* SIGLIB_PTR_DECL pSrcY,
                                                   const SLArrayIndex_t sampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__              // Defined by TI compiler
  _nassert((int)pSrcX % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pSrcY % 8 == 0);
#  endif
#endif

  SLData_t SumXY = (*pSrcX) * SDS_Log(*pSrcY);
  SLData_t SumX = *pSrcX;
  SLData_t SumY = SDS_Log(*pSrcY);
  SLData_t SumXSquared = (*pSrcX) * (*pSrcX);
  pSrcX++;
  pSrcY++;

  for (SLArrayIndex_t i = 1; i < sampleLength; i++) {
    SumXY += (*pSrcX) * SDS_Log(*pSrcY);
    SumX += *pSrcX;
    SumY += SDS_Log(*pSrcY);
    SumXSquared += (*pSrcX) * (*pSrcX);
    pSrcX++;
    pSrcY++;
  }

  return (((sampleLength * SumXY) - (SumX * SumY)) / ((sampleLength * SumXSquared) - (SumX * SumX)));
}    // End of SDA_ExpraRegressionCoeff()

/********************************************************
 * Function: SDA_ExpraCorrelationCoeff
 *
 * Parameters:
 *      const SLData_t * SIGLIB_PTR_DECL pSrc1 - Buffer X data pointer
 *      const SLData_t * SIGLIB_PTR_DECL pSrc2 - Buffer Y data pointer
 *      const SLArrayIndex_t sampleLength - Buffer length
 *
 * Return value: Correlation coefficient.
 *
 * Description: The correlation coefficient for the
 *  supplied data.
 *
 *  Assuming the data can be modeled according to:
 *
 *      y = C.exp(Mx)
 *
 *  Gives:
 *
 *                      n.sum(x.ln(y)) - sum(x) . sum(ln(y))
 *  r = ------------------------------------------------------------------
 *      sqrt ((n.sum(x^2) - (sum(x))^2).(n.sum(ln(y)^2) - (sum(ln(y)))^2))
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_ExpraCorrelationCoeff(const SLData_t* SIGLIB_PTR_DECL pSrcX, const SLData_t* SIGLIB_PTR_DECL pSrcY,
                                                    const SLArrayIndex_t sampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__              // Defined by TI compiler
  _nassert((int)pSrcX % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pSrcY % 8 == 0);
#  endif
#endif

  SLData_t SumXY = (*pSrcX) * SDS_Log(*pSrcY);
  SLData_t SumX = *pSrcX;
  SLData_t SumY = SDS_Log(*pSrcY);
  SLData_t SumXSquared = (*pSrcX) * (*pSrcX);
  SLData_t SumYSquared = SDS_Log(*pSrcY) * SDS_Log(*pSrcY);
  pSrcX++;
  pSrcY++;

  for (SLArrayIndex_t i = 1; i < sampleLength; i++) {
    SumXY += (*pSrcX) * SDS_Log(*pSrcY);
    SumX += *pSrcX;
    SumY += SDS_Log(*pSrcY);
    SumXSquared += (*pSrcX) * (*pSrcX);
    SumYSquared += SDS_Log(*pSrcY) * SDS_Log(*pSrcY);
    pSrcX++;
    pSrcY++;
  }

  return (((sampleLength * SumXY) - (SumX * SumY)) /
          SDS_Sqrt(((sampleLength * SumXSquared) - (SumX * SumX)) * ((sampleLength * SumYSquared) - (SumY * SumY))));
}    // End of SDA_ExpraRegressionCoeff()

/********************************************************
 * Function: SDA_ExpraEstimateX
 *
 * Parameters:
 *      const SLData_t * SIGLIB_PTR_DECL pSrc1 - Buffer X data pointer
 *      const SLData_t * SIGLIB_PTR_DECL pSrc2 - Buffer Y data pointer
 *      const SLData_t YVal  - Y value
 *      const SLArrayIndex_t sampleLength - Buffer length
 *
 * Return value: Estimate X
 *
 * Description: Estimate a value for X, given a value
 *  for Y.
 *
 *  Assuming the data can be modeled according to:
 *
 *      y = C.exp(Mx)
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_ExpraEstimateX(const SLData_t* SIGLIB_PTR_DECL pSrcX, const SLData_t* SIGLIB_PTR_DECL pSrcY, const SLData_t YVal,
                                             const SLArrayIndex_t sampleLength)
{
  return ((SDS_Log(YVal) - SDA_ExpraConstantCoeff(pSrcX, pSrcY, sampleLength)) / SDA_ExpraRegressionCoeff(pSrcX, pSrcY, sampleLength));
}    // End of SDA_ExpraEstimateX()

/********************************************************
 * Function: SDA_ExpraEstimateY
 *
 * Parameters:
 *      const SLData_t * SIGLIB_PTR_DECL pSrc1 - Buffer X data pointer
 *      const SLData_t * SIGLIB_PTR_DECL pSrc2 - Buffer Y data pointer
 *      const SLData_t YVal  - Y value
 *      const SLArrayIndex_t sampleLength - Buffer length
 *
 * Return value: Estimate Y
 *
 * Description: Estimate a value for Y, given a value
 *  for X.
 *
 *  Assuming the data can be modeled according to:
 *
 *      y = C.exp(Mx)
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_ExpraEstimateY(const SLData_t* SIGLIB_PTR_DECL pSrcX, const SLData_t* SIGLIB_PTR_DECL pSrcY, const SLData_t XVal,
                                             const SLArrayIndex_t sampleLength)
{
  return (SDS_Exp((XVal * SDA_ExpraRegressionCoeff(pSrcX, pSrcY, sampleLength)) + SDA_ExpraConstantCoeff(pSrcX, pSrcY, sampleLength)));
}    // End of SDA_ExpraEstimateY()

/********************************************************
 * Function: SDA_PowraConstantCoeff
 *
 * Parameters:
 *      const SLData_t * SIGLIB_PTR_DECL pSrcX - Buffer X data pointer
 *      const SLData_t * SIGLIB_PTR_DECL pSrcY - Buffer Y data pointer
 *      const SLArrayIndex_t sampleLength - Buffer length
 *
 * Return value: Constant coefficient.
 *
 * Description: The constant coefficient for the
 *  supplied data.
 *
 *  Assuming the data can be modeled according to:
 *
 *      y = C.x^M
 *
 *  Gives:
 *
 *      sum(ln(y)) - M.sum(ln(x))
 *  C = -------------------------
 *                  n
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_PowraConstantCoeff(const SLData_t* SIGLIB_PTR_DECL pSrcX, const SLData_t* SIGLIB_PTR_DECL pSrcY,
                                                 const SLArrayIndex_t sampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__              // Defined by TI compiler
  _nassert((int)pSrcX % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pSrcY % 8 == 0);
#  endif
#endif

  SLData_t SumX = SDS_Log(*pSrcX);
  SLData_t SumY = SDS_Log(*pSrcY);
  pSrcX++;
  pSrcY++;

  for (SLArrayIndex_t i = 1; i < sampleLength; i++) {
    SumX += SDS_Log(*pSrcX);
    SumY += SDS_Log(*pSrcY);
    pSrcX++;
    pSrcY++;
  }

  pSrcX -= sampleLength;
  pSrcY -= sampleLength;

  return ((SumY - (SDA_PowraRegressionCoeff(pSrcX, pSrcY, sampleLength) * SumX)) / sampleLength);
}    // End of SDA_PowraConstantCoeff()

/********************************************************
 * Function: SDA_PowraRegressionCoeff
 *
 * Parameters:
 *      const SLData_t * SIGLIB_PTR_DECL pSrc1 - Buffer X data pointer
 *      const SLData_t * SIGLIB_PTR_DECL pSrc2 - Buffer Y data pointer
 *      const SLArrayIndex_t sampleLength - Buffer length
 *
 * Return value: Regression coefficient.
 *
 * Description: The regression coefficient for the
 *  supplied data.
 *
 *  Assuming the data can be modeled according to:
 *
 *      y = C.x^M
 *
 *  Gives:
 *
 *      n.sum(x.ln(y)) - sum(ln(x)) . sum(ln(y))
 *  M = ----------------------------------------
 *          n.sum(ln(x)^2) - (sum(ln(x)))^2
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_PowraRegressionCoeff(const SLData_t* SIGLIB_PTR_DECL pSrcX, const SLData_t* SIGLIB_PTR_DECL pSrcY,
                                                   const SLArrayIndex_t sampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__              // Defined by TI compiler
  _nassert((int)pSrcX % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pSrcY % 8 == 0);
#  endif
#endif

  SLData_t SumXY = SDS_Log(*pSrcX) * SDS_Log(*pSrcY);
  SLData_t SumX = SDS_Log(*pSrcX);
  SLData_t SumY = SDS_Log(*pSrcY);
  SLData_t SumXSquared = SDS_Log(*pSrcX) * SDS_Log(*pSrcX);
  pSrcX++;
  pSrcY++;

  for (SLArrayIndex_t i = 1; i < sampleLength; i++) {
    SumXY += SDS_Log(*pSrcX) * SDS_Log(*pSrcY);
    SumX += SDS_Log(*pSrcX);
    SumY += SDS_Log(*pSrcY);
    SumXSquared += SDS_Log(*pSrcX) * SDS_Log(*pSrcX);
    pSrcX++;
    pSrcY++;
  }

  return (((sampleLength * SumXY) - (SumX * SumY)) / ((sampleLength * SumXSquared) - (SumX * SumX)));
}    // End of SDA_PowraRegressionCoeff()

/********************************************************
 * Function: SDA_PowraCorrelationCoeff
 *
 * Parameters:
 *      const SLData_t * SIGLIB_PTR_DECL pSrc1 - Buffer X data pointer
 *      const SLData_t * SIGLIB_PTR_DECL pSrc2 - Buffer Y data pointer
 *      const SLArrayIndex_t sampleLength - Buffer length
 *
 * Return value: Correlation coefficient.
 *
 * Description: The correlation coefficient for the
 *  supplied data.
 *
 *  Assuming the data can be modeled according to:
 *
 *      y = C.x^M
 *
 *  Gives:
 *
 *                      n.sum(ln(x).ln(y)) - sum(ln(x)) . sum(ln(y))
 *  r =
 *--------------------------------------------------------------------------
 *      sqrt ((n.sum(ln(x)^2) - (sum(ln(x)))^2).(n.sum(ln(y)^2) -
 *(sum(ln(y)))^2))
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_PowraCorrelationCoeff(const SLData_t* SIGLIB_PTR_DECL pSrcX, const SLData_t* SIGLIB_PTR_DECL pSrcY,
                                                    const SLArrayIndex_t sampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__              // Defined by TI compiler
  _nassert((int)pSrcX % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pSrcY % 8 == 0);
#  endif
#endif

  SLData_t SumXY = SDS_Log(*pSrcX) * SDS_Log(*pSrcY);
  SLData_t SumX = SDS_Log(*pSrcX);
  SLData_t SumY = SDS_Log(*pSrcY);
  SLData_t SumXSquared = SDS_Log(*pSrcX) * SDS_Log(*pSrcX);
  SLData_t SumYSquared = SDS_Log(*pSrcY) * SDS_Log(*pSrcY);
  pSrcX++;
  pSrcY++;

  for (SLArrayIndex_t i = 1; i < sampleLength; i++) {
    SumXY += SDS_Log(*pSrcX) * SDS_Log(*pSrcY);
    SumX += SDS_Log(*pSrcX);
    SumY += SDS_Log(*pSrcY);
    SumXSquared += SDS_Log(*pSrcX) * SDS_Log(*pSrcX);
    SumYSquared += SDS_Log(*pSrcY) * SDS_Log(*pSrcY);
    pSrcX++;
    pSrcY++;
  }

  return (((sampleLength * SumXY) - (SumX * SumY)) /
          SDS_Sqrt(((sampleLength * SumXSquared) - (SumX * SumX)) * ((sampleLength * SumYSquared) - (SumY * SumY))));
}    // End of SDA_PowraRegressionCoeff()

/********************************************************
 * Function: SDA_PowraEstimateX
 *
 * Parameters:
 *      const SLData_t * SIGLIB_PTR_DECL pSrc1 - Buffer X data pointer
 *      const SLData_t * SIGLIB_PTR_DECL pSrc2 - Buffer Y data pointer
 *      const SLData_t YVal  - Y value
 *      const SLArrayIndex_t sampleLength - Buffer length
 *
 * Return value: Estimate X
 *
 * Description: Estimate a value for X, given a value
 *  for Y.
 *
 *  Assuming the data can be modeled according to:
 *
 *      y = C.x^M
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_PowraEstimateX(const SLData_t* SIGLIB_PTR_DECL pSrcX, const SLData_t* SIGLIB_PTR_DECL pSrcY, const SLData_t YVal,
                                             const SLArrayIndex_t sampleLength)
{
  return (SDS_Exp((SDS_Log(YVal) - SDA_PowraConstantCoeff(pSrcX, pSrcY, sampleLength)) / SDA_PowraRegressionCoeff(pSrcX, pSrcY, sampleLength)));
}    // End of SDA_PowraEstimateX()

/********************************************************
 * Function: SDA_PowraEstimateY
 *
 * Parameters:
 *      const SLData_t * SIGLIB_PTR_DECL pSrc1 - Buffer X data pointer
 *      const SLData_t * SIGLIB_PTR_DECL pSrc2 - Buffer Y data pointer
 *      const SLData_t YVal  - Y value
 *      const SLArrayIndex_t sampleLength - Buffer length
 *
 * Return value: Estimate Y
 *
 * Description: Estimate a value for Y, given a value
 *  for X.
 *
 *  Assuming the data can be modeled according to:
 *
 *      y = C.x^M
 *
 ********************************************************/

SLData_t SIGLIB_FUNC_DECL SDA_PowraEstimateY(const SLData_t* SIGLIB_PTR_DECL pSrcX, const SLData_t* SIGLIB_PTR_DECL pSrcY, const SLData_t XVal,
                                             const SLArrayIndex_t sampleLength)
{
  return (SDS_Exp((SDS_Log(XVal) * SDA_PowraRegressionCoeff(pSrcX, pSrcY, sampleLength)) + SDA_PowraConstantCoeff(pSrcX, pSrcY, sampleLength)));
}    // End of SDA_PowraEstimateY()

/********************************************************
 * Function: SDA_Detrend
 *
 * Parameters:
 *  const SLData_t * SIGLIB_PTR_DECL pSrc,  Source data pointer
 *  SLData_t * SIGLIB_PTR_DECL pDst,        Destination data pointer
 *  SLData_t * SIGLIB_PTR_DECL pRamp,       Ramp pointer
 *  const SLArrayIndex_t sampleLength       Sample length
 *
 * Return value:
 *  None
 *
 * Description:
 *  Uses y = M.x + C to generate the best straight-line
 *  fit to the data in the source array, this is then
 *  removed from the data before writing the results to
 *  the destination array.
 *
 *  The first iteration of this function and any where
 *  the vector length changes will take longer than
 *  subsequent iterations because the reference vector
 *  needs to be allocated. If execution time is
 *  important then this function can be called during
 *  the application initialisation process to
 *  initialise the state.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_Detrend(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst, SLData_t* SIGLIB_PTR_DECL pRamp,
                                  const SLArrayIndex_t sampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDst % 8 == 0);
  _nassert((int)pRamp % 8 == 0);
#  endif
#endif

  const SLData_t* pSrcStart = pSrc;

  for (SLArrayIndex_t i = 0; i < sampleLength; i++) {    // Fill index array with ramp
    *(pRamp + i) = (SLData_t)i;
  }

  // Calculate constant and regression coefficient
  SLData_t M = SDA_LinraRegressionCoeff(pRamp, pSrcStart, sampleLength);
  SLData_t C = SDA_LinraConstantCoeff(pRamp, pSrcStart, sampleLength);

  // Detrend the signal by removing the estimate from the original
  for (SLArrayIndex_t i = 0; i < sampleLength; i++) {
    *pDst++ = *pSrc++ - (M * ((SLData_t)i) + C);
  }
}    // End of SDA_Detrend()

/********************************************************
 * Function: SDA_ExtractTrend
 *
 * Parameters:
 *  const SLData_t * SIGLIB_PTR_DECL pSrc,  Source data pointer
 *  SLData_t * SIGLIB_PTR_DECL pDst,        Destination data pointer
 *  SLData_t * SIGLIB_PTR_DECL pRamp,       Ramp pointer
 *  const SLArrayIndex_t sampleLength       Sample length
 *
 * Return value:
 *  None
 *
 * Description:
 *  Uses y = M.x + C to generate the best straight-line
 *  fit to the data in the source array, this is then
 *  written to the destination array.
 *
 *  The first iteration of this function and any where
 *  the vector length changes will take longer than
 *  subsequent iterations because the reference vector
 *  needs to be allocated. If execution time is
 *  important then this function can be called during
 *  the application initialisation process to
 *  initialise the state.
 *
 ********************************************************/

void SIGLIB_FUNC_DECL SDA_ExtractTrend(const SLData_t* SIGLIB_PTR_DECL pSrc, SLData_t* SIGLIB_PTR_DECL pDst, SLData_t* SIGLIB_PTR_DECL pRamp,
                                       const SLArrayIndex_t sampleLength)
{
#if (SIGLIB_ARRAYS_ALIGNED)
#  ifdef __TMS320C6X__             // Defined by TI compiler
  _nassert((int)pSrc % 8 == 0);    // Align arrays on 64 bit double word boundary for LDDW
  _nassert((int)pDst % 8 == 0);
  _nassert((int)pRamp % 8 == 0);
#  endif
#endif

  const SLData_t* pSrcStart = pSrc;

  for (SLArrayIndex_t i = 0; i < sampleLength; i++) {    // Fill index array with ramp
    *(pRamp + i) = (SLData_t)i;
  }

  // Calculate constant and regression coefficient
  SLData_t M = SDA_LinraRegressionCoeff(pRamp, pSrcStart, sampleLength);
  SLData_t C = SDA_LinraConstantCoeff(pRamp, pSrcStart, sampleLength);

  // Write the trend line of the original
  for (SLArrayIndex_t i = 0; i < sampleLength; i++) {
    *pDst++ = M * ((SLData_t)i) + C;
  }
}    // End of SDA_ExtractTrend()
