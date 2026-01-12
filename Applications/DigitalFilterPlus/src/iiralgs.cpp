// IIR algorithms digital filter functions
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
extern int global_NegateIIRAlphaSign;    // Global NegateIIRAlphaSign for biquads

/*
  Name : IIRAlgsFilterDesignWindow::IIRAlgsFilterDesignWindow
  Description : Child widget that is used to get the IIR filter spec.
  Notes :
*/

IIRAlgsFilterDesignWindow::IIRAlgsFilterDesignWindow(wxWindow* parent, wxWindowID id)
    : wxPanel(parent, id)

{
  // Top-level layout - a vertical box sizer to contain all the controls
  wxStaticBoxSizer* VSizer = new wxStaticBoxSizer(wxVERTICAL, this, "IIR Design Algorithms");

  wxBoxSizer* HSizer1 = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizer2PP = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizer2P = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizer2 = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizer3 = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizer4P = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizer4 = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizer5P = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizer5 = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizer6P = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer* HSizer6 = new wxBoxSizer(wxHORIZONTAL);

  VSizer->Add(HSizer1, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add(HSizer2PP, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add(HSizer2P, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add(HSizer2, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add(HSizer3, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add(HSizer4P, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add(HSizer4, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add(HSizer5P, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add(HSizer5, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add(HSizer6P, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add(HSizer6, 0, wxALIGN_CENTER | wxALL, 2);

  // A combo box for chosing the filter type
  HSizer1->Add(new wxStaticText(this, -1, "Filter type : "), 0, wxALIGN_TOP | wxALL, 0);
  static const wxString FilterChoices[] = {"Notch Filter"};
  FilterTypeComboBox = new wxComboBox(this, IIRALGS_FILTER_TYPE_CHANGED, wxEmptyString, wxDefaultPosition, wxDefaultSize, WXSIZEOF(FilterChoices),
                                      FilterChoices, wxCB_READONLY);
  FilterTypeComboBox->SetSelection(0);    // Set the default selection
  HSizer1->Add(FilterTypeComboBox, 0, wxALIGN_LEFT | wxALL, 0);

  // Text control for notch filter centre frequency
  HSizer2P->Add(new wxStaticText(this, -1, "Notch filter parameters"), 0, wxALIGN_TOP | wxALL, 0);
  HSizer2->Add(new wxStaticText(this, -1, "Centre Frequency   : "), 0, wxALIGN_TOP | wxALL, 0);
  CentreFrequencyLine = new wxTextCtrl(this, -1, "0.0");
  HSizer2->Add(CentreFrequencyLine);

  // Text control for notch filter pole magnitude
  HSizer3->Add(new wxStaticText(this, -1, "Pole Magnitude :        "), 0, wxALIGN_TOP | wxALL, 0);
  PoleMagnitudeLine = new wxTextCtrl(this, -1, "0.0");
  HSizer3->Add(PoleMagnitudeLine);

  // This spin box requests the IIR filter order
  HSizer4->Add(new wxStaticText(this, -1, "Filter order : "), 0, wxALIGN_TOP | wxALL, 0);
  FilterOrderSpinBox =
      new wxSpinCtrl(this, IIRBLT_FILTER_ORDER_SPINCTRL, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 2, MAX_BLT_ORDER);
  FilterOrderSpinBox->SetValue(2);    // Set default value
  HSizer4->Add(FilterOrderSpinBox);

  // Insert a CheckBox for the gain scaling
  HSizer5->Add(new wxStaticText(this, -1, "0 dB Pass-band scaling :  "), 0, wxALIGN_TOP | wxALL, 0);
  ScaleGainCheckBox = new wxCheckBox(this, -1, _T(" &Enable"));
  ScaleGainCheckBox->SetValue(TRUE);
#if wxUSE_TOOLTIPS
  ScaleGainCheckBox->SetToolTip(_T("Click here to scale the pass-band gain to 0 dB"));
#endif    // wxUSE_TOOLTIPS
  HSizer5->Add(ScaleGainCheckBox);

  // This button calculates the filter and displays the graph
  wxButton* CalculateButton = new wxButton(this, IIRALGS_GET_FILTER_SPEC, "Calculate");
  HSizer6->Add(CalculateButton, 0, wxALIGN_CENTER | wxALL, 0);
  CalculateButton->SetDefault();
  //  SetDefaultItem (CalculateButton);

  FilterTypeComboBox->SetFocus();    // give the FilterTypeComboBox the focus at the beginning

  // Default is Hilbert transformer
  //  RaisedCosineAlphaLine->Enable (FALSE);
  //  RaisedCosineSymRateLine->Enable (FALSE);
  //  GaussianRadioButton1->Enable (FALSE);
  //  GaussianRadioButton2->Enable (FALSE);
  //  GaussianAlphaLine->Enable (FALSE);

  SetAutoLayout(TRUE);
  SetSizer(VSizer);

  VSizer->SetSizeHints(this);
  VSizer->Fit(this);

  // Debug
  //  NumCoeffsSpinControl->SetValue (61);      // Set default value
}

/*
  Name : IIRAlgsFilterDesignWindow::OnFilterTypeComboBoxChanged
  Description : Enable / disable appropriate filter entry points
  Notes :
*/

void IIRAlgsFilterDesignWindow::OnFilterTypeComboBoxChanged(wxCommandEvent& WXUNUSED(event))

{
  SUF_Log("IIRAlgsFilterDesignWindow::OnFilterTypeComboBoxChanged\n");

  switch (FilterTypeComboBox->GetSelection()) {
  case IIR_NOTCH_FILTER:
    //    RaisedCosineAlphaLine->Enable (FALSE);
    //    RaisedCosineSymRateLine->Enable (FALSE);
    //    GaussianRadioButton1->Enable (FALSE);
    //    GaussianRadioButton2->Enable (FALSE);
    //    GaussianAlphaLine->Enable (FALSE);
    break;
  }
}

/*
  Name : IIRAlgsFilterDesignWindow::GetFilterSpec
  Description : Get the filter spec. from the user entries
  Notes :
*/

void IIRAlgsFilterDesignWindow::OnGetFilterSpec(wxCommandEvent& WXUNUSED(event))

{
  SUF_Log("IIRAlgsFilterDesignWindow::OnGetFilterSpec\n");

  char* stopstring;
  SLArrayIndex_t i;
  SLComplexRect_s TmpRectangular;

  FilterType = (SLArrayIndex_t)FilterTypeComboBox->GetSelection();    // Get the filter type

  if (FilterType == IIR_NOTCH_FILTER) {
    CentreFrequency = strtod(CentreFrequencyLine->GetValue(), &stopstring);
    PoleMagnitude = strtod(PoleMagnitudeLine->GetValue(), &stopstring);

    if (CentreFrequency < SIGLIB_ZERO) {
      wxMessageBox("The filter centre frequency must be >= 0 Hz", "Input error", wxOK | wxICON_EXCLAMATION, NULL);
      return;
    }
    if (CentreFrequency > (SampleRate / SIGLIB_TWO)) {
      wxMessageBox("The filter centre frequency must be less than the Nyquist frequency", "Input error", wxOK | wxICON_EXCLAMATION, NULL);
      return;
    }
    if (PoleMagnitude < SIGLIB_ZERO) {
      wxMessageBox("The magnitude of the pole must be greater than 0.0", "Input error", wxOK | wxICON_EXCLAMATION, NULL);
      return;
    }
    if (PoleMagnitude > SIGLIB_ONE) {
      wxMessageBox("The magnitude of the pole must be less than 1.0", "Input error", wxOK | wxICON_EXCLAMATION, NULL);
      return;
    }
  }

  FilterOrder = (SLArrayIndex_t)FilterOrderSpinBox->GetValue();    // Filter order
  if (FilterOrder == 1)                                            // Ensure even
  {
    FilterOrder = 2;
    FilterOrderSpinBox->SetValue(FilterOrder);
  }

  if (FilterOrder & 0x1)    // Ensure even
  {
    FilterOrder += 1;
    FilterOrderSpinBox->SetValue(FilterOrder);
  }

  //  if (FilterOrder > MAX_BLT_ORDER)            // Ensure in range
  if (FilterOrder > 16)    // Ensure in range
  {
    //    FilterOrder = MAX_BLT_ORDER;
    FilterOrder = 16;
    FilterOrderSpinBox->SetValue(FilterOrder);
  }

  NumberOfBiquads = FilterOrder >> 1;

  //  Debug
  //  char String[20];
  //  sprintf (String, "Filter order = %d", FilterOrder);
  //  wxMessageBox (String, "OnGetFilterSpec", wxOK | wxICON_EXCLAMATION, NULL);
  //  sprintf (String, "CentreFrequency = %1.20lf", CentreFrequency);
  //  sprintf (String, "PoleMagnitude = %1.20lf", PoleMagnitude);
  //  wxMessageBox (String, "OnGetFilterSpec", wxOK | wxICON_EXCLAMATION, NULL);
  //  if (FilterType == IIR_NOTCH_FILTER)
  //    wxMessageBox ("Notch filter", "OnGetFilterSpec", wxOK |
  //    wxICON_EXCLAMATION, NULL);

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
  if (FilterType == IIR_NOTCH_FILTER)    // Calculate coefficients
  {
    fprintf(pFile, "IIR-Notch Filter\n\n");
  }

  fprintf(pFile, "[Sample rate]\n");
  fprintf(pFile, "%1.20lf\n\n", SampleRate);

  fprintf(pFile, "[Filter specification]\n");
  if (FilterType == IIR_NOTCH_FILTER) {
    fprintf(pFile, "IIR_NOTCH_FILTER");
    fprintf(pFile, "[Sample Rate]\n");
    fprintf(pFile, "%lf\n\n", SampleRate);
    fprintf(pFile, "[Centre frequency]\n");
    fprintf(pFile, "%lf\n\n", CentreFrequency);
    fprintf(pFile, "[Pole magnitude]\n");
    fprintf(pFile, "%lf\n\n", PoleMagnitude);
  }

  fprintf(pFile, "[FIlter order]\n");
  fprintf(pFile, "%d\n\n", FilterOrder);

  (void)fclose(pFile); /* Close output file */

  if (FilterType == IIR_NOTCH_FILTER)    // Calculate coefficients
  {
    SIF_IirNotchFilter(pIIRCoeffs, CentreFrequency / SampleRate, FilterOrder);
  }

#if DEBUG_LOG_FILE
  SUF_Debugfprintf("OnFilterSpecChanged : IIR Biquad calculated\n");
  SUF_Debugfprintf("CentreFrequency = %lf\n", CentreFrequency);
#endif

  if (ScaleGainCheckBox->IsChecked())    // Scale the gain if requested
  {
    if (CentreFrequency <= (SampleRate / SIGLIB_FOUR)) /* If centre freq of notch is < half Nyquist
                                                          point then scale at Nyquist point */
    {
      SDA_IirModifyFilterGain(pIIRCoeffs, pIIRCoeffs, SIGLIB_HALF, SIGLIB_ONE, NumberOfBiquads);
    } else {
      SDA_IirModifyFilterGain(pIIRCoeffs, pIIRCoeffs, SIGLIB_ZERO, SIGLIB_ONE, NumberOfBiquads);
    }
  }

#if DEBUG_LOG_FILE
  SUF_Debugfprintf("OnFilterSpecChanged : Gain normalized\n");
#endif

  strcpy(FileNameString, ProjectNameString);
  strcat(FileNameString, ".dfc");

  if ((pFile = fopen(FileNameString, "w")) == NULL) /* Open o/p file */
  {
    wxMessageBox("Could not open file to write filter coefficients", "GetFilterSpec", wxOK | wxICON_EXCLAMATION, NULL);
    return;
  }

  fprintf(pFile, DFPLUS_COPYRIGHT_NOTICE);
  fprintf(pFile, "File : %s\n\n", FileNameString);
  if (FilterType == IIR_NOTCH_FILTER) {
    fprintf(pFile, "IIR notch filter coefficients\n\n");
  }
  fprintf(pFile, "Coefficient order for each biquad :\n\tb[0], b[1], "
                 "b[2],\n\ta[1], a[2].\n\n");

  // Write floating point coefficients to file
  fprintf(pFile, "\t/* Floating point coefficients */\n");
  fprintf(pFile, "const SLData_t CoefficientArray [] =\n{\n");

  /* Write IIR coefficients to file */
  if (global_NegateIIRAlphaSign == 0)    // Don't NegateIIRAlphaSign for biquads
  {
    for (i = 0; i < NumberOfBiquads; i++) {
      fprintf(pFile, "\t%1.20le, %1.20le, %1.20le,\n\t%1.20le, %1.20le", pIIRCoeffs[(5 * i) + 0], pIIRCoeffs[(5 * i) + 1], pIIRCoeffs[(5 * i) + 2],
              pIIRCoeffs[(5 * i) + 3], pIIRCoeffs[(5 * i) + 4]);

      if (i < ((short)NumberOfBiquads - 1))
        fprintf(pFile, ",\n\n");
    }
  } else    // NegateIIRAlphaSign for biquads
  {
    for (i = 0; i < NumberOfBiquads; i++) {
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

  if (global_NegateIIRAlphaSign == 0)    // Don't NegateIIRAlphaSign for biquads
  {
    for (i = 0; i < NumberOfBiquads; i++) {
      fprintf(pFile, "\t0x%llx, 0x%llx, 0x%llx, 0x%llx, 0x%llx", FloatToQFormatInteger(pIIRCoeffs[(5 * i) + 0], global_mValue, global_nValue),
              FloatToQFormatInteger(pIIRCoeffs[(5 * i) + 1], global_mValue, global_nValue),
              FloatToQFormatInteger(pIIRCoeffs[(5 * i) + 2], global_mValue, global_nValue),
              FloatToQFormatInteger(pIIRCoeffs[(5 * i) + 3], global_mValue, global_nValue),
              FloatToQFormatInteger(pIIRCoeffs[(5 * i) + 4], global_mValue, global_nValue));

      if (i < ((short)NumberOfBiquads - 1))
        fprintf(pFile, ",\n");
    }
  } else    // NegateIIRAlphaSign for biquads
  {
    for (i = 0; i < NumberOfBiquads; i++) {
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

  if (global_NegateIIRAlphaSign == 0)    // Don't NegateIIRAlphaSign for biquads
  {
    for (i = 0; i < NumberOfBiquads; i++) {
      fprintf(pFile, "\t%x, %x, %x, %x, %x", 0xffff & ((short)(pIIRCoeffs[(5 * i) + 0] * 32767.0)),
              0xffff & ((short)(pIIRCoeffs[(5 * i) + 1] * 32767.0)), 0xffff & ((short)(pIIRCoeffs[(5 * i) + 2] * 32767.0)),
              0xffff & ((short)(pIIRCoeffs[(5 * i) + 3] * 32767.0)), 0xffff & ((short)(pIIRCoeffs[(5 * i) + 4] * 32767.0)));

      if (i < ((short)NumberOfBiquads - 1))
        fprintf(pFile, ",\n");
    }
  } else    // NegateIIRAlphaSign for biquads
  {
    for (i = 0; i < NumberOfBiquads; i++) {
      fprintf(pFile, "\t%x, %x, %x, %x, %x", 0xffff & ((short)(pIIRCoeffs[(5 * i) + 0] * 32767.0)),
              0xffff & ((short)(pIIRCoeffs[(5 * i) + 1] * 32767.0)), 0xffff & ((short)(pIIRCoeffs[(5 * i) + 2] * 32767.0)),
              0xffff & ((short)(pIIRCoeffs[(5 * i) + 3] * -32767.0)), 0xffff & ((short)(pIIRCoeffs[(5 * i) + 4] * -32767.0)));

      if (i < ((short)NumberOfBiquads - 1))
        fprintf(pFile, ",\n");
    }
  }

  fprintf(pFile, "\n};\n\n");

  (void)fclose(pFile);

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

  } /* End of calculate filter performance */

  for (i = 0; i < NumberOfBiquads; i++) {    // Generate array for pole zero plot
    pPolesAndZeros[(4 * i) + 0] = SCV_PolarToRectangular(SCV_Polar(PoleMagnitude, SIGLIB_TWO_PI * (CentreFrequency / SampleRate)));
    pPolesAndZeros[(4 * i) + 1] = SCV_PolarToRectangular(SCV_Polar(SIGLIB_ONE, SIGLIB_TWO_PI * (CentreFrequency / SampleRate)));
    pPolesAndZeros[(4 * i) + 2] = SCV_Conjugate(SCV_PolarToRectangular(SCV_Polar(PoleMagnitude, SIGLIB_TWO_PI * (CentreFrequency / SampleRate))));
    pPolesAndZeros[(4 * i) + 3] = SCV_Conjugate(SCV_PolarToRectangular(SCV_Polar(SIGLIB_ONE, SIGLIB_TWO_PI * (CentreFrequency / SampleRate))));
  }

  strcpy(FileNameString, ProjectNameString);
  strcat(FileNameString, ".dfz");

  if ((pFile = fopen(FileNameString, "w")) == NULL) { /* Open o/p file */
    wxMessageBox("Could not open file to write filter coefficients", "OnGetFilterSpec", wxOK | wxICON_EXCLAMATION, NULL);
    return;
  }

  fprintf(pFile, DFPLUS_COPYRIGHT_NOTICE);
  fprintf(pFile, "File : %s\n\n", FileNameString);
  fprintf(pFile, "IIR filter z-plane coefficients designed with the pole-zero "
                 "placement Method\n\n");

  fprintf(pFile, "Polar coordinates of Poles and Zeros for IIR filter with %d biquads\n\n", (short)NumberOfBiquads);
  fprintf(pFile, "Coefficient order for each pole / zero :\n\tPole Magnitude, Pole "
                 "Angle (degrees)\n\tZero Magnitude, Zero Angle (degrees).\n\n");

  fprintf(pFile, "const SLData_t zDomainCoefficientArray [] =\n{\n");

  /* Write IIR poles and zeros to file */
  for (i = 0; i < (short)NumberOfBiquads; i++) {
    fprintf(pFile, "\t%1.20le, %1.20le,\n", PoleMagnitude, 360.0 * (CentreFrequency / SampleRate));
    fprintf(pFile, "\t%1.20le, %1.20le,\n\n", SIGLIB_ONE, 360.0 * (CentreFrequency / SampleRate));
    fprintf(pFile, "\t%1.20le, %1.20le,\n", PoleMagnitude, (360.0 - (360.0 * (CentreFrequency / SampleRate))));
    fprintf(pFile, "\t%1.20le, %1.20le", SIGLIB_ONE, (360.0 - (360.0 * (CentreFrequency / SampleRate))));
    if (i < ((short)NumberOfBiquads - 1))
      fprintf(pFile, ",\n\n");
  }
  fprintf(pFile, "\n};\n\n");

  fprintf(pFile,
          "Rectangular coordinates of Poles and Zeros for IIR filter with %d "
          "biquads\n\n",
          (short)NumberOfBiquads);
  fprintf(pFile, "Coefficient order for each pole / zero :\n\tPole Real, Pole "
                 "Imaginary\n\tZero Real, Zero Imaginary.\n\n");

  fprintf(pFile, "const SLData_t zDomainRectangularCoefficientArray [] =\n{\n");

  /* Write IIR poles and zeros to file */
  for (i = 0; i < (short)NumberOfBiquads; i++) {
    TmpRectangular = SCV_PolarToRectangular(SCV_Polar(PoleMagnitude, SIGLIB_TWO_PI * (CentreFrequency / SampleRate)));
    fprintf(pFile, "\t%1.20le, %1.20le,\n", TmpRectangular.real, TmpRectangular.imag);
    TmpRectangular = SCV_PolarToRectangular(SCV_Polar(SIGLIB_ONE, SIGLIB_TWO_PI * (CentreFrequency / SampleRate)));
    fprintf(pFile, "\t%1.20le, %1.20le,\n\n", TmpRectangular.real, TmpRectangular.imag);

    TmpRectangular = SCV_Conjugate(SCV_PolarToRectangular(SCV_Polar(PoleMagnitude, SIGLIB_TWO_PI * (CentreFrequency / SampleRate))));
    fprintf(pFile, "\t%1.20le, %1.20le,\n", TmpRectangular.real, TmpRectangular.imag);
    TmpRectangular = SCV_Conjugate(SCV_PolarToRectangular(SCV_Polar(SIGLIB_ONE, SIGLIB_TWO_PI * (CentreFrequency / SampleRate))));
    fprintf(pFile, "\t%1.20le, %1.20le", TmpRectangular.real, TmpRectangular.imag);

    if (i < ((short)NumberOfBiquads - 1))
      fprintf(pFile, ",\n\n");
  }
  fprintf(pFile, "\n};\n\n");

  (void)fclose(pFile);

  pFilterInfo->IIRFilterOrder = (2 * NumberOfBiquads);

  SLData_t IntegralSum = SIGLIB_ZERO;
  SDA_Integrate(pImpulseResponse, pStepResponse, SIGLIB_MAX, SIGLIB_ONE, &IntegralSum, GRAPH_WIDTH);    // Calculate step response

  wxCommandEvent* DummyCommand = new wxCommandEvent();    // Post event to say that coeffs have been updated
  GlobalGraphDisplayWindow->OnGraphTypeChanged(*DummyCommand);
  delete (DummyCommand);

} /* End of IIRAlgsFilterDesignWindow::OnGetFilterSpec () */

void IIRAlgsFilterDesignWindow::SetProjectNameString(wxString String)
{
  ProjectNameString = String;
}

void IIRAlgsFilterDesignWindow::SetSampleRate(double sampleRate)
{
  SampleRate = sampleRate;
}

void IIRAlgsFilterDesignWindow::SetGainArray(SLData_t* gain)
{
  pGain = gain;
}

void IIRAlgsFilterDesignWindow::SetPhaseArray(SLData_t* phase)
{
  pPhase = phase;
}

void IIRAlgsFilterDesignWindow::SetGroupDelayArray(SLData_t* groupDelay)
{
  pGroupDelay = groupDelay;
}

void IIRAlgsFilterDesignWindow::SetImpulseResponseArray(SLData_t* impulseResponse)
{
  pImpulseResponse = impulseResponse;
}

void IIRAlgsFilterDesignWindow::SetStepResponseArray(SLData_t* stepResponse)
{
  pStepResponse = stepResponse;
}

void IIRAlgsFilterDesignWindow::SetPoleZeroPlotArray(SLComplexRect_s* PoleZeroArray)
{
  pPolesAndZeros = PoleZeroArray;
}

void IIRAlgsFilterDesignWindow::SetFilterInfoStruct(filter_info_t* filterInfo)
{
  pFilterInfo = filterInfo;
}

BEGIN_EVENT_TABLE(IIRAlgsFilterDesignWindow, wxPanel)
EVT_COMBOBOX(IIRALGS_FILTER_TYPE_CHANGED, IIRAlgsFilterDesignWindow::OnFilterTypeComboBoxChanged)
EVT_BUTTON(IIRALGS_GET_FILTER_SPEC, IIRAlgsFilterDesignWindow::OnGetFilterSpec)
END_EVENT_TABLE()
