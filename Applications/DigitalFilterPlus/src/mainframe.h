// Digital filter design program mainFrame include file
// This software is provided under the terms of the GNU General Public License
// V2, as published by the Free Software Foundation. Copyright (c) 1999-2024
// Delta Numerix, All rights reserved.

#ifndef _MAINFRAME_H
#define _MAINFRAME_H

#include "dfplus.h"    // Include project header files

#define NUMBER_OF_TOOLBAR_BUTTONS 8

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
  void OnGetFilterSpec(wxCommandEvent& event);
  void OnViewCoeffs(wxCommandEvent& event);
  void OnViewSpec(wxCommandEvent& event);
  void OnViewZPlane(wxCommandEvent& event);
  void OnOptionsMenu(wxCommandEvent& event);
  void OnOpenUsersGuide(wxCommandEvent& event);
  void OnAbout(wxCommandEvent& event);
  void OnExit(wxCommandEvent& event);
  void OnSize(wxSizeEvent& event);
  void DisplayStatusLineText(wxString& string);

    private:
  FIRWinFilterDesignWindow* FIRWinDesign;
  FIRSimpleRemezFilterDesignWindow* FIRSimpleRemezDesign;
  FIRRemezFilterDesignWindow* FIRRemezDesign;
  FIRAlgsFilterDesignWindow* FIRAlgsDesign;
  IIRPZPFilterDesignWindow* IIRPZPDesign;
  IIRTraditionalFilterDesignWindow* IIRTraditionalDesign;
  IIRAlgsFilterDesignWindow* IIRAlgsDesign;

  IniFile* AppIniFile;

  short MFmValue;
  short MFnValue;
  SLData_t LogMagnGraphScale;

  wxSplitterWindow* SplitterWindow;

  wxWindow* GraphDisplayWindow;
  wxWindow* FilterSpecWindow;

  wxString InstallDirectoryPathString;

  GetFilterProjectDialog* FilterSpecDialog;
  GraphDisplay* FilterGraphDisplay;

  DisplayTextDialog* TextDisplayDialog;

  wxString ProjectNameString;
  short FilterType;
  SLData_t SampleRate;

  SLData_t* pGain;
  SLData_t* pPhase;
  SLData_t* pGroupDelay;
  SLData_t* pImpulseResponse;
  SLData_t* pStepResponse;
  SLComplexRect_s* pPolesAndZeros;

  filter_info_t FilterInfo;

  short ChildWindowsCreated = FALSE;    // Filter spec and graph windows have NOT been created

  DECLARE_EVENT_TABLE()
};

enum {
  DFPLUS_EXIT = wxID_HIGHEST + 1,
  DFPLUS_GET_FILTER_SPEC,
  DFPLUS_VIEW_COEFFS,
  DFPLUS_VIEW_SPEC,
  DFPLUS_VIEW_ZPLANE,
  DFPLUS_OPTIONS,
  DFPLUS_OPEN_USERS_GUIDE,
  DFPLUS_ABOUT,
  ID_TOOLBAR,
  DFPLUS_TOOLBAR_NEW,
  DFPLUS_TOOLBAR_VIEW_COEFFS,
  DFPLUS_TOOLBAR_VIEW_SPEC,
  DFPLUS_TOOLBAR_VIEW_ZPLANE,
  DFPLUS_TOOLBAR_VIEW_OPTIONS,
  DFPLUS_TOOLBAR_VIEW_OPEN_USERS_GUIDE,
  DFPLUS_TOOLBAR_VIEW_ABOUT,
  DFPLUS_TOOLBAR_EXIT
};

#endif    // End of #ifndef _MAINFRAME_H
