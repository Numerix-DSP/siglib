// Dialog class to request a string from the user
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

#include "dfplus.h"
#include "ngl.h"
#include <math.h>
#include <siglib.h>
#include <stdlib.h>

/*
  Function Name : GetStringDialog::GetStringDialog
  Description : Dialog to get a string from the user
  Notes :
*/

// GetStringDialog::GetStringDialog (wxFrame *parent, wxWindowID id, const
// wxString &title, const wxString &OriginalString)
GetStringDialog::GetStringDialog(wxFrame* parent, wxWindowID id, const wxString& title)
    : wxDialog(parent, id, title, wxDefaultPosition, wxDefaultSize, wxCAPTION | wxMINIMIZE_BOX | wxSYSTEM_MENU)

{
  //  wxMessageBox ("Got Here", "GetStringDialog", wxOK | wxICON_EXCLAMATION,
  //  NULL);

  // Make the top-level layout; a vertical box to contain all widgets and
  // sub-layouts.
  wxBoxSizer* VSizer = new wxBoxSizer(wxVERTICAL);

  wxBoxSizer* HSizer1 = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizer2 = new wxBoxSizer(wxHORIZONTAL);

  VSizer->Add(HSizer1, 0, wxALIGN_CENTER | wxALL, 5);
  VSizer->Add(HSizer2, 0, wxALIGN_CENTER | wxALL, 5);

  wxString LocalString = "String";

  //  if (LocalString != "")
  //    LocalString == OriginalString;

  // Get string
  HSizer1->Add(new wxStaticText(this, -1, "Enter string : "));
  StringLineEdit = new wxTextCtrl(this, -1, LocalString);
  StringLineEdit->SetSize(280, -1);
  HSizer1->Add(StringLineEdit);

  // Add OK button
  wxButton* OKButton = new wxButton(this, STRING_DIALOG_OK, "Ok");
  HSizer2->Add(OKButton, 0, wxALIGN_CENTER | wxALL, 5);
  OKButton->SetDefault();

  // Add Cancel button
  wxButton* CancelButton = new wxButton(this, STRING_DIALOG_CANCEL, "Cancel");
  HSizer2->Add(CancelButton, 0, wxALIGN_CENTER | wxALL, 5);

  SetAutoLayout(TRUE);
  SetSizer(VSizer);

  VSizer->SetSizeHints(this);
  VSizer->Fit(this);

  StringLineEdit->SetFocus();
}

/*
  Function Name : GetStringDialog::OnOk
  Description : Protected function to get the filter spec from the dialog
  Notes :
*/

void GetStringDialog::OnOk(wxCommandEvent& WXUNUSED(event))

{
  //  wxMessageBox (_T("Got here"), "GetStringDialog::OnOk", wxOK |
  //  wxICON_EXCLAMATION, NULL);
  SUF_Log("GetStringDialog::OnOk\n");

  String = StringLineEdit->GetValue();    // Get the project name

  //  Debug - print string
  //  wxMessageBox (String, "OnGetString", wxOK | wxICON_EXCLAMATION, NULL);

  EndModal(1);    // Return TRUE - we have a new string
}

/*
  Function Name : GetStringDialog::OnCancel
  Description : Cancel the operation
  Notes :
*/

void GetStringDialog::OnCancel(wxCommandEvent& WXUNUSED(event))

{
  //  wxMessageBox (_T("Got here"), "GetStringDialog::OnCancel", wxOK |
  //  wxICON_EXCLAMATION, NULL);
  SUF_Log("GetStringDialog::OnCancel\n");

  EndModal(0);    // Return FALSE - we have cancelled
}

/*
  Function Name : GetStringDialog::GetString
  Description : Public function to return the string
  Notes :
*/

wxString GetStringDialog::GetString(void)
{
  return (String);
}

BEGIN_EVENT_TABLE(GetStringDialog, wxDialog)
EVT_BUTTON(STRING_DIALOG_OK, GetStringDialog::OnOk)
EVT_BUTTON(STRING_DIALOG_CANCEL, GetStringDialog::OnCancel)
END_EVENT_TABLE()
