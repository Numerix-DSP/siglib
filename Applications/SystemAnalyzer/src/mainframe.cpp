// Top level graph functions
// This software is provided under the terms of the GNU General Public License
// V2, as published by the Free Software Foundation.
// Copyright (c) 1999-2026 Delta Numerix, All rights reserved.

// Include files
// #ifdef __GNUG__
// #pragma implementation "mainframe.cpp"
// #pragma interface "mainframe.cpp"
// #endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/splitter.h"
#include "wx/wxprec.h"

// These pragmas prevents the compiler from precompiling the #defines below so
// that changing them doesn't "take place" later!
#ifdef __VISUALC__
#  pragma hdrstop
#endif /* */

// for all others, include the necessary headers
#ifndef WX_PRECOMP
#  include "wx/wx.h"
#endif

#include "mainframe.h"
class MyApp : public wxApp
{
    public:
  //  virtual bool OnInit();
  bool OnInit(void);
};

IMPLEMENT_APP(MyApp)

/*
    Function Name : main
    Description : Top level function
    Notes :
*/
// `Main program' equivalent: the program execution "starts" here
bool MyApp::OnInit()
{
  mainFrame* AppMainFrame = new mainFrame(_T ("Signal Analyzer"), wxDefaultPosition, wxDefaultSize);
  SetTopWindow(AppMainFrame);    // Tell the application that it's our main window
  AppMainFrame->Show(TRUE);      // Show it
  return TRUE;
}
