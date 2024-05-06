// FIR Simple Remez exchange algorithm digital filter functions
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

#include <stdlib.h>
#if __APPLE__
#  include <malloc/malloc.h>
#else
#  include <malloc.h>
#endif
#include "dfplus.h"
#include "ngl.h"
#include "remez_gen.h"
#include <math.h>
#include <siglib.h>

#define BANDPASS 1 /* Filter types */
#define DIFFERENTIATOR 2
#define HILBERT 3

extern GraphDisplay* GlobalGraphDisplayWindow;

extern int global_mValue;    // Global m and n values for Q number format
extern int global_nValue;

/*
  Name : FIRSimpleRemezFilterDesignWindow::FIRSimpleRemezFilterDesignWindow
  Description : Child widget that is used to get the IIR BLT filter spec.
  Notes :
*/

FIRSimpleRemezFilterDesignWindow::FIRSimpleRemezFilterDesignWindow(wxWindow* parent, wxWindowID id)
    : wxPanel(parent, id)

{
  // Top-level layout - a vertical box sizer to contain all the controls
  wxStaticBoxSizer* VSizer = new wxStaticBoxSizer(wxVERTICAL, this, "FIR Remez Algorithm Design");

  wxBoxSizer* HSizer1 = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizer15 = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizer21 = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizer22 = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizer23 = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizer24 = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizer3 = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizer31 = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizer32 = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizer4 = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizer41 = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizer42 = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizerP5 = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizer5 = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizer6P = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizer6 = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizer7 = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizer8P = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizer8 = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizer9P = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizer9 = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizer9a = new wxBoxSizer(wxHORIZONTAL);

  VSizer->Add(HSizer1, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add(HSizer15, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add(HSizer21, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add(HSizer22, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add(HSizer23, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add(HSizer24, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add(HSizer3, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add(HSizer31, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add(HSizer32, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add(HSizer4, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add(HSizer41, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add(HSizer42, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add(HSizerP5, 0, wxALIGN_CENTER | wxALL, 2);
  VSizer->Add(HSizer5, 0, wxALIGN_CENTER | wxALL, 2);
  VSizer->Add(HSizer6P, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add(HSizer6, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add(HSizer7, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add(HSizer8P, 0, wxALIGN_CENTER | wxALL, 2);
  VSizer->Add(HSizer8, 0, wxALIGN_CENTER | wxALL, 2);
  VSizer->Add(HSizer9P, 0, wxALIGN_CENTER | wxALL, 2);
  VSizer->Add(HSizer9, 0, wxALIGN_CENTER | wxALL, 2);
  VSizer->Add(HSizer9a, 0, wxALIGN_CENTER | wxALL, 2);

  // This is the FIR filter specification section
  // A combo box for getting filter band type
  HSizer1->Add(new wxStaticText(this, -1, "Filter type : "), 0, wxALIGN_TOP | wxALL, 0);
  static const wxString FreqChoices[] = {"Low pass", "High pass", "Band pass", "Band stop"};
  FilterBandComboBox = new wxComboBox(this, FIRSIMPLEREMEZ_FILTER_BAND_COMBO_BOX, wxEmptyString, wxDefaultPosition, wxDefaultSize,
                                      WXSIZEOF(FreqChoices), FreqChoices, wxCB_READONLY);
  FilterBandComboBox->SetSelection(0);    // Set the default selection
  HSizer1->Add(FilterBandComboBox, 0, wxALIGN_LEFT | wxALL, 0);

  HSizer21->Add(new wxStaticText(this, -1, "Pass band : "), 0, wxALIGN_TOP | wxALL, 0);
  // Text control for pass-band FLow
  HSizer22->Add(new wxStaticText(this, -1, "Cut-off frequency 1 :  "), 0, wxALIGN_TOP | wxALL, 0);
  PassBandFc1Line = new wxTextCtrl(this, -1, "0.0");
  HSizer22->Add(PassBandFc1Line);

  // Text control for pass-band FHigh
  HSizer23->Add(new wxStaticText(this, -1, "Cut-off frequency 2 :  "), 0, wxALIGN_TOP | wxALL, 0);
  PassBandFc2Line = new wxTextCtrl(this, -1, "0.0");
  HSizer23->Add(PassBandFc2Line);
  PassBandFc2Line->Enable(FALSE);    // Disable because default is LPF

  // Text control for pass band ripple
  HSizer24->Add(new wxStaticText(this, -1, "Ripple (dB)   :             "), 0, wxALIGN_TOP | wxALL, 0);
  PassBandRippleLine = new wxTextCtrl(this, -1, "1.0");
  HSizer24->Add(PassBandRippleLine);

  HSizer3->Add(new wxStaticText(this, -1, " "), 0, wxALIGN_TOP | wxALL, 0);
  HSizer31->Add(new wxStaticText(this, -1, "Transition Band"), 0, wxALIGN_TOP | wxALL, 0);
  // Text control for transition bandwidth
  HSizer32->Add(new wxStaticText(this, -1, "Bandwidth :              "), 0, wxALIGN_TOP | wxALL, 0);
  TransitionBWLine = new wxTextCtrl(this, -1, "0.0");
  HSizer32->Add(TransitionBWLine);

  HSizer4->Add(new wxStaticText(this, -1, " "), 0, wxALIGN_TOP | wxALL, 0);
  HSizer41->Add(new wxStaticText(this, -1, "Stop band : "), 0, wxALIGN_TOP | wxALL, 0);
  // Text control for stop band attenuation
  HSizer42->Add(new wxStaticText(this, -1, "Attenuation (dB) :       "), 0, wxALIGN_TOP | wxALL, 0);
  StopBandAttenuationLine = new wxTextCtrl(this, -1, "0.0");
  HSizer42->Add(StopBandAttenuationLine);

  HSizerP5->Add(new wxStaticText(this, -1, " "), 0, wxALIGN_TOP | wxALL, 0);
  // This button estimates the filter order
  wxButton* EstimateButton = new wxButton(this, FIRSIMPLEREMEZ_ESTIMATE_ORDER, "Estimate Order");
  HSizer5->Add(EstimateButton, 0, wxALIGN_CENTER | wxALL, 0);
  EstimateButton->SetDefault();

  // This spin box requests the number of poly-phase filter banks
  HSizer6->Add(new wxStaticText(this, -1, "Poly-phase filter banks : "), 0, wxALIGN_TOP | wxALL, 0);
  PolyPhaseSpinControl = new wxSpinCtrl(this, -1, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, MAX_FIR_PP_BANKS, 1);
  PolyPhaseSpinControl->SetValue(1);    // Set default value
  HSizer6->Add(PolyPhaseSpinControl);

  // This spin box requests the number of filter coefficients
  HSizer7->Add(new wxStaticText(this, -1, "Number of coefficients : "), 0, wxALIGN_TOP | wxALL, 0);
  NumCoeffsSpinControl = new wxSpinCtrl(this, -1, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, MAX_FIR_LENGTH, 1);
  NumCoeffsSpinControl->SetValue(1);    // Set default value
  HSizer7->Add(NumCoeffsSpinControl);

  // This button calculates the filter and displays the graph
  wxButton* CalculateButton = new wxButton(this, FIRSIMPLEREMEZ_GET_FILTER_SPEC, "Calculate");
  HSizer8->Add(CalculateButton, 0, wxALIGN_CENTER | wxALL, 0);
  CalculateButton->SetDefault();
  //  SetDefaultItem (CalculateButton);

  HSizer9->Add(new wxStaticText(this, -1, "Large filters may take a few seconds"), 0, wxALIGN_TOP | wxALIGN_CENTER | wxALL, 0);
  HSizer9a->Add(new wxStaticText(this, -1, "to calculate"), 0, wxALIGN_TOP | wxALIGN_CENTER | wxALL, 0);

  FilterBandComboBox->SetFocus();    // give the FilterBand1FLowLine the focus at the beginning

  SetAutoLayout(TRUE);
  SetSizer(VSizer);

  VSizer->SetSizeHints(this);
  VSizer->Fit(this);
}

/*
  Name : FIRSimpleRemezFilterDesignWindow::GetFilterSpec
  Description : Get the filter spec. from the user entries
  Notes :
*/

void FIRSimpleRemezFilterDesignWindow::OnGetFilterSpec(wxCommandEvent& WXUNUSED(event))

{
  char* stopstring;
  SLArrayIndex_t i, j, k;
  short RemezReturn;

#if DEBUG_LOG_FILE
  SUF_Debugfprintf("FIRSimpleRemezFilterDesignWindow::OnGetFilterSpec : Starting\n");
  SUF_Debugfprintf("\nvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv\n");
#endif
  SUF_Log("FIRSimpleRemezFilterDesignWindow::OnGetFilterSpec\n");

  SLData_t Fc1 = strtod(PassBandFc1Line->GetValue(), &stopstring);
  SLData_t Fc2 = strtod(PassBandFc2Line->GetValue(), &stopstring);
  SLData_t TBW = strtod(TransitionBWLine->GetValue(), &stopstring);
  SLData_t PBR = strtod(PassBandRippleLine->GetValue(), &stopstring);
  SLData_t SBA = strtod(StopBandAttenuationLine->GetValue(), &stopstring);

  if ((TBW <= SIGLIB_ZERO) || (TBW >= (SampleRate / SIGLIB_TWO))) {
    wxMessageBox("The transition bandwidth must be greater than 0Hz\n and less "
                 "than the Nyquist frequency",
                 "Input error", wxOK | wxICON_EXCLAMATION, NULL);
    return;
  }

  if ((Fc1 < SIGLIB_ZERO) || (Fc1 > (SampleRate / SIGLIB_TWO))) {
    wxMessageBox("The lower cut-off frequency must be greater than 0Hz\n and "
                 "less than the Nyquist frequency",
                 "Input error", wxOK | wxICON_EXCLAMATION, NULL);
    return;
  }

  if ((FilterBandComboBox->GetSelection() == IIR_BPF) || (FilterBandComboBox->GetSelection() == IIR_BSF))    // BSF and BPF
  {
    if ((Fc2 < SIGLIB_ZERO) || (Fc2 > (SampleRate / SIGLIB_TWO))) {
      wxMessageBox("The upper cut-off frequency must be greater than 0Hz\n and "
                   "less than the Nyquist frequency",
                   "Input error", wxOK | wxICON_EXCLAMATION, NULL);
      return;
    }

    if (Fc1 >= Fc2) {
      wxMessageBox("Lower cut-off frequency much be less than the upper one", "Input error", wxOK | wxICON_EXCLAMATION, NULL);
      return;
    }
  }

  if (PBR == SIGLIB_ZERO) {
    wxMessageBox("The pass-band ripple must be non-zero", "Input error", wxOK | wxICON_EXCLAMATION, NULL);
    return;
  }

  if (SBA == SIGLIB_ZERO) {
    wxMessageBox("The stop-band attenuation must be non-zero", "Input error", wxOK | wxICON_EXCLAMATION, NULL);
    return;
  }

  switch (FilterBandComboBox->GetSelection())    // Range for BSF and BPF are half
  {
  case IIR_LPF: /* Low pass  */
    if ((TBW + Fc1) > (SampleRate / SIGLIB_TWO))
      wxMessageBox("The transition bandwidth is too wide for the pass-bandwidth", "Input error", wxOK | wxICON_EXCLAMATION, NULL);
    break;
  case IIR_HPF: /* High pass */
                //    if ((TBW + Fc1) > (SampleRate / SIGLIB_TWO))
    //      wxMessageBox ("The transition bandwidth is too wide for the
    //      pass-bandwidth", "Input error", wxOK | wxICON_EXCLAMATION, NULL);
    if (TBW > Fc1)
      wxMessageBox("The transition bandwidth must be less than the cut-off frequency", "Input error", wxOK | wxICON_EXCLAMATION, NULL);
    break;
  case IIR_BPF: /* Band pass */
    if (((SIGLIB_TWO * TBW) + (Fc2 - Fc1)) > (SampleRate / SIGLIB_TWO))
      wxMessageBox("The transition bandwidths are too wide for the pass-bandwidth", "Input error", wxOK | wxICON_EXCLAMATION, NULL);
    if (TBW > Fc1)
      wxMessageBox("The transition bandwidth must be less than the lower "
                   "cut-off frequency",
                   "Input error", wxOK | wxICON_EXCLAMATION, NULL);
    if ((Fc2 + TBW) > (SampleRate / SIGLIB_TWO))
      wxMessageBox("The transition bandwidth is too large for the upper "
                   "cut-off frequency",
                   "Input error", wxOK | wxICON_EXCLAMATION, NULL);
    break;
  case IIR_BSF: /* Notch */
    if (((SIGLIB_TWO * TBW) + (Fc2 - Fc1)) > (SampleRate / SIGLIB_TWO))
      wxMessageBox("The transition bandwidths are too wide for the stop-bandwidth", "Input error", wxOK | wxICON_EXCLAMATION, NULL);
    if (TBW > Fc1)
      wxMessageBox("The transition bandwidth must be less than the lower "
                   "cut-off frequency",
                   "Input error", wxOK | wxICON_EXCLAMATION, NULL);
    if ((Fc2 + TBW) > (SampleRate / SIGLIB_TWO))
      wxMessageBox("The transition bandwidth is too large for the upper "
                   "cut-off frequency",
                   "Input error", wxOK | wxICON_EXCLAMATION, NULL);
    break;
  }

  if (PBR <= SIGLIB_ZERO) {
    wxMessageBox("The pass-band ripple must be greater than zero", "Input error", wxOK | wxICON_EXCLAMATION, NULL);
    return;
  }

  if (SBA <= SIGLIB_ZERO) {
    wxMessageBox("The stop-band attenuation must be greater than zero", "Input error", wxOK | wxICON_EXCLAMATION, NULL);
    return;
  }

  NumberOfPPFilterBanks = (SLArrayIndex_t)PolyPhaseSpinControl->GetValue();    // Get the number of filter banks
  NumberOfFilterCoeffs = (SLArrayIndex_t)NumCoeffsSpinControl->GetValue();     // Get the number of coefficients

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

#if DEBUG_LOG_FILE
  SUF_Debugfprintf("FIRSimpleRemezFilterDesignWindow::OnGetFilterSpec : Got spec\n");
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

  fprintf(pFile, "[Design Type]\n");
  fprintf(pFile, "FIR-Remez\n\n");

  fprintf(pFile, "[Sample rate]\n");
  fprintf(pFile, "%1.20lf\n\n", SampleRate);

  fprintf(pFile, "[Number of coefficients]\n");
  fprintf(pFile, "%d\n\n", NumberOfFilterCoeffs);

  fprintf(pFile, "Fc1 = %lf, Fc2 = %lf, TBW = %lf, PBR = %lf, SBA = %lf\n", Fc1, Fc2, TBW, PBR, SBA);

  fprintf(pFile, "\n\n");

  (void)fclose(pFile); /* Close output file */

#if DEBUG_LOG_FILE
  SUF_Debugfprintf("FIRSimpleRemezFilterDesignWindow::OnGetFilterSpec : "
                   "Written spec file\n");
#endif

#if DEBUG_LOG_FILE
  SUF_Debugfprintf("Fc1 = %lf, Fc2 = %lf, TBW = %lf, PBR = %lf, SBA = %lf\n", Fc1, Fc2, TBW, PBR, SBA);
  SUF_Debugfprintf("# PP filter banks = %d\n", NumberOfPPFilterBanks);
  SUF_Debugfprintf("# coeffs = %d\n", NumberOfFilterCoeffs);
#endif

  {
    SLArrayIndex_t NumberOfFilterBands = 0L;
    SLData_t* pBands = SUF_VectorArrayAllocate(2 * SIMPLE_REMEZ_BANDS + 1);
    SLData_t* pFIRRippleW = SUF_VectorArrayAllocate(SIMPLE_REMEZ_BANDS + 1);
    SLData_t* pFIRGain = SUF_VectorArrayAllocate(SIMPLE_REMEZ_BANDS + 1);
    SLData_t* pTmpCoeffs = SUF_VectorArrayAllocate(NumberOfFilterCoeffs + 1);

    if ((pBands == NULL) || (pFIRRippleW == NULL) || (pFIRGain == NULL) || (pTmpCoeffs == NULL)) {
      wxMessageBox("Could not allocate memory for Remez exchange algorithm", "System error", wxOK | wxICON_EXCLAMATION, NULL);
      return;
    }

    switch (FilterBandComboBox->GetSelection())    // Range for BSF and BPF are half
    {
    case IIR_LPF: /* Low pass */
      pFIRGain[0] = 1.0;
      pFIRGain[1] = 0.0;
      pBands[0] = 0.0;
      pBands[1] = Fc1 / SampleRate;
      pBands[2] = (Fc1 + TBW) / SampleRate;
      pBands[3] = 0.5;
      pFIRRippleW[0] = PBR;
      pFIRRippleW[1] = SBA;
      NumberOfFilterBands = 2L;
      break;
    case IIR_HPF: /* High pass */
      pFIRGain[0] = 0.0;
      pFIRGain[1] = 1.0;
      pBands[0] = 0.0;
      pBands[1] = (Fc1 - TBW) / SampleRate;
      pBands[2] = Fc1 / SampleRate;
      pBands[3] = 0.5;
      pFIRRippleW[0] = SBA;
      pFIRRippleW[1] = PBR;
      NumberOfFilterBands = 2L;
      break;
    case IIR_BPF: /* Band pass */
      pFIRGain[0] = 0.0;
      pFIRGain[1] = 1.0;
      pFIRGain[2] = 0.0;
      pBands[0] = 0.0;
      pBands[1] = (Fc1 - TBW) / SampleRate;
      pBands[2] = Fc1 / SampleRate;
      pBands[3] = Fc2 / SampleRate;
      pBands[4] = (Fc2 + TBW) / SampleRate;
      pBands[5] = 0.5;
      pFIRRippleW[0] = SBA;
      pFIRRippleW[1] = PBR;
      pFIRRippleW[2] = SBA;
      NumberOfFilterBands = 3L;
      break;
    case IIR_BSF: /* Notch */
      pFIRGain[0] = 1.0;
      pFIRGain[1] = 0.0;
      pFIRGain[2] = 1.0;
      pBands[0] = 0.0;
      pBands[1] = (Fc1 - TBW) / SampleRate;
      pBands[2] = Fc1 / SampleRate;
      pBands[3] = Fc2 / SampleRate;
      pBands[4] = (Fc2 + TBW) / SampleRate;
      pBands[5] = 0.5;
      pFIRRippleW[0] = PBR;
      pFIRRippleW[1] = SBA;
      pFIRRippleW[2] = PBR;
      NumberOfFilterBands = 3L;
      break;
    }

#if DEBUG_LOG_FILE
    SUF_Debugfprintf("Spec :\n");
    for (i = 0; i < NumberOfFilterBands; i++) {
      SUF_Debugfprintf("Gain = %lf, BandL = %lf, BandH = %lf, Ripple = %lf\n", pFIRGain[i], pBands[2 * i], pBands[2 * i + 1], pFIRRippleW[i]);
    }
    SUF_Debugfprintf("\n");
#endif

#if DEBUG_LOG_FILE
    SUF_Debugfprintf("FIRSimpleRemezFilterDesignWindow::OnGetFilterSpec : Starting Remez\n");
#endif

    SLData_t* deviat = SUF_VectorArrayAllocate(SIMPLE_REMEZ_BANDS + 1);
    SLData_t* extrm = SUF_VectorArrayAllocate(NumberOfFilterCoeffs + 1);

    if ((deviat == NULL) || (extrm == NULL)) {
      wxMessageBox("Could not allocate memory for Remez exchange algorithm", "System error", wxOK | wxICON_EXCLAMATION, NULL);
      return;
    }

    short RemezReturn = remez(NumberOfFilterCoeffs, BANDPASS, NumberOfFilterBands, pBands, pFIRGain, pFIRRippleW, pTmpCoeffs, deviat, extrm);

    SDA_Copy(pTmpCoeffs, pFIRCoeffs,
             NumberOfFilterCoeffs);    // Copy data to output array

    free(pBands);
    free(pFIRRippleW);
    free(pFIRGain);
    free(pTmpCoeffs);
    free(deviat);
    free(extrm);

    if (RemezReturn == 1) {
      wxMessageBox("Remez error - invalid filter length", "Input error", wxOK | wxICON_EXCLAMATION, NULL);
      return;
    }
    if (RemezReturn == 2) {
      wxMessageBox("Remez error - invalid filter type", "Input error", wxOK | wxICON_EXCLAMATION, NULL);
      return;
    }
    if (RemezReturn == 3) {
      wxMessageBox("Remez error - the filter failed to converge\nPlease choose "
                   "a simpler filter.",
                   "Input error", wxOK | wxICON_EXCLAMATION, NULL);
      return;
    }
  }

#if DEBUG_LOG_FILE
  SUF_Debugfprintf("FIRSimpleRemezFilterDesignWindow::OnGetFilterSpec : Completed Remez\n");

  {
    short c;
    SUF_Debugfprintf("NumberOfFilterCoeffs = %lf\n", NumberOfFilterCoeffs);
    SUF_Debugfprintf("pFIRCoeffs[] = \n");
    for (c = 0; c < NumberOfFilterCoeffs; c++) {
      if ((c == 10) || (c == 20) || (c == 30) || (c == 40) || (c == 50) || (c == 60) || (c == 70))
        SUF_Debugfprintf("\n");
      SUF_Debugfprintf("%lf, ", pFIRCoeffs[c]);
    }
    SUF_Debugfprintf("\n");
  }
#endif

  for (i = 0; i < NumberOfFilterCoeffs; i++) {    // Check to see if the Remez has converged
    if ((*(pFIRCoeffs + i) > 1e10) || (SDS_Abs(*(pFIRCoeffs + i)) < 1e-40)) {
      wxMessageBox("The remez algorithm has failed to converge\n This is probably "
                   "because the specification is too demanding\n Please reduce the "
                   "specification or the number of coefficients.",
                   "Input error", wxOK | wxICON_EXCLAMATION, NULL);
      return;
    }
  }

#if DEBUG_LOG_FILE
  SUF_Debugfprintf("FIRSimpleRemezFilterDesignWindow::OnGetFilterSpec : "
                   "Completed convergence verification\n");
#endif

  strcpy(FileNameString, ProjectNameString);
  strcat(FileNameString, ".dfc");

  if ((pFile = fopen(FileNameString, "w")) == NULL) { /* Open o/p file */
    wxMessageBox("Could not open file to write filter coefficients", "OnGetFilterSpec", wxOK | wxICON_EXCLAMATION, NULL);
    return;
  }

  fprintf(pFile, DFPLUS_COPYRIGHT_NOTICE);
  fprintf(pFile, "File : %s\n\n", FileNameString);
  fprintf(pFile, "FIR filter coefficients designed with the Remez Exchange Algorithm\n\n");

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

      fprintf(pFile, "};\n\n");
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

      fprintf(pFile, "};\n\n");
    }
  }

  (void)fclose(pFile);

#if DEBUG_LOG_FILE
  SUF_Debugfprintf("FIRSimpleRemezFilterDesignWindow::OnGetFilterSpec : "
                   "Written coeffs file\n");
#endif

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

    if (X < SIGLIB_ZERO) {    // SDS_Sign (X) == -1)
      ThetaT = (SDS_Atan(JY / X)) + (SDS_Sign(JY) * SIGLIB_PI);
    }

    else if (X > SIGLIB_ZERO) {
      ThetaT = SDS_Atan(JY / X);
    }

    else {    // X == 0.0
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

    //    DBGain = sqrt ((X*X)+(JY*JY));
    pGain[GC] = DBGain;
    pPhase[GC] = ThetaT;
    pGroupDelay[GC] = GPDel;
  }
#if DEBUG_LOG_FILE
  SUF_Debugfprintf("FIRSimpleRemezFilterDesignWindow::OnGetFilterSpec : "
                   "Calculated response\n");
#endif

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

#if DEBUG_LOG_FILE
  SUF_Debugfprintf("OnFilterSpecChanged : Initiating graph plot\n");
#endif

  wxCommandEvent* DummyCommand = new wxCommandEvent();    // Post event to say that coeffs have been updated
  GlobalGraphDisplayWindow->OnGraphTypeChanged(*DummyCommand);
  delete (DummyCommand);

#if DEBUG_LOG_FILE
  SUF_Debugfprintf("FIRSimpleRemezFilterDesignWindow::OnGetFilterSpec : "
                   "Completed all processing\n");
#endif

} /* End of FIRSimpleRemezFilterDesignWindow::OnGetFilterSpec () */

/*
  Name : FIRSimpleRemezFilterDesignWindow::OnEstimateOrder
  Description : Estimate the required filter order
  Notes :
*/

void FIRSimpleRemezFilterDesignWindow::OnEstimateOrder(wxCommandEvent& WXUNUSED(event))

{
  char* stopstring;
  SUF_Log("FIRSimpleRemezFilterDesignWindow::OnEstimateOrder\n");

  SLData_t Fc1 = strtod(PassBandFc1Line->GetValue(), &stopstring);
  SLData_t Fc2 = strtod(PassBandFc2Line->GetValue(), &stopstring);
  SLData_t TBW = strtod(TransitionBWLine->GetValue(), &stopstring);
  SLData_t PBR = strtod(PassBandRippleLine->GetValue(), &stopstring);
  SLData_t SBA = strtod(StopBandAttenuationLine->GetValue(), &stopstring);

  switch (FilterBandComboBox->GetSelection())    // Range for BSF and BPF are half
  {
  case IIR_LPF: /* Low pass / High pass */
    NumCoeffsSpinControl->SetValue(SUF_FirKaiserApproximation(Fc1, Fc1 + TBW, PBR, SBA, SampleRate));
    break;
  case IIR_HPF:
    NumCoeffsSpinControl->SetValue(SUF_FirKaiserApproximation(SampleRate - Fc1, (SampleRate - Fc1) + TBW, PBR, SBA, SampleRate));
    break;
  case IIR_BPF: /* Band pass / Notch */
    NumCoeffsSpinControl->SetValue(SUF_FirKaiserApproximation((Fc2 - Fc1) / 2.0, ((Fc2 - Fc1) / 2.0) + TBW, PBR, SBA, SampleRate));
    break;
  case IIR_BSF:
    NumCoeffsSpinControl->SetValue(
        SUF_FirKaiserApproximation(SampleRate - ((Fc2 - Fc1) / 2.0), SampleRate - (((Fc2 - Fc1) / 2.0) + TBW), PBR, SBA, SampleRate));
    break;
  }
}

/*
  Name : FIRSimpleRemezFilterDesignWindow::OnFilterSpecChanged
  Description : Enable / disable appropriate filter band edges etc.
  Notes :
*/

void FIRSimpleRemezFilterDesignWindow::OnFilterSpecChanged(wxCommandEvent& WXUNUSED(event))

{
  //  wxMessageBox ("Got Here", "OnFilterSpecChanged", wxOK |
  //  wxICON_EXCLAMATION, NULL);
  SUF_Log("FIRSimpleRemezFilterDesignWindow::OnFilterSpecChanged\n");

  switch (FilterBandComboBox->GetSelection())    // Range for BSF and BPF are half
  {
  case IIR_LPF: /* Low pass / High pass */
  case IIR_HPF:
    PassBandFc1Line->Enable(TRUE);
    PassBandFc2Line->Enable(FALSE);
    break;
  case IIR_BPF: /* Band pass / Notch */
  case IIR_BSF:
    PassBandFc1Line->Enable(TRUE);
    PassBandFc2Line->Enable(TRUE);
    break;
  }
}

void FIRSimpleRemezFilterDesignWindow::SetProjectNameString(wxString String)
{
  ProjectNameString = String;
}

void FIRSimpleRemezFilterDesignWindow::SetSampleRate(double sampleRate)
{
  SampleRate = sampleRate;
}

void FIRSimpleRemezFilterDesignWindow::SetGainArray(SLData_t* gain)
{
  pGain = gain;
}

void FIRSimpleRemezFilterDesignWindow::SetPhaseArray(SLData_t* phase)
{
  pPhase = phase;
}

void FIRSimpleRemezFilterDesignWindow::SetGroupDelayArray(SLData_t* groupDelay)
{
  pGroupDelay = groupDelay;
}

void FIRSimpleRemezFilterDesignWindow::SetImpulseResponseArray(SLData_t* impulseResponse)
{
  pImpulseResponse = impulseResponse;
}

void FIRSimpleRemezFilterDesignWindow::SetStepResponseArray(SLData_t* stepResponse)
{
  pStepResponse = stepResponse;
}

void FIRSimpleRemezFilterDesignWindow::SetFilterInfoStruct(filter_info_t* filterInfo)
{
  pFilterInfo = filterInfo;
}

BEGIN_EVENT_TABLE(FIRSimpleRemezFilterDesignWindow, wxPanel)
EVT_COMBOBOX(FIRSIMPLEREMEZ_FILTER_BAND_COMBO_BOX, FIRSimpleRemezFilterDesignWindow::OnFilterSpecChanged)
EVT_BUTTON(FIRSIMPLEREMEZ_GET_FILTER_SPEC, FIRSimpleRemezFilterDesignWindow::OnGetFilterSpec)
EVT_BUTTON(FIRSIMPLEREMEZ_ESTIMATE_ORDER, FIRSimpleRemezFilterDesignWindow::OnEstimateOrder)
END_EVENT_TABLE()
