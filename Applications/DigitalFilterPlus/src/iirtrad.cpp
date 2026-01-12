// Traditional IIR digital filter design functions
//   Bilinear Transform
//   Matched z-Transform
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
#include "iir_gen.h"

void BesselFilter(filter_t* FilterSpecification);    // Bessel filter design function
#include "bessel.cpp"

extern GraphDisplay* GlobalGraphDisplayWindow;

extern int global_mValue;    // Global m and n values for Q number format
extern int global_nValue;
extern int global_NegateIIRAlphaSign;    // Global NegateIIRAlphaSign for biquads

/*
  Name : IIRTraditionalFilterDesignWindow::IIRTraditionalFilterDesignWindow
  Description : Child widget that is used to get the IIR BLT filter spec.
  Notes :
*/

IIRTraditionalFilterDesignWindow::IIRTraditionalFilterDesignWindow(wxWindow* parent, wxWindowID id)
    : wxPanel(parent, id)

{
  // Top-level layout - a vertical box sizer to contain all the controls
  wxStaticBoxSizer* VSizer = new wxStaticBoxSizer(wxVERTICAL, this, "Traditional IIR Filter Design");

  wxBoxSizer* HSizerP1 = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizer1 = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizerP2 = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizer2 = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizer2P = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizerP3 = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizer31 = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizer32 = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizer33 = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizer34 = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizerP4 = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizer4 = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizer5 = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizerP6 = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizer6 = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizerP7 = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizer7 = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizerP8 = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizer8 = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizerP9 = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizer9 = new wxBoxSizer(wxHORIZONTAL);

  VSizer->Add(HSizerP1, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add(HSizer1, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add(HSizerP2, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add(HSizer2, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add(HSizer2P, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add(HSizerP3, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add(HSizer31, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add(HSizer32, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add(HSizer33, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add(HSizer34, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add(HSizerP4, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add(HSizer4, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add(HSizer5, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add(HSizerP6, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add(HSizer6, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add(HSizerP7, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add(HSizer7, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add(HSizerP8, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add(HSizer8, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add(HSizerP9, 0, wxALIGN_CENTER | wxALL, 2);
  VSizer->Add(HSizer9, 0, wxALIGN_CENTER | wxALL, 2);
  VSizer->Add(0, 0, 1);    // Add a spacer to get the alignment right

  // A combo box for getting filter band type
  HSizer1->Add(new wxStaticText(this, -1, "Filter type : "), 0, wxALIGN_TOP | wxALL, 0);
  static const wxString FreqChoices[] = {"Low pass", "High pass", "Band pass", "Band stop"};
  FilterBandComboBox =
      new wxComboBox(this, IIRBLT_FILTER_BAND_COMBO_BOX, wxEmptyString, wxDefaultPosition, wxDefaultSize, WXSIZEOF(FreqChoices), FreqChoices,
                     //            wxCB_READONLY | wxTE_PROCESS_ENTER);
                     wxCB_READONLY);
  FilterBandComboBox->SetSelection(0);    // Set the default selection
  HSizer1->Add(FilterBandComboBox, 0, wxALIGN_LEFT | wxALL, 0);

  // A combo box for getting the filter type
  HSizer2->Add(new wxStaticText(this, -1, "Design Method : "), 0, wxALIGN_TOP | wxALL, 0);
  static const wxString DesignMethodChoices[] = {"Butterworth", "Chebyschev", "Inv. Chebyschev", "Elliptic", "Bessel"};
  DesignMethodComboBox = new wxComboBox(this, IIRBLT_FILTER_TYPE_COMBO_BOX, wxEmptyString, wxDefaultPosition, wxDefaultSize,
                                        WXSIZEOF(DesignMethodChoices), DesignMethodChoices, wxCB_READONLY);
  DesignMethodComboBox->SetSelection(0);    // Set the default selection
  HSizer2->Add(DesignMethodComboBox, 0, wxALIGN_LEFT | wxALL, 0);

  // A combo box for getting the translation type
  HSizer2P->Add(new wxStaticText(this, -1, "Translation type : "), 0, wxALIGN_TOP | wxALL, 0);
  static const wxString TranslationChoices[] = {"Bilinear Transform", "Matched z-Transform"};
  TranslationMethodComboBox = new wxComboBox(this, IIRBLT_TRANSLATION_TYPE_COMBO_BOX, wxEmptyString, wxDefaultPosition, wxDefaultSize,
                                             WXSIZEOF(TranslationChoices), TranslationChoices, wxCB_READONLY);
  TranslationMethodComboBox->SetSelection(0);    // Set the default selection
  HSizer2P->Add(TranslationMethodComboBox, 0, wxALIGN_LEFT | wxALL, 0);

  // Insert the LineEdit entry boxes filter band edges
  HSizer31->Add(new wxStaticText(this, -1, "Band Edge 1 : "), 0, wxALIGN_TOP | wxALL, 0);
  BandEdge1Line = new wxTextCtrl(this, IIRBLT_BE1, "0.0", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
  BandEdge1Line->SetSize(wxSize(90, -1));
  HSizer31->Add(BandEdge1Line);

  HSizer32->Add(new wxStaticText(this, -1, "Band Edge 2 : "), 0, wxALIGN_TOP | wxALL, 0);
  BandEdge2Line = new wxTextCtrl(this, IIRBLT_BE2, "0.0", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
  BandEdge2Line->SetSize(wxSize(90, -1));
  HSizer32->Add(BandEdge2Line);

  HSizer33->Add(new wxStaticText(this, -1, "Band Edge 3 : "), 0, wxALIGN_TOP | wxALL, 0);
  BandEdge3Line = new wxTextCtrl(this, IIRBLT_BE3, "0.0", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
  BandEdge3Line->SetSize(wxSize(90, -1));
  HSizer33->Add(BandEdge3Line);

  HSizer34->Add(new wxStaticText(this, -1, "Band Edge 4 : "), 0, wxALIGN_TOP | wxALL, 0);
  BandEdge4Line = new wxTextCtrl(this, IIRBLT_BE4, "0.0", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
  BandEdge4Line->SetSize(wxSize(90, -1));
  HSizer34->Add(BandEdge4Line);

  //  Pass band ripple
  HSizer4->Add(new wxStaticText(this, -1, "Pass-band Ripple (dB): "), 0, wxALIGN_TOP | wxALL, 0);
  PassBandRippleLine = new wxTextCtrl(this, IIRBLT_PBR, "0.0", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
  PassBandRippleLine->SetSize(wxSize(76, -1));
  HSizer4->Add(PassBandRippleLine);

  //  Stop band attenuation
  HSizer5->Add(new wxStaticText(this, -1, "Stop-band Atten. (dB): "), 0, wxALIGN_TOP | wxALL, 0);
  StopBandAttenLine = new wxTextCtrl(this, IIRBLT_SBA, "0.0", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
  StopBandAttenLine->SetSize(wxSize(78, -1));
  HSizer5->Add(StopBandAttenLine);

  // This spin box requests the IIR filter order
  HSizer6->Add(new wxStaticText(this, -1, "Filter order : "), 0, wxALIGN_TOP | wxALL, 0);
  FilterOrderSpinBox =
      new wxSpinCtrl(this, IIRBLT_FILTER_ORDER_SPINCTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, MAX_BLT_ORDER);
  FilterOrderSpinBox->SetValue(1);    // Set default value
  HSizer6->Add(FilterOrderSpinBox);

  // Insert a CheckBox for the gain scaling
  HSizer7->Add(new wxStaticText(this, -1, "0 dB Pass-band scaling :  "), 0, wxALIGN_TOP | wxALL, 0);
  ScaleGainCheckBox = new wxCheckBox(this, -1, _T(" &Enable"));
  ScaleGainCheckBox->SetValue(TRUE);
#if wxUSE_TOOLTIPS
  ScaleGainCheckBox->SetToolTip(_T("Click here to scale the pass-band gain to 0 dB"));
#endif    // wxUSE_TOOLTIPS
  HSizer7->Add(ScaleGainCheckBox);

  // Insert a CheckBox for the pre-warping
  HSizer8->Add(new wxStaticText(this, -1, "Pre-warping :                    "), 0, wxALIGN_TOP | wxALL, 0);
  PreWarpingCheckBox = new wxCheckBox(this, -1, _T(" &Enable"));
  PreWarpingCheckBox->SetValue(TRUE);
#if wxUSE_TOOLTIPS
  PreWarpingCheckBox->SetToolTip(_T("Click here to enable pre-warping with the bilinear transform"));
#endif    // wxUSE_TOOLTIPS
  HSizer8->Add(PreWarpingCheckBox);

  // This button calculates the filter and displays the graph
  wxButton* CalculateButton = new wxButton(this, IIRBLT_GET_FILTER_SPEC, "Calculate");
  HSizer9->Add(CalculateButton, 0, wxALIGN_CENTER | wxALL, 0);
  CalculateButton->SetDefault();
  //  SetDefaultItem (CalculateButton);

  FilterBandComboBox->SetFocus();    // give the Pole1Magn the focus at the beginning

  SetAutoLayout(TRUE);
  SetSizer(VSizer);

  VSizer->SetSizeHints(this);
  VSizer->Fit(this);

  // Initialise for low pass Butterworth filter input
  BandEdge1Line->Enable(TRUE);
  BandEdge2Line->Enable(FALSE);
  BandEdge3Line->Enable(FALSE);
  BandEdge4Line->Enable(FALSE);
  PassBandRippleLine->Enable(FALSE);
  StopBandAttenLine->Enable(FALSE);
  FilterOrderSpinBox->Enable(TRUE);

  // Debug
  //  BandEdge1Line->SetValue ("1000.0");
  //  BandEdge2Line->SetValue ("2000.0");
  //  BandEdge3Line->SetValue ("3000.0");
  //  BandEdge4Line->SetValue ("4000.0");
  //  PassBandRippleLine->SetValue ("3.0");
  //  StopBandAttenLine->SetValue ("70.0");
}

/*
  Name : IIRTraditionalFilterDesignWindow::OnFilterSpecChanged
  Description : Enable / disable appropriate filter band edges etc.
  Notes :
*/

void IIRTraditionalFilterDesignWindow::OnFilterSpecChanged(wxCommandEvent& WXUNUSED(event))

{
  //  wxMessageBox ("Got Here",
  //  "IIRTraditionalFilterDesignWindow::OnFilterSpecChanged", wxOK |
  //  wxICON_EXCLAMATION, NULL);
  SUF_Log("IIRTraditionalFilterDesignWindow::OnFilterSpecChanged\n");

  // If we are using the bilinear transform then enable pre-warp option
  if (TranslationMethodComboBox->GetSelection() == IIR_BILINEAR)
    PreWarpingCheckBox->Enable(TRUE);
  else
    PreWarpingCheckBox->Enable(FALSE);

  switch (FilterBandComboBox->GetSelection())    // Range for BSF and BPF are half
  {
  case IIR_LPF: /* Low pass / High pass */
  case IIR_HPF:
    if (DesignMethodComboBox->GetSelection() == IIR_BESSEL)
      FilterOrderSpinBox->SetRange(1, MAX_BESSEL_ORDER);             // Set range
    else                                                             // Not Bessel
      FilterOrderSpinBox->SetRange(1, MAX_BLT_ORDER);                // Set range
    FilterOrderSpinBox->SetValue(FilterOrderSpinBox->GetValue());    // Refresh contents
    break;
  case IIR_BPF: /* Band pass / Notch */
  case IIR_BSF:
    if (DesignMethodComboBox->GetSelection() == IIR_BESSEL)
      FilterOrderSpinBox->SetRange(1, MAX_BESSEL_ORDER);             // Set range
    else                                                             // Not Bessel
      FilterOrderSpinBox->SetRange(1, MAX_BLT_ORDER >> 1);           // Set range
    FilterOrderSpinBox->SetValue(FilterOrderSpinBox->GetValue());    // Refresh contents
    break;
  }

  if (DesignMethodComboBox->GetSelection() != IIR_ELLIPTIC)    // Filter is not Elliptic type
  {
    switch (FilterBandComboBox->GetSelection()) {
    case IIR_LPF: /* Low pass / High pass */
    case IIR_HPF:
      BandEdge1Line->Enable(TRUE);
      BandEdge2Line->Enable(FALSE);
      BandEdge3Line->Enable(FALSE);
      BandEdge4Line->Enable(FALSE);
      break;
    case IIR_BPF: /* Band pass / Notch */
    case IIR_BSF:
      BandEdge1Line->Enable(TRUE);
      BandEdge2Line->Enable(TRUE);
      BandEdge3Line->Enable(FALSE);
      BandEdge4Line->Enable(FALSE);
      break;
    }

    PassBandRippleLine->Enable(FALSE);
    StopBandAttenLine->Enable(FALSE);
    FilterOrderSpinBox->Enable(TRUE);

    if (DesignMethodComboBox->GetSelection() == IIR_CHEBYSCHEV) {    // Filter is Cheb
      PassBandRippleLine->Enable(TRUE);
    }

    if (DesignMethodComboBox->GetSelection() == IIR_INVERSE_CHEBYSCHEV) {    // Filter is InvCheb
      StopBandAttenLine->Enable(TRUE);
    }
  }

  else {    // Filter is Elliptic type
    switch (FilterBandComboBox->GetSelection()) {
    case IIR_LPF:
    case IIR_HPF:
      BandEdge1Line->Enable(TRUE);
      BandEdge2Line->Enable(TRUE);
      BandEdge3Line->Enable(FALSE);
      BandEdge4Line->Enable(FALSE);
      break;
    case IIR_BPF:
    case IIR_BSF:
      BandEdge1Line->Enable(TRUE);
      BandEdge2Line->Enable(TRUE);
      BandEdge3Line->Enable(TRUE);
      BandEdge4Line->Enable(TRUE);
      break;
    }

    PassBandRippleLine->Enable(TRUE);
    StopBandAttenLine->Enable(TRUE);
    FilterOrderSpinBox->Enable(FALSE);
  }
}

/*
  Name : IIRTraditionalFilterDesignWindow::GetFilterSpec
  Description : Get the filter spec. from the user entries
  Notes :
*/

void IIRTraditionalFilterDesignWindow::OnGetFilterSpec(wxCommandEvent& WXUNUSED(event))

{
  char* stopstring;
  short i;
  SLComplexRect_s *pZPlanePoles, *pZPlaneZeros;
  filter_t FilterSpecification;
  SLComplexPolar_s TmpPolar;
  SLData_t Tmp;
  SLData_t w1, w2, w3, w4, wst;
  SLData_t CentreFreqForScaling;
  SLData_t Fc1, Fc2;    // Cut off frequencies for Bessel filter

  //  wxMessageBox ("Got Here",
  //  "IIRTraditionalFilterDesignWindow::OnGetFilterSpec", wxOK |
  //  wxICON_EXCLAMATION, NULL);
  SUF_Log("IIRTraditionalFilterDesignWindow::OnGetFilterSpec\n");

#if DEBUG_LOG_FILE
  SUF_Debugfprintf("\nvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv\n");
  SUF_Debugfprintf("IIRTraditionalFilterDesignWindow::OnGetFilterSpec : Starting\n");
#endif

  FilterSpecification.BandSpec = (SLArrayIndex_t)FilterBandComboBox->GetSelection();        // Get the fitler type
  FilterSpecification.DesignAlgo = (SLArrayIndex_t)DesignMethodComboBox->GetSelection();    // Get the fitler design type

  // Allocate memory for S-plane poles and zeros
  FilterSpecification.pPoles = SUF_ComplexRectArrayAllocate(2 * MAX_BLT_ORDER);
  FilterSpecification.pZeros = SUF_ComplexRectArrayAllocate(2 * MAX_BLT_ORDER);

  if ((FilterSpecification.pPoles == 0) || (FilterSpecification.pZeros == 0)) {
    wxMessageBox("Could not allocate memory for IIR filter", "Internal error", wxOK | wxICON_EXCLAMATION, NULL);
    return;
  }

  // If the filter is Band pass or band stop then limit the
  // prototype filter length to an even order
  if ((FilterSpecification.BandSpec == IIR_BPF) || (FilterSpecification.BandSpec == IIR_BSF)) {
    FilterOrderSpinBox->SetValue(FilterOrderSpinBox->GetValue() & 0x0fffe);    // Ensure even order
    if (FilterOrderSpinBox->GetValue() < 2)                                    // And > 2
      FilterOrderSpinBox->SetValue(2);
  }

  FilterSpecification.Order = (SLArrayIndex_t)FilterOrderSpinBox->GetValue();    // Filter order
  if (DesignMethodComboBox->GetSelection() == IIR_BESSEL) {
    if (FilterSpecification.Order > MAX_BESSEL_ORDER)    // Ensure in range
    {
      FilterSpecification.Order = MAX_BESSEL_ORDER;
      FilterOrderSpinBox->SetValue(MAX_BESSEL_ORDER);
    }
  } else {                                              // Not Bessel filter
    if (FilterSpecification.Order > MAX_BLT_ORDER) {    // Ensure in range
      FilterSpecification.Order = MAX_BLT_ORDER;
      FilterOrderSpinBox->SetValue(MAX_BLT_ORDER);
    }
  }

#if DEBUG_LOG_FILE
  SUF_Debugfprintf("Sample rate = %1.20lf\n", SampleRate);
#endif

  // Initialise Centre freq. for scaling for LPF, BSF and HPF
  // For BPF centre freq. is initialised differently
  // for each filter design type.
  switch (FilterSpecification.BandSpec) {
  case IIR_LPF:
  case IIR_BSF:
    CentreFreqForScaling = SIGLIB_ZERO;
    break;
  case IIR_HPF:
    CentreFreqForScaling = (SampleRate / SIGLIB_TWO) * 0.99;
    break;
  }

  if (FilterSpecification.DesignAlgo == IIR_BESSEL) {
    switch (FilterSpecification.BandSpec) {
    case IIR_LPF:
    case IIR_HPF:
      Fc1 = strtod(BandEdge1Line->GetValue(), &stopstring);    // Pass band edge

      // Verify filter band edges
      if (Fc1 >= (SampleRate / SIGLIB_TWO)) {
        wxMessageBox("The pass band cut off frequency must be less than the "
                     "Nyquist point",
                     "Input error", wxOK | wxICON_EXCLAMATION, NULL);
        return;
      }
      if (Fc1 <= SIGLIB_ZERO) {
        wxMessageBox("The pass band cut off frequency must be greater than zero", "Input error", wxOK | wxICON_EXCLAMATION, NULL);
        return;
      }

      // If we are using the bilinear transform AND pre-warping is selected
      // then pre-warp the frequencies
      if ((TranslationMethodComboBox->GetSelection() == IIR_BILINEAR) && (PreWarpingCheckBox->IsChecked())) {
        Fc1 = SDS_PreWarp(Fc1, SampleRate);
      }
      break;
    case IIR_BPF:
    case IIR_BSF:
      Fc1 = strtod(BandEdge1Line->GetValue(), &stopstring);    // Lower edge
      Fc2 = strtod(BandEdge2Line->GetValue(), &stopstring);    // Upper edge

      // Verify filter band edges
      if ((Fc1 >= (SampleRate / SIGLIB_TWO)) || (Fc2 >= (SampleRate / SIGLIB_TWO))) {
        wxMessageBox("The two cut off frequencies must be less than the Nyquist point", "Input error", wxOK | wxICON_EXCLAMATION, NULL);
        return;
      }
      if ((Fc1 <= SIGLIB_ZERO) || (Fc2 <= SIGLIB_ZERO)) {
        wxMessageBox("The two cut off frequencies must both be greater than zero", "Input error", wxOK | wxICON_EXCLAMATION, NULL);
        return;
      }
      if (Fc1 >= Fc2) {
        wxMessageBox("The lower cut of frequency must be less than the upper", "Input error", wxOK | wxICON_EXCLAMATION, NULL);
        return;
      }

      // If we are using the bilinear transform AND pre-warping is selected
      // then pre-warp the frequencies
      if ((TranslationMethodComboBox->GetSelection() == IIR_BILINEAR) && (PreWarpingCheckBox->IsChecked())) {
        Fc1 = SDS_PreWarp(Fc1, SampleRate);
        Fc2 = SDS_PreWarp(Fc2, SampleRate);
      }

      w1 = (Fc1 * SIGLIB_TWO_PI) / SampleRate;
      w2 = (Fc2 * SIGLIB_TWO_PI) / SampleRate;

      FilterSpecification.OhmegaZero = SDS_Sqrt(w1 * w2);
      FilterSpecification.LowerBandEdge = (w2 * w2 - FilterSpecification.OhmegaZero * FilterSpecification.OhmegaZero) / w2;
      Fc1 = (FilterSpecification.LowerBandEdge * SampleRate) / SIGLIB_TWO_PI;

      break;
    }

    /* Invert cut-off frequencies for BSF and HPF */
    if (FilterSpecification.BandSpec == IIR_BSF || FilterSpecification.BandSpec == IIR_HPF) {
      Fc1 = (SampleRate * SampleRate) / (SIGLIB_TWO_PI * SIGLIB_TWO_PI * Fc1);
    }

    switch (FilterSpecification.BandSpec) {
    case IIR_LPF:
      CentreFreqForScaling = SIGLIB_ZERO;
      break;
    case IIR_HPF:
      CentreFreqForScaling = (SampleRate / SIGLIB_TWO) * 0.99;
      break;
    case IIR_BPF:
      CentreFreqForScaling = (SampleRate * (w1 + w2)) / (SIGLIB_TWO * SIGLIB_TWO_PI);    // Centre frequency is mid point between two cutoffs
      break;
    case IIR_BSF:
      CentreFreqForScaling = SIGLIB_ZERO;
      break;
    }
  }

  else if (FilterSpecification.DesignAlgo != IIR_ELLIPTIC) {
#if DEBUG_LOG_FILE
    SUF_Debugfprintf("OnFilterSpecChanged : Non Elliptic filter\n");
#endif

    switch (FilterSpecification.BandSpec) {
    case IIR_LPF:
    case IIR_HPF:
      // Filter type : NOT Elliptic - LPF and BPF
      // Cut off freq is limit of pass band
      FilterSpecification.LowerBandEdge = strtod(BandEdge1Line->GetValue(), &stopstring) * (SIGLIB_TWO_PI / SampleRate);    // Pass band edge

      // Verify filter band edges
      if (FilterSpecification.LowerBandEdge >= SIGLIB_PI) {
        wxMessageBox("The pass band cut off frequency must be less than the "
                     "Nyquist point",
                     "Input error", wxOK | wxICON_EXCLAMATION, NULL);
        return;
      }
      if (FilterSpecification.LowerBandEdge <= SIGLIB_ZERO) {
        wxMessageBox("The pass band cut off frequency must be greater than zero", "Input error", wxOK | wxICON_EXCLAMATION, NULL);
        return;
      }

      // If we are using the bilinear transform AND pre-warping is selected
      // then pre-warp the frequencies
      if ((TranslationMethodComboBox->GetSelection() == IIR_BILINEAR) && (PreWarpingCheckBox->IsChecked())) {
        FilterSpecification.LowerBandEdge = SDS_PreWarp(FilterSpecification.LowerBandEdge, SIGLIB_TWO_PI);
      }
      break;
    case IIR_BPF:
    case IIR_BSF:
#if DEBUG_LOG_FILE
      SUF_Debugfprintf("OnFilterSpecChanged : Non Elliptic filter - BPF and BSF\n");
#endif
      // Filter type : NOT Elliptic - BPF and BSF
      // Cut off frequencies are limits of pass band
      w1 = strtod(BandEdge1Line->GetValue(), &stopstring) * SIGLIB_TWO_PI / SampleRate;    // Lower edge
      w2 = strtod(BandEdge2Line->GetValue(), &stopstring) * SIGLIB_TWO_PI / SampleRate;    // Upper edge

      // Verify filter band edges
      if ((w1 >= SIGLIB_PI) || (w2 >= SIGLIB_PI)) {
        wxMessageBox("The two cut off frequencies must be less than the Nyquist point", "Input error", wxOK | wxICON_EXCLAMATION, NULL);
        return;
      }
      if ((w1 <= SIGLIB_ZERO) || (w2 <= SIGLIB_ZERO)) {
        wxMessageBox("The two cut off frequencies must all be greater than zero", "Input error", wxOK | wxICON_EXCLAMATION, NULL);
        return;
      }
      if (w1 >= w2) {
        wxMessageBox("The lower cut of frequency must be less than the upper", "Input error", wxOK | wxICON_EXCLAMATION, NULL);
        return;
      }

      // If we are using the bilinear transform AND pre-warping is selected
      // then pre-warp the frequencies
      if ((TranslationMethodComboBox->GetSelection() == IIR_BILINEAR) && (PreWarpingCheckBox->IsChecked())) {
        w1 = SDS_PreWarp(w1, SIGLIB_TWO_PI);
        w2 = SDS_PreWarp(w2, SIGLIB_TWO_PI);
      }

      // Centre frequency is mid point between two cutoffs
      if (FilterSpecification.BandSpec == IIR_BPF) {
        CentreFreqForScaling = (strtod(BandEdge1Line->GetValue(), &stopstring) + strtod(BandEdge2Line->GetValue(), &stopstring)) / SIGLIB_TWO;
      }

      FilterSpecification.OhmegaZero = SDS_Sqrt(w1 * w2);
      FilterSpecification.LowerBandEdge = (w2 * w2 - FilterSpecification.OhmegaZero * FilterSpecification.OhmegaZero) / w2;
      break;
    }

    /* Invert cut-off frequencies for BSF and HPF */
    if (FilterSpecification.BandSpec == IIR_BSF || FilterSpecification.BandSpec == IIR_HPF) {
      FilterSpecification.LowerBandEdge = SIGLIB_ONE / FilterSpecification.LowerBandEdge;
    }

    /* Specify pass band ripple or stop band attenuation for cheby types */
    if (FilterSpecification.DesignAlgo == IIR_CHEBYSCHEV) {
      FilterSpecification.Ripple_dB = strtod(PassBandRippleLine->GetValue(),
                                             &stopstring);    // Ripple / attenuation in dB (+ve number)
      if (FilterSpecification.Ripple_dB <= SIGLIB_ZERO) {
        wxMessageBox("The pass-band ripple must be greater than zero", "Input error", wxOK | wxICON_EXCLAMATION, NULL);
        return;
      }

      FilterSpecification.Attenuation_dB = FilterSpecification.Ripple_dB;
    }

    if (FilterSpecification.DesignAlgo == IIR_INVERSE_CHEBYSCHEV) {
      FilterSpecification.Attenuation_dB = strtod(StopBandAttenLine->GetValue(),
                                                  &stopstring);    // Ripple / attenuation in dB (+ve number)
      if (FilterSpecification.Attenuation_dB <= SIGLIB_ZERO) {
        wxMessageBox("The stop-band attenuation must be greater than zero", "Input error", wxOK | wxICON_EXCLAMATION, NULL);
        return;
      }

      FilterSpecification.Ripple_dB = FilterSpecification.Attenuation_dB;
      FilterSpecification.UpperBandEdge = FilterSpecification.LowerBandEdge;
    }
  } else /* Elliptic filter design */
  {
#if DEBUG_LOG_FILE
    SUF_Debugfprintf("OnFilterSpecChanged : Elliptic filter\n");
#endif

    switch (FilterSpecification.BandSpec) {
    case IIR_LPF:
    case IIR_HPF:
      // Filter type : Elliptic - LPF HPF
      // Cut off freq is limit of pass band
      FilterSpecification.LowerBandEdge = strtod(BandEdge1Line->GetValue(), &stopstring) * SIGLIB_TWO_PI / SampleRate;    // Lower edge
      FilterSpecification.UpperBandEdge = strtod(BandEdge2Line->GetValue(), &stopstring) * SIGLIB_TWO_PI / SampleRate;    // Upper edge

      // Verify filter band edges
      if ((FilterSpecification.LowerBandEdge >= SIGLIB_PI) || (FilterSpecification.UpperBandEdge >= SIGLIB_PI)) {
        wxMessageBox("The two cut off frequencies must be less than the Nyquist point", "Input error", wxOK | wxICON_EXCLAMATION, NULL);
        return;
      }
      if ((FilterSpecification.LowerBandEdge <= SIGLIB_ZERO) || (FilterSpecification.UpperBandEdge <= SIGLIB_ZERO)) {
        wxMessageBox("The two cut off frequencies must all be greater than zero", "Input error", wxOK | wxICON_EXCLAMATION, NULL);
        return;
      }
      if (FilterSpecification.LowerBandEdge >= FilterSpecification.UpperBandEdge) {
        wxMessageBox("The lower cut of frequency must be less than the upper", "Input error", wxOK | wxICON_EXCLAMATION, NULL);
        return;
      }

      // If we are using the bilinear transform AND pre-warping is selected
      // then pre-warp the frequencies
      if ((TranslationMethodComboBox->GetSelection() == IIR_BILINEAR) && (PreWarpingCheckBox->IsChecked())) {
        FilterSpecification.LowerBandEdge = SDS_PreWarp(FilterSpecification.LowerBandEdge, SIGLIB_TWO_PI);
        FilterSpecification.UpperBandEdge = SDS_PreWarp(FilterSpecification.UpperBandEdge, SIGLIB_TWO_PI);
      }

      if (FilterSpecification.BandSpec == IIR_HPF) {
        Tmp = FilterSpecification.LowerBandEdge;    // Swap low pass and high pass
        FilterSpecification.LowerBandEdge = FilterSpecification.UpperBandEdge;
        FilterSpecification.UpperBandEdge = Tmp;
        FilterSpecification.LowerBandEdge = SIGLIB_ONE / FilterSpecification.LowerBandEdge;
        FilterSpecification.UpperBandEdge = SIGLIB_ONE / FilterSpecification.UpperBandEdge;
      }
      break;
    case IIR_BPF:
    case IIR_BSF:
      // Filter type : Elliptic - BPF
      // Cut off frequencies are limits of pass band
      if (FilterSpecification.BandSpec == IIR_BPF) {
        w1 = strtod(BandEdge1Line->GetValue(), &stopstring) * SIGLIB_TWO_PI / SampleRate;    // 1st edge
        w2 = strtod(BandEdge2Line->GetValue(), &stopstring) * SIGLIB_TWO_PI / SampleRate;    // 2nd edge
        w3 = strtod(BandEdge3Line->GetValue(), &stopstring) * SIGLIB_TWO_PI / SampleRate;    // 3rd edge
        w4 = strtod(BandEdge4Line->GetValue(), &stopstring) * SIGLIB_TWO_PI / SampleRate;    // 4th edge

        if ((w1 <= SIGLIB_ZERO) || (w2 <= SIGLIB_ZERO) || (w3 <= SIGLIB_ZERO) || (w4 <= SIGLIB_ZERO)) {
          wxMessageBox("The four cut off frequencies must all be greater than zero", "Input error", wxOK | wxICON_EXCLAMATION, NULL);
          return;
        }
        if ((w1 >= SIGLIB_PI) || (w2 >= SIGLIB_PI) || (w3 >= SIGLIB_PI) || (w4 >= SIGLIB_PI)) {
          wxMessageBox("The four cut off frequencies must be less than the "
                       "Nyquist point",
                       "Input error", wxOK | wxICON_EXCLAMATION, NULL);
          return;
        }
        if ((w1 > w2) || (w2 > w3) || (w3 > w4)) {
          wxMessageBox("The cut off frequencies must be in order of increasing "
                       "frequency",
                       "Input error", wxOK | wxICON_EXCLAMATION, NULL);
          return;
        }

        // Centre frequency is mid point between two cutoffs
        CentreFreqForScaling = ((w2 * SampleRate / SIGLIB_TWO_PI) + (w3 * SampleRate / SIGLIB_TWO_PI)) / SIGLIB_TWO;

        // If we are using the bilinear transform AND pre-warping is selected
        // then pre-warp the frequencies
        if ((TranslationMethodComboBox->GetSelection() == IIR_BILINEAR) && (PreWarpingCheckBox->IsChecked())) {
          w1 = SDS_PreWarp(w1, SIGLIB_TWO_PI);
          w2 = SDS_PreWarp(w2, SIGLIB_TWO_PI);
          w3 = SDS_PreWarp(w3, SIGLIB_TWO_PI);
          w4 = SDS_PreWarp(w4, SIGLIB_TWO_PI);
        }

        FilterSpecification.OhmegaZero = SDS_Sqrt(w3 * w2);
        FilterSpecification.LowerBandEdge = (w3 * w3 - FilterSpecification.OhmegaZero * FilterSpecification.OhmegaZero) / w3;
        FilterSpecification.UpperBandEdge = (w4 * w4 - FilterSpecification.OhmegaZero * FilterSpecification.OhmegaZero) / w4;
        wst = (FilterSpecification.OhmegaZero * FilterSpecification.OhmegaZero - w1 * w1) / w1;
      } else /* F_BS */
      {
        // Filter type : Elliptic - BSF
        // Cut off frequencies are limits of stop band
        w4 = strtod(BandEdge1Line->GetValue(), &stopstring) * SIGLIB_TWO_PI / SampleRate;    // Lower edge
        w3 = strtod(BandEdge2Line->GetValue(), &stopstring) * SIGLIB_TWO_PI / SampleRate;    // Lower edge
        w2 = strtod(BandEdge3Line->GetValue(), &stopstring) * SIGLIB_TWO_PI / SampleRate;    // Lower edge
        w1 = strtod(BandEdge4Line->GetValue(), &stopstring) * SIGLIB_TWO_PI / SampleRate;    // Lower edge

        if ((w1 <= SIGLIB_ZERO) || (w2 <= SIGLIB_ZERO) || (w3 <= SIGLIB_ZERO) || (w4 <= SIGLIB_ZERO)) {
          wxMessageBox("The four cut off frequencies must all be greater than zero", "Input error", wxOK | wxICON_EXCLAMATION, NULL);
          return;
        }
        if ((w1 >= SIGLIB_PI) || (w2 >= SIGLIB_PI) || (w3 >= SIGLIB_PI) || (w4 >= SIGLIB_PI)) {
          wxMessageBox("The four cut off frequencies must be less than the "
                       "Nyquist point",
                       "Input error", wxOK | wxICON_EXCLAMATION, NULL);
          return;
        }
        if ((w4 > w3) || (w3 > w2) || (w2 > w1)) {
          wxMessageBox("The cut off frequencies must be in order of increasing "
                       "frequency",
                       "Input error", wxOK | wxICON_EXCLAMATION, NULL);
          return;
        }

        // If we are using the bilinear transform AND pre-warping is selected
        // then pre-warp the frequencies
        if ((TranslationMethodComboBox->GetSelection() == IIR_BILINEAR) && (PreWarpingCheckBox->IsChecked())) {
          w4 = SDS_PreWarp(w4, SIGLIB_TWO_PI);
          w3 = SDS_PreWarp(w3, SIGLIB_TWO_PI);
          w2 = SDS_PreWarp(w2, SIGLIB_TWO_PI);
          w1 = SDS_PreWarp(w1, SIGLIB_TWO_PI);
        }

        FilterSpecification.OhmegaZero = SDS_Sqrt(w2 * w3);
        FilterSpecification.LowerBandEdge = w1 / (FilterSpecification.OhmegaZero * FilterSpecification.OhmegaZero - w1 * w1);
        FilterSpecification.UpperBandEdge = w2 / (FilterSpecification.OhmegaZero * FilterSpecification.OhmegaZero - w2 * w2);
        wst = w3 / (w3 * w3 - FilterSpecification.OhmegaZero * FilterSpecification.OhmegaZero);
      }
      if (wst < FilterSpecification.UpperBandEdge) {
        FilterSpecification.UpperBandEdge = wst;
      }
      break;
    }

    FilterSpecification.Ripple_dB = strtod(PassBandRippleLine->GetValue(),
                                           &stopstring);    // Ripple in dB (+ve number)
    if (FilterSpecification.Ripple_dB <= SIGLIB_ZERO) {
      wxMessageBox("The pass-band ripple must be greater than zero", "Input error", wxOK | wxICON_EXCLAMATION, NULL);
      return;
    }

    FilterSpecification.Attenuation_dB = strtod(StopBandAttenLine->GetValue(),
                                                &stopstring);    // Attenuation in dB (+ve number)
    if (FilterSpecification.Attenuation_dB <= SIGLIB_ZERO) {
      wxMessageBox("The stop-band attenuation must be greater than zero", "Input error", wxOK | wxICON_EXCLAMATION, NULL);
      return;
    }
  }

#if DEBUG_LOG_FILE
  SUF_Debugfprintf("\nOnFilterSpecChanged : Got Spec\n");

  SUF_Debugfprintf("FilterSpecification.Order = %d\n", FilterSpecification.Order);
  SUF_Debugfprintf("FilterSpecification.BandSpec = %d\n", FilterSpecification.BandSpec);
  SUF_Debugfprintf("FilterSpecification.DesignAlgo = %d\n", FilterSpecification.DesignAlgo);
  SUF_Debugfprintf("FilterSpecification.Ripple_dB = %lf\n", FilterSpecification.Ripple_dB);
  SUF_Debugfprintf("FilterSpecification.Attenuation_dB = %lf\n", FilterSpecification.Attenuation_dB);
  SUF_Debugfprintf("FilterSpecification.OhmegaZero = %lf\n", FilterSpecification.OhmegaZero);
  SUF_Debugfprintf("FilterSpecification.LowerBandEdge = %lf\n", FilterSpecification.LowerBandEdge);
  SUF_Debugfprintf("FilterSpecification.UpperBandEdge = %lf\n", FilterSpecification.UpperBandEdge);

  SUF_Debugfprintf("TranslationMethodComboBox = %d\n", TranslationMethodComboBox->GetSelection());

  if (FilterSpecification.DesignAlgo == IIR_BESSEL) {
    SUF_Debugfprintf("Fc1 = %lf\n", Fc1);
    SUF_Debugfprintf("Fc2 = %lf\n", Fc2);
  }
#endif

  static char FileNameString[500];
  FILE* pFile;

  strcpy(FileNameString, ProjectNameString);
  strcat(FileNameString, ".dfs");

  if ((pFile = fopen(FileNameString, "w")) == NULL) { /* Open o/p file */
    wxMessageBox("Could not open file to write filter specification", "OnGetFilterSpec", wxOK | wxICON_EXCLAMATION, NULL);
    return;
  }

  fprintf(pFile, DFPLUS_COPYRIGHT_NOTICE);
  fprintf(pFile, "File : %s\n\n", FileNameString);
  fprintf(pFile, "IIR filter coefficients designed with the Bilinear Transform "
                 "Algorithm\n\n");

  fprintf(pFile, "[Design Type]\n");
  if (TranslationMethodComboBox->GetSelection() == IIR_BILINEAR)    // If we are using the bilinear transform
    fprintf(pFile, "IIR-Bilinear\n\n");
  else
    fprintf(pFile, "IIR-Matched z-Transform\n\n");

  fprintf(pFile, "[Sample rate]\n");
  fprintf(pFile, "%1.20lf\n\n", SampleRate);

  fprintf(pFile, "[IIR Specification]\n");
  if (FilterSpecification.BandSpec == IIR_LPF)
    fprintf(pFile, "Low Pass Filter\n\n");
  else if (FilterSpecification.BandSpec == IIR_HPF)
    fprintf(pFile, "High Pass Filter\n\n");
  else if (FilterSpecification.BandSpec == IIR_BPF)
    fprintf(pFile, "Band Pass Filter\n\n");
  else if (FilterSpecification.BandSpec == IIR_BSF)
    fprintf(pFile, "Band Stop Filter\n\n");

  fprintf(pFile, "[IIR Design Method]\n");
  if (FilterSpecification.DesignAlgo == IIR_BUTTERWORTH)
    fprintf(pFile, "Butterworth Filter\n\n");
  else if (FilterSpecification.DesignAlgo == IIR_CHEBYSCHEV)
    fprintf(pFile, "Tchebyschev Filter\n\n");
  else if (FilterSpecification.DesignAlgo == IIR_INVERSE_CHEBYSCHEV)
    fprintf(pFile, "Inverse Tchebyschev Filter\n\n");
  else if (FilterSpecification.DesignAlgo == IIR_ELLIPTIC)
    fprintf(pFile, "Elliptic Filter\n\n");
  else if (FilterSpecification.DesignAlgo == IIR_BESSEL)
    fprintf(pFile, "Bessel Filter\n\n");

  fprintf(pFile, "[Filter Order]\n");
  fprintf(pFile, "%d\n\n", FilterSpecification.Order);

  fprintf(pFile, "[Pass Band Ripple]\n");
  fprintf(pFile, "%lf\n\n", FilterSpecification.Ripple_dB);

  fprintf(pFile, "[Stop Band Attenuation]\n");
  fprintf(pFile, "%lf\n\n", FilterSpecification.Attenuation_dB);

  fprintf(pFile, "[Cut Off Frequency #1]\n");
  fprintf(pFile, "%lf\n\n", strtod(BandEdge1Line->GetValue(), &stopstring));

  fprintf(pFile, "[Cut Off Frequency #2]\n");
  fprintf(pFile, "%lf\n\n", strtod(BandEdge3Line->GetValue(), &stopstring));

  fprintf(pFile, "[Cut Off Frequency #3]\n");
  fprintf(pFile, "%lf\n\n", strtod(BandEdge2Line->GetValue(), &stopstring));

  fprintf(pFile, "[Cut Off Frequency #4]\n");
  fprintf(pFile, "%lf\n\n", strtod(BandEdge4Line->GetValue(), &stopstring));

  fprintf(pFile, "\n\n");

  (void)fclose(pFile); /* Close output file */

  // Finished writing specification file

  // Generate analog filter prototype
  if (FilterSpecification.DesignAlgo == IIR_BESSEL) {
    BesselFilter(&FilterSpecification);    // Only specify the filter order to this function

    // Translate cut off frequency of S-plane prototype filter
#if DEBUG_LOG_FILE
    SUF_Debugfprintf("OnFilterSpecChanged : Calculating Bessel filter S-Plane "
                     "prototype frequency transforms\n");
#endif
    for (i = 0; i < FilterSpecification.Order; i++)
      FilterSpecification.pPoles[i] = SCV_VectorMultiplyScalar(FilterSpecification.pPoles[i], ((Fc1 * SIGLIB_TWO_PI) / SampleRate));
#if DEBUG_LOG_FILE
    SUF_Debugfprintf("OnFilterSpecChanged : Bessel filter S-Plane prototype "
                     "frequencies transformed\n");
#endif
  } else {    // Non Bessel filter

    if (FilterSpecification.DesignAlgo == IIR_ELLIPTIC) {
      elliptic_filter_roots(&FilterSpecification);
    } else {
      non_elliptic_filter_roots(&FilterSpecification);
    }

#if DEBUG_LOG_FILE
    {
      if (FilterSpecification.DesignAlgo == IIR_BUTTERWORTH)
        SUF_Debugfprintf("\nButterworth Zeros and Poles, order %d\n", ((short)FilterSpecification.Order));
      else if (FilterSpecification.DesignAlgo == IIR_CHEBYSCHEV)
        SUF_Debugfprintf("\nTchebyschev Zeros and Poles, order %d\n", ((short)FilterSpecification.Order));
      else if (FilterSpecification.DesignAlgo == IIR_INVERSE_CHEBYSCHEV)
        SUF_Debugfprintf("\nInverse Tchebyschev Zeros and Poles, order %d\n", ((short)FilterSpecification.Order));
      else if (FilterSpecification.DesignAlgo == IIR_ELLIPTIC)
        SUF_Debugfprintf("\nElliptic Zeros and Poles, order %d\n", ((short)FilterSpecification.Order));
      else if (FilterSpecification.DesignAlgo == IIR_BESSEL)
        SUF_Debugfprintf("\nBessel Zeros and Poles, order %d\n", ((short)FilterSpecification.Order));

      short q;
      for (q = 0; q < FilterSpecification.Order; q++) {
        // Print both the S-plane poles and zeros
        SUF_Debugfprintf("s-Plane Poles and zeros # %d\n", q);
        SUF_Debugfprintf("P = %lf, %lf; Z = %lf, %lf\n", FilterSpecification.pPoles[q].real, FilterSpecification.pPoles[q].imag,
                         FilterSpecification.pZeros[q].real, FilterSpecification.pZeros[q].imag);
      }
    }
#endif

    if ((FilterSpecification.Order & 0x1) == 0x1) {    // If odd order filter
      SLComplexRect_s TmpP, TmpZ;
      TmpP = FilterSpecification.pPoles[0];
      TmpZ = FilterSpecification.pZeros[0];

      for (i = 0; i < (FilterSpecification.Order - 1); i++) {    // Shift the order to put 1st order filter at end
        FilterSpecification.pPoles[i] = FilterSpecification.pPoles[i + 1];
        FilterSpecification.pZeros[i] = FilterSpecification.pZeros[i + 1];
      }
      FilterSpecification.pPoles[FilterSpecification.Order - 1] = TmpP;
      FilterSpecification.pZeros[FilterSpecification.Order - 1] = TmpZ;
    }
  }

#if COMMENT
  // Store s-plane poles and zeros
  if (FilterSpecification.DesignAlgo == IIR_BUTTERWORTH)
    SUF_Debugfprintf("SLComplexRect_s SPlaneButterworthZerosPoles%d[] =\n{\n", ((short)FilterSpecification.Order));
  else if (FilterSpecification.DesignAlgo == IIR_CHEBYSCHEV)
    SUF_Debugfprintf("SLComplexRect_s SPlaneTchebyschevZerosPoles%d[] =\n{\n", ((short)FilterSpecification.Order));
  else if (FilterSpecification.DesignAlgo == IIR_INVERSE_CHEBYSCHEV)
    SUF_Debugfprintf("SLComplexRect_s SPlaneInverseTchebyschevZerosPoles%d[] =\n{\n", ((short)FilterSpecification.Order));
  else if (FilterSpecification.DesignAlgo == IIR_ELLIPTIC)
    SUF_Debugfprintf("SLComplexRect_s SPlaneEllipticZerosPoles%d[] =\n{\n", ((short)FilterSpecification.Order));
  else if (FilterSpecification.DesignAlgo == IIR_BESSEL)
    SUF_Debugfprintf("SLComplexRect_s SPlaneBesselZerosPoles%d[] =\n{\n", ((short)FilterSpecification.Order));

  {
    SLArrayIndex_t qq;
    for (qq = 0; qq < FilterSpecification.Order; qq++) {
      SUF_Debugfprintf("\t{%25.25le, %25.25le}\n", FilterSpecification.pZeros[qq].real, FilterSpecification.pZeros[qq].imag);
    }
    for (qq = 0; qq < FilterSpecification.Order; qq++) {
      SUF_Debugfprintf("\t{%25.25le, %25.25le}\n", FilterSpecification.pPoles[qq].real, FilterSpecification.pPoles[qq].imag);
    }
    SUF_Debugfprintf("};\n\n");
  }
#endif

#if DEBUG_LOG_FILE
  SUF_Debugfprintf("OnFilterSpecChanged : Calculated analog prototype\n");
#endif

  // Allocate memory for z-Plane - BP & BS use twice filter size
  pZPlanePoles = SUF_ComplexRectArrayAllocate(FilterSpecification.Order * 2);
  pZPlaneZeros = SUF_ComplexRectArrayAllocate(FilterSpecification.Order * 2);
  if ((pZPlanePoles == NULL) || (pZPlaneZeros == NULL)) {
    wxMessageBox("Could not allocate memory for IIR filter", "Internal error", wxOK | wxICON_EXCLAMATION, NULL);
    return;
  }

  if (FilterSpecification.BandSpec == IIR_HPF) {
#if DEBUG_LOG_FILE
    SUF_Debugfprintf("OnFilterSpecChanged : Calculating high_pass_transform\n");
#endif

    high_pass_transform(FilterSpecification.pPoles, FilterSpecification.Order);
    high_pass_transform(FilterSpecification.pZeros, FilterSpecification.Order);

  } else if ((FilterSpecification.BandSpec == IIR_BPF) || (FilterSpecification.BandSpec == IIR_BSF)) {
#if DEBUG_LOG_FILE
    SUF_Debugfprintf("OnFilterSpecChanged : Calculating bandpass_bandstop_transform\n");
#endif

    bandpass_bandstop_transform(FilterSpecification.pPoles, FilterSpecification.Order, FilterSpecification.BandSpec,
                                FilterSpecification.OhmegaZero);
    bandpass_bandstop_transform(FilterSpecification.pZeros, FilterSpecification.Order, FilterSpecification.BandSpec,
                                FilterSpecification.OhmegaZero);
  }

  if ((FilterSpecification.BandSpec == IIR_LPF) || (FilterSpecification.BandSpec == IIR_HPF)) {
    NumberOfBiquads = (FilterSpecification.Order + 1) >> 1;
  } else {
    FilterSpecification.Order *= 2;    // Order twice the size for BP and BS filters
    NumberOfBiquads = FilterSpecification.Order >> 1;
  }

#if DEBUG_LOG_FILE
  SUF_Debugfprintf("OnFilterSpecChanged : Frequency translation done\n");
#endif

  if (TranslationMethodComboBox->GetSelection() == IIR_BILINEAR) {
    // Frequencies are already pre-warped if required
    // Sample rate = 1.0
    SDA_BilinearTransform(FilterSpecification.pZeros, FilterSpecification.pPoles, pZPlaneZeros, pZPlanePoles, SIGLIB_ONE, SIGLIB_ZERO, SIGLIB_OFF,
                          FilterSpecification.Order, FilterSpecification.Order);
  } else    // Matched z-Transform calculation
  {
    for (i = 0; i < FilterSpecification.Order; i++) {
      if (FilterSpecification.pPoles[i].imag > SampleRate)
        FilterSpecification.pPoles[i].imag = SampleRate;
      if (FilterSpecification.pZeros[i].imag > SampleRate)
        FilterSpecification.pZeros[i].imag = SampleRate;
    }
    SDA_MatchedZTransform(FilterSpecification.pZeros, FilterSpecification.pPoles, pZPlaneZeros, pZPlanePoles, SIGLIB_ONE, FilterSpecification.Order,
                          FilterSpecification.Order);
  }

#if DEBUG_LOG_FILE
  SUF_Debugfprintf("OnFilterSpecChanged : Bilinear / Matched z-Transform calculated\n");
  {
    short q;
    for (q = 0; q < FilterSpecification.Order; q++) {
      // Print both the S-plane poles and zeros
      SUF_Debugfprintf("\ns-Plane Poles and zeros # %d\n", q);
      SUF_Debugfprintf("P = %lf, %lf; Z = %lf, %lf\n", FilterSpecification.pPoles[q].real, FilterSpecification.pPoles[q].imag,
                       FilterSpecification.pZeros[q].real, FilterSpecification.pZeros[q].imag);
      // Print both the z-plane poles and zeros
      SUF_Debugfprintf("z-Plane Poles and zeros # %d\n", q);
      SUF_Debugfprintf("P = %lf, %lf; Z = %lf, %lf\n", pZPlanePoles[q].real, pZPlanePoles[q].imag, pZPlaneZeros[q].real, pZPlaneZeros[q].imag);
    }
  }
#endif

#define ORIG_PZ 1
#if ORIG_PZ
  if ((FilterSpecification.Order & 0x1) == 0) {     // Even order filter
    for (i = 0; i < (short)NumberOfBiquads; i++)    // Generate coeff table
    {
      TmpPolar = SCV_RectangularToPolar(pZPlaneZeros[i * 2]);                               // Skip conjugate poles and zeros
      pIIRCoeffs[(5 * i) + 0] = SIGLIB_ONE;                                                 // b0
      pIIRCoeffs[(5 * i) + 1] = -(SIGLIB_TWO * TmpPolar.magn * SDS_Cos(TmpPolar.angle));    // b1
      pIIRCoeffs[(5 * i) + 2] = TmpPolar.magn * TmpPolar.magn;                              // b2
      TmpPolar = SCV_RectangularToPolar(pZPlanePoles[i * 2]);
      pIIRCoeffs[(5 * i) + 3] = -(SIGLIB_TWO * TmpPolar.magn * SDS_Cos(TmpPolar.angle));    // a1
      pIIRCoeffs[(5 * i) + 4] = TmpPolar.magn * TmpPolar.magn;                              // a2
    }
  } else {                                                                                  // Odd order filter
    for (i = 0; i < (short)NumberOfBiquads - 1; i++) {                                      // Generate coeff table
      TmpPolar = SCV_RectangularToPolar(pZPlaneZeros[i * 2]);                               // Skip conjugate poles and zeros
      pIIRCoeffs[(5 * i) + 0] = SIGLIB_ONE;                                                 // b0
      pIIRCoeffs[(5 * i) + 1] = -(SIGLIB_TWO * TmpPolar.magn * SDS_Cos(TmpPolar.angle));    // b1
      pIIRCoeffs[(5 * i) + 2] = TmpPolar.magn * TmpPolar.magn;                              // b2
      TmpPolar = SCV_RectangularToPolar(pZPlanePoles[i * 2]);
      pIIRCoeffs[(5 * i) + 3] = -(SIGLIB_TWO * TmpPolar.magn * SDS_Cos(TmpPolar.angle));    // a1
      pIIRCoeffs[(5 * i) + 4] = TmpPolar.magn * TmpPolar.magn;                              // a2
    }

    i = ((short)NumberOfBiquads) - 1;                       // Calculate last biquad - odd order
    pIIRCoeffs[(5 * i) + 0] = SIGLIB_ONE;                   // b0
    pIIRCoeffs[(5 * i) + 1] = -pZPlaneZeros[i * 2].real;    // b1
    pIIRCoeffs[(5 * i) + 2] = SIGLIB_ZERO;                  // b2
    pIIRCoeffs[(5 * i) + 3] = -pZPlanePoles[i * 2].real;    // a1
    pIIRCoeffs[(5 * i) + 4] = SIGLIB_ZERO;                  // a2
  }

#else
  SLComplexRect_s *p_PolesTmp, *p_ZerosTmp;
  p_PolesTmp = SUF_ComplexRectArrayAllocate(MAX_BLT_ORDER);
  p_ZerosTmp = SUF_ComplexRectArrayAllocate(MAX_BLT_ORDER);

  for (i = 0; i < (short)NumberOfBiquads; i++)    // Generate coeff table
  {
    p_PolesTmp[i] = pZPlanePoles[i * 2];    // Skip conjugate poles
    p_ZerosTmp[i] = pZPlaneZeros[i * 2];    // Skip conjugate zeros
  }

  SDA_IirZplaneToCoeffs(p_ZerosTmp, p_PolesTmp, pIIRCoeffs, NumberOfBiquads, NumberOfBiquads);

  SUF_MemoryFree(p_PolesTmp);
  SUF_MemoryFree(p_ZerosTmp);
#endif

#if DEBUG_LOG_FILE
  SUF_Debugfprintf("OnFilterSpecChanged : Biquads calculated\n");
  SUF_Debugfprintf("CentreFreqForScaling = %lf\n", CentreFreqForScaling);
#endif

  if (ScaleGainCheckBox->IsChecked())    // Scale the gain if requested
    SDA_IirModifyFilterGain(pIIRCoeffs, pIIRCoeffs, CentreFreqForScaling / SampleRate, SIGLIB_ONE, NumberOfBiquads);

#if DEBUG_LOG_FILE
  SUF_Debugfprintf("OnFilterSpecChanged : Gain normalized\n");
#endif

  strcpy(FileNameString, ProjectNameString);
  strcat(FileNameString, ".dfc");

  if ((pFile = fopen(FileNameString, "w")) == NULL) { /* Open o/p file */
    wxMessageBox("Could not open file to write filter coefficients", "OnGetFilterSpec", wxOK | wxICON_EXCLAMATION, NULL);
    return;
  }

#define PRINT_IIR_COEFFS 0
#if PRINT_IIR_COEFFS    // Print the IIR filter coefficients to the debug file
  if (FilterSpecification.DesignAlgo == IIR_BUTTERWORTH)
    SUF_Debugfprintf("SLData_t IIRButterworth%d [] =\n{\n", (short)FilterSpecification.Order);
  else if (FilterSpecification.DesignAlgo == IIR_CHEBYSCHEV)
    SUF_Debugfprintf("SLData_t IIRTchebyschev%d [] =\n{\n", (short)FilterSpecification.Order);
  else if (FilterSpecification.DesignAlgo == IIR_INVERSE_CHEBYSCHEV)
    SUF_Debugfprintf("SLData_t IIRInverseTchebyschev%d [] =\n{\n", (short)FilterSpecification.Order);
  else if (FilterSpecification.DesignAlgo == IIR_ELLIPTIC)
    SUF_Debugfprintf("SLData_t IIRElliptic%d [] =\n{\n", (short)FilterSpecification.Order);
  else if (FilterSpecification.DesignAlgo == IIR_BESSEL)
    SUF_Debugfprintf("SLData_t IIRBessel%d [] =\n{\n", (short)FilterSpecification.Order);

  /* Write IIR coefficients to file */
  for (i = 0; i < (short)NumberOfBiquads; i++) {
    SUF_Debugfprintf("\t%1.20le, %1.20le, %1.20le,\n\t%1.20le, %1.20le", pIIRCoeffs[(5 * i) + 0], pIIRCoeffs[(5 * i) + 1], pIIRCoeffs[(5 * i) + 2],
                     pIIRCoeffs[(5 * i) + 3], pIIRCoeffs[(5 * i) + 4]);

    if (i < ((short)NumberOfBiquads - 1))
      SUF_Debugfprintf(",\n\n");
  }

  SUF_Debugfprintf("\n};\n\n");
#endif

  fprintf(pFile, DFPLUS_COPYRIGHT_NOTICE);
  fprintf(pFile, "File : %s\n\n", FileNameString);

  if (TranslationMethodComboBox->GetSelection() == IIR_BILINEAR)
    fprintf(pFile, "IIR filter coefficients designed with the Bilinear "
                   "Transform Method\n\n");
  else
    fprintf(pFile, "IIR filter coefficients designed with the Matched "
                   "z-Transform Method\n\n");
  fprintf(pFile, "Coefficient order for each biquad :\n\tb[0], b[1], "
                 "b[2],\n\ta[1], a[2].\n\n");

  fprintf(pFile, "Filter Order = %d\n\n",
          (short)FilterSpecification.Order);    // Print filter order
  fprintf(pFile, "Sample rate = %1.20lf\n\n", SampleRate);

  fprintf(pFile, "IIR Specification = ");
  if (FilterSpecification.BandSpec == IIR_LPF)
    fprintf(pFile, "Low Pass ");
  else if (FilterSpecification.BandSpec == IIR_HPF)
    fprintf(pFile, "High Pass ");
  else if (FilterSpecification.BandSpec == IIR_BPF)
    fprintf(pFile, "Band Pass ");
  else if (FilterSpecification.BandSpec == IIR_BSF)
    fprintf(pFile, "Band Stop ");

  if (FilterSpecification.DesignAlgo == IIR_BUTTERWORTH)
    fprintf(pFile, "Butterworth Filter\n\n");
  else if (FilterSpecification.DesignAlgo == IIR_CHEBYSCHEV)
    fprintf(pFile, "Tchebyschev Filter\n\n");
  else if (FilterSpecification.DesignAlgo == IIR_INVERSE_CHEBYSCHEV)
    fprintf(pFile, "Inverse Tchebyschev Filter\n\n");
  else if (FilterSpecification.DesignAlgo == IIR_ELLIPTIC)
    fprintf(pFile, "Elliptic Filter\n\n");
  else if (FilterSpecification.DesignAlgo == IIR_BESSEL)
    fprintf(pFile, "Bessel Filter\n\n");

  // Write floating point coefficients to file
  fprintf(pFile, "\t/* Floating point coefficients */\n");
  fprintf(pFile, "const SLData_t CoefficientArray [] =\n{\n");

  /* Write IIR coefficients to file */
  if (global_NegateIIRAlphaSign == 0) {    // Don't NegateIIRAlphaSign for biquads
    for (i = 0; i < (short)NumberOfBiquads; i++) {
      fprintf(pFile, "\t%1.20le, %1.20le, %1.20le,\n\t%1.20le, %1.20le", pIIRCoeffs[(5 * i) + 0], pIIRCoeffs[(5 * i) + 1], pIIRCoeffs[(5 * i) + 2],
              pIIRCoeffs[(5 * i) + 3], pIIRCoeffs[(5 * i) + 4]);

      if (i < ((short)NumberOfBiquads - 1))
        fprintf(pFile, ",\n\n");
    }
  } else {    // NegateIIRAlphaSign for biquads
    for (i = 0; i < (short)NumberOfBiquads; i++) {
      fprintf(pFile, "\t%1.20le, %1.20le, %1.20le,\n\t%1.20le, %1.20le", pIIRCoeffs[(5 * i) + 0], pIIRCoeffs[(5 * i) + 1], pIIRCoeffs[(5 * i) + 2],
              -pIIRCoeffs[(5 * i) + 3], -pIIRCoeffs[(5 * i) + 4]);

      if (i < ((short)NumberOfBiquads - 1))
        fprintf(pFile, ",\n\n");
    }
  }

  fprintf(pFile, "\n};\n\n");

  fprintf(pFile, "`~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~'\n\n");

  // Write Q format fixed point coefficients to file
  fprintf(pFile, "\t/* Q format fixed point coefficients */\n");
  fprintf(pFile, "\t\t/* m = %d, n = %d */\n\n", global_mValue, global_nValue);
  fprintf(pFile, "const SLData_t CoefficientArray [] =\n{\n");

  if (global_NegateIIRAlphaSign == 0) {    // Don't NegateIIRAlphaSign for biquads
    for (i = 0; i < (short)NumberOfBiquads; i++) {
      fprintf(pFile, "\t0x%llx, 0x%llx, 0x%llx, 0x%llx, 0x%llx", FloatToQFormatInteger(pIIRCoeffs[(5 * i) + 0], global_mValue, global_nValue),
              FloatToQFormatInteger(pIIRCoeffs[(5 * i) + 1], global_mValue, global_nValue),
              FloatToQFormatInteger(pIIRCoeffs[(5 * i) + 2], global_mValue, global_nValue),
              FloatToQFormatInteger(pIIRCoeffs[(5 * i) + 3], global_mValue, global_nValue),
              FloatToQFormatInteger(pIIRCoeffs[(5 * i) + 4], global_mValue, global_nValue));

      if (i < ((short)NumberOfBiquads - 1))
        fprintf(pFile, ",\n");
    }
  } else {    // NegateIIRAlphaSign for biquads
    for (i = 0; i < (short)NumberOfBiquads; i++) {
      fprintf(pFile, "\t0x%llx, 0x%llx, 0x%llx, 0x%llx, 0x%llx", FloatToQFormatInteger(pIIRCoeffs[(5 * i) + 0], global_mValue, global_nValue),
              FloatToQFormatInteger(pIIRCoeffs[(5 * i) + 1], global_mValue, global_nValue),
              FloatToQFormatInteger(pIIRCoeffs[(5 * i) + 2], global_mValue, global_nValue),
              FloatToQFormatInteger(-pIIRCoeffs[(5 * i) + 3], global_mValue, global_nValue),
              FloatToQFormatInteger(-pIIRCoeffs[(5 * i) + 4], global_mValue, global_nValue));

      if (i < ((short)NumberOfBiquads - 1))
        fprintf(pFile, ",\n");
    }
  }

  fprintf(pFile, "\n};\n\n");
  fprintf(pFile, "`~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~'\n\n");

  // Write 16 bit fixed point coefficients to file
  fprintf(pFile, "\t/* 16 bit fixed point coefficients */\n");
  fprintf(pFile, "const SLData_t CoefficientArray [] =\n{\n");

  if (global_NegateIIRAlphaSign == 0) {    // Don't NegateIIRAlphaSign for biquads
    for (i = 0; i < (short)NumberOfBiquads; i++) {
      fprintf(pFile, "\t%x, %x, %x, %x, %x", 0xffff & ((short)(pIIRCoeffs[(5 * i) + 0] * 32767.0)),
              0xffff & ((short)(pIIRCoeffs[(5 * i) + 1] * 32767.0)), 0xffff & ((short)(pIIRCoeffs[(5 * i) + 2] * 32767.0)),
              0xffff & ((short)(pIIRCoeffs[(5 * i) + 3] * 32767.0)), 0xffff & ((short)(pIIRCoeffs[(5 * i) + 4] * 32767.0)));

      if (i < ((short)NumberOfBiquads - 1))
        fprintf(pFile, ",\n");
    }
  } else {    // NegateIIRAlphaSign for biquads
    for (i = 0; i < (short)NumberOfBiquads; i++) {
      fprintf(pFile, "\t%x, %x, %x, %x, %x", 0xffff & ((short)(pIIRCoeffs[(5 * i) + 0] * 32767.0)),
              0xffff & ((short)(pIIRCoeffs[(5 * i) + 1] * 32767.0)), 0xffff & ((short)(pIIRCoeffs[(5 * i) + 2] * 32767.0)),
              0xffff & ((short)(pIIRCoeffs[(5 * i) + 3] * -32767.0)), 0xffff & ((short)(pIIRCoeffs[(5 * i) + 4] * -32767.0)));

      if (i < ((short)NumberOfBiquads - 1))
        fprintf(pFile, ",\n");
    }
  }

  fprintf(pFile, "\n};\n\n");

  (void)fclose(pFile);

  // Coefficient file written

#if DEBUG_LOG_FILE
  SUF_Debugfprintf("OnFilterSpecChanged : DFC file written, Starting analysis\n");
#endif

  {
    //  char SString [80];
    //  sprintf (SString, "NumberOfBiquads = %d", (short)NumberOfBiquads);
    //  wxMessageBox (SString, "NumberOfBiquads", wxOK | wxICON_EXCLAMATION,
    //  NULL);

    SLData_t *pFDPRealData, *pFDPImagData, *pFDPFFTCoeffs, *pIIRStateArray;

    pFDPRealData = SUF_VectorArrayAllocate(FFT_SIZE);
    pFDPImagData = SUF_VectorArrayAllocate(FFT_SIZE);
    pFDPFFTCoeffs = SUF_FftCoefficientAllocate(FFT_SIZE);
    pIIRStateArray = SUF_IirStateArrayAllocate(NumberOfBiquads);

    if ((pFDPRealData == NULL) || (pFDPImagData == NULL) || (pFDPFFTCoeffs == NULL) || (pIIRStateArray == NULL)) {
      wxMessageBox("Could not allocate memory for IIR filter analysis", "System error", wxOK | wxICON_EXCLAMATION, NULL);
      return;
    }

    SIF_Iir(pIIRStateArray, NumberOfBiquads); /* Calculate impulse response */
    pFDPRealData[0] = SDS_Iir(SIGLIB_ONE, pIIRStateArray, pIIRCoeffs, NumberOfBiquads);
    for (i = 1; i < FFT_SIZE; i++) {
      pFDPRealData[i] = SDS_Iir(SIGLIB_ZERO, pIIRStateArray, pIIRCoeffs, NumberOfBiquads);
    }

#if DEBUG_LOG_FILE
    SUF_Debugfprintf("OnFilterSpecChanged : Mid analysis\n");
#endif

    SDA_Copy(pFDPRealData, pImpulseResponse, GRAPH_WIDTH); /* Save impulse response */

    SIF_Fft(pFDPFFTCoeffs, SIGLIB_NULL_FIX_DATA_PTR, FFT_SIZE);                                             /* Initialise FFT */
    SDA_Rfft(pFDPRealData, pFDPImagData, pFDPFFTCoeffs, SIGLIB_NULL_FIX_DATA_PTR, FFT_SIZE, LOG2_FFT_SIZE); /* Perform FFT */

    /* Calculate results and copy to arrays for display */
    SDA_LogMagnitude(pFDPRealData, pFDPImagData, pGain, GRAPH_WIDTH);  /* Calc real power fm complex in dBs */
    SDA_PhaseWrapped(pFDPRealData, pFDPImagData, pPhase, GRAPH_WIDTH); /* Phase */

    SLData_t CurrentPhaseValue = SIGLIB_ZERO;
    SDA_GroupDelay(pPhase, pGroupDelay, &CurrentPhaseValue, GRAPH_WIDTH); /* Group delay */

    free(pFDPRealData); /* Free memory */
    free(pFDPImagData);
    free(pFDPFFTCoeffs);
    free(pIIRStateArray);
    free(FilterSpecification.pPoles);
    free(FilterSpecification.pZeros);
    free(pZPlanePoles);
    free(pZPlaneZeros);

  } /* End of calculate filter performance */

#if DEBUG_LOG_FILE
  SUF_Debugfprintf("OnFilterSpecChanged : Analysis complete\n");
#endif

  // Generate array for pole zero plot
  // Array format is :
  // Complex Pole; Complex Zero; Complex Pole; . . . .
  // Array includes both conjugates of a pair
  for (i = 0; i < (short)FilterSpecification.Order; i++) {
    TmpPolar = SCV_RectangularToPolar(pZPlanePoles[i]);
    pPolesAndZeros[(2 * i) + 0] = pZPlanePoles[i];

    TmpPolar = SCV_RectangularToPolar(pZPlaneZeros[i]);
    pPolesAndZeros[(2 * i) + 1] = pZPlaneZeros[i];
  }

#if DEBUG_LOG_FILE
  SUF_Debugfprintf("OnFilterSpecChanged : Pole zero plot array calculated\n");
#endif

  strcpy(FileNameString, ProjectNameString);
  strcat(FileNameString, ".dfz");

  if ((pFile = fopen(FileNameString, "w")) == NULL) { /* Open o/p file */
    wxMessageBox("Could not open file to write filter coefficients", "OnGetFilterSpec", wxOK | wxICON_EXCLAMATION, NULL);
    return;
  }

  fprintf(pFile, DFPLUS_COPYRIGHT_NOTICE);
  fprintf(pFile, "File : %s\n\n", FileNameString);

  if (TranslationMethodComboBox->GetSelection() == IIR_BILINEAR)
    fprintf(pFile, "IIR filter z-domain coefficients designed with the "
                   "Bilinear Transform Method\n\n");
  else
    fprintf(pFile, "IIR filter z-domain coefficients designed with the Matched "
                   "z-Transform Method\n\n");

  fprintf(pFile, "IIR Specification = ");
  if (FilterSpecification.BandSpec == IIR_LPF)
    fprintf(pFile, "Low Pass ");
  else if (FilterSpecification.BandSpec == IIR_HPF)
    fprintf(pFile, "High Pass ");
  else if (FilterSpecification.BandSpec == IIR_BPF)
    fprintf(pFile, "Band Pass ");
  else if (FilterSpecification.BandSpec == IIR_BSF)
    fprintf(pFile, "Band Stop ");

  if (FilterSpecification.DesignAlgo == IIR_BUTTERWORTH)
    fprintf(pFile, "Butterworth Filter\n\n");
  else if (FilterSpecification.DesignAlgo == IIR_CHEBYSCHEV)
    fprintf(pFile, "Tchebyschev Filter\n\n");
  else if (FilterSpecification.DesignAlgo == IIR_INVERSE_CHEBYSCHEV)
    fprintf(pFile, "Inverse Tchebyschev Filter\n\n");
  else if (FilterSpecification.DesignAlgo == IIR_ELLIPTIC)
    fprintf(pFile, "Elliptic Filter\n\n");
  else if (FilterSpecification.DesignAlgo == IIR_BESSEL)
    fprintf(pFile, "Bessel Filter\n\n");

  fprintf(pFile, "Polar coordinates of Poles and Zeros for IIR filter with %d biquads\n\n", (short)NumberOfBiquads);
  fprintf(pFile, "Coefficient order for each biquad :\n\tPole Magnitude, Pole Angle "
                 "(degrees)\n\tZero Magnitude, Zero Angle (degrees).\n\n");

  fprintf(pFile, "const SLData_t zDomainPolarCoefficientArray [] =\n{\n");

  /* Write IIR poles and zeros to file */
  for (i = 0; i < (short)FilterSpecification.Order; i++) {
    TmpPolar = SCV_RectangularToPolar(pZPlanePoles[i]);
    fprintf(pFile, "\t%1.20le, %1.20le,\n", TmpPolar.magn, TmpPolar.angle * SIGLIB_THREE_SIXTY_OVER_TWO_PI);
    TmpPolar = SCV_RectangularToPolar(pZPlaneZeros[i]);
    fprintf(pFile, "\t%1.20le, %1.20le", TmpPolar.magn, TmpPolar.angle * SIGLIB_THREE_SIXTY_OVER_TWO_PI);

    if (i < ((short)FilterSpecification.Order - 1))
      fprintf(pFile, ",\n\n");
  }
  fprintf(pFile, "\n};\n\n");

  fprintf(pFile,
          "Rectangular coordinates of Poles and Zeros for IIR filter with %d "
          "biquads\n\n",
          (short)NumberOfBiquads);
  fprintf(pFile, "Coefficient order for each biquad :\n\tPole Real, Pole "
                 "Imaginary\n\tZero Real, Zero Imaginary.\n\n");

  fprintf(pFile, "const SLData_t zDomainRectangularCoefficientArray [] =\n{\n");

  /* Write IIR poles and zeros to file */
  for (i = 0; i < (short)FilterSpecification.Order; i++) {
    fprintf(pFile, "\t%1.20le, %1.20le,\n", pZPlanePoles[i].real, pZPlanePoles[i].imag);
    fprintf(pFile, "\t%1.20le, %1.20le", pZPlaneZeros[i].real, pZPlaneZeros[i].imag);

    if (i < ((short)FilterSpecification.Order - 1))
      fprintf(pFile, ",\n\n");
  }
  fprintf(pFile, "\n};\n\n");

  (void)fclose(pFile);

#if DEBUG_LOG_FILE
  SUF_Debugfprintf("OnFilterSpecChanged : z-domain coefficient file written\n");
#endif

  pFilterInfo->IIRFilterOrder = FilterSpecification.Order;

  SLData_t IntegralSum = SIGLIB_ZERO;
  SDA_Integrate(pImpulseResponse, pStepResponse, SIGLIB_MAX, SIGLIB_ONE, &IntegralSum, GRAPH_WIDTH);    // Calculate step response

#if DEBUG_LOG_FILE
  SUF_Debugfprintf("OnFilterSpecChanged : Initiating graph plot\n");
#endif

  wxCommandEvent* DummyCommand = new wxCommandEvent();    // Post event to say that coeffs have been updated
  GlobalGraphDisplayWindow->OnGraphTypeChanged(*DummyCommand);
  delete (DummyCommand);

#if DEBUG_LOG_FILE
  SUF_Debugfprintf("IIRTraditionalFilterDesignWindow::OnGetFilterSpec : "
                   "Completed all processing\n\n");
#endif

} /* End of IIRTraditionalFilterDesignWindow::OnGetFilterSpec () */

void IIRTraditionalFilterDesignWindow::SetProjectNameString(wxString String)
{
  ProjectNameString = String;
}

void IIRTraditionalFilterDesignWindow::SetSampleRate(double sampleRate)
{
  SampleRate = sampleRate;
}

void IIRTraditionalFilterDesignWindow::SetGainArray(SLData_t* gain)
{
  pGain = gain;
}

void IIRTraditionalFilterDesignWindow::SetPhaseArray(SLData_t* phase)
{
  pPhase = phase;
}

void IIRTraditionalFilterDesignWindow::SetGroupDelayArray(SLData_t* groupDelay)
{
  pGroupDelay = groupDelay;
}

void IIRTraditionalFilterDesignWindow::SetImpulseResponseArray(SLData_t* impulseResponse)
{
  pImpulseResponse = impulseResponse;
}

void IIRTraditionalFilterDesignWindow::SetStepResponseArray(SLData_t* stepResponse)
{
  pStepResponse = stepResponse;
}

void IIRTraditionalFilterDesignWindow::SetPoleZeroPlotArray(SLComplexRect_s* PoleZeroArray)
{
  pPolesAndZeros = PoleZeroArray;
}

void IIRTraditionalFilterDesignWindow::SetFilterInfoStruct(filter_info_t* filterInfo)
{
  pFilterInfo = filterInfo;
}

BEGIN_EVENT_TABLE(IIRTraditionalFilterDesignWindow, wxPanel)
EVT_COMBOBOX(IIRBLT_FILTER_BAND_COMBO_BOX, IIRTraditionalFilterDesignWindow::OnFilterSpecChanged)
EVT_COMBOBOX(IIRBLT_FILTER_TYPE_COMBO_BOX, IIRTraditionalFilterDesignWindow::OnFilterSpecChanged)
EVT_COMBOBOX(IIRBLT_TRANSLATION_TYPE_COMBO_BOX, IIRTraditionalFilterDesignWindow::OnFilterSpecChanged)
EVT_BUTTON(IIRBLT_GET_FILTER_SPEC, IIRTraditionalFilterDesignWindow::OnGetFilterSpec)
//  EVT_TEXT_ENTER  (IIRBLT_FILTER_BAND_COMBO_BOX,
//  IIRTraditionalFilterDesignWindow::OnGetFilterSpec) EVT_TEXT_ENTER
//  (IIRBLT_BE1, IIRTraditionalFilterDesignWindow::OnGetFilterSpec)
//  EVT_TEXT_ENTER  (IIRBLT_BE2,
//  IIRTraditionalFilterDesignWindow::OnGetFilterSpec) EVT_TEXT_ENTER
//  (IIRBLT_BE3, IIRTraditionalFilterDesignWindow::OnGetFilterSpec)
//  EVT_TEXT_ENTER  (IIRBLT_BE4,
//  IIRTraditionalFilterDesignWindow::OnGetFilterSpec) EVT_TEXT_ENTER
//  (IIRBLT_PBR, IIRTraditionalFilterDesignWindow::OnGetFilterSpec)
//  EVT_TEXT_ENTER  (IIRBLT_SBA,
//  IIRTraditionalFilterDesignWindow::OnGetFilterSpec)

EVT_TEXT_ENTER(wxID_ANY, IIRTraditionalFilterDesignWindow::OnGetFilterSpec)
END_EVENT_TABLE()
