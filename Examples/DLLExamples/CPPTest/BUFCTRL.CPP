// BufCtrl.CPP for SigLib DLL Test program
// Buffer handling class, based on CWnd
// Copyright (c) 2023 Alpha Numerix All rights reserved.

// Include files
#include <afxwin.h>
#include "sltest.h"

CBuffer::CBuffer()
{

}

void CBuffer::Create()
{
    BufSize = 0;            // Init array size to 0
}

HANDLE CBuffer::GetHandle() const
{
    return (HBuf);
}

void CBuffer::SetHandle(HANDLE HBuffer)
{
    HBuf = HBuffer;
}

void CBuffer::SetSize (UINT Size)
{
    BufSize = Size;
}

UINT CBuffer::GetSize()
{
    return (BufSize);
}


double CBuffer::GetMax()
{
    UINT        i;
    double      Max;
    double      *PtrDMem;       // Ptr to double type data array


    PtrDMem = (double *)::LocalLock (HBuf);

    if (BufSize > 0) {
        Max = *PtrDMem++;

        for (i = 1; i < BufSize; i++) {
            if (*PtrDMem++ > Max) {
                Max = *(PtrDMem -1);
            }
        }

        ::LocalUnlock (HBuf);           // Free the Sample array

        return (Max);
        }
    else {
        return (0.0);
    }

}       /* End of find_max() */



void CBuffer::PutSample(int nIndex, double sample) const
{
    int dummy;
    double  dummy2;

    dummy = (int) nIndex;
    dummy2 = sample;
}

double CBuffer::GetSample(int nIndex) const
{
    int dummy;

    dummy = (int) nIndex;
    return (0);
}


                // Clipboard operations
BOOL CBuffer::Undo()
{ return (BOOL)::SendMessage(m_hWnd, EM_UNDO, 0, 0); }

void CBuffer::Clear()
{ ::SendMessage(m_hWnd, WM_CLEAR, 0, 0); }

void CBuffer::Copy()
{ ::SendMessage(m_hWnd, WM_COPY, 0, 0); }

void CBuffer::Cut()
{ ::SendMessage(m_hWnd, WM_CUT, 0, 0); }

void CBuffer::Paste()
{ ::SendMessage(m_hWnd, WM_PASTE, 0, 0); }


