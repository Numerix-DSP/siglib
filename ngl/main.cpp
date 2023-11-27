// Numerix Graphics Library
// Top level graph functions
// Copyright (c) 2022, Delta Numerix, All rights reserved.

/*
    wxWindows program main functions
*/

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

#include "mainframe.h"

class           MyApp:public wxApp {
public:
  virtual bool    OnInit (
    );
};


mainFrame      *AppMainFrame = (mainFrame *) NULL;

IMPLEMENT_APP (MyApp)

/*
    Function Name: main
    Description: Top level function
    Notes:
*/
// `Main program' equivalent: the program execution "starts" here
bool MyApp::OnInit (
  )
{

  AppMainFrame = new mainFrame (_T ("Main Window"), wxDefaultPosition, wxDefaultSize);

  SetTopWindow (AppMainFrame);                                      // Tell the application that it's our main window
  AppMainFrame->Show (TRUE);                                        // Show it

  return TRUE;
}
