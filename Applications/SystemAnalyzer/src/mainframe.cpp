// Top level graph functions
// Copyright (c) 1999-2024 Delta Numerix, All rights reserved.

// Include files
// #ifdef __GNUG__
// #pragma implementation "mainframe.cpp"
// #pragma interface "mainframe.cpp"
// #endif  /*//  */

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"
#include "wx/splitter.h"

// These pragmas prevents the compiler from precompiling the #defines below so that
// changing them doesn't "take place" later!
#ifdef __VISUALC__
#pragma hdrstop
#endif                                                              /* */

// for all others, include the necessary headers
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif                                                              /* */

#include "mainframe.h"
 class MyApp: public wxApp  { public:
//  virtual bool OnInit();
  bool OnInit (
  void);
};

IMPLEMENT_APP (MyApp) 

/*
    Function Name : main
    Description : Top level function
    Notes :
*/
// `Main program' equivalent: the program execution "starts" here
bool MyApp::OnInit (
  )  { mainFrame * AppMainFrame = new mainFrame (_T ("Signal Analyzer"), wxDefaultPosition, wxDefaultSize);
  SetTopWindow (AppMainFrame);                                    // Tell the application that it's our main window
  AppMainFrame->Show (TRUE);                                        // Show it
  return TRUE;
}


