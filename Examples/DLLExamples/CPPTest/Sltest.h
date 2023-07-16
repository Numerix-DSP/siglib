// SLTEST.H Header file for SigLib DLL test project

// Copyright (C) Delta Numerix 2000. All rights reserved.

// Symbolic definition of constants

// Define application constants

#define FFT_LENGTH              512
#define LOG2_FFT_LENGTH         SAI_FftLengthLog2(FFT_LENGTH)       // Log2 FFT length,


// Menu constants
// File menu

#define IDM_EXIT                100


// Options menu

#define IDM_GENERATE_SINE       200
#define IDM_GENERATE_TRIANGLE   220
#define IDM_GENERATE_SQUARE     230
#define IDM_FFT_SIGNAL          240
#define IDM_GETSIGLIBVERSION    250


// Help menu

#define IDM_CONTENTS            300
#define IDM_ABOUT               310


// User defined messages

#define WM_DUMMY                (WM_USER + 100)


// Window colours

#define RGB_BLACK               RGB (0, 0, 0)
#define RGB_BLUE                RGB (0, 0, 128)
#define RGB_GREEN               RGB (0, 128, 0)
#define RGB_CYAN                RGB (0, 128, 128)
#define RGB_RED                 RGB (128, 0, 0)
#define RGB_MAGENTA             RGB (128, 0, 128)
#define RGB_BROWN               RGB (128, 128, 0)
#define RGB_GRAY                RGB (128, 128, 128)
#define RGB_WHITE               RGB (172, 172, 172)
#define RGB_LIGHTBLUE           RGB (0, 0, 255)
#define RGB_LIGHTGREEN          RGB (0, 255, 0)
#define RGB_LIGHTCYAN           RGB (0, 255, 255)
#define RGB_LIGHTRED            RGB (255, 0, 0)
#define RGB_LIGHTMAGENTA        RGB (255, 0, 255)
#define RGB_YELLOW              RGB (255, 255, 0)
#define RGB_BRIGHTWHITE         RGB (255, 255, 255)



// SLTestApp class definitions

class           CSLTestApp:public CWinApp {
public:
  virtual BOOL InitInstance (
    );
  BOOL            OnIdle (
  LONG lCount);
};



// Status window

class           CWinStatus:public CWnd {
public:
  CWinStatus (
    );
  void            SetMessage (
  const char *Message);
  afx_msg void    OnPaint (
    );
                  DECLARE_MESSAGE_MAP (
    )
private:
  char            Buffer[80];
};



// Buffer handling class

class           CBuffer:public CWnd                                 // Derive from CWnd for windows fuctions eg clipboard
{
public:
  CBuffer (
    );
  void            Create (
    );
  HANDLE          GetHandle (
    ) const;
  void            SetHandle (
  HANDLE hBuffer);
  void            PutSample (
  int nIndex,
  double sample) const;
  double          GetSample (
  int nIndex) const;
  void            SetSize (
  UINT size);
  UINT            GetSize (
    );
  double          GetMax (
    );

// Clipboard operations
  BOOL            Undo (
    );
  void            Clear (
    );
  void            Copy (
    );
  void            Cut (
    );
  void            Paste (
    );

private:
                  UINT BufSize;
  HANDLE          HBuf;
};



// Graph window

class           CWinGraph:public CWnd {
public:
  CWinGraph (
    );
  afx_msg int     OnCreate (
  LPCREATESTRUCT lpCreateStruct);
  afx_msg void    OnClose (
    );
  afx_msg void    OnExit (
    );
  afx_msg void    OnLButtonDown (
  UINT nFlags,
  CPoint point);
  afx_msg void    OnPaint (
    );
  afx_msg void    OnMouseMove (
  UINT nFlags,
  CPoint point);
  afx_msg void    OnSize (
  UINT nType,
  int cx,
  int cy);
  afx_msg BOOL    OnEraseBkgnd (
  CDC * pDC);
  int             Status;                                           // Graph status
  void            SetGraph (
  int S);
                  DECLARE_MESSAGE_MAP (
    )
private:
                  CPen Plot1Pen;
  CPen            ErasePen;
  CPen            GraticulePen;
  HCURSOR         HGCursor;
  HLOCAL          HDOldDataMem;                                     // Int type data array
  int             FirstGraph;
};


// Main window

class           CSLTestWindow:public CFrameWnd {
public:
// User defined functions
  CSLTestWindow (
    );
  HCURSOR         GetHCursor (
    );
  afx_msg void    CSLTestWindow::OnGenerateSine (
    );
  afx_msg void    CSLTestWindow::OnGenerateTriangle (
    );
  afx_msg void    CSLTestWindow::OnGenerateSquare (
    );
  afx_msg void    CSLTestWindow::OnFFTSignal (
    );
  afx_msg void    CSLTestWindow::OnGetSigLibDLLVersion (
    );

// Redefined MFC functions
  afx_msg void    OnExit (
    );
  afx_msg void    OnAbout (
    );
  afx_msg void    OnContents (
    );
  afx_msg void    OnPaint (
    );
  afx_msg int     OnCreate (
  LPCREATESTRUCT lpCreateStruct);
  afx_msg void    OnSize (
  UINT nType,
  int cx,
  int cy);
  afx_msg void    OnSysColorChange (
    );
  afx_msg LONG    OnDropFiles (
  UINT wParam,
  LONG lParam);
                  DECLARE_MESSAGE_MAP (
    )
private:
                  CWinStatus WinStatus;
  CWinGraph       WinGraph;
  CFont           Font;

  UINT            DLLLoaded;
  HCURSOR         HGCursor;
  int             CharHeight;
  void            UpdateTitle (
    );
};
