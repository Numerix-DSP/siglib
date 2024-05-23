// FIR windowing digital filter functions
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

#include "wx/spinctrl.h"
#if __APPLE__
#  include <malloc/malloc.h>
#else
#  include <malloc.h>
#endif
#include <math.h>
#include <stdlib.h>
#include <siglib.h>    // SigLib DSP library
#include "ngl.h"
#include "dfplus.h"

extern GraphDisplay* GlobalGraphDisplayWindow;

extern int global_mValue;    // Global m and n values for Q number format
extern int global_nValue;

/*
  Name : FIRWinFilterDesignWindow::FIRWinFilterDesignWindow
  Description : Child widget that is used to get the FIR filter spec.
  Notes :
*/

FIRWinFilterDesignWindow::FIRWinFilterDesignWindow(wxWindow* parent, wxWindowID id)
    : wxPanel(parent, id)

{
  //  wxMessageBox (_T("Got here"),
  //  "FIRWinFilterDesignWindow::FIRWinFilterDesignWindow", wxOK |
  //  wxICON_EXCLAMATION, NULL);

  // Top-level layout - a vertical box sizer to contain all the controls
  wxStaticBoxSizer* VSizer = new wxStaticBoxSizer(wxVERTICAL, this, "FIR Windowing Design");

  wxBoxSizer* HSizer11 = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizer12 = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizer13 = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizer14 = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizer21 = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizer22 = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizer23 = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizer24 = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizer31 = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizer32 = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizer33 = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizer34 = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizer41 = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizer42 = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizer43 = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizer44 = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizer51 = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizer52 = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizer53 = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizer54 = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizerP6 = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizer6 = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizer7 = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizer8 = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizer9 = new wxBoxSizer(wxHORIZONTAL);

  VSizer->Add(HSizer11, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add(HSizer12, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add(HSizer13, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add(HSizer14, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add(HSizer21, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add(HSizer22, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add(HSizer23, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add(HSizer24, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add(HSizer31, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add(HSizer32, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add(HSizer33, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add(HSizer34, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add(HSizer41, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add(HSizer42, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add(HSizer43, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add(HSizer44, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add(HSizer51, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add(HSizer52, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add(HSizer53, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add(HSizer54, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add(HSizerP6, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add(HSizer6, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add(HSizer7, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add(HSizer8, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add(HSizer9, 0, wxALIGN_CENTER | wxALL, 2);

  // This is the FIR filter specification section
  HSizer11->Add(new wxStaticText(this, -1, "Filter band 1 : "), 0, wxALIGN_TOP | wxALL, 0);
  // Text control for filter band 1 FLow
  HSizer12->Add(new wxStaticText(this, -1, "FLow : "), 0, wxALIGN_TOP | wxALL, 0);
  FilterBand1FLowLine = new wxTextCtrl(this, -1, "0.0");
  HSizer12->Add(FilterBand1FLowLine);

  // Text control for filter band 1 FHigh
  HSizer13->Add(new wxStaticText(this, -1, "FHigh : "), 0, wxALIGN_TOP | wxALL, 0);
  FilterBand1FHighLine = new wxTextCtrl(this, -1, "0.0");
  HSizer13->Add(FilterBand1FHighLine);

  // Text control for filter band 1 Gain
  HSizer14->Add(new wxStaticText(this, -1, "Gain   : "), 0, wxALIGN_TOP | wxALL, 0);
  FilterBand1GainLine = new wxTextCtrl(this, -1, "0.0");
  HSizer14->Add(FilterBand1GainLine);

  HSizer21->Add(new wxStaticText(this, -1, "Filter band 2 : "), 0, wxALIGN_TOP | wxALL, 0);
  // Text control for filter band 2 FLow
  HSizer22->Add(new wxStaticText(this, -1, "FLow : "), 0, wxALIGN_TOP | wxALL, 0);
  FilterBand2FLowLine = new wxTextCtrl(this, -1, "0.0");
  HSizer22->Add(FilterBand2FLowLine);

  // Text control for filter band 2 FHigh
  HSizer23->Add(new wxStaticText(this, -1, "FHigh : "), 0, wxALIGN_TOP | wxALL, 0);
  FilterBand2FHighLine = new wxTextCtrl(this, -1, "0.0");
  HSizer23->Add(FilterBand2FHighLine);

  // Text control for filter band 2 Gain
  HSizer24->Add(new wxStaticText(this, -1, "Gain   : "), 0, wxALIGN_TOP | wxALL, 0);
  FilterBand2GainLine = new wxTextCtrl(this, -1, "0.0");
  HSizer24->Add(FilterBand2GainLine);

  HSizer31->Add(new wxStaticText(this, -1, "Filter band 3 : "), 0, wxALIGN_TOP | wxALL, 0);
  // Text control for filter band 3 FLow
  HSizer32->Add(new wxStaticText(this, -1, "FLow : "), 0, wxALIGN_TOP | wxALL, 0);
  FilterBand3FLowLine = new wxTextCtrl(this, -1, "0.0");
  HSizer32->Add(FilterBand3FLowLine);

  // Text control for filter band 3 FHigh
  HSizer33->Add(new wxStaticText(this, -1, "FHigh : "), 0, wxALIGN_TOP | wxALL, 0);
  FilterBand3FHighLine = new wxTextCtrl(this, -1, "0.0");
  HSizer33->Add(FilterBand3FHighLine);

  // Text control for filter band 3 Gain
  HSizer34->Add(new wxStaticText(this, -1, "Gain   : "), 0, wxALIGN_TOP | wxALL, 0);
  FilterBand3GainLine = new wxTextCtrl(this, -1, "0.0");
  HSizer34->Add(FilterBand3GainLine);

  HSizer41->Add(new wxStaticText(this, -1, "Filter band 4 : "), 0, wxALIGN_TOP | wxALL, 0);
  // Text control for filter band 4 FLow
  HSizer42->Add(new wxStaticText(this, -1, "FLow : "), 0, wxALIGN_TOP | wxALL, 0);
  FilterBand4FLowLine = new wxTextCtrl(this, -1, "0.0");
  HSizer42->Add(FilterBand4FLowLine);

  // Text control for filter band 4 FHigh
  HSizer43->Add(new wxStaticText(this, -1, "FHigh : "), 0, wxALIGN_TOP | wxALL, 0);
  FilterBand4FHighLine = new wxTextCtrl(this, -1, "0.0");
  HSizer43->Add(FilterBand4FHighLine);

  // Text control for filter band 4 Gain
  HSizer44->Add(new wxStaticText(this, -1, "Gain   : "), 0, wxALIGN_TOP | wxALL, 0);
  FilterBand4GainLine = new wxTextCtrl(this, -1, "0.0");
  HSizer44->Add(FilterBand4GainLine);

  HSizer51->Add(new wxStaticText(this, -1, "Filter band 5 : "), 0, wxALIGN_TOP | wxALL, 0);
  // Text control for filter band 5 FLow
  HSizer52->Add(new wxStaticText(this, -1, "FLow : "), 0, wxALIGN_TOP | wxALL, 0);
  FilterBand5FLowLine = new wxTextCtrl(this, -1, "0.0");
  HSizer52->Add(FilterBand5FLowLine);

  // Text control for filter band 5 FHigh
  HSizer53->Add(new wxStaticText(this, -1, "FHigh : "), 0, wxALIGN_TOP | wxALL, 0);
  FilterBand5FHighLine = new wxTextCtrl(this, -1, "0.0");
  HSizer53->Add(FilterBand5FHighLine);

  // Text control for filter band 5 Gain
  HSizer54->Add(new wxStaticText(this, -1, "Gain   : "), 0, wxALIGN_TOP | wxALL, 0);
  FilterBand5GainLine = new wxTextCtrl(this, -1, "0.0");
  HSizer54->Add(FilterBand5GainLine);

  // A combo box for chosing the window type
  HSizer6->Add(new wxStaticText(this, -1, "Window type : "), 0, wxALIGN_TOP | wxALL, 0);
  static const wxString WindowChoices[] = {
      "Hanning",         "Hamming",   "Blackman", "Bartlett Triangle - Zero Endpoints", "Bartlett Triangle - Non Zero Endpoints", "Kaiser",
      "Blackman Harris", "Rectangle", "Hamming"};
  // WindowTypeComboBox = new wxComboBox (this, -1, wxEmptyString,
  // wxDefaultPosition, wxSize (110, -1),
  WindowTypeComboBox = new wxComboBox(this, -1, wxEmptyString, wxDefaultPosition,
                                      wxDefaultSize,    // wxSize (200, -1),
                                      WXSIZEOF(WindowChoices), WindowChoices, wxCB_READONLY);
  WindowTypeComboBox->SetSelection(0);    // Set the default selection
  HSizer6->Add(WindowTypeComboBox, 0, wxALIGN_LEFT | wxALL, 0);

  // This spin box requests the number of poly-phase filter banks
  HSizer7->Add(new wxStaticText(this, -1, "Poly-phase filter banks : "), 0, wxALIGN_TOP | wxALL, 0);
  PolyPhaseSpinControl = new wxSpinCtrl(this, -1, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, MAX_FIR_PP_BANKS, 1);
  PolyPhaseSpinControl->SetValue(1);    // Set default value
  HSizer7->Add(PolyPhaseSpinControl);

  // This spin box requests the number of filter coefficients
  HSizer8->Add(new wxStaticText(this, -1, "Number of coefficients : "), 0, wxALIGN_TOP | wxALL, 0);
  NumCoeffsSpinControl = new wxSpinCtrl(this, -1, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 2, MAX_FIR_LENGTH, 1);
  NumCoeffsSpinControl->SetValue(1);    // Set default value
  HSizer8->Add(NumCoeffsSpinControl);

  // This button calculates the filter and displays the graph
  wxButton* CalculateButton = new wxButton(this, FIRWIN_GET_FILTER_SPEC, "Calculate");
  HSizer9->Add(CalculateButton, 0, wxALIGN_CENTER | wxALL, 0);
  CalculateButton->SetDefault();
  //  SetDefaultItem (CalculateButton);

  FilterBand1FLowLine->SetFocus();              // give the FilterBand1FLowLine the focus at the beginning
  FilterBand1FLowLine->SetSelection(-1, -1);    // Select entire string

  SetAutoLayout(TRUE);
  SetSizer(VSizer);

  VSizer->SetSizeHints(this);
  VSizer->Fit(this);

  // Debug
  //  FilterBand1FLowLine->SetValue ("0.0");
  //  FilterBand1FHighLine->SetValue ("2400");
  //  FilterBand1GainLine->SetValue ("1");
  //  NumCoeffsSpinControl->SetValue (61);      // Set default value

  //  wxMessageBox (_T("Got here"),
  //  "FIRWinFilterDesignWindow::FIRWinFilterDesignWindow", wxOK |
  //  wxICON_EXCLAMATION, NULL);
}

/*
  Name : FIRWinFilterDesignWindow::GetFIRFilterSpec
  Description : Get the filter spec. from the user entries
  Notes :
*/

void FIRWinFilterDesignWindow::OnGetFilterSpec(wxCommandEvent& WXUNUSED(event))

{
  //  wxMessageBox (_T("Got here"), "FIRWinFilterDesignWindow::OnGetFilterSpec",
  //  wxOK | wxICON_EXCLAMATION, NULL);
  SUF_Log("FIRWinFilterDesignWindow::OnGetFilterSpec\n");

  char* stopstring;
  SLArrayIndex_t i, j, k;

  NumberOfFilterBands = 0;

  if ((strtod(FilterBand1FLowLine->GetValue(), &stopstring) != 0.0) || (strtod(FilterBand1FHighLine->GetValue(), &stopstring) != 0.0)) {
    pFIRWLow[NumberOfFilterBands] = strtod(FilterBand1FLowLine->GetValue(), &stopstring);
    pFIRWHigh[NumberOfFilterBands] = strtod(FilterBand1FHighLine->GetValue(), &stopstring);
    pFIRGain[NumberOfFilterBands] = strtod(FilterBand1GainLine->GetValue(), &stopstring);

    NumberOfFilterBands++;
  }

  // Debug
  //  char String[20];
  //  sprintf (String, "FilterBand1FLowLine = %1.20lf", pFIRWLow [0]);
  //  wxMessageBox (String, "OnGetFilterSpec", wxOK | wxICON_EXCLAMATION, NULL);
  //  sprintf (String, "FilterBand1FHighLine = %1.20lf", pFIRWHigh [0]);
  //  wxMessageBox (String, "OnGetFilterSpec", wxOK | wxICON_EXCLAMATION, NULL);
  //  sprintf (String, "FilterBand1GainLine = %1.20lf", pFIRGain [0]);
  //  wxMessageBox (String, "OnGetFilterSpec", wxOK | wxICON_EXCLAMATION, NULL);

  if ((strtod(FilterBand2FLowLine->GetValue(), &stopstring) != 0.0) || (strtod(FilterBand2FHighLine->GetValue(), &stopstring) != 0.0)) {
    pFIRWLow[NumberOfFilterBands] = strtod(FilterBand2FLowLine->GetValue(), &stopstring);
    pFIRWHigh[NumberOfFilterBands] = strtod(FilterBand2FHighLine->GetValue(), &stopstring);
    pFIRGain[NumberOfFilterBands] = strtod(FilterBand2GainLine->GetValue(), &stopstring);

    NumberOfFilterBands++;
  }

  if ((strtod(FilterBand3FLowLine->GetValue(), &stopstring) != 0.0) || (strtod(FilterBand3FHighLine->GetValue(), &stopstring) != 0.0)) {
    pFIRWLow[NumberOfFilterBands] = strtod(FilterBand3FLowLine->GetValue(), &stopstring);
    pFIRWHigh[NumberOfFilterBands] = strtod(FilterBand3FHighLine->GetValue(), &stopstring);
    pFIRGain[NumberOfFilterBands] = strtod(FilterBand3GainLine->GetValue(), &stopstring);

    NumberOfFilterBands++;
  }

  if ((strtod(FilterBand4FLowLine->GetValue(), &stopstring) != 0.0) || (strtod(FilterBand4FHighLine->GetValue(), &stopstring) != 0.0)) {
    pFIRWLow[NumberOfFilterBands] = strtod(FilterBand4FLowLine->GetValue(), &stopstring);
    pFIRWHigh[NumberOfFilterBands] = strtod(FilterBand4FHighLine->GetValue(), &stopstring);
    pFIRGain[NumberOfFilterBands] = strtod(FilterBand4GainLine->GetValue(), &stopstring);

    NumberOfFilterBands++;
  }

  if ((strtod(FilterBand5FLowLine->GetValue(), &stopstring) != 0.0) || (strtod(FilterBand5FHighLine->GetValue(), &stopstring) != 0.0)) {
    pFIRWLow[NumberOfFilterBands] = strtod(FilterBand5FLowLine->GetValue(), &stopstring);
    pFIRWHigh[NumberOfFilterBands] = strtod(FilterBand5FHighLine->GetValue(), &stopstring);
    pFIRGain[NumberOfFilterBands] = strtod(FilterBand5GainLine->GetValue(), &stopstring);

    NumberOfFilterBands++;
  }

  for (i = 0; i < NumberOfFilterBands; i++) {
    if ((pFIRWLow[i] < SIGLIB_ZERO) || (pFIRWLow[i] > (SampleRate / SIGLIB_TWO)) || (pFIRWHigh[i] < SIGLIB_ZERO) ||
        (pFIRWHigh[i] > (SampleRate / SIGLIB_TWO))) {
      wxMessageBox("The frequency limits must be greater than 0Hz\n and less "
                   "than the Nyquist frequency",
                   "Input error", wxOK | wxICON_EXCLAMATION, NULL);
      return;
    }
  }
  for (i = 0; i < NumberOfFilterBands; i++) {
    if (pFIRGain[i] < SIGLIB_ZERO) {
      wxMessageBox("The gain values must be greater than zero", "Input error", wxOK | wxICON_EXCLAMATION, NULL);
      return;
    }
  }

  if (NumberOfFilterBands == 0) {
    wxMessageBox("You must specify at least one filter band", "Input error", wxOK | wxICON_EXCLAMATION, NULL);
    return;
  }

  NumberOfPPFilterBanks = (SLArrayIndex_t)PolyPhaseSpinControl->GetValue();    // Get the number of filter banks
  NumberOfFilterCoeffs = NumCoeffsSpinControl->GetValue();                     // Get the number of coefficients

  // Verify filter order
  if (NumberOfFilterCoeffs < 1) {
    NumCoeffsSpinControl->SetValue(1);
    NumberOfFilterCoeffs = 1;
  }

  if (NumberOfFilterCoeffs > MAX_FIR_LENGTH) {
    NumCoeffsSpinControl->SetValue(MAX_FIR_LENGTH);
    NumberOfFilterCoeffs = MAX_FIR_LENGTH;
  }

  if (NumberOfPPFilterBanks > NumberOfFilterCoeffs) {
    wxMessageBox("The number of poly-phase filter banks must be\nless than the "
                 "number of coefficients",
                 "Input error", wxOK | wxICON_EXCLAMATION, NULL);
    return;
  }

  //  Debug
  //  char String[20];
  //  sprintf (String, "# filter bands = %d", NumberOfFilterBands);
  //  wxMessageBox (String, "OnGetFilterSpec", wxOK | wxICON_EXCLAMATION, NULL);
  //  sprintf (String, "# PP filter banks = %d", NumberOfPPFilterBanks);
  //  wxMessageBox (String, "OnGetFilterSpec", wxOK | wxICON_EXCLAMATION, NULL);
  //  sprintf (String, "# coeffs = %d", NumberOfFilterCoeffs);
  //  wxMessageBox (String, "OnGetFilterSpec", wxOK | wxICON_EXCLAMATION, NULL);

  WindowType = (SLArrayIndex_t)WindowTypeComboBox->GetSelection();    // Get the window type

  static char FileNameString[500];
  FILE* pFile;

  strcpy(FileNameString, ProjectNameString);
  strcat(FileNameString, ".dfs");

  if ((pFile = fopen(FileNameString, "w")) == NULL) /* Open o/p file */
  {
    wxMessageBox("Could not open file to write filter specification", "Filter Specification", wxOK | wxICON_EXCLAMATION, NULL);
    return;
  }

  fprintf(pFile, DFPLUS_COPYRIGHT_NOTICE);
  fprintf(pFile, "File : %s\n\n", FileNameString);

  fprintf(pFile, "[Design Type]\n");
  fprintf(pFile, "FIR-Window\n\n");

  fprintf(pFile, "[Sample rate]\n");
  fprintf(pFile, "%1.20lf\n\n", SampleRate);

  fprintf(pFile, "[Frequency bands]\n");
  fprintf(pFile, "%d\n\n", NumberOfFilterBands);

  fprintf(pFile, "[Number of coefficients]\n");
  fprintf(pFile, "%d\n\n", NumberOfFilterCoeffs);

  fprintf(pFile, "[Window]\n");
  if (WindowType == SIGLIB_HANNING)
    fprintf(pFile, "HANNING\n\n");
  else if (WindowType == SIGLIB_HAMMING)
    fprintf(pFile, "HAMMING\n\n");
  else if (WindowType == SIGLIB_BLACKMAN)
    fprintf(pFile, "BLACKMAN\n\n");
  else if (WindowType == SIGLIB_BARTLETT_TRIANGLE_ZERO_END_POINTS)
    fprintf(pFile, "BARTLETT TRIANGLE ZERO END POINTS\n\n");
  else if (WindowType == SIGLIB_BARTLETT_TRIANGLE_NON_ZERO_END_POINTS)
    fprintf(pFile, "BARTLETT TRIANGLE NON ZERO END POINTS\n\n");
  else if (WindowType == SIGLIB_KAISER)
    fprintf(pFile, "KAISER\n\n");
  else if (WindowType == SIGLIB_BLACKMAN_HARRIS)
    fprintf(pFile, "BLACKMAN_HARRIS\n\n");
  else if (WindowType == SIGLIB_RECTANGLE)
    fprintf(pFile, "RECTANGLE\n\n");

  fprintf(pFile, "[Frequency specification]\n");

  /* Copy spec to file */
  for (i = 0; i < NumberOfFilterBands; i++) {
    fprintf(pFile,
            "FIRWLow[%d] = %1.20lf, FIRWHigh[%d] = %1.20lf, FIRGain[%d] = "
            "%1.20lf\n",
            i, pFIRWLow[i], i, pFIRWHigh[i], i, pFIRGain[i]);
  }

  fprintf(pFile, "\n\n");

  (void)fclose(pFile); /* Close output file */

  for (i = 0; i < NumberOfFilterBands; i++) {
    pFIRWHigh[i] *= (SIGLIB_TWO_PI / SampleRate);
    pFIRWLow[i] *= (SIGLIB_TWO_PI / SampleRate);
  }

  // Calculate coefficients
  SDA_Clear(pFIRCoeffs, NumberOfFilterCoeffs);

  SLArrayIndex_t Alpha = (NumberOfFilterCoeffs - 1) >> 1; /* Alpha is the middle tap */
  for (i = 0; i < NumberOfFilterCoeffs; i++) {
    for (j = 0; j < NumberOfFilterBands; j++) {
      if (i != Alpha) {
        pFIRCoeffs[i] += ((pFIRGain[j] / (SIGLIB_PI * ((SLData_t)(i - Alpha)))) *
                          (SDS_Sin(pFIRWHigh[j] * ((SLData_t)(i - Alpha))) - SDS_Sin(pFIRWLow[j] * ((SLData_t)(i - Alpha)))));
      } else {
        pFIRCoeffs[i] += (SIGLIB_INV_PI * pFIRGain[j] * (pFIRWHigh[j] - pFIRWLow[j]));
      }
    }
  }

  SLData_t* pWindow = (SLData_t*)malloc(NumberOfFilterCoeffs * sizeof(SLData_t));
  if (pWindow == NULL) {
    wxMessageBox("Could not allocate window memory", "Filter Specification", wxOK | wxICON_EXCLAMATION, NULL);
    return;
  }

  SIF_Window(pWindow, (const enum SLWindow_t)WindowType, SIGLIB_ONE, NumberOfFilterCoeffs);
  SDA_Window(pFIRCoeffs, pFIRCoeffs, pWindow, NumberOfFilterCoeffs);
  free(pWindow);

  strcpy(FileNameString, ProjectNameString);
  strcat(FileNameString, ".dfc");

  if ((pFile = fopen(FileNameString, "w")) == NULL) /* Open o/p file */
  {
    wxMessageBox("Could not open file to write filter coefficients", "Filter Specification", wxOK | wxICON_EXCLAMATION, NULL);
    return;
  }

  fprintf(pFile, DFPLUS_COPYRIGHT_NOTICE);
  fprintf(pFile, "File : %s\n\n", FileNameString);
  fprintf(pFile, "FIR filter coefficients designed with the Windowing Algorithm\n\n");

  // Write floating point coefficients to file
  fprintf(pFile, "\t/* Floating point coefficients */\n\n");
  fprintf(pFile, "#define FILTER_LENGTH %d\n\n", NumberOfFilterCoeffs);
  fprintf(pFile, "const SLData_t CoefficientArray [] =\n{\n");

  for (i = 0; i < NumberOfFilterCoeffs; i++) {
    fprintf(pFile, "\t%1.20le", pFIRCoeffs[i]);

    if (i < ((short)NumberOfFilterCoeffs - 1))
      fprintf(pFile, ",\n");
  }

  fprintf(pFile, "\n};\n\n");

  fprintf(pFile, "\n`~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~'\n\n");

  // Write Q format fixed point coefficients to file
  fprintf(pFile, "\t/* Q format fixed point coefficients */\n");
  fprintf(pFile, "\t\t/* m = %d, n = %d */\n\n", global_mValue, global_nValue);
  fprintf(pFile, "#define FILTER_LENGTH %d\n\n", NumberOfFilterCoeffs);
  fprintf(pFile, "const SLData_t CoefficientArray [] =\n{\n");

  for (i = 0; i < NumberOfFilterCoeffs; i++) {
    fprintf(pFile, "\t0x%llx", FloatToQFormatInteger(pFIRCoeffs[i], global_mValue, global_nValue));

    if (i < ((short)NumberOfFilterCoeffs - 1))
      fprintf(pFile, ",\n");
  }

  fprintf(pFile, "\n};\n\n");

  fprintf(pFile, "\n`~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~'\n\n");

  // Write 16 bit fixed point coefficients to file
  fprintf(pFile, "\t/* 16 bit fixed point coefficients */\n\n");
  fprintf(pFile, "#define FILTER_LENGTH %d\n\n", NumberOfFilterCoeffs);
  fprintf(pFile, "const SLData_t CoefficientArray [] =\n{\n");

  for (i = 0; i < NumberOfFilterCoeffs; i++) {
    fprintf(pFile, "\t0x%x", 0xffff & (short)(pFIRCoeffs[i] * 32767.0));

    if (i < ((short)NumberOfFilterCoeffs - 1))
      fprintf(pFile, ",\n");
  }

  fprintf(pFile, "\n};\n\n");

  // Save floating point poly-phase filter banks to coeff file
  if (NumberOfPPFilterBanks > 1) {
    fprintf(pFile, "\n`~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~'\n\n");
    fprintf(pFile, "\t/* Floating point poly-phase filter coefficients */\n");

    for (i = 0; i < NumberOfPPFilterBanks; i++) {
      fprintf(pFile, "const SLData_t CoefficientArray%d [] =\n{\n", (short)i);

      for (j = i, k = 0; j < NumberOfFilterCoeffs; j++) {
        if (k == 0)
          fprintf(pFile, "\t%1.20le,\n", pFIRCoeffs[j]);
        k++;
        k %= NumberOfPPFilterBanks;
      }

      fprintf(pFile, "\n};\n\n");
    }
  }

  // Save Q format fixed point poly-phase filter banks to coeff file
  if (NumberOfPPFilterBanks > 1) {
    fprintf(pFile, "\n`~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~'\n\n");
    fprintf(pFile, "\t/* Q format fixed point poly-phase filter coefficients */\n");
    fprintf(pFile, "\t\t/* m = %d, n = %d */\n\n", global_mValue, global_nValue);

    for (i = 0; i < NumberOfPPFilterBanks; i++) {
      fprintf(pFile, "const SLData_t CoefficientArray%d [] =\n{\n", (short)i);

      for (j = i, k = 0; j < NumberOfFilterCoeffs; j++) {
        if (k == 0)
          fprintf(pFile, "\t0x%llx,\n", FloatToQFormatInteger(pFIRCoeffs[j], global_mValue, global_nValue));
        k++;
        k %= NumberOfPPFilterBanks;
      }

      fprintf(pFile, "};\n\n");
    }
  }

  // Save fixed point poly-phase filter banks to coeff file
  if (NumberOfPPFilterBanks > 1) {
    fprintf(pFile, "\n`~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~'\n\n");
    fprintf(pFile, "\t/* 16 bit fixed point poly-phase filter coefficients */\n");

    for (i = 0; i < NumberOfPPFilterBanks; i++) {
      fprintf(pFile, "const SLData_t CoefficientArray%d [] =\n{\n", (short)i);

      for (j = i, k = 0; j < NumberOfFilterCoeffs; j++) {
        if (k == 0)
          fprintf(pFile, "\t0x%x,\n", (short)(pFIRCoeffs[j] * 32767.0));
        k++;
        k %= NumberOfPPFilterBanks;
      }

      fprintf(pFile, "\n};\n\n");
    }
  }

  (void)fclose(pFile);

  SLArrayIndex_t GC;
  SLData_t FreqStep = SIGLIB_ZERO;
  SLData_t Freq = SIGLIB_ZERO;
  SLArrayIndex_t FirstCalc = SIGLIB_TRUE;    // Indicates first calculation of group delay

  SLData_t ThetaT = SIGLIB_ZERO;
  SLData_t X, JY;
  SLData_t DBGain = SIGLIB_ZERO;
  SLData_t GPDel = SIGLIB_ZERO;

  /* Only calculate up to Nyquist point */
  FreqStep = SampleRate / (SIGLIB_TWO * GRAPH_WIDTH);

  for (Freq = SIGLIB_ZERO, GC = 0; GC < GRAPH_WIDTH; Freq += FreqStep, GC++) {
    for (X = SIGLIB_ZERO, JY = SIGLIB_ZERO, i = 0; i < NumberOfFilterCoeffs; i++) {
      X += (pFIRCoeffs[i] * SDS_Cos((SIGLIB_TWO_PI / SampleRate) * Freq * ((SLData_t)i)));
      JY -= (pFIRCoeffs[i] * SDS_Sin((SIGLIB_TWO_PI / SampleRate) * Freq * ((SLData_t)i)));
    }

    if (X < SIGLIB_ZERO)    // SDS_Sign (X) == -1)
    {
      ThetaT = (SDS_Atan(JY / X)) + (SDS_Sign(JY) * SIGLIB_PI);
    }

    else if (X > SIGLIB_ZERO) {
      ThetaT = SDS_Atan(JY / X);
    }

    else    // X == 0.0
    {
      //      ThetaT = (SIGLIB_HALF_PI) * SDS_Sign (JY);
      if (JY < SIGLIB_ZERO) {
        ThetaT = -(SIGLIB_HALF_PI);
      } else {
        ThetaT = SIGLIB_HALF_PI;
      }
    }

    GPDel = df_group_delay(FreqStep, FirstCalc, ThetaT);
    FirstCalc = SIGLIB_FALSE;

    if ((X == SIGLIB_ZERO) && (JY == SIGLIB_ZERO)) {
      DBGain = -SIGLIB_ONE_HUNDRED;
    } else {
      /* 10 log10 - was 20 log10 */
      //      DBGain = SIGLIB_TEN * 0.4342944819 *
      //      SDS_Log(SDS_Sqrt((X*X)+(JY*JY)));
      DBGain = SIGLIB_TEN * 0.4342944819 * SDS_Log((X * X) + (JY * JY));
    }

    pGain[GC] = DBGain;
    pPhase[GC] = ThetaT;
    pGroupDelay[GC] = GPDel;
  }

  /* Copy coefficients to impulse response display array */
  if (NumberOfFilterCoeffs > GRAPH_WIDTH) {
    SDA_Copy(pFIRCoeffs, pImpulseResponse, GRAPH_WIDTH);
  } else {
    SDA_Clear(pImpulseResponse, GRAPH_WIDTH);    // Clear arrays
    SDA_Copy(pFIRCoeffs, pImpulseResponse, NumberOfFilterCoeffs);
  }

  SLData_t IntegralSum = SIGLIB_ZERO;
  SDA_Integrate(pImpulseResponse, pStepResponse, SIGLIB_MAX, SIGLIB_ONE, &IntegralSum, GRAPH_WIDTH);    // Calculate step response

  pFilterInfo->FIRFilterOrder = NumberOfFilterCoeffs;
  pFilterInfo->IIRFilterOrder = 0;

  strcpy(FileNameString, ProjectNameString);
  strcat(FileNameString, ".dfz");
  if ((pFile = fopen(FileNameString, "w")) == NULL) /* Open o/p file */
  {
    wxMessageBox("Could not open file to write filter coefficients", "OnGetFilterSpec", wxOK | wxICON_EXCLAMATION, NULL);
    return;
  }
  fprintf(pFile, DFPLUS_COPYRIGHT_NOTICE);
  fprintf(pFile, "File : %s\n\n", FileNameString);
  fprintf(pFile, "This functionality is only available for IIR filters\n\n");
  (void)fclose(pFile);

  wxCommandEvent* DummyCommand = new wxCommandEvent();    // Post event to say that coeffs have been updated
  GlobalGraphDisplayWindow->OnGraphTypeChanged(*DummyCommand);
  delete (DummyCommand);

#if DEBUG_LOG_FILE
  SUF_Debugfprintf("FIRWinFilterDesignWindow::OnGetFilterSpec : Completed all "
                   "processing\n\n");
#endif
} /* End of FIRWinFilterDesignWindow::OnGetFilterSpec () */

void FIRWinFilterDesignWindow::SetProjectNameString(wxString String)
{
  ProjectNameString = String;
}

void FIRWinFilterDesignWindow::SetSampleRate(double sampleRate)
{
  SampleRate = sampleRate;
}

void FIRWinFilterDesignWindow::SetGainArray(SLData_t* gain)
{
  pGain = gain;
}

void FIRWinFilterDesignWindow::SetPhaseArray(SLData_t* phase)
{
  pPhase = phase;
}

void FIRWinFilterDesignWindow::SetGroupDelayArray(SLData_t* groupDelay)
{
  pGroupDelay = groupDelay;
}

void FIRWinFilterDesignWindow::SetImpulseResponseArray(SLData_t* impulseResponse)
{
  pImpulseResponse = impulseResponse;
}

void FIRWinFilterDesignWindow::SetStepResponseArray(SLData_t* stepResponse)
{
  pStepResponse = stepResponse;
}

void FIRWinFilterDesignWindow::SetFilterInfoStruct(filter_info_t* filterInfo)
{
  pFilterInfo = filterInfo;
}

BEGIN_EVENT_TABLE(FIRWinFilterDesignWindow, wxPanel)
EVT_BUTTON(FIRWIN_GET_FILTER_SPEC, FIRWinFilterDesignWindow::OnGetFilterSpec)
END_EVENT_TABLE()
