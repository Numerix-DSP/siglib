// STChild.CPP: Child window member functions for SigLib DLL test program
// Copyright (C) Alpha Numerix 1995. All rights reserved.

/* Include files */
#include <afxwin.h>
#include "sltest.h"

/* Declare global variables and arrays */
extern CBuffer          Signal, FreqR, FreqI;
extern CSLTestApp       SLTestApp;
extern CSLTestWindow    *MainWindowPtr;

// CWinGraph functions:

CWinGraph::CWinGraph()
{
    HGCursor = SLTestApp.LoadCursor ("GraphCursor");
}


afx_msg int CWinGraph::OnCreate (LPCREATESTRUCT lpCreateStruct)
{
    LPCREATESTRUCT  dummy;

    dummy = lpCreateStruct;     //Dummy for compilation

    HDOldDataMem = ::LocalAlloc (LMEM_MOVEABLE | LMEM_ZEROINIT, ((UINT)(512 * sizeof (int))));

    Plot1Pen.CreatePen (PS_SOLID, 1, RGB_YELLOW);
    ErasePen.CreatePen (PS_SOLID, 1, RGB_BLACK);
    GraticulePen.CreatePen (PS_SOLID, 1, RGB_WHITE);

    FirstGraph = 1;

    return (0);
}



afx_msg void CWinGraph::OnClose()
{

}


afx_msg void CWinGraph::OnExit()
{
    ::LocalFree (HDOldDataMem);         // Free old data array memory
    DestroyWindow();
}



afx_msg void CWinGraph::OnLButtonDown (UINT nFlags,
                    CPoint point)
{
    UINT    dummy1;
    CPoint  dummy2;

    dummy1 = nFlags;        // Dummy use for compilation
    dummy2 = point;

    return;
}

afx_msg void CWinGraph::OnPaint()
{
    CPaintDC        PaintDC (this);
    CClientDC       ClientDC (this);

    RECT            Rect;
    UINT            i;
    double          *BufferPtr;
    double          Max;
    UINT            BufferSize;
    HANDLE          HBuffer;

    UINT            Height, Width, YZero;
    int             *OldDataBufferPtr;
    int             YPos;
    double          XStep, YStep;


    GetClientRect (&Rect);

    if (FirstGraph)
    {
        PaintDC.SelectStockObject (BLACK_BRUSH);        // Black background
        PaintDC.Rectangle (&Rect);
    }


    Height = Rect.bottom - Rect.top;
    Width = Rect.right - Rect.left;

                        // If signal array contains data, draw graph
    if ((BufferSize = Signal.GetSize()) != 0)
    {
        Max = Signal.GetMax();                          // Get the maximum value
        HBuffer = Signal.GetHandle();

        BufferPtr = (double *)::LocalLock (HBuffer);
        OldDataBufferPtr = (int *)::LocalLock (HDOldDataMem);

        XStep = ((double)Width) / ((double)(BufferSize-1));
        YStep = ((double)Height) / ((double)2.0);
        YZero = Rect.bottom / 2;

        if (Max)            // != 0
        {
            YPos = YZero - (int)((*BufferPtr++ * YStep) / Max);
            for (i = 0; i < (BufferSize-1);)    // -1 for i++ in loop
            {                                   // Remove old graph
                if (!FirstGraph)
                {
                    ClientDC.SelectObject (&ErasePen);  // Get the pen to erase the previous line
                    ClientDC.MoveTo ((Rect.left + (UINT)(((double)i) * XStep)), *OldDataBufferPtr);
                    *OldDataBufferPtr++ = YPos;
                    ClientDC.LineTo ((Rect.left + (UINT)(((double)(i+1)) * XStep)), *OldDataBufferPtr);
                }
                else
                    *OldDataBufferPtr++ = YPos;
                                                // Plot new graph
                ClientDC.SelectObject (&Plot1Pen);  // Get the pen to draw the graph
                ClientDC.MoveTo ((Rect.left + (UINT)(((double)i++) * XStep)), YPos);
                YPos = YZero - (int)((*BufferPtr++ * YStep) / Max);
                ClientDC.LineTo ((Rect.left + (UINT)(((double)i) * XStep)), YPos);
            }
        }
        else                                    // Data is all zero
        {
            ClientDC.MoveTo (Rect.left, YZero);
            ClientDC.LineTo (Rect.right, YZero);
        }

        ::LocalUnlock (HBuffer);            // Unlock the sample array
        ::LocalUnlock (HDOldDataMem);       // Unlock the old graph data array
    }


    ClientDC.SelectObject (&GraticulePen);  // Get the pen to draw the axes
    for (i = 1; i < 10; i++)
    {
        ClientDC.MoveTo (Rect.left, Rect.top + (i * Height / 10));
        ClientDC.LineTo (Rect.right, Rect.top + (i * Height / 10));

        ClientDC.MoveTo (Rect.left + (i * Width / 10), Rect.top);
        ClientDC.LineTo (Rect.left + (i * Width / 10), Rect.bottom);
    }
    FirstGraph = 0;
}



// On mouse move

afx_msg void CWinGraph::OnMouseMove (UINT nFlags, CPoint point)
{
    UINT    dummy1;
    CPoint  dummy2;

    dummy1 = nFlags;        // Dummy use for compilation
    dummy2 = point;

    SetCursor (HGCursor);
    return;
}



// Erase Background - Issued when window revealed

afx_msg BOOL CWinGraph::OnEraseBkgnd (CDC *pDC)
{
    CDC     *dummy;         // For compilation warning level 4
    dummy = pDC;

    FirstGraph = 1;         // Redraw the graticules etc

    return (0);
}



// Resize

afx_msg void CWinGraph::OnSize (UINT nType, int cx, int cy)
{
    UINT    dummy;
    int     dummy1;

    dummy = nType;          // Dummy use for compilation
    dummy1 = cx;
    dummy1 = cy;

    FirstGraph = 1;         // Redraw the graticules etc
}


void CWinGraph::SetGraph (int S)
{
    Status = S;
    Invalidate (FALSE);
    UpdateWindow();
}

BEGIN_MESSAGE_MAP (CWinGraph, CWnd)
    ON_WM_LBUTTONDOWN()
    ON_WM_PAINT()
    ON_WM_CREATE()
    ON_WM_CLOSE()
    ON_WM_MOUSEMOVE()
    ON_WM_SIZE()
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CWinStatus functions:

CWinStatus::CWinStatus()
{
    Buffer [0] = '\0';
    Buffer [sizeof (Buffer) - 1] = '\0';
}

afx_msg void CWinStatus::OnPaint()
{
    CPaintDC PaintDC    (this);
    RECT            Rect;

    GetClientRect (&Rect);
    Rect.top = Rect.bottom - 20;

                    // Gray background & black border:
    PaintDC.SelectStockObject (LTGRAY_BRUSH);
    PaintDC.Rectangle (&Rect);

                        // Draw white line at top:
    PaintDC.SelectStockObject (WHITE_PEN);
    PaintDC.MoveTo (Rect.left + 1, Rect.top + 1);
    PaintDC.LineTo (Rect.right - 1, Rect.top + 1);

                        // Draw text:
    PaintDC.SetBkMode (TRANSPARENT);
    PaintDC.DrawText (Buffer,
            -1,
            &Rect,
            DT_CENTER | DT_SINGLELINE | DT_VCENTER);
}

void CWinStatus::SetMessage (const char *Message)
{
    strncpy (Buffer,Message,sizeof (Buffer)-1);
    Invalidate (FALSE);
    UpdateWindow();
}

BEGIN_MESSAGE_MAP (CWinStatus, CWnd)
    ON_WM_PAINT()
END_MESSAGE_MAP()
