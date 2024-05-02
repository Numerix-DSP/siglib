// System analyzer program top level functions
// Copyright (c) 1999-2024 Delta Numerix, All rights reserved.

// Include files
#ifdef __GNUG__
#pragma implementation "sa.cpp"
#pragma interface "sa.cpp"
#endif                                                              /* */

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif                                                              /* */

#if wxUSE_TOOLTIPS
#include "wx/tooltip.h"
#endif                                                              /* */

#include "wx/string.h"
#include "wx/file.h"
#include "wx/textfile.h"
#include "wx/mimetype.h"
#include <wx/spinctrl.h>

#if defined(__WXGTK__) || defined(__WXMOTIF__) || defined(__WXMAC__) || defined(__WXMGL__) || defined(__WXX11__)
#include "sa.xpm"                                                   // the application icon
#endif                                                              /* */

#include <stdlib.h>
#if __APPLE__
#include <malloc/malloc.h>
#else                                                               /* */
#include <malloc.h>
#endif                                                              /* */
#include <math.h>
#include "sa.h"
#include "mainframe.h"                                              // mainframe header - must be located after other project headers
 GraphDisplay * GlobalGraphDisplayWindow;


/*
    Function Name : mainFrame::mainFrame
    Description : Top level frame
    Notes : This should contain the filter designer and graph children
*/
//JEScale mainFrame::mainFrame (wxString Title, wxPoint Position, wxSize Size)
//JEScale //v28      : wxFrame ((wxFrame *) NULL, -1, Title, Position, Size, wxDEFAULT_FRAME_STYLE & ~ (wxRESIZE_BORDER | wxRESIZE_BOX | wxMAXIMIZE_BOX))
//JEScale        : wxFrame ((wxFrame *) NULL, -1, Title, Position, Size, wxDEFAULT_FRAME_STYLE & ~ (wxRESIZE_BORDER | wxMAXIMIZE_BOX))
mainFrame::mainFrame (wxString Title, wxPoint Position, wxSize Size) :wxFrame ((wxFrame *) NULL, -1, Title, Position, Size)
//       : wxFrame ((wxFrame *) NULL, -1, Title, Position, Size, wxCAPTION | wxMINIMIZE_BOX | wxSYSTEM_MENU)
{

//  wxMessageBox ("Got Here", "main", wxOK | wxICON_EXCLAMATION, NULL);
  SetTitle ("Numerix System Analyzer");
  int screenX = wxSystemSettings::GetMetric (wxSYS_SCREEN_X);
  int screenY = wxSystemSettings::GetMetric (wxSYS_SCREEN_Y);
  if (screenX > 1080) {
    SetSize (wxRect (400, SA_TOP, (int) (screenX * 0.6), (int) (screenY * 0.8)));  // X, Y, W, H - used for a resizeable border
  }

  else {
    SetSize (wxRect (SA_LEFT, SA_TOP, SA_WIDTH, SA_HEIGHT));       // X, Y, W, H - used for a resizeable border
  }
  SetAutoLayout (TRUE);                                            // Autolayout the window
#if defined(__WXMSW__)
  SetIcon (wxICON (ApplicationIcon));                               // Select the application icon
#endif                                                              /* */

// Create the menubar
  wxMenuBar      *menuBar = new wxMenuBar;
  wxMenu * fileMenu = new wxMenu;
  wxMenu * helpMenu = new wxMenu;
  fileMenu->Append (SA_EXIT, _T ("E&xit"));
  helpMenu->Append (SA_ABOUT, _T ("&About"));
  menuBar->Append (fileMenu, _T ("&File"));
  menuBar->Append (helpMenu, _T ("&Help"));
  SetMenuBar (menuBar);
  CreateStatusBar (3);
  SetStatusText (_T ("Numerix System Analyzer"), 0);
  int WidthArray[] = { 400, -1, -1 };
  SetStatusWidths (3, WidthArray);
  
// Create splitter window
    SplitterWindow = new wxSplitterWindow (this, -1, wxDefaultPosition, GetClientSize ());
  
#if DEBUG_LOG_FILE
    SUF_Debugfprintf ("mainFrame::mainFrame : Splitter window created\n");

#endif                                                              /* */
  
// Create filter specification entry window
    TopProcessWindow = new wxWindow (SplitterWindow, -1);
  ProcessWindow = new SAProcWindow (TopProcessWindow, -1);
  ProcessWindow->Show (TRUE);
  
// Create graph window
    GraphDisplayWindow = new wxWindow (SplitterWindow, -1);
  DataGraphDisplay = new GraphDisplay (GraphDisplayWindow, -1);
  GlobalGraphDisplayWindow = DataGraphDisplay;
  
// Arrange splitter window
//  SplitterWindow->SetSashSize (0);
    SplitterWindow->SetSashInvisible (true);
  wxSize ClientSize = GetClientSize ();
  SplitterWindow->SetMinimumPaneSize ((ClientSize.GetWidth () * 3) / 10);

// Split the screen vertically and allocate the space
  SplitterWindow->SplitVertically (TopProcessWindow, GraphDisplayWindow, (ClientSize.GetWidth () * 3) / 10);
  ChildWindowsCreated = TRUE;                                     // Child windows have been created, now we can resize them
  pData = (double *) malloc (MAXIMUM_GRAPH_WIDTH * sizeof (double)); // Create data arrays
  int i;
  for (i = 0; i < MAXIMUM_GRAPH_WIDTH; i++)                        // Clear array contents
  {
    pData[i] = 0.0;
  }
  DataGraphDisplay->SetScaling (GRAPH_SCALE);                     // Set initial graph scaling mode
  DataGraphDisplay->SetSampleRate (44100.0);                        // Set sample rate
  DataGraphDisplay->SetDataArray (pData);                           // Set graph pointers to arrays
  ProcessWindow->SetDataArray (pData);
}



/*
    Function Name : mainFrame::~mainFrame
    Description : Destructor for top level SA frame
    Notes :
*/
  mainFrame::~mainFrame ()  { free (pData);
}



/**/

/********************************************************
* Function : mainFrame::FindExecutablePath ()
*
* Parameters :
*
* Return value :
*   void
*
* Description : Find the directory path of the executable
*               This function has been taken from the
*               wxWindows web site and modified slightly
*               because it does not scan the environment
*               variable.
* Find the absolute path where this application has been run from.
* argv0 is wxTheApp->argv[0]
* cwd is the current working directory (at startup)
*
********************************************************/
  wxString mainFrame::FindExecutablePath (const wxString & argv0,
                                           const wxString & cwd)  {
  wxString str;
  if (wxIsAbsolutePath (argv0))
     {
    return wxPathOnly (argv0);
    }

  else
     {

// Is it a relative path?
    wxString currentDir (
  cwd);
    if ((wxPathOnly (argv0) == ".") || (wxPathOnly (argv0) == ""))
      return (currentDir);
    if (currentDir.Last () != wxFILE_SEP_PATH)
      currentDir += wxFILE_SEP_PATH;
    
#if defined(__WXGTK__) || defined(__WXMOTIF__)
      str = currentDir + argv0;

#else                                                               /* */
      str = currentDir + argv0 + wxString (".exe");

#endif                                                              /* */
//  SUF_Debugfprintf ("mainFrame::FindExecutablePath : str = \"%s\"\n", str);
    if (wxFileExists (str))
      return wxPathOnly (str);
    }
  
// OK, it's neither an absolute path nor a relative path.
// Search PATH.
    wxPathList pathList;
  pathList.AddEnvList (wxT ("PATH"));
  str = pathList.FindAbsoluteValidPath (argv0);
  if (!str.IsEmpty ())
    return wxPathOnly (str);
  
// Failed
    return wxEmptyString;
}



/*
    Function Name : mainFrame::About
    Description : Menu command : Help|About
    Notes :
*/
void mainFrame::OnAbout (
  wxCommandEvent & WXUNUSED (event))  {
  char String[250];
  wxCommandEvent DummyEvent = sprintf (String, "%s\nFor Technical Support, please contact : support@numerix-dsp.com", SA_COPYRIGHT_NOTICE);
  wxMessageBox (String, "About SA", wxOK | wxICON_INFORMATION, NULL);
} 

/*
    Function Name : mainFrame::OnExit
    Description : Called from exit menu
    Notes :
*/
void mainFrame::OnExit (
  wxCommandEvent & WXUNUSED (event))  { Close (TRUE);
} 

/*
    Function Name : mainFrame::OnSize
    Description : Called when size changes
    Notes :
*/
void mainFrame::OnSize (
  wxSizeEvent & event)  {
//  wxMessageBox ("Got Here", "mainFrame::OnSize", wxOK | wxICON_EXCLAMATION, NULL);
  if (ChildWindowsCreated == TRUE)                                 // Only adjust size of child windows, if they have been created
  { wxSize ClientSize = GetClientSize ();
   SplitterWindow->SetSize (ClientSize); SplitterWindow->SetMinimumPaneSize ((ClientSize.GetWidth () * 3) / 10);
// Split the screen vertically and allocate the space
   SplitterWindow->SetSashPosition ((ClientSize.GetWidth () * 3) / 10); }
   event.Skip ();                                                 // Skip original event handler
   }

   

/*
    Function Name : mainFrame::DisplayStatusLineText
    Description : Display text for graph
    Notes :
*/
   void mainFrame::DisplayStatusLineText (wxString & string)  {

//  wxMessageBox ("Got Here", "mainFrame::DisplayGraphText", wxOK | wxICON_EXCLAMATION, NULL);
   SetStatusText (string, 1);                                      // This prints the location and magnitude at the cursor
   } void mainFrame::DisplayStatusLineText2 (wxString & string)  {


//  wxMessageBox ("Got Here", "mainFrame::DisplayStatusLineText2", wxOK | wxICON_EXCLAMATION, NULL);
   SetStatusText (string, 2);                                      // This prints the location and magnitude at the cursor
   } BEGIN_EVENT_TABLE (mainFrame, wxFrame)

   EVT_MENU (SA_ABOUT, mainFrame::OnAbout)
   EVT_MENU (SA_EXIT, mainFrame::OnExit)
   EVT_SIZE (mainFrame::OnSize)
   END_EVENT_TABLE ()  DisplayTextDialog::DisplayTextDialog (wxFrame * parent, wxWindowID id, const wxString & title,
                                                                  const wxPoint & pos,
                                                                  const wxSize & size) :wxDialog (parent,
                                                                                                   id,
                                                                                                   title,
                                                                                                   pos,
                                                                                                   size,
                                                                                                   wxCAPTION | wxSYSTEM_MENU | wxRESIZE_BORDER)  {

//  wxMessageBox ("Got Here", "DisplayTextDialog", wxOK | wxICON_EXCLAMATION, NULL);

// Get size for text control
   wxSize TextBoxSize = GetSize (); TextBoxSize.SetWidth (TextBoxSize.GetWidth () - 7); TextBoxSize.SetHeight (TextBoxSize.GetHeight () - 27); 
// Create text control
   TextBox = new wxTextCtrl (this, -1, "", wxDefaultPosition, TextBoxSize, wxTE_MULTILINE | wxTE_READONLY | wxHSCROLL);
   TextBox->SetFont (wxFont (8, wxMODERN, wxNORMAL, wxNORMAL, FALSE, "", wxFONTENCODING_SYSTEM));
   } void DisplayTextDialog::SetText (wxString & string)  {


//  wxMessageBox ("Got Here", "DisplayTextDialog::SetText", wxOK | wxICON_EXCLAMATION, NULL);
   TextBox->SetValue (string); } void DisplayTextDialog::OnSize (wxSizeEvent & event)  {


//  wxMessageBox ("Got Here", "DisplayTextDialog::OnSize", wxOK | wxICON_EXCLAMATION, NULL);

// Get size for text control
   wxSize TextBoxSize = GetSize (); TextBoxSize.SetWidth (TextBoxSize.GetWidth () - 7); TextBoxSize.SetHeight (TextBoxSize.GetHeight () - 27); 
// Adjust text control size
   TextBox->SetSize (TextBoxSize); event.Skip ();                 // Skip original event handler
   } BEGIN_EVENT_TABLE (DisplayTextDialog, wxDialog)  EVT_SIZE (DisplayTextDialog::OnSize)  END_EVENT_TABLE () 
