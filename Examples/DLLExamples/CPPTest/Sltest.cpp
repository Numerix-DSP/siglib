// SLTest.CPP SigLib DLL Test program
// Copyright (c) 2023 Alpha Numerix All rights reserved.

// Include files
#include <afxwin.h>
#include <afxdlgs.h>
#include <shellapi.h>
#include "sltest.h"
#include "siglib.h"

// Define constants
#define     DEBUG           0

//Declare the data arrays
CBuffer         Signal, FreqR, FreqI, Coeff;
//Declare a pointer to the main window
CSLTestWindow  *MainWindowPtr;
//Declare an instance of the derived application class
CSLTestApp      SLTestApp;

//Implement the CSLTestWindow member functions

CSLTestWindow::CSLTestWindow ()
{
  const char     *ClassName;
  char            StatusMessage[80];
  HLOCAL          HDSignalLocalMem, HDFreqRLocalMem, HDFreqILocalMem, HDCoeffLocalMem, HDResultLocalMem;  // Double type data array

  LoadAccelTable ("SLTestAccel");
  HGCursor = SLTestApp.LoadCursor ("GraphCursor");

  ClassName = AfxRegisterWndClass (CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW,
                                   SLTestApp.LoadStandardCursor (IDC_ARROW),
                                   (HBRUSH)::GetStockObject (WHITE_BRUSH), SLTestApp.LoadIcon (AFX_IDI_STD_FRAME));

  Create (ClassName, "SLTest", WS_OVERLAPPEDWINDOW, rectDefault, NULL, "SLTestMenu");

// Initialise memory

  if (Signal.GetSize () == 0) {
    HDSignalLocalMem =::LocalAlloc (LMEM_MOVEABLE | LMEM_ZEROINIT, ((UINT) (FFT_LENGTH * sizeof (SLData_t))));
    HDFreqRLocalMem =::LocalAlloc (LMEM_MOVEABLE | LMEM_ZEROINIT, ((UINT) (FFT_LENGTH * sizeof (SLData_t))));
    HDFreqILocalMem =::LocalAlloc (LMEM_MOVEABLE | LMEM_ZEROINIT, ((UINT) (FFT_LENGTH * sizeof (SLData_t))));
    HDResultLocalMem =::LocalAlloc (LMEM_MOVEABLE | LMEM_ZEROINIT, ((UINT) (FFT_LENGTH * sizeof (SLData_t))));
    HDCoeffLocalMem =::LocalAlloc (LMEM_MOVEABLE | LMEM_ZEROINIT, ((UINT) (((FFT_LENGTH * 3) / 4) * sizeof (SLData_t))));
  }
  else {
    HDSignalLocalMem =::LocalReAlloc (Signal.GetHandle (), ((UINT) (FFT_LENGTH * sizeof (SLData_t))), LMEM_MOVEABLE);
    HDFreqRLocalMem =::LocalReAlloc (FreqR.GetHandle (), ((UINT) (FFT_LENGTH * sizeof (SLData_t))), LMEM_MOVEABLE);
    HDFreqILocalMem =::LocalReAlloc (FreqI.GetHandle (), ((UINT) (FFT_LENGTH * sizeof (SLData_t))), LMEM_MOVEABLE);
    HDCoeffLocalMem =::LocalReAlloc (Coeff.GetHandle (), ((UINT) (((FFT_LENGTH * 3) / 4) * sizeof (SLData_t))), LMEM_MOVEABLE);
  }

  Signal.SetHandle (HDSignalLocalMem);                              // Set memory handle
  FreqR.SetHandle (HDFreqRLocalMem);
  FreqI.SetHandle (HDFreqILocalMem);
  Coeff.SetHandle (HDCoeffLocalMem);

  Signal.SetSize (FFT_LENGTH);                                      // Set size
  FreqR.SetSize (FFT_LENGTH);
  FreqI.SetSize (FFT_LENGTH);
  Coeff.SetSize (FFT_LENGTH);

  sprintf (StatusMessage, "Buffer Size : %d samples, Max : %lf", Signal.GetSize (), Signal.GetMax ());
  WinStatus.SetMessage (StatusMessage);
}


// Instance initialisation function

BOOL CSLTestApp::InitInstance (
  )
{
  MainWindowPtr = new CSLTestWindow ();
  m_pMainWnd = MainWindowPtr;
  m_pMainWnd->ShowWindow (m_nCmdShow);

  return TRUE;
}


// Idle task uses the lCount argument to prioritize the task
// Note the call to the base-class version of OnIdle.

BOOL CSLTestApp::OnIdle (
  LONG lCount)
{
  CWinApp::OnIdle (lCount);
  return TRUE;
}



// Update the window title

void CSLTestWindow::UpdateTitle (
  )
{
  CString Title;

  Title = "SLTest";
  SetWindowText (Title);
}




// Exit

afx_msg void CSLTestWindow::OnExit (
  )
{
  ::LocalFree (Signal.GetHandle ());
  ::LocalFree (FreqR.GetHandle ());
  ::LocalFree (FreqI.GetHandle ());
  ::LocalFree (Coeff.GetHandle ());

  DestroyWindow ();
}



// Redefined MFC functions
// Update the screen

afx_msg void CSLTestWindow::OnPaint (
  )
{
  CString String1 = " Fsd";
  CString String2 = "   0";
  CString String3 = "-Fsd";

  CPaintDC PaintDC (
  this);                                                            // 'this' is a keyword, that supplies
// a pointer to the address of the
// current object.
  CRect Rect;

  PaintDC.SetBkColor (RGB_BRIGHTWHITE);                             // Grey background

  PaintDC.SetTextColor (RGB_LIGHTBLUE);                             // Blue text

  GetClientRect (Rect);

  PaintDC.TextOut ((Rect.right * 3) / 32, ((Rect.bottom / 20) - 10), String1, String1.GetLength ());
// Instead of 10, use char height / 2

  PaintDC.TextOut ((Rect.right * 3) / 32, ((Rect.bottom * 3 / 8) + (Rect.bottom / 20) - 10), String2, String2.GetLength ());

  PaintDC.TextOut ((Rect.right * 3) / 32, ((Rect.bottom * 3 / 4) + (Rect.bottom / 20) - 10), String3, String3.GetLength ());
}


// Create the winspect window

afx_msg int CSLTestWindow::OnCreate (
  LPCREATESTRUCT lpCreateStruct)
{
  LPCREATESTRUCT dummy;

  CClientDC ClientDC (
  this);
  RECT Rect = { 0, 0, 0, 0 };
  TEXTMETRIC TM;


  dummy = lpCreateStruct;                                           // Dummy use for compilation

  ClientDC.GetTextMetrics (&TM);
  CharHeight = TM.tmHeight + TM.tmExternalLeading;

  WinGraph.Create (NULL, NULL, WS_CHILD | WS_VISIBLE, Rect, this, NULL);

  WinStatus.Create (NULL, NULL, WS_CHILD | WS_VISIBLE, Rect, this, NULL);

  Signal.Create ();
  FreqR.Create ();
  FreqI.Create ();
  Coeff.Create ();

  WinStatus.SetMessage ("SigLib Test Status : 0");

  return (TRUE);
}


// Resize

afx_msg void CSLTestWindow::OnSize (
  UINT nType,
  int cx,
  int cy)
{
  UINT dummy;
  RECT Rect;

  dummy = nType;                                                    // Dummy use for compilation

// Set graph window size
  Rect.top = cy / 20;
  Rect.bottom = ((cy * 3) / 4) + cy / 20;

  Rect.left = cx / 4 - cx / 27;
  Rect.right = (cx * 26) / 27;

  WinGraph.MoveWindow (&Rect);

// Set status window size
  Rect.left = 0;
  Rect.top = cy - 5 * CharHeight / 4;
  Rect.right = cx;
  Rect.bottom = cy;

  WinStatus.MoveWindow (&Rect);
}


// Contents dialog box

afx_msg void CSLTestWindow::OnContents (
  )
{
  CModalDialog Contents (
  "ContentsBox",
  this);
  Contents.DoModal ();
}



// About dialog box

afx_msg void CSLTestWindow::OnAbout (
  )
{
  CModalDialog About (
  "AboutBox",
  this);
  About.DoModal ();
}



// Redraw on system colour change

afx_msg void CSLTestWindow::OnSysColorChange (
  )
{
  Invalidate (TRUE);
  UpdateWindow ();
}



// Generate sine wave

afx_msg void CSLTestWindow::OnGenerateSine (
  )
{
  SLData_t SignalPhase;
  SLData_t       *BufferPtr;
  HANDLE HBuffer;
  char StatusMessage[80];

  SignalPhase = 0.0;
  HBuffer = Signal.GetHandle ();
  BufferPtr = (SLData_t *)::LocalLock (HBuffer);

  SDA_SignalGenerate (BufferPtr, SIGLIB_SINE_WAVE, (SLData_t) 0.9,
                      SIGLIB_FILL, (SLData_t) 0.01562, SIGLIB_ZERO, SIGLIB_ZERO, SIGLIB_ZERO, &SignalPhase, SIGLIB_NULL_DATA_PTR, Signal.GetSize ());
  ::LocalUnlock (HBuffer);                                          // Unlock the sample array

  WinGraph.SendMessage (WM_PAINT);                                  // Only update graph, not surround

  sprintf (StatusMessage, "Buffer Size : %d samples, Max : %lf", Signal.GetSize (), Signal.GetMax ());
  WinStatus.SetMessage (StatusMessage);
}



// Generate triangle wave

afx_msg void CSLTestWindow::OnGenerateTriangle (
  )
{
  SLData_t SignalPhase;
  SLData_t       *BufferPtr;
  HANDLE HBuffer;
  char StatusMessage[80];

  SignalPhase = 0.0;
  HBuffer = Signal.GetHandle ();
  BufferPtr = (SLData_t *)::LocalLock (HBuffer);
  SDA_SignalGenerate (BufferPtr, SIGLIB_TRIANGLE_WAVE, (SLData_t) 0.9,
                      SIGLIB_FILL, (SLData_t) 0.01562, SIGLIB_ZERO, SIGLIB_ZERO, SIGLIB_ZERO, &SignalPhase, SIGLIB_NULL_DATA_PTR, Signal.GetSize ());
  ::LocalUnlock (HBuffer);                                          // Unlock the sample array

  WinGraph.SendMessage (WM_PAINT);                                  // Only update graph, not surround

  sprintf (StatusMessage, "Buffer Size : %d samples, Max : %lf", Signal.GetSize (), Signal.GetMax ());
  WinStatus.SetMessage (StatusMessage);
}



// Generate square wave

afx_msg void CSLTestWindow::OnGenerateSquare (
  )
{
  SLData_t SignalPhase;
  SLData_t       *BufferPtr;
  HANDLE HBuffer;
  char StatusMessage[80];

  SignalPhase = 0.0;
  HBuffer = Signal.GetHandle ();
  BufferPtr = (SLData_t *)::LocalLock (HBuffer);

  SDA_SignalGenerate (BufferPtr, SIGLIB_SQUARE_WAVE, (SLData_t) 0.9,
                      SIGLIB_FILL, (SLData_t) 0.01562, SIGLIB_ZERO, SIGLIB_HALF, SIGLIB_ZERO, &SignalPhase, SIGLIB_NULL_DATA_PTR, Signal.GetSize ());
  ::LocalUnlock (HBuffer);                                          // Unlock the sample array

  WinGraph.SendMessage (WM_PAINT);                                  // Only update graph, not surround

  sprintf (StatusMessage, "Buffer Size : %d samples, Max : %lf", Signal.GetSize (), Signal.GetMax ());
  WinStatus.SetMessage (StatusMessage);
}



// Get FFT of signal

afx_msg void CSLTestWindow::OnFFTSignal (
  )
{
  static int FirstTime = TRUE;
  SLData_t       *SignalBufferPtr, *FreqRBufferPtr, *FreqIBufferPtr, *CoeffBufferPtr;
  HANDLE HSignalBuffer, HFreqRBuffer, HFreqIBuffer, HCoeffBuffer;
  char StatusMessage[80];

  HSignalBuffer = Signal.GetHandle ();
  HFreqRBuffer = FreqR.GetHandle ();
  HFreqIBuffer = FreqI.GetHandle ();
  HCoeffBuffer = Coeff.GetHandle ();

  SignalBufferPtr = (SLData_t *)::LocalLock (HSignalBuffer);
  FreqRBufferPtr = (SLData_t *)::LocalLock (HFreqRBuffer);
  FreqIBufferPtr = (SLData_t *)::LocalLock (HFreqIBuffer);
  CoeffBufferPtr = (SLData_t *)::LocalLock (HCoeffBuffer);

  if (FirstTime) {
    SIF_Fft (CoeffBufferPtr, SIGLIB_NULL_ARRAY_INDEX_PTR, Signal.GetSize ());
    FirstTime = 0;
  }

  SDA_Copy (SignalBufferPtr, FreqRBufferPtr, (int) Signal.GetSize ());
  SDA_Rfft (FreqRBufferPtr, FreqIBufferPtr, CoeffBufferPtr, SIGLIB_NULL_ARRAY_INDEX_PTR, (int) Signal.GetSize (), ((SLArrayIndex_t) (SDS_Log2 (FFT_LENGTH) + SIGLIB_MIN_THRESHOLD)) // Log2 FFT length, and avoid quantization issues);            /* Perform FFT */
            SDA_Magnitude (FreqRBufferPtr, FreqIBufferPtr, SignalBufferPtr, Signal.GetSize ()); /* Calc real power fm complex */
            ::LocalUnlock (HSignalBuffer);                          // Unlock the sample arrays
            ::LocalUnlock (HFreqRBuffer);::LocalUnlock (HFreqIBuffer);::LocalUnlock (HCoeffBuffer); WinGraph.SendMessage (WM_PAINT);  // Only update graph, not surround
            sprintf (StatusMessage, "Buffer Size : %d samples, Max : %lf", Signal.GetSize (), Signal.GetMax ());
            WinStatus.SetMessage (StatusMessage);}




// Get SigLib DLL Version

            afx_msg void CSLTestWindow::OnGetSigLibDLLVersion () {
            char StatusMessage[40];
            sprintf (StatusMessage, "SigLib DLL Version : %lf", SUF_SiglibVersion ());
            MessageBox (StatusMessage, "SigLib DLL Version", MB_OK | MB_ICONEXCLAMATION);}




            HCURSOR CSLTestWindow::GetHCursor () {

            return (HGCursor);}



// Message map

            BEGIN_MESSAGE_MAP (CSLTestWindow, CFrameWnd)
// User defined functions
            ON_COMMAND (IDM_GENERATE_SINE, OnGenerateSine)
            ON_COMMAND (IDM_GENERATE_TRIANGLE, OnGenerateTriangle)
            ON_COMMAND (IDM_GENERATE_SQUARE, OnGenerateSquare)
            ON_COMMAND (IDM_FFT_SIGNAL, OnFFTSignal)
            ON_COMMAND (IDM_GETSIGLIBVERSION, OnGetSigLibDLLVersion)
            ON_COMMAND (IDM_EXIT, OnExit)
            ON_COMMAND (IDM_ABOUT, OnAbout)
            ON_COMMAND (IDM_CONTENTS, OnContents)
// Redefined MFC functions
            ON_WM_PAINT ()
            ON_WM_CREATE ()
            ON_WM_SIZE ()
            ON_WM_SYSCOLORCHANGE ()
            END_MESSAGE_MAP ()
