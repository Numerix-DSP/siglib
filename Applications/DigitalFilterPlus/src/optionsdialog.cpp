// Dialog class to get the application options from the user
// This software is provided under the terms of the GNU General Public License
// V2, as published by the Free Software Foundation.
// Copyright (c) 1999-2026 Delta Numerix, All rights reserved.

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
  Function Name : GetOptionsDialog::GetOptionsDialog
  Description : Dialog to get options
  Notes :
*/

GetOptionsDialog::GetOptionsDialog(wxFrame* parent, wxWindowID id, const wxString& title)
    : wxDialog(parent, id, title, wxDefaultPosition, wxDefaultSize, wxCAPTION | wxMINIMIZE_BOX | wxSYSTEM_MENU)

{
  //  wxMessageBox ("Got Here", "GetOptionsDialog", wxOK | wxICON_EXCLAMATION,
  //  NULL);

  // Make the top-level layout; a horizontal box to contain all widgets and
  // sub-layouts.

  wxBoxSizer* VSizer = new wxBoxSizer(wxVERTICAL);

  wxBoxSizer* HSizerp1 = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizerSOpt = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizerQOpt = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizerGOpt = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizerOkCan = new wxBoxSizer(wxHORIZONTAL);

  VSizer->Add(HSizerp1, 0, wxALIGN_CENTER | wxALL, 5);
  VSizer->Add(HSizerSOpt, 0, wxALIGN_CENTER | wxALL, 5);
  VSizer->Add(HSizerQOpt, 0, wxALIGN_CENTER | wxALL, 5);
  VSizer->Add(HSizerGOpt, 0, wxALIGN_CENTER | wxALL, 5);
  VSizer->Add(HSizerOkCan, 0, wxALIGN_CENTER | wxALL, 5);

  // Insert a CheckBox for the sign control
  HSizerSOpt->Add(new wxStaticText(this, -1, "Negate biquad alpha coeffs for MAC :  "), 0, wxALIGN_TOP | wxALL, 0);
  NegateIIRAlphaSignCheckBox = new wxCheckBox(this, GRAPH_OPTIONS_IIR_ALPHA_SIGN_CHECK, _T(" &Enable"));
  NegateIIRAlphaSignCheckBox->SetValue(0);
#if wxUSE_TOOLTIPS
  NegateIIRAlphaSignCheckBox->SetToolTip(_T("Click here to negate Biquad alpha coeffs for MAC"));
#endif    // wxUSE_TOOLTIPS
  HSizerSOpt->Add(NegateIIRAlphaSignCheckBox);

  //  wxMessageBox ("Got Here 1", "GetOptionsDialog", wxOK | wxICON_EXCLAMATION,
  //  NULL);

  // Insert two graph scale string controls for m and n
  HSizerQOpt->Add(new wxStaticText(this, -1, "Q Number format m.n :  m : "), 0, wxALIGN_TOP | wxALL, 0);
  mSpinControl = new wxSpinCtrl(this, -1, wxEmptyString, wxDefaultPosition, wxSize(65, -1), wxSP_ARROW_KEYS, 1, 63, 1);
  // mSpinControl->SetValue (8);      // Set default value
  HSizerQOpt->Add(mSpinControl);

  HSizerQOpt->Add(new wxStaticText(this, -1, " .n : "));
  nSpinControl = new wxSpinCtrl(this, -1, wxEmptyString, wxDefaultPosition, wxSize(65, -1), wxSP_ARROW_KEYS, 1, 63, 1);
  // nSpinControl->SetValue (24);     // Set default value
  HSizerQOpt->Add(nSpinControl);

  //  wxMessageBox ("Got Here 2", "GetOptionsDialog", wxOK | wxICON_EXCLAMATION,
  //  NULL);

  // Insert a CheckBox for the gain scaling
  HSizerGOpt->Add(new wxStaticText(this, -1, "Automatic log magnitude graph scaling :  "), 0, wxALIGN_TOP | wxALL, 0);
  ScaleGainCheckBox = new wxCheckBox(this, GRAPH_OPTIONS_AUTO_SCALE_CHECK, _T(" &Enable"));
  ScaleGainCheckBox->SetValue(TRUE);
#if wxUSE_TOOLTIPS
  ScaleGainCheckBox->SetToolTip(_T("Click here to enable automatic log magnitude graph scaling"));
#endif    // wxUSE_TOOLTIPS
  HSizerGOpt->Add(ScaleGainCheckBox);

  //  wxMessageBox ("Got Here 3", "GetOptionsDialog", wxOK | wxICON_EXCLAMATION,
  //  NULL);

  // Add graph scale string control
  HSizerGOpt->Add(new wxStaticText(this, -1, "     Graph Scale (dB) : "));
  ScaleLineEdit = new wxTextCtrl(this, -1, "300.0");
  ScaleLineEdit->SetSize(80, -1);
  ScaleLineEdit->Enable(FALSE);
  HSizerGOpt->Add(ScaleLineEdit);

  // Add OK button
  wxButton* OKButton = new wxButton(this, OPTIONS_DIALOG_OK, "Ok");
  HSizerOkCan->Add(OKButton, 0, wxALIGN_CENTER | wxALL, 5);
  OKButton->SetDefault();

  // Add Cancel button
  wxButton* CancelButton = new wxButton(this, OPTIONS_DIALOG_CANCEL, "Cancel");
  HSizerOkCan->Add(CancelButton, 0, wxALIGN_CENTER | wxALL, 5);

  SetAutoLayout(TRUE);
  SetSizer(VSizer);

  VSizer->SetSizeHints(this);
  VSizer->Fit(this);

  mSpinControl->SetFocus();
}

/*
  Function Name : GetOptionsDialog::OnAutoGraphScale
  Description : Protected function to when automatic scaling selected
  Notes :
*/

void GetOptionsDialog::OnAutoGraphScale(wxCommandEvent& WXUNUSED(event))

{
  //  wxMessageBox (_T("Got here"), "GetOptionsDialog::OnAutoGraphScale", wxOK |
  //  wxICON_EXCLAMATION, NULL); SUF_Log
  //  ("GetOptionsDialog::OnAutoGraphScale\n");

  if (ScaleGainCheckBox->IsChecked())
    ScaleLineEdit->Enable(FALSE);
  else
    ScaleLineEdit->Enable(TRUE);
}

/*
  Function Name : GetOptionsDialog::OnOk
  Description : Protected function to get the filter spec from the dialog
  Notes :
*/

void GetOptionsDialog::OnOk(wxCommandEvent& WXUNUSED(event))

{
  //  wxMessageBox (_T("Got here"), "GetOptionsDialog::OnOk", wxOK |
  //  wxICON_EXCLAMATION, NULL); SUF_Log ("GetOptionsDialog::OnOk\n");

  mValue = (short)mSpinControl->GetValue();    // Get m value
  nValue = (short)nSpinControl->GetValue();    // Get n value

  if ((mValue < 1) || (mValue > 63)) {
    wxMessageBox(_T("m must be between 1 and 63"), "Options Error", wxOK | wxICON_EXCLAMATION, NULL);
  }

  else if ((nValue < 1) || (nValue > 63)) {
    wxMessageBox(_T("n must be between 1 and 63"), "Options Error", wxOK | wxICON_EXCLAMATION, NULL);
  }

  if ((mValue + nValue) > 64) {
    wxMessageBox(_T("m + n must be less than or equal to 64"), "Options Error", wxOK | wxICON_EXCLAMATION, NULL);
    mValue = 1;    // n will be limited to 63 by spin control so set m to 1
    mSpinControl->SetValue(1);
  }

  if (ScaleGainCheckBox->IsChecked())
    Scale = SIGLIB_ZERO;
  else {
    if (fabs((SLData_t)atof(ScaleLineEdit->GetValue())) < 1.0)    // Set minimum scale value
      ScaleLineEdit->SetValue("1.0");
    Scale = fabs((SLData_t)atof(ScaleLineEdit->GetValue()));    // Get the graph scale
  }

  //  Debug - print string
  //  char String [80];
  //  sprintf (String, "Scale = %lf", Scale);
  //  wxMessageBox (String, "OnGetString", wxOK | wxICON_EXCLAMATION, NULL);

  EndModal(1);    // Return TRUE - we have a new string
}

/*
  Function Name : GetOptionsDialog::OnCancel
  Description : Cancel the operation
  Notes :
*/

void GetOptionsDialog::OnCancel(wxCommandEvent& WXUNUSED(event))

{
  //  wxMessageBox (_T("Got here"), "GetOptionsDialog::OnCancel", wxOK |
  //  wxICON_EXCLAMATION, NULL); SUF_Log ("GetOptionsDialog::OnCancel\n");

  EndModal(0);    // Return FALSE - we have cancelled
}

/*
  Function Name : GetOptionsDialog::GetNegateIIRAlphaSign
  Description : Public function to return the NegateIIRAlphaSign
  Notes :
*/

short GetOptionsDialog::GetNegateIIRAlphaSign(void)
{
  if (NegateIIRAlphaSignCheckBox->GetValue() == TRUE)
    return 1;
  else
    return 0;
}

/*
  Function Name : GetOptionsDialog::SetNegateIIRAlphaSign
  Description : Public function to set the NegateIIRAlphaSign
  Notes :
*/

void GetOptionsDialog::SetNegateIIRAlphaSign(short NewNegateIIRAlphaSign)
{
  if (NewNegateIIRAlphaSign == 0)    // If NewNegateIIRAlphaSign value is zero disable negation
  {
    NegateIIRAlphaSignCheckBox->SetValue(FALSE);
  } else {
    NegateIIRAlphaSignCheckBox->SetValue(TRUE);
  }
}

/*
  Function Name : GetOptionsDialog::GetM
  Description : Public function to return the scale
  Notes :
*/

short GetOptionsDialog::GetM(void)
{
  return (mValue);
}

/*
  Function Name : GetOptionsDialog::SetM
  Description : Public function to set the scale
  Notes :
*/

void GetOptionsDialog::SetM(short NewM)
{
  //  wxMessageBox (_T("Got here"), "GetOptionsDialog::SetM", wxOK |
  //  wxICON_EXCLAMATION, NULL);

  mValue = NewM;

  if (mValue <= 0)    // If m value is zero use DEFAULT_M_VALUE
  {
    mValue = DEFAULT_M_VALUE;
  }

  mSpinControl->SetValue((int)mValue);
}

/*
  Function Name : GetOptionsDialog::GetN
  Description : Public function to return the scale
  Notes :
*/

short GetOptionsDialog::GetN(void)
{
  return (nValue);
}

/*
  Function Name : GetOptionsDialog::SetN
  Description : Public function to set the scale
  Notes :
*/

void GetOptionsDialog::SetN(short NewN)
{
  nValue = NewN;

  if (nValue <= 0)    // If n value is zero use DEFAULT_M_VALUE
  {
    nValue = DEFAULT_N_VALUE;
  }

  nSpinControl->SetValue(nValue);
}

/*
  Function Name : GetOptionsDialog::GetScale
  Description : Public function to return the scale
  Notes :
*/

SLData_t GetOptionsDialog::GetScale(void)
{
  if (Scale < SIGLIB_ZERO) {
    Scale = -Scale;
  }

  return (Scale);
}

/*
  Function Name : GetOptionsDialog::SetScale
  Description : Public function to set the scale
  Notes :
*/

void GetOptionsDialog::SetScale(SLData_t NewScale)
{
  Scale = NewScale;

  if (Scale == SIGLIB_ZERO)    // If scale value is zero use auto scaling
  {
    ScaleGainCheckBox->SetValue(TRUE);
    ScaleLineEdit->Enable(FALSE);
  } else {
    wxString DialogString;
    DialogString.sprintf("%lf", NewScale);
    ScaleLineEdit->SetValue(DialogString);
    ScaleLineEdit->Enable(TRUE);
    ScaleGainCheckBox->SetValue(FALSE);
  }
}

BEGIN_EVENT_TABLE(GetOptionsDialog, wxDialog)
EVT_CHECKBOX(GRAPH_OPTIONS_AUTO_SCALE_CHECK, GetOptionsDialog::OnAutoGraphScale)
EVT_BUTTON(OPTIONS_DIALOG_OK, GetOptionsDialog::OnOk)
EVT_BUTTON(OPTIONS_DIALOG_CANCEL, GetOptionsDialog::OnCancel)
END_EVENT_TABLE()
