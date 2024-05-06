// IIR pole zero placement digital filter functions
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

#if __APPLE__
#  include <malloc/malloc.h>
#else
#  include <malloc.h>
#endif
#include "dfplus.h"
#include "ngl.h"
#include <math.h>
#include <siglib.h>
#include <stdlib.h>

extern GraphDisplay* GlobalGraphDisplayWindow;

extern int global_mValue;    // Global m and n values for Q number format
extern int global_nValue;
extern int global_NegateIIRAlphaSign;    // Global NegateIIRAlphaSign for biquads

/*
  Name : IIRPZPFilterDesignWindow::IIRPZPFilterDesignWindow
  Description : Child widget that is used to get the IIR PZP filter spec.
  Notes :
*/

IIRPZPFilterDesignWindow::IIRPZPFilterDesignWindow(wxWindow* parent, wxWindowID id)
    : wxPanel(parent, id)

{
  // Top-level layout - a vertical box sizer to contain all the controls
  wxStaticBoxSizer* VSizer = new wxStaticBoxSizer(wxVERTICAL, this, "IIR Pole-zero Placement Design");

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
  VSizer->Add(HSizerP6, 0, wxALIGN_CENTER | wxALL, 2);
  VSizer->Add(HSizer6, 0, wxALIGN_CENTER | wxALL, 2);
  VSizer->Add(0, 0, 1);    // Add a spacer to get the alignment right

  // This should be the filter specification section
  HSizer11->Add(new wxStaticText(this, -1, "Pole 1 magn. : "), 0, wxALIGN_TOP | wxALL, 0);
  Pole1Magn = new wxTextCtrl(this, -1, "0.0");
  HSizer11->Add(Pole1Magn);

  HSizer12->Add(new wxStaticText(this, -1, "Pole 1 angle : "), 0, wxALIGN_TOP | wxALL, 0);
  Pole1Angle = new wxTextCtrl(this, -1, "0.0");
  HSizer12->Add(Pole1Angle);

  HSizer13->Add(new wxStaticText(this, -1, "Zero 1 magn. : "), 0, wxALIGN_TOP | wxALL, 0);
  Zero1Magn = new wxTextCtrl(this, -1, "0.0");
  HSizer13->Add(Zero1Magn);

  HSizer14->Add(new wxStaticText(this, -1, "Zero 1 angle : "), 0, wxALIGN_TOP | wxALL, 0);
  Zero1Angle = new wxTextCtrl(this, -1, "0.0");
  HSizer14->Add(Zero1Angle);

  // Insert the LineEdit entry box Pole / zero 2
  HSizer21->Add(new wxStaticText(this, -1, "Pole 2 magn. : "), 0, wxALIGN_TOP | wxALL, 0);
  Pole2Magn = new wxTextCtrl(this, -1, "0.0");
  HSizer21->Add(Pole2Magn);

  HSizer22->Add(new wxStaticText(this, -1, "Pole 2 angle : "), 0, wxALIGN_TOP | wxALL, 0);
  Pole2Angle = new wxTextCtrl(this, -1, "0.0");
  HSizer22->Add(Pole2Angle);

  HSizer23->Add(new wxStaticText(this, -1, "Zero 2 magn. : "), 0, wxALIGN_TOP | wxALL, 0);
  Zero2Magn = new wxTextCtrl(this, -1, "0.0");
  HSizer23->Add(Zero2Magn);

  HSizer24->Add(new wxStaticText(this, -1, "Zero 2 angle : "), 0, wxALIGN_TOP | wxALL, 0);
  Zero2Angle = new wxTextCtrl(this, -1, "0.0");
  HSizer24->Add(Zero2Angle);

  // Insert the LineEdit entry box Pole / zero 3
  HSizer31->Add(new wxStaticText(this, -1, "Pole 3 magn. : "), 0, wxALIGN_TOP | wxALL, 0);
  Pole3Magn = new wxTextCtrl(this, -1, "0.0");
  HSizer31->Add(Pole3Magn);

  HSizer32->Add(new wxStaticText(this, -1, "Pole 3 angle : "), 0, wxALIGN_TOP | wxALL, 0);
  Pole3Angle = new wxTextCtrl(this, -1, "0.0");
  HSizer32->Add(Pole3Angle);

  HSizer33->Add(new wxStaticText(this, -1, "Zero 3 magn. : "), 0, wxALIGN_TOP | wxALL, 0);
  Zero3Magn = new wxTextCtrl(this, -1, "0.0");
  HSizer33->Add(Zero3Magn);

  HSizer34->Add(new wxStaticText(this, -1, "Zero 3 angle : "), 0, wxALIGN_TOP | wxALL, 0);
  Zero3Angle = new wxTextCtrl(this, -1, "0.0");
  HSizer34->Add(Zero3Angle);

  // Insert the LineEdit entry box Pole / zero 4
  HSizer41->Add(new wxStaticText(this, -1, "Pole 4 magn. : "), 0, wxALIGN_TOP | wxALL, 0);
  Pole4Magn = new wxTextCtrl(this, -1, "0.0");
  HSizer41->Add(Pole4Magn);

  HSizer42->Add(new wxStaticText(this, -1, "Pole 4 angle : "), 0, wxALIGN_TOP | wxALL, 0);
  Pole4Angle = new wxTextCtrl(this, -1, "0.0");
  HSizer42->Add(Pole4Angle);

  HSizer43->Add(new wxStaticText(this, -1, "Zero 4 magn. : "), 0, wxALIGN_TOP | wxALL, 0);
  Zero4Magn = new wxTextCtrl(this, -1, "0.0");
  HSizer43->Add(Zero4Magn);

  HSizer44->Add(new wxStaticText(this, -1, "Zero 4 angle : "), 0, wxALIGN_TOP | wxALL, 0);
  Zero4Angle = new wxTextCtrl(this, -1, "0.0");
  HSizer44->Add(Zero4Angle);

  // Insert the LineEdit entry box Pole / zero 5
  HSizer51->Add(new wxStaticText(this, -1, "Pole 5 magn. : "), 0, wxALIGN_TOP | wxALL, 0);
  Pole5Magn = new wxTextCtrl(this, -1, "0.0");
  HSizer51->Add(Pole5Magn);

  HSizer52->Add(new wxStaticText(this, -1, "Pole 5 angle : "), 0, wxALIGN_TOP | wxALL, 0);
  Pole5Angle = new wxTextCtrl(this, -1, "0.0");
  HSizer52->Add(Pole5Angle);

  HSizer53->Add(new wxStaticText(this, -1, "Zero 5 magn. : "), 0, wxALIGN_TOP | wxALL, 0);
  Zero5Magn = new wxTextCtrl(this, -1, "0.0");
  HSizer53->Add(Zero5Magn);

  HSizer54->Add(new wxStaticText(this, -1, "Zero 5 angle : "), 0, wxALIGN_TOP | wxALL, 0);
  Zero5Angle = new wxTextCtrl(this, -1, "0.0");
  HSizer54->Add(Zero5Angle);

  // This button calculates the filter and displays the graph
  wxButton* CalculateButton = new wxButton(this, IIRPZP_GET_FILTER_SPEC, "Calculate");
  HSizer6->Add(CalculateButton, 0, wxALIGN_CENTER | wxALL, 0);
  CalculateButton->SetDefault();
  //  SetDefaultItem (CalculateButton);

  Pole1Magn->SetFocus();              // give the Pole1Magn the focus at the beginning
  Pole1Magn->SetSelection(-1, -1);    // Select entire string

  // Debug
  //  Pole1Magn->SetValue ("0.9"); Pole1Angle->SetValue ("10.0");
  //  Zero1Magn->SetValue ("1.0"); Zero1Angle->SetValue ("20.0");
  //  Pole2Magn->SetValue ("0.9"); Pole2Angle->SetValue ("30.0");
  //  Zero2Magn->SetValue ("1.0"); Zero2Angle->SetValue ("40.0");
  //  Pole3Magn->SetValue ("0.9"); Pole3Angle->SetValue ("50.0");
  //  Zero3Magn->SetValue ("1.0"); Zero3Angle->SetValue ("60.0");
  //  Pole4Magn->SetValue ("0.9"); Pole4Angle->SetValue ("70.0");
  //  Zero4Magn->SetValue ("1.0"); Zero4Angle->SetValue ("80.0");
  //  Pole5Magn->SetValue ("0.9"); Pole5Angle->SetValue ("90.0");
  //  Zero5Magn->SetValue ("1.0"); Zero5Angle->SetValue ("100.0");

  SetAutoLayout(TRUE);
  SetSizer(VSizer);

  VSizer->SetSizeHints(this);
  VSizer->Fit(this);
}

/*
  Name : IIRPZPFilterDesignWindow::GetFilterSpec
  Description : Get the filter spec. from the user entries
  Notes :
*/

void IIRPZPFilterDesignWindow::OnGetFilterSpec(wxCommandEvent& WXUNUSED(event))

{
  SUF_Log("IIRPZPFilterDesignWindow::OnGetFilterSpec\n");

  char* stopstring;
  short i;
  SLComplexRect_s TmpRectangular;

  NumberOfBiquads = 0;

  if ((strtod(Pole1Magn->GetValue(), &stopstring) != 0.0) || (strtod(Pole1Angle->GetValue(), &stopstring) != 0.0) ||
      (strtod(Zero1Magn->GetValue(), &stopstring) != 0.0) || (strtod(Zero1Angle->GetValue(), &stopstring) != 0.0)) {
    pDP[NumberOfBiquads] = strtod(Pole1Magn->GetValue(), &stopstring);
    pAP[NumberOfBiquads] = strtod(Pole1Angle->GetValue(), &stopstring);
    pDZ[NumberOfBiquads] = strtod(Zero1Magn->GetValue(), &stopstring);
    pAZ[NumberOfBiquads] = strtod(Zero1Angle->GetValue(), &stopstring);

    if (pDP[NumberOfBiquads] >= SIGLIB_ONE) {
      wxMessageBox("The magnitude of pole number one must be less than 1.0", "Input error", wxOK | wxICON_EXCLAMATION, NULL);
      return;
    }

    NumberOfBiquads++;
  }

  if ((strtod(Pole2Magn->GetValue(), &stopstring) != 0.0) || (strtod(Pole2Angle->GetValue(), &stopstring) != 0.0) ||
      (strtod(Zero2Magn->GetValue(), &stopstring) != 0.0) || (strtod(Zero2Angle->GetValue(), &stopstring) != 0.0)) {
    pDP[NumberOfBiquads] = strtod(Pole2Magn->GetValue(), &stopstring);
    pAP[NumberOfBiquads] = strtod(Pole2Angle->GetValue(), &stopstring);
    pDZ[NumberOfBiquads] = strtod(Zero2Magn->GetValue(), &stopstring);
    pAZ[NumberOfBiquads] = strtod(Zero2Angle->GetValue(), &stopstring);

    if (pDP[NumberOfBiquads] >= SIGLIB_ONE) {
      wxMessageBox("The magnitude of pole number two must be less than 1.0", "Input error", wxOK | wxICON_EXCLAMATION, NULL);
      return;
    }

    NumberOfBiquads++;
  }

  if ((strtod(Pole3Magn->GetValue(), &stopstring) != 0.0) || (strtod(Pole3Angle->GetValue(), &stopstring) != 0.0) ||
      (strtod(Zero3Magn->GetValue(), &stopstring) != 0.0) || (strtod(Zero3Angle->GetValue(), &stopstring) != 0.0)) {
    pDP[NumberOfBiquads] = strtod(Pole3Magn->GetValue(), &stopstring);
    pAP[NumberOfBiquads] = strtod(Pole3Angle->GetValue(), &stopstring);
    pDZ[NumberOfBiquads] = strtod(Zero3Magn->GetValue(), &stopstring);
    pAZ[NumberOfBiquads] = strtod(Zero3Angle->GetValue(), &stopstring);

    if (pDP[NumberOfBiquads] >= SIGLIB_ONE) {
      wxMessageBox("The magnitude of pole number three must be less than 1.0", "Input error", wxOK | wxICON_EXCLAMATION, NULL);
      return;
    }

    NumberOfBiquads++;
  }

  if ((strtod(Pole4Magn->GetValue(), &stopstring) != 0.0) || (strtod(Pole4Angle->GetValue(), &stopstring) != 0.0) ||
      (strtod(Zero4Magn->GetValue(), &stopstring) != 0.0) || (strtod(Zero4Angle->GetValue(), &stopstring) != 0.0)) {
    pDP[NumberOfBiquads] = strtod(Pole4Magn->GetValue(), &stopstring);
    pAP[NumberOfBiquads] = strtod(Pole4Angle->GetValue(), &stopstring);
    pDZ[NumberOfBiquads] = strtod(Zero4Magn->GetValue(), &stopstring);
    pAZ[NumberOfBiquads] = strtod(Zero4Angle->GetValue(), &stopstring);

    if (pDP[NumberOfBiquads] >= SIGLIB_ONE) {
      wxMessageBox("The magnitude of pole number four must be less than 1.0", "Input error", wxOK | wxICON_EXCLAMATION, NULL);
      return;
    }

    NumberOfBiquads++;
  }

  if ((strtod(Pole5Magn->GetValue(), &stopstring) != 0.0) || (strtod(Pole5Angle->GetValue(), &stopstring) != 0.0) ||
      (strtod(Zero5Magn->GetValue(), &stopstring) != 0.0) || (strtod(Zero5Angle->GetValue(), &stopstring) != 0.0)) {
    pDP[NumberOfBiquads] = strtod(Pole5Magn->GetValue(), &stopstring);
    pAP[NumberOfBiquads] = strtod(Pole5Angle->GetValue(), &stopstring);
    pDZ[NumberOfBiquads] = strtod(Zero5Magn->GetValue(), &stopstring);
    pAZ[NumberOfBiquads] = strtod(Zero5Angle->GetValue(), &stopstring);

    if (pDP[NumberOfBiquads] >= SIGLIB_ONE) {
      wxMessageBox("The magnitude of pole number five must be less than 1.0", "Input error", wxOK | wxICON_EXCLAMATION, NULL);
      return;
    }

    NumberOfBiquads++;
  }

  if (NumberOfBiquads == 0) {
    wxMessageBox("You must specify at least one set of poles and zeros", "Input error", wxOK | wxICON_EXCLAMATION, NULL);
    return;
  }

  if (NumberOfBiquads > MAX_PZP_BIQUADS) {
    wxMessageBox("The maximum number of biquads is 5", "Input error", wxOK | wxICON_EXCLAMATION, NULL);
    return;
  }

  //  Debug
  //  char String[20];
  //  sprintf (String, "# biquads = %d", NumberOfBiquads);
  //  wxMessageBox (String, "OnGetFilterSpec", wxOK | wxICON_EXCLAMATION, NULL);

  static char FileNameString[500];
  FILE* pFile;
  strcpy(FileNameString, ProjectNameString);
  strcat(FileNameString, ".dfs");

  if ((pFile = fopen(FileNameString, "w")) == NULL) /* Open o/p file */
  {
    wxMessageBox("Could not open file to write filter specification", "GetFilterSpec", wxOK | wxICON_EXCLAMATION, NULL);
    return;
  }

  fprintf(pFile, DFPLUS_COPYRIGHT_NOTICE);
  fprintf(pFile, "File : %s\n\n", FileNameString);

  fprintf(pFile, "[Design Type]\n");
  fprintf(pFile, "IIR-PoleZeroPlacement\n\n");

  fprintf(pFile, "[Sample rate]\n");
  fprintf(pFile, "%1.20lf\n\n", SampleRate);

  fprintf(pFile, "[Filter Order]\n");
  fprintf(pFile, "%d\n\n", (short)(NumberOfBiquads * 2));

  fprintf(pFile, "[Pole-Zero Specification]\n");

  for (i = 0; i < NumberOfBiquads; i++) {
    fprintf(pFile, "%1.20lf, %1.20le\n", pDP[i], pAP[i]);
    fprintf(pFile, "%1.20lf, %1.20le\n\n", pDZ[i], pAZ[i]);
  }

  fprintf(pFile, "\n\n");

  (void)fclose(pFile); /* Close output file */

  for (i = 0; i < NumberOfBiquads; i++)    // Generate coeff table
  {
    pIIRCoeffs[(5 * i) + 0] = SIGLIB_ONE;                                                                   // b0
    pIIRCoeffs[(5 * i) + 1] = -SIGLIB_TWO * pDZ[i] * SDS_Cos(pAZ[i] * SIGLIB_TWO_PI_OVER_THREE_SIXTY);      // b1
    pIIRCoeffs[(5 * i) + 2] = pDZ[i] * pDZ[i];                                                              // b2
    pIIRCoeffs[(5 * i) + 3] = -(SIGLIB_TWO * pDP[i] * SDS_Cos(pAP[i] * SIGLIB_TWO_PI_OVER_THREE_SIXTY));    // a1
    pIIRCoeffs[(5 * i) + 4] = pDP[i] * pDP[i];                                                              // a2
  }

  strcpy(FileNameString, ProjectNameString);
  strcat(FileNameString, ".dfc");

  if ((pFile = fopen(FileNameString, "w")) == NULL) /* Open o/p file */
  {
    wxMessageBox("Could not open file to write filter coefficients", "GetFilterSpec", wxOK | wxICON_EXCLAMATION, NULL);
    return;
  }

  fprintf(pFile, DFPLUS_COPYRIGHT_NOTICE);
  fprintf(pFile, "File : %s\n\n", FileNameString);
  fprintf(pFile, "IIR filter coefficients designed with the Pole Zero "
                 "Placement Method\n\n");
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

  for (i = 0; i < NumberOfBiquads; i++)    // Generate array for pole zero plot
  {
    pPolesAndZeros[(4 * i) + 0] = SCV_PolarToRectangular(SCV_Polar(pDP[i], SIGLIB_TWO_PI_OVER_THREE_SIXTY * pAP[i]));
    pPolesAndZeros[(4 * i) + 1] = SCV_PolarToRectangular(SCV_Polar(pDZ[i], SIGLIB_TWO_PI_OVER_THREE_SIXTY * pAZ[i]));
    pPolesAndZeros[(4 * i) + 2] = SCV_Conjugate(SCV_PolarToRectangular(SCV_Polar(pDP[i], SIGLIB_TWO_PI_OVER_THREE_SIXTY * pAP[i])));
    pPolesAndZeros[(4 * i) + 3] = SCV_Conjugate(SCV_PolarToRectangular(SCV_Polar(pDZ[i], SIGLIB_TWO_PI_OVER_THREE_SIXTY * pAZ[i])));
  }

  strcpy(FileNameString, ProjectNameString);
  strcat(FileNameString, ".dfz");

  if ((pFile = fopen(FileNameString, "w")) == NULL) /* Open o/p file */
  {
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
    fprintf(pFile, "\t%1.20le, %1.20le,\n", pDP[i], pAP[i]);
    fprintf(pFile, "\t%1.20le, %1.20le,\n\n", pDZ[i], pAZ[i]);
    fprintf(pFile, "\t%1.20le, %1.20le,\n", pDP[i], (360.0 - pAP[i]));
    fprintf(pFile, "\t%1.20le, %1.20le", pDZ[i], (360.0 - pAZ[i]));
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
    TmpRectangular = SCV_PolarToRectangular(SCV_Polar(pDP[i], pAP[i] * SIGLIB_TWO_PI_OVER_THREE_SIXTY));
    fprintf(pFile, "\t%1.20le, %1.20le,\n", TmpRectangular.real, TmpRectangular.imag);
    TmpRectangular = SCV_PolarToRectangular(SCV_Polar(pDZ[i], pAZ[i] * SIGLIB_TWO_PI_OVER_THREE_SIXTY));
    fprintf(pFile, "\t%1.20le, %1.20le,\n\n", TmpRectangular.real, TmpRectangular.imag);

    TmpRectangular = SCV_PolarToRectangular(SCV_Polar(pDP[i], (360.0 - pAP[i]) * SIGLIB_TWO_PI_OVER_THREE_SIXTY));
    fprintf(pFile, "\t%1.20le, %1.20le,\n", TmpRectangular.real, TmpRectangular.imag);
    TmpRectangular = SCV_PolarToRectangular(SCV_Polar(pDZ[i], (360.0 - pAZ[i]) * SIGLIB_TWO_PI_OVER_THREE_SIXTY));
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

} /* End of IIRPZPFilterDesignWindow::OnGetFilterSpec () */

void IIRPZPFilterDesignWindow::SetProjectNameString(wxString String)
{

  ProjectNameString = String;
}

void IIRPZPFilterDesignWindow::SetSampleRate(double sampleRate)
{

  SampleRate = sampleRate;
}

void IIRPZPFilterDesignWindow::SetGainArray(SLData_t* gain)
{
  pGain = gain;
}

void IIRPZPFilterDesignWindow::SetPhaseArray(SLData_t* phase)
{

  pPhase = phase;
}

void IIRPZPFilterDesignWindow::SetGroupDelayArray(SLData_t* groupDelay)
{

  pGroupDelay = groupDelay;
}

void IIRPZPFilterDesignWindow::SetImpulseResponseArray(SLData_t* impulseResponse)
{

  pImpulseResponse = impulseResponse;
}

void IIRPZPFilterDesignWindow::SetStepResponseArray(SLData_t* stepResponse)
{
  pStepResponse = stepResponse;
}

void IIRPZPFilterDesignWindow::SetPoleZeroPlotArray(SLComplexRect_s* PoleZeroArray)
{
  pPolesAndZeros = PoleZeroArray;
}

void IIRPZPFilterDesignWindow::SetFilterInfoStruct(filter_info_t* filterInfo)
{
  pFilterInfo = filterInfo;
}

BEGIN_EVENT_TABLE(IIRPZPFilterDesignWindow, wxPanel)
EVT_BUTTON(IIRPZP_GET_FILTER_SPEC, IIRPZPFilterDesignWindow::OnGetFilterSpec)
END_EVENT_TABLE()
