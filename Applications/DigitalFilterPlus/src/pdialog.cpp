// Digital filter project dialog functions
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

#include <math.h>
#include <stdlib.h>
#include <siglib.h>    // SigLib DSP library
#include "ngl.h"
#include "dfplus.h"

/*
  Function Name : GetFilterProjectDialog::GetFilterProjectDialog
  Description : Dialog to get the filter project details
  Notes :
*/

GetFilterProjectDialog::GetFilterProjectDialog(wxFrame* parent, wxWindowID id, const wxString& title)
    : wxDialog(parent, id, title, wxDefaultPosition, wxDefaultSize, wxCAPTION | wxMINIMIZE_BOX | wxSYSTEM_MENU)

{
  //  wxMessageBox ("Got Here", "GetFilterProjectDialog", wxOK |
  //  wxICON_EXCLAMATION, NULL);

  // Make the top-level layout; a vertical box to contain all widgets and
  // sub-layouts.
  wxBoxSizer* VSizer = new wxBoxSizer(wxVERTICAL);

  wxBoxSizer* HSizer1 = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizer2 = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizer3 = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizer4 = new wxBoxSizer(wxHORIZONTAL);

  VSizer->Add(HSizer1, 0, wxALIGN_CENTER | wxALL, 5);
  VSizer->Add(HSizer2, 0, wxALIGN_CENTER | wxALL, 5);
  VSizer->Add(HSizer3, 0, wxALIGN_CENTER | wxALL, 5);
  VSizer->Add(HSizer4, 0, wxALIGN_CENTER | wxALL, 5);

  // Get project name
  HSizer1->Add(new wxStaticText(this, -1, "Project name : "));
  ProjectLineEdit = new wxTextCtrl(this, -1, "Project");
  HSizer1->Add(ProjectLineEdit);

  // Get sample rate
  HSizer2->Add(new wxStaticText(this, -1, "Sample rate: "));
  SampleRateLineEdit = new wxTextCtrl(this, -1, "0.0");
  HSizer2->Add(SampleRateLineEdit);

  // Get filter design method
  HSizer3->Add(new wxStaticText(this, -1, "Filter Design Method : "));
  static const wxString FilterTypeChoices[] = {
      "FIR - Windowing",         "FIR - Remez Exchange Algorithm", "FIR - Complex Remez Exchange Algorithm",
      "FIR - Design Algorithms", "IIR - Pole-Zero Placement",      "IIR - Traditional Filter Design",
      "IIR - Design Algorithms",
  };
  FilterTypeComboBox =
      new wxComboBox(this, -1, wxEmptyString, wxDefaultPosition, wxDefaultSize, WXSIZEOF(FilterTypeChoices), FilterTypeChoices, wxCB_READONLY);
  FilterTypeComboBox->SetSelection(0);    // Set the default selection
  HSizer3->Add(FilterTypeComboBox);

  // Now get filter spec and move into design mode
  wxButton* ContinueButton = new wxButton(this, -1, "Continue");
  HSizer4->Add(ContinueButton, 0, wxALIGN_CENTER | wxALL, 5);
  ContinueButton->SetDefault();

  SetAutoLayout(TRUE);
  SetSizer(VSizer);

  VSizer->SetSizeHints(this);
  VSizer->Fit(this);

  CentreOnParent(wxBOTH);

  ProjectLineEdit->SetFocus();

  // Debug
  //  SampleRateLineEdit->SetValue ("9600");
}

/*
  Function Name : GetFilterProjectDialog::OnGetFilterSpec
  Description : Protected function to get the filter spec from the dialog
  Notes :
*/

void GetFilterProjectDialog::OnGetFilterSpec(wxCommandEvent& WXUNUSED(event))

{
  //  wxMessageBox (_T("Got here"), "GetFilterProjectDialog::OnGetFilterSpec",
  //  wxOK | wxICON_EXCLAMATION, NULL);
  SUF_Log("GetFilterProjectDialog::OnGetFilterSpec\n");

  ProjectNameString = ProjectLineEdit->GetValue();    // Get the project name

  //  Debug - print project name
  //  wxMessageBox (ProjectNameString, "OnGetFilterSpec - Project Name", wxOK |
  //  wxICON_EXCLAMATION, NULL);

  char* stopstring;
  SampleRate = strtod(SampleRateLineEdit->GetValue(),
                      &stopstring);    // Get the sample rate

  //  Debug - print sample rate
  //  char DebugString[40];
  //  sprintf (DebugString, "%lf", SampleRate);
  //  wxMessageBox (DebugString, "Sample rate", wxOK | wxICON_EXCLAMATION,
  //  NULL);

  FilterType = FilterTypeComboBox->GetSelection();    // Get the filter type

  //  Debug - print filter type
  //  if (FilterType == FIR_WINDOW_FILTER)
  //    wxMessageBox ("FIR WIN", "Filter type", wxOK | wxICON_EXCLAMATION,
  //    NULL);
  //  else if (FilterType == FIR_SIMPLE_REMEZ_FILTER)
  //    wxMessageBox ("FIR Remez", "Filter type", wxOK | wxICON_EXCLAMATION,
  //    NULL);
  //  else if (FilterType == FIR_REMEZ_FILTER)
  //    wxMessageBox ("FIR Complex Remez", "Filter type", wxOK |
  //    wxICON_EXCLAMATION, NULL);
  //  else if (FilterType == FIR_ALGS_FILTER)
  //    wxMessageBox ("FIR Algorithms", "Filter type", wxOK |
  //    wxICON_EXCLAMATION, NULL);
  //  else if (FilterType == IIR_PZP_FILTER)
  //    wxMessageBox ("IIR PZP", "Filter type", wxOK | wxICON_EXCLAMATION,
  //    NULL);
  //  else if (FilterType == IIR_FILTER)
  //    wxMessageBox ("IIR BLT", "Filter type", wxOK | wxICON_EXCLAMATION,
  //    NULL);
  //  else if (FilterType == IIR_ALGS_FILTER)
  //    wxMessageBox ("IIR Algorithms", "Filter type", wxOK |
  //    wxICON_EXCLAMATION, NULL);

  SLArrayIndex_t DoneFlag;
  DoneFlag = SIGLIB_TRUE;

  // Only exit when a valid project name and sample rate has been entered
  if (ProjectLineEdit->GetValue().Length() == 0) {
    wxMessageBox("Please enter a valid project name", "Input Error", wxOK | wxICON_EXCLAMATION, NULL);
    DoneFlag = SIGLIB_FALSE;
  }
  if (SampleRate <= SIGLIB_ZERO) {
    wxMessageBox("Please enter a valid sample rate", "Input Error", wxOK | wxICON_EXCLAMATION, NULL);
    DoneFlag = SIGLIB_FALSE;
  }

  static char FileNameString[500];    // Warn if it is an existing project

  strcpy(FileNameString, ProjectNameString);
  strcat(FileNameString, ".dfs");

  if (wxFileExists(FileNameString) && (DoneFlag == SIGLIB_TRUE)) {
    short answer = wxMessageBox("You have chosen an existing project, are you sure you "
                                "want to overwrite it ?",
                                "Input Verification", wxYES_NO | wxICON_QUESTION, NULL);
    if (answer == wxNO) {
      DoneFlag = SIGLIB_FALSE;
    }
  }

  if (DoneFlag == SIGLIB_TRUE) {
    EndModal(0);
  }
}

/*
  Function Name : GetFilterProjectDialog::GetProjectNameString
  Description : Public function to return the project name string
  Notes :
*/

wxString GetFilterProjectDialog::GetProjectNameString(void)
{
  return (ProjectNameString);
}

/*
  Function Name : GetFilterProjectDialog::SetProjectNameString
  Description : Public function to set the project name string
  Notes :
*/

void GetFilterProjectDialog::SetProjectNameString(wxString& NewProjectNameString)
{
  ProjectNameString = NewProjectNameString;
  ProjectLineEdit->SetValue(ProjectNameString);
}

/*
  Function Name : GetFilterProjectDialog::GetFilterType
  Description : Public function to return the filter type
  Notes :
*/

short GetFilterProjectDialog::GetFilterType(void)
{
  return (FilterType);
}

/*
  Function Name : GetFilterProjectDialog::SetFilterType
  Description : Public function to set the filter type
  Notes :
*/

void GetFilterProjectDialog::SetFilterType(short NewFilterType)
{
  FilterType = NewFilterType;
  FilterTypeComboBox->SetSelection(FilterType);    // Set the filter type selection
}

/*
  Function Name : GetFilterProjectDialog::GetSampleRate
  Description : Public function to return the sample rate
  Notes :
*/

SLData_t GetFilterProjectDialog::GetSampleRate(void)
{
  if (SampleRate < SIGLIB_ZERO)
    SampleRate = -SampleRate;

  return (SampleRate);
}

/*
  Function Name : GetFilterProjectDialog::SetSampleRate
  Description : Public function to set the sample rate
  Notes :
*/

void GetFilterProjectDialog::SetSampleRate(SLData_t NewSampleRate)
{
  SampleRate = NewSampleRate;

  wxString DialogString;
  DialogString.sprintf("%1.2lf", SampleRate);
  SampleRateLineEdit->SetValue(DialogString);
}

void GetFilterProjectDialog::OnClose(wxCloseEvent& event)
{
  SUF_Log("GetFilterProjectDialog::OnClose\n");

  if (event.CanVeto()) {
    event.Veto();
  }
}

BEGIN_EVENT_TABLE(GetFilterProjectDialog, wxDialog)
//  EVT_BUTTON  (DIALOG_GET_FILTER_SPEC,
//  GetFilterProjectDialog::OnGetFilterSpec)
EVT_BUTTON(-1, GetFilterProjectDialog::OnGetFilterSpec)
EVT_CLOSE(GetFilterProjectDialog::OnClose)
END_EVENT_TABLE()
