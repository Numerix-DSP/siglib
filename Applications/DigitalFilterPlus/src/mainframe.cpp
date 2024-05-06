// Top level graph functions
// This software is provided under the terms of the GNU General Public License
// V2, as published by the Free Software Foundation. Copyright (c) 1999-2024
// Delta Numerix, All rights reserved.

// Include files
// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers
#ifndef WX_PRECOMP
#  include "wx/wx.h"
#endif

#if wxUSE_TOOLTIPS
#  include "wx/tooltip.h"
#endif

#include "wx/splitter.h"
#include "wx/toolbar.h"

// Toolbar bitmaps
#include "bitmaps/about.xpm"
#include "bitmaps/exit.xpm"
#include "bitmaps/help.xpm"
#include "bitmaps/new.xpm"
#include "bitmaps/options.xpm"
#include "bitmaps/vcoeffs.xpm"
#include "bitmaps/vspec.xpm"
#include "bitmaps/vzdcoeffs.xpm"
#include "mainframe.h"

// Define a new application
class MyApp : public wxApp
{
    public:
  bool OnInit(void);
  bool InitToolbar(wxToolBar* toolBar);
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

  mainFrame* AppMainFrame = new mainFrame(_T("Digital Filter+"), wxDefaultPosition, wxDefaultSize);

  AppMainFrame->CreateToolBar(wxNO_BORDER | wxTB_HORIZONTAL | wxTB_DOCKABLE, ID_TOOLBAR);
  InitToolbar(AppMainFrame->GetToolBar());

  SetTopWindow(AppMainFrame);    // Tell the application that it's our main window
  AppMainFrame->Show(TRUE);      // Show it

  return TRUE;
}

/*
  Function Name : MyApp::InitToolbar
  Description : Initialise the toolbar
  Notes :
*/

bool MyApp::InitToolbar(wxToolBar* toolBar)
{
  toolBar->SetMargins(5, 5);

  // Set up toolbar
  wxBitmap* toolBarBitmaps[NUMBER_OF_TOOLBAR_BUTTONS];

  toolBarBitmaps[0] = new wxBitmap(new_xpm);
  toolBarBitmaps[1] = new wxBitmap(vcoeffs_xpm);
  toolBarBitmaps[2] = new wxBitmap(vspec_xpm);
  toolBarBitmaps[3] = new wxBitmap(vzdcoeffs_xpm);
  toolBarBitmaps[4] = new wxBitmap(options_xpm);
  toolBarBitmaps[5] = new wxBitmap(help_xpm);
  toolBarBitmaps[6] = new wxBitmap(about_xpm);
  toolBarBitmaps[7] = new wxBitmap(exit_xpm);

  int width = 16;
  int currentX = 5;

  toolBar->AddTool(DFPLUS_TOOLBAR_NEW, wxT("New Project"), *(toolBarBitmaps[0]), wxT("New Project"));
  currentX += width + 5;
  toolBar->AddSeparator();
  toolBar->AddTool(DFPLUS_TOOLBAR_VIEW_COEFFS, wxT("View Filter Coefficients"), *(toolBarBitmaps[1]), wxT("View Filter Coefficients"));
  currentX += width + 5;
  toolBar->AddTool(DFPLUS_TOOLBAR_VIEW_SPEC, wxT("View Specification"), *(toolBarBitmaps[2]), wxT("View Specification"));
  currentX += width + 5;
  toolBar->AddTool(DFPLUS_TOOLBAR_VIEW_ZPLANE, wxT("View z-Domain Coefficients"), *(toolBarBitmaps[3]), wxT("View z-Domain Coefficients"));
  currentX += width + 5;
  toolBar->AddSeparator();
  toolBar->AddTool(DFPLUS_TOOLBAR_VIEW_OPTIONS, wxT("Options"), *(toolBarBitmaps[4]), wxT("Options"));
  currentX += width + 5;
  toolBar->AddSeparator();
  toolBar->AddTool(DFPLUS_TOOLBAR_VIEW_OPEN_USERS_GUIDE, wxT("View User's Guide"), *(toolBarBitmaps[5]), wxT("View User's Guide"));
  currentX += width + 5;
  toolBar->AddTool(DFPLUS_TOOLBAR_VIEW_ABOUT, wxT("About"), *(toolBarBitmaps[6]), wxT("About"));
  currentX += width + 5;
  toolBar->AddSeparator();
  toolBar->AddTool(DFPLUS_TOOLBAR_EXIT, wxT("Exit"), *(toolBarBitmaps[7]), wxT("Exit"));

  toolBar->Realize();

  // Can delete the bitmaps since they're reference counted
  for (int i = 0; i < NUMBER_OF_TOOLBAR_BUTTONS; i++) {
    delete toolBarBitmaps[i];
  }

  return TRUE;
}
