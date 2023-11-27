// Numerix Graphics Library
// Demo App
// Copyright (c) 2023 Delta Numerix, All rights reserved.

// Include files
// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#if wxUSE_TOOLTIPS
#include "wx/tooltip.h"
#endif

#include "wx/string.h"
#include "wx/file.h"
#include "wx/mimetype.h"
#include <wx/spinctrl.h>
#include <stdlib.h>
#if __APPLE__
#include <malloc/malloc.h>
#else
#include <malloc.h>
#endif

#if defined(__WXGTK__) || defined(__WXMOTIF__) || defined(__WXMAC__) || defined(__WXMGL__) || defined(__WXX11__)
#include "demo.xpm"                                                 // the application icon
#endif

#include <math.h>
#include "ngl.h"
#include "demo.h"
#include "mainframe.h"                                              // mainFrame header - must be located after other project headers

GraphDisplay   *GlobalGraphDisplay;


/*
    Function Name: mainFrame::mainFrame
    Description: Top level frame
    Notes: This should contain the filter designer and graph children
*/

mainFrame::mainFrame (const wxString & Title,
                      const wxPoint & Position,
                      const wxSize & Size):wxFrame (
  (wxFrame *) NULL,
  wxID_ANY,
  Title,
  Position,
  Size,
  wxDEFAULT_FRAME_STYLE)
{
  SetTitle ("Numerix Graphics Library Demonstration");

  SetSize (wxRect (100, 20, 580, 665));                             // X, Y, W, H - used for a resizeable border
  SetAutoLayout (TRUE);                                             // Autolayout the window
#if defined(__WXMSW__)
  SetIcon (wxICON (ApplicationIcon));                               // Select the application icon
#endif

// Create the menubar
  wxMenuBar      *menuBar = new wxMenuBar;
  wxMenu         *fileMenu = new wxMenu;
  wxMenu         *helpMenu = new wxMenu;

  fileMenu->AppendSeparator ();
  fileMenu->Append (DEMO_EXIT, "E&xit");

  helpMenu->Append (DEMO_ABOUT, "&About");

  menuBar->Append (fileMenu, "&File");
  menuBar->Append (helpMenu, "&Help");
  SetMenuBar (menuBar);

  CreateStatusBar (2);
  SetStatusText ("Numerix Graphics Library Demonstration", 0);
//  SetStatusText ("Status bar text 2", 1);
//  SetStatusText ("Status bar text 3", 2);

// Create graph window
//  wxWindow *MyWindow = new wxWindow (this, -1, wxDefaultPosition, GetClientSize());
//  FilterGraphDisplay = new GraphDisplay (MyWindow);
//  GlobalGraphDisplay = FilterGraphDisplay;

  wxBoxSizer     *VSizer = new wxBoxSizer (wxVERTICAL);

  wxPanel        *MyPanel = new wxPanel (this, -1, wxDefaultPosition, wxDefaultSize);
//  wxPanel *MyPanel = new wxPanel (this, -1, wxDefaultPosition, GetClientSize());
  FilterGraphDisplay = new GraphDisplay (MyPanel);
  GlobalGraphDisplay = FilterGraphDisplay;

  VSizer->Add (MyPanel, 0, wxALIGN_CENTER | wxALL, 0);

  SetAutoLayout (TRUE);
  SetSizer (VSizer);

  VSizer->SetSizeHints (this);
  VSizer->Fit (this);


// Allocate and clear memory for graph
  pData = (double *) malloc (GRAPH_WIDTH * sizeof (double));
  for (int i = 0; i < GRAPH_WIDTH; i++) {
    *(pData + i) = 0.0;
  }

// 2 * Pole / Zero
  pPolesAndZeros = (SLComplexRect_s *) malloc (2 * 10 * sizeof (SLComplexRect_s));

  FilterGraphDisplay->SetDataArray (pData);
  FilterGraphDisplay->SetPoleZeroPlotArray (pPolesAndZeros);

}


/*
    Function Name: mainFrame::~mainFrame
    Description: Destructor for top level frame
    Notes:
*/

mainFrame::~mainFrame ()
{
  free (pData);
  free (pPolesAndZeros);
}



/*
    Function Name: mainFrame::About
    Description: Menu command: Help|About
    Notes:
*/

void mainFrame::OnAbout (
  wxCommandEvent & WXUNUSED (event))
{
  wxMessageBox (_T ("Numerix Graphics Library Demo V1.20 (C) 2007, Delta Numerix"),
                _T ("About Numerix Graphics Library Demo"), wxOK | wxICON_INFORMATION, NULL);
}



void mainFrame::OnExit (
  wxCommandEvent & WXUNUSED (event))
{
  Close (TRUE);
}


/*
    Function Name: mainFrame::SetGraphText
    Description: Display text for graph
    Notes:
*/

void mainFrame::DisplayStatusLineText (
  wxString & string)
{
//  wxMessageBox (_T("Got Here"), _T("mainFrame::DisplayStatusLineText"), wxOK | wxICON_EXCLAMATION, NULL);

  SetStatusText (string, 1);
}


BEGIN_EVENT_TABLE (mainFrame, wxFrame)
  EVT_MENU (DEMO_EXIT, mainFrame::OnExit)
  EVT_MENU (DEMO_ABOUT, mainFrame::OnAbout)
END_EVENT_TABLE ()
