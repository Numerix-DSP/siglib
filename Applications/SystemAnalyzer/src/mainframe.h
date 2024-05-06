// Digital filter design program mainFrame include file
// This software is provided under the terms of the GNU General Public License
// V2, as published by the Free Software Foundation. Copyright (c) 1999-2024
// Delta Numerix, All rights reserved.

#ifndef _MAINFRAME_H
#define _MAINFRAME_H

#include "sa.h"    // Include project header files

/*
    Function Name : mainFrame
    Description : Top level frame
    Notes : This should contain the filter designer and graph children
*/

class mainFrame : public wxFrame
{
    public:
  mainFrame(wxString aTitle, wxPoint Position, wxSize Size);
  ~mainFrame();
  wxString FindExecutablePath(const wxString& argv0, const wxString& cwd);
  void OnAbout(wxCommandEvent& event);
  void OnExit(wxCommandEvent& event);
  void OnSize(wxSizeEvent& event);
  void DisplayStatusLineText(wxString& string);
  void DisplayStatusLineText2(wxString& string);

    private:
  SAProcWindow* ProcessWindow;
  wxSplitterWindow* SplitterWindow;

  wxWindow* GraphDisplayWindow;
  wxWindow* TopProcessWindow;

  wxString InstallDirectoryPathString;

  GraphDisplay* DataGraphDisplay;

  DisplayTextDialog* TextDisplayDialog;

  double SampleRate;

  double* pData;

  int ChildWindowsCreated = FALSE;    // Filter spec and graph windows have NOT been created

  DECLARE_EVENT_TABLE()
};

enum {
  SA_EXIT = wxID_HIGHEST + 1,
  SA_GET_FILTER_SPEC,
  SA_VIEW_COEFFS,
  SA_VIEW_SPEC,
  SA_VIEW_ZPLANE,
  SA_OPEN_USERS_GUIDE,
  SA_CHECK_VERSION,
  SA_ENTER_KEY,
  SA_ABOUT
};

#endif    // End of #ifndef _MAINFRAME_H
