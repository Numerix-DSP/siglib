// FIR algorithms digital filter functions
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
#include <math.h>
#include <stdlib.h>
#if __APPLE__
#  include <malloc/malloc.h>
#else
#  include <malloc.h>
#endif
#include <siglib.h>    // SigLib DSP library
#include "ngl.h"
#include "dfplus.h"

extern GraphDisplay* GlobalGraphDisplayWindow;

extern int global_mValue;    // Global m and n values for Q number format
extern int global_nValue;

/*
  Name : FIRAlgsFilterDesignWindow::FIRAlgsFilterDesignWindow
  Description : Child widget that is used to get the FIR filter spec.
  Notes :
*/

FIRAlgsFilterDesignWindow::FIRAlgsFilterDesignWindow(wxWindow* parent, wxWindowID id)
    : wxPanel(parent, id)

{
  // Top-level layout - a vertical box sizer to contain all the controls
  wxStaticBoxSizer* VSizer = new wxStaticBoxSizer(wxVERTICAL, this, "FIR Design Algorithms");

  wxBoxSizer* HSizer1 = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizer2PP = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizer2P = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizer2 = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizer3 = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizer3GPP = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizer3GP = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizer3G = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizer3GA = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizer4P = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizer4 = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizer5 = new wxBoxSizer(wxHORIZONTAL);

  VSizer->Add(HSizer1, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add(HSizer2PP, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add(HSizer2P, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add(HSizer2, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add(HSizer3, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add(HSizer3GPP, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add(HSizer3GP, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add(HSizer3G, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add(HSizer3GA, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add(HSizer4P, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add(HSizer4, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add(HSizer5, 0, wxALIGN_CENTER | wxALL, 2);

  // A combo box for chosing the filter type
  HSizer1->Add(new wxStaticText(this, -1, "Filter type : "), 0, wxALIGN_TOP | wxALL, 0);
  static const wxString FilterChoices[] = {"Hilbert transformer", "Raised cosine", "Square root raised cosine", "Gaussian filter"};
  FilterTypeComboBox = new wxComboBox(this, FIRALGS_FILTER_TYPE_CHANGED, wxEmptyString, wxDefaultPosition, wxDefaultSize, WXSIZEOF(FilterChoices),
                                      FilterChoices, wxCB_READONLY);
  FilterTypeComboBox->SetSelection(0);    // Set the default selection
  HSizer1->Add(FilterTypeComboBox, 0, wxALIGN_LEFT | wxALL, 0);

  // Text control for raised cosine alpha
  HSizer2P->Add(new wxStaticText(this, -1, "Raised cosine filter parameters"), 0, wxALIGN_TOP | wxALL, 0);
  HSizer2->Add(new wxStaticText(this, -1, "Alpha           : "), 0, wxALIGN_TOP | wxALL, 0);
  RaisedCosineAlphaLine = new wxTextCtrl(this, -1, "0.0");
  HSizer2->Add(RaisedCosineAlphaLine);

  // Text control for raised cosine symbol rate
  HSizer3->Add(new wxStaticText(this, -1, "Symbol rate : "), 0, wxALIGN_TOP | wxALL, 0);
  RaisedCosineSymRateLine = new wxTextCtrl(this, -1, "0.0");
  HSizer3->Add(RaisedCosineSymRateLine);

#if COMMENT    // This code tried to put the RCF parameters in a box
  wxStaticBox* RCStaticBox = new wxStaticBox(this, -1, "Raised cosine filter parameters", wxDefaultPosition, wxDefaultSize);
  HSizer2->Add(RCStaticBox, 0, wxALIGN_TOP | wxALL, 0);

  wxStaticBoxSizer* RCVSizer = new wxStaticBoxSizer(RCStaticBox, wxVERTICAL);
  wxBoxSizer* RCHSizer1 = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* RCHSizer2 = new wxBoxSizer(wxHORIZONTAL);
  RCVSizer->Add(RCHSizer1, 0, wxALIGN_LEFT | wxALL, 2);
  RCVSizer->Add(RCHSizer2, 0, wxALIGN_LEFT | wxALL, 2);

  RCHSizer1->Add(new wxStaticText(this, -1, "Alpha           : "), 0, wxALIGN_TOP | wxALL, 0);
  RaisedCosineAlphaLine = new wxTextCtrl(this, -1, "0.0");
  RCHSizer1->Add(RaisedCosineAlphaLine);

  // Text control for raised cosine symbol rate
  RCHSizer2->Add(new wxStaticText(this, -1, "Symbol rate : "), 0, wxALIGN_TOP | wxALL, 0);
  RaisedCosineSymRateLine = new wxTextCtrl(this, -1, "0.0");
  RCHSizer2->Add(RaisedCosineSymRateLine);
#endif

  // Text control for Gaussian filter parameter
  HSizer3GP->Add(new wxStaticText(this, -1, "Gaussian filter parameter"), 0, wxALIGN_TOP | wxALL, 0);

  // Radio button for Gaussian filter type
  HSizer3G->Add(new wxStaticText(this, -1, "Design mode     "), 0, wxALIGN_TOP | wxALL, 0);
  GaussianRadioButton1 = new wxRadioButton(this, ID_GAUSSIAN_RADIOBUTTON_1, _T("&Std. Dev."), wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
  GaussianRadioButton2 = new wxRadioButton(this, ID_GAUSSIAN_RADIOBUTTON_2, _T("&Bandwidth"), wxDefaultPosition, wxDefaultSize);
  HSizer3G->Add(GaussianRadioButton1);
  HSizer3G->Add(GaussianRadioButton2);
  GaussianRadioButton1->SetValue(TRUE);
  GaussianRadioButton2->SetValue(FALSE);

  // Text control for Gaussian filter standard distribution or Bandwidth
  HSizer3GA->Add(new wxStaticText(this, -1, "Std Dev. or BW  "), 0, wxALIGN_TOP | wxALL, 0);
  GaussianAlphaLine = new wxTextCtrl(this, -1, "0.0");
  HSizer3GA->Add(GaussianAlphaLine);

  // This spin box requests the number of filter coefficients
  HSizer4->Add(new wxStaticText(this, -1, "Number of coefficients : "), 0, wxALIGN_TOP | wxALL, 0);
  NumCoeffsSpinControl = new wxSpinCtrl(this, -1, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, MAX_FIR_LENGTH, 1);
  NumCoeffsSpinControl->SetValue(1);    // Set default value
  HSizer4->Add(NumCoeffsSpinControl);

  // This button calculates the filter and displays the graph
  wxButton* CalculateButton = new wxButton(this, FIRALGS_GET_FILTER_SPEC, "Calculate");
  HSizer5->Add(CalculateButton, 0, wxALIGN_CENTER | wxALL, 0);
  CalculateButton->SetDefault();
  //  SetDefaultItem (CalculateButton);

  FilterTypeComboBox->SetFocus();    // give the FilterTypeComboBox the focus at the beginning

  // Default is Hilbert transformer
  RaisedCosineAlphaLine->Enable(FALSE);
  RaisedCosineSymRateLine->Enable(FALSE);
  GaussianRadioButton1->Enable(FALSE);
  GaussianRadioButton2->Enable(FALSE);
  GaussianAlphaLine->Enable(FALSE);

  SetAutoLayout(TRUE);
  SetSizer(VSizer);

  VSizer->SetSizeHints(this);
  VSizer->Fit(this);
}

/*
  Name : FIRAlgsFilterDesignWindow::OnFilterTypeComboBoxChanged
  Description : Enable / disable appropriate filter entry points
  Notes :
*/

void FIRAlgsFilterDesignWindow::OnFilterTypeComboBoxChanged(wxCommandEvent& WXUNUSED(event))

{
  //  wxMessageBox ("Got Here", "OnFilterTypeComboBoxChanged", wxOK |
  //  wxICON_EXCLAMATION, NULL);
  SUF_Log("FIRAlgsFilterDesignWindow::OnFilterTypeComboBoxChanged\n");

  switch (FilterTypeComboBox->GetSelection()) {
  case FIR_HILBERT_TRANSFORM:
    RaisedCosineAlphaLine->Enable(FALSE);
    RaisedCosineSymRateLine->Enable(FALSE);
    GaussianRadioButton1->Enable(FALSE);
    GaussianRadioButton2->Enable(FALSE);
    GaussianAlphaLine->Enable(FALSE);
    break;
  case FIR_RAISED_COSINE:
    RaisedCosineAlphaLine->Enable(TRUE);
    RaisedCosineSymRateLine->Enable(TRUE);
    GaussianRadioButton1->Enable(FALSE);
    GaussianRadioButton2->Enable(FALSE);
    GaussianAlphaLine->Enable(FALSE);
    break;
  case FIR_ROOT_RAISED_COSINE:
    RaisedCosineAlphaLine->Enable(TRUE);
    RaisedCosineSymRateLine->Enable(TRUE);
    GaussianRadioButton1->Enable(FALSE);
    GaussianRadioButton2->Enable(FALSE);
    GaussianAlphaLine->Enable(FALSE);
    break;
  case FIR_GAUSSIAN:
    RaisedCosineAlphaLine->Enable(FALSE);
    RaisedCosineSymRateLine->Enable(FALSE);
    GaussianRadioButton1->Enable(TRUE);
    GaussianRadioButton2->Enable(TRUE);
    GaussianAlphaLine->Enable(TRUE);
    break;
  }
}

/*
  Name : FIRAlgsFilterDesignWindow::GetFIRFilterSpec
  Description : Get the filter spec. from the user entries
  Notes :
*/

void FIRAlgsFilterDesignWindow::OnGetFilterSpec(wxCommandEvent& WXUNUSED(event))

{
  SUF_Log("FIRAlgsFilterDesignWindow::OnGetFilterSpec\n");

  char* stopstring;
  SLArrayIndex_t i;

  FilterType = (SLArrayIndex_t)FilterTypeComboBox->GetSelection();    // Get the filter type

  if ((FilterType == FIR_RAISED_COSINE) || (FilterType == FIR_ROOT_RAISED_COSINE)) {
    RaisedCosineAlpha = strtod(RaisedCosineAlphaLine->GetValue(), &stopstring);
    RaisedCosineSymRate = strtod(RaisedCosineSymRateLine->GetValue(), &stopstring);

    if ((RaisedCosineAlpha == SIGLIB_ZERO) || (RaisedCosineSymRate == SIGLIB_ZERO)) {
      wxMessageBox("The symbol rate and Alpha value\nmust be greater than zero", "Input error", wxOK | wxICON_EXCLAMATION, NULL);
      return;
    }
    if (RaisedCosineAlpha >= SIGLIB_ONE) {
      wxMessageBox("The Alpha value must be less than one", "Input error", wxOK | wxICON_EXCLAMATION, NULL);
      return;
    }
    if (RaisedCosineSymRate >= SampleRate) {
      wxMessageBox("The symbol rate must be less than the sample rate", "Input error", wxOK | wxICON_EXCLAMATION, NULL);
      return;
    }
  }

  if (FilterType == FIR_GAUSSIAN) {
    GaussianAlpha = strtod(GaussianAlphaLine->GetValue(), &stopstring);

    if (GaussianRadioButton1->GetValue() == TRUE) {    // Standard distribution
      if (GaussianAlpha == SIGLIB_ZERO) {
        wxMessageBox("The standard deviation of the distribution \nmust be "
                     "greater than zero",
                     "Input error", wxOK | wxICON_EXCLAMATION, NULL);
        return;
      }
    } else {    // Bandwidth
      if (GaussianAlpha == SIGLIB_ZERO) {
        wxMessageBox("The bandwidth \nmust be greater than zero", "Input error", wxOK | wxICON_EXCLAMATION, NULL);
        return;
      }
      if (GaussianAlpha >= SampleRate / SIGLIB_TWO) {
        wxMessageBox("The bandwidth \nmust be less than the Nyquist frequency", "Input error", wxOK | wxICON_EXCLAMATION, NULL);
        return;
      }
    }
  }

  NumberOfFilterCoeffs = NumCoeffsSpinControl->GetValue();    // Get the number of coefficients

  // Verify filter order
  if (NumberOfFilterCoeffs < 1) {
    NumCoeffsSpinControl->SetValue(1);
    NumberOfFilterCoeffs = 1;
  }

  if (NumberOfFilterCoeffs > MAX_FIR_LENGTH) {
    NumCoeffsSpinControl->SetValue(MAX_FIR_LENGTH);
    NumberOfFilterCoeffs = MAX_FIR_LENGTH;
  }

  //  Debug
  //  char String[20];
  //  sprintf (String, "# coeffs = %d", NumberOfFilterCoeffs);
  //  wxMessageBox (String, "OnGetFilterSpec", wxOK | wxICON_EXCLAMATION, NULL);
  //  sprintf (String, "RaisedCosineAlpha = %1.20lf", RaisedCosineAlpha);
  //  wxMessageBox (String, "OnGetFilterSpec", wxOK | wxICON_EXCLAMATION, NULL);
  //  if (FilterType == FIR_HILBERT_TRANSFORM)
  //    wxMessageBox ("Hilbert transform", "OnGetFilterSpec", wxOK |
  //    wxICON_EXCLAMATION, NULL);
  //  else if (FilterType == FIR_RAISED_COSINE)
  //    wxMessageBox ("Raised cosine filter", "OnGetFilterSpec", wxOK |
  //    wxICON_EXCLAMATION, NULL);
  //  else if (FilterType == FIR_ROOT_RAISED_COSINE)
  //    wxMessageBox ("Square root raised cosine filter", "OnGetFilterSpec",
  //    wxOK | wxICON_EXCLAMATION, NULL);

  static char FileNameString[500];
  FILE* pFile;

  strcpy(FileNameString, ProjectNameString);
  strcat(FileNameString, ".dfs");

  if ((pFile = fopen(FileNameString, "w")) == NULL) { /* Open o/p file */
    wxMessageBox("Could not open file to write filter specification", "Filter Specification", wxOK | wxICON_EXCLAMATION, NULL);
    return;
  }

  fprintf(pFile, DFPLUS_COPYRIGHT_NOTICE);
  fprintf(pFile, "File : %s\n\n", FileNameString);

  fprintf(pFile, "[Design Type]\n");
  if (FilterType == FIR_HILBERT_TRANSFORM) {    // Calculate coefficients
    fprintf(pFile, "FIR-HilbertTransform\n\n");
  } else if (FilterType == FIR_RAISED_COSINE) {
    fprintf(pFile, "FIR-RaisedCosine\n\n");
  } else if (FilterType == FIR_ROOT_RAISED_COSINE) {
    fprintf(pFile, "FIR-SquareRootRaisedCosine\n\n");
  }

  fprintf(pFile, "[Sample rate]\n");
  fprintf(pFile, "%1.20lf\n\n", SampleRate);

  fprintf(pFile, "[Filter specification]\n");
  if (FilterType == FIR_HILBERT_TRANSFORM) {
    fprintf(pFile, "FIR_HILBERT_TRANSFORM");
  } else if (FilterType == FIR_RAISED_COSINE) {
    fprintf(pFile, "FIR_RAISED_COSINE");
    fprintf(pFile, "[Alpha]\n");
    fprintf(pFile, "%lf\n\n", RaisedCosineAlpha);
  } else if (FilterType == FIR_ROOT_RAISED_COSINE) {
    fprintf(pFile, "FIR_ROOT_RAISED_COSINE");
    fprintf(pFile, "[Alpha]\n");
    fprintf(pFile, "%lf\n\n", RaisedCosineAlpha);
  }

  else if (FilterType == FIR_GAUSSIAN) {
    fprintf(pFile, "FIR_GAUSSIAN");
    if (GaussianRadioButton1->GetValue() == TRUE) {
      fprintf(pFile, "[Type]\nStandardDeviation\n");
      fprintf(pFile, "%lf\n\n", GaussianAlpha);
    } else {
      fprintf(pFile, "[Type]\nBandwidth\n");
      fprintf(pFile, "%lf\n\n", GaussianAlpha);
    }
  }

  fprintf(pFile, "[Number of coefficients]\n");
  fprintf(pFile, "%d\n\n", NumberOfFilterCoeffs);

  (void)fclose(pFile); /* Close output file */

  if (FilterType == FIR_HILBERT_TRANSFORM) {    // Calculate coefficients
    SIF_HilbertTransformer(pFIRCoeffs, NumberOfFilterCoeffs);
  } else if (FilterType == FIR_RAISED_COSINE) {
    SIF_RaisedCosineFilter(pFIRCoeffs, SampleRate / RaisedCosineSymRate, RaisedCosineAlpha, NumberOfFilterCoeffs);
  } else if (FilterType == FIR_ROOT_RAISED_COSINE) {
    SIF_RootRaisedCosineFilter(pFIRCoeffs, SampleRate / RaisedCosineSymRate, RaisedCosineAlpha, NumberOfFilterCoeffs);
  } else if (FilterType == FIR_GAUSSIAN) {
    if (GaussianRadioButton1->GetValue() == TRUE) {
      SIF_GaussianFilter(pFIRCoeffs, GaussianAlpha, NumberOfFilterCoeffs);
    } else {
      SIF_GaussianFilter2(pFIRCoeffs, GaussianAlpha / SampleRate,
                          NumberOfFilterCoeffs);    // Normalize bandwidth
    }
  }

  strcpy(FileNameString, ProjectNameString);
  strcat(FileNameString, ".dfc");
  if (FilterType == FIR_HILBERT_TRANSFORM) {
    fprintf(pFile, "FIR filter coefficients designed with the Hilbert "
                   "Transform Algorithm\n\n");
  } else if (FilterType == FIR_RAISED_COSINE) {
    fprintf(pFile, "FIR filter coefficients designed with the Raised Cosine "
                   "Algorithm\n\n");
  } else if (FilterType == FIR_ROOT_RAISED_COSINE) {
    fprintf(pFile, "FIR filter coefficients designed with the Square Root "
                   "Raised Cosine Algorithm\n\n");
  } else if (FilterType == FIR_GAUSSIAN) {
    fprintf(pFile, "Gaussian FIR filter coefficients\n\n");
  }

  if ((pFile = fopen(FileNameString, "w")) == NULL) /* Open o/p file */
  {
    wxMessageBox("Could not open file to write filter coefficients", "Filter Specification", wxOK | wxICON_EXCLAMATION, NULL);
    return;
  }

  fprintf(pFile, DFPLUS_COPYRIGHT_NOTICE);
  fprintf(pFile, "File : %s\n\n", FileNameString);

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
  if ((pFile = fopen(FileNameString, "w")) == NULL) { /* Open o/p file */
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

} /* End of FIRAlgsFilterDesignWindow::OnGetFilterSpec () */

void FIRAlgsFilterDesignWindow::SetProjectNameString(wxString String)
{
  ProjectNameString = String;
}

void FIRAlgsFilterDesignWindow::SetSampleRate(double sampleRate)
{
  SampleRate = sampleRate;
}

void FIRAlgsFilterDesignWindow::SetGainArray(SLData_t* gain)
{
  pGain = gain;
}

void FIRAlgsFilterDesignWindow::SetPhaseArray(SLData_t* phase)
{
  pPhase = phase;
}

void FIRAlgsFilterDesignWindow::SetGroupDelayArray(SLData_t* groupDelay)
{
  pGroupDelay = groupDelay;
}

void FIRAlgsFilterDesignWindow::SetImpulseResponseArray(SLData_t* impulseResponse)
{
  pImpulseResponse = impulseResponse;
}

void FIRAlgsFilterDesignWindow::SetStepResponseArray(SLData_t* stepResponse)
{
  pStepResponse = stepResponse;
}

void FIRAlgsFilterDesignWindow::SetFilterInfoStruct(filter_info_t* filterInfo)
{
  pFilterInfo = filterInfo;
}

BEGIN_EVENT_TABLE(FIRAlgsFilterDesignWindow, wxPanel)
EVT_COMBOBOX(FIRALGS_FILTER_TYPE_CHANGED, FIRAlgsFilterDesignWindow::OnFilterTypeComboBoxChanged)
EVT_BUTTON(FIRALGS_GET_FILTER_SPEC, FIRAlgsFilterDesignWindow::OnGetFilterSpec)
END_EVENT_TABLE()
