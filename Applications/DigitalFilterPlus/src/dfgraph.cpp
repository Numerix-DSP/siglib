// Digital filter project top level graph functions
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
#include <math.h>
#include <stdlib.h>
#include <siglib.h>    // SigLib DSP library
#include "ngl.h"
#include "dfplus.h"

/*
  Name : GraphDisplay::GraphDisplay
  Description : Child widget that is used to display the filter graphs
  Notes :
*/

GraphDisplay::GraphDisplay(wxWindow* parent, wxWindowID id)
    : wxPanel(parent, id)

{
  //  wxMessageBox (_T("Got here"), "GraphDisplay::GraphDisplay", wxOK |
  //  wxICON_EXCLAMATION, NULL);
  SUF_Log("GraphDisplay::GraphDisplay\n");

  char DisplayString[20];

#if COMMENT
  setFrameStyle(Box | Plain);
  setLineWidth(1);
  setMidLineWidth(1);
#endif

  // Make the top-level layout; a vertical box to contain all widgets and
  // sub-layouts.
  wxBoxSizer* VSizer = new wxBoxSizer(wxVERTICAL);

  wxBoxSizer* HSizer1 = new wxBoxSizer(wxHORIZONTAL);
  VSizer->Add(HSizer1, 0, wxALIGN_CENTER | wxALL, 0);
  wxBoxSizer* HSizer2 = new wxBoxSizer(wxHORIZONTAL);
  VSizer->Add(HSizer2, 0, wxALIGN_CENTER | wxALL, 0);

#if COMMENT
  wxWindow* GraphWindow = new wxWindow(this, -1, wxDefaultPosition, wxSize(GRAPH_WIDTH, GRAPH_HEIGHT));
  HSizer1->Add(GraphWindow, 0, wxALIGN_CENTER | wxALL, 0);

  // Declare the graphs here
  // The size is the size of the graph client area, where the actual graph is
  // drawn
  MagnitudeGraph = new LineGraph(GraphWindow, -1, GraphWindow->GetSize());
#endif

  //  wxWindow * GraphWindow = new wxWindow (this, -1, wxDefaultPosition,
  //  wxDefaultSize); HSizer1->Add (GraphWindow, 0, wxALIGN_CENTER | wxALL, 0);

  //  wxSize Size = MagnitudeGraph->GetSize (); //Get the graph Size and set the
  //  window size + 20 Size.SetWidth (Size.GetWidth () + 20);
  //  GraphWindow->SetSize (Size);

  wxPanel* GraphPanel1 = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(0, GRAPH_HEIGHT + 80));
  HSizer1->Add(GraphPanel1, 0, wxALIGN_CENTER | wxALL, 0);

  // Declare the graphs here
  // The size is the size of the graph client area, where the actual graph is
  // drawn

  //  MagnitudeGraph = new LineGraph (GraphWindow, wxID_ANY, wxSize
  //  (GRAPH_WIDTH, GRAPH_HEIGHT));
  MagnitudeGraph = new LineGraph(this, -1, wxSize(GRAPH_WIDTH, GRAPH_HEIGHT));
  //  MagnitudeGraph->Show (FALSE);
  MagnitudeGraph->SetLeftLabel("0 Hz");
  sprintf(DisplayString, "%1.3lf Hz", SampleRate / SIGLIB_FOUR);
  MagnitudeGraph->SetCenterLabel(DisplayString);
  sprintf(DisplayString, "%1.3lf Hz", SampleRate / SIGLIB_TWO);
  MagnitudeGraph->SetRightLabel(DisplayString);
  MagnitudeGraph->SetStatusBarValueText("Frequency (Hz)");
  //////  MagnitudeGraph->SetStatusBarIndexMultiplier (1.);
  MagnitudeGraph->SetStatusBarIndexMultiplier((SampleRate / SIGLIB_TWO) / ((double)GRAPH_WIDTH));
  //  SUF_Debugfprintf ("SetStatusBarIndexMultiplier = %lf\n", ((SampleRate /
  //  SIGLIB_TWO) / ((double)GRAPH_WIDTH)));

  LogMagnGraphScale = 0.0;

  //  PhaseGraph = new LineGraph (GraphWindow, wxID_ANY, wxSize (GRAPH_WIDTH,
  //  GRAPH_HEIGHT));
  PhaseGraph = new LineGraph(this, wxID_ANY, wxSize(GRAPH_WIDTH, GRAPH_HEIGHT));
  //  PhaseGraph->Show (FALSE);
  PhaseGraph->SetLeftLabel("0 Hz");
  sprintf(DisplayString, "%1.3lf Hz", SampleRate / SIGLIB_FOUR);
  PhaseGraph->SetCenterLabel(DisplayString);
  sprintf(DisplayString, "%1.3lf Hz", SampleRate / SIGLIB_TWO);
  PhaseGraph->SetRightLabel(DisplayString);
  PhaseGraph->SetStatusBarValueText("Frequency (Hz)");
  PhaseGraph->SetStatusBarIndexMultiplier((SampleRate / SIGLIB_TWO) / ((double)GRAPH_WIDTH));

  //  GroupDelayGraph = new LineGraph(GraphWindow, wxID_ANY, wxSize
  //  (GRAPH_WIDTH, GRAPH_HEIGHT));
  GroupDelayGraph = new LineGraph(this, wxID_ANY, wxSize(GRAPH_WIDTH, GRAPH_HEIGHT));
  //  GroupDelayGraph->Show (FALSE);
  GroupDelayGraph->SetLeftLabel("0 Hz");
  sprintf(DisplayString, "%1.3lf Hz", SampleRate / SIGLIB_FOUR);
  GroupDelayGraph->SetCenterLabel(DisplayString);
  sprintf(DisplayString, "%1.3lf Hz", SampleRate / SIGLIB_TWO);
  GroupDelayGraph->SetRightLabel(DisplayString);
  GroupDelayGraph->SetStatusBarValueText("Frequency (Hz)");
  GroupDelayGraph->SetStatusBarIndexMultiplier((SampleRate / SIGLIB_TWO) / ((double)GRAPH_WIDTH));

  //  ImpulseResponseGraph = new LineGraph(GraphWindow, wxID_ANY, wxSize
  //  (GRAPH_WIDTH, GRAPH_HEIGHT));
  ImpulseResponseGraph = new LineGraph(this, wxID_ANY, wxSize(GRAPH_WIDTH, GRAPH_HEIGHT));
  //  ImpulseResponseGraph->Show (FALSE);
  ImpulseResponseGraph->SetLeftLabel("   0");
  ImpulseResponseGraph->SetCenterLabel("Sample\nnumber");
  sprintf(DisplayString, "     %1.3d", GRAPH_WIDTH);
  ImpulseResponseGraph->SetRightLabel(DisplayString);
  ImpulseResponseGraph->SetStatusBarValueText("Impulse sample");
  ImpulseResponseGraph->SetStatusBarIndexMultiplier(1.0);

  //  StepResponseGraph = new LineGraph(GraphWindow, wxID_ANY, wxSize
  //  (GRAPH_WIDTH, GRAPH_HEIGHT));
  StepResponseGraph = new LineGraph(this, wxID_ANY, wxSize(GRAPH_WIDTH, GRAPH_HEIGHT));
  //  StepResponseGraph->Show (FALSE);
  StepResponseGraph->SetLeftLabel("   0");
  StepResponseGraph->SetCenterLabel("Sample\nnumber");
  sprintf(DisplayString, "     %1.3d", GRAPH_WIDTH);
  StepResponseGraph->SetRightLabel(DisplayString);
  StepResponseGraph->SetStatusBarValueText("Step sample");
  StepResponseGraph->SetStatusBarIndexMultiplier(1.0);

  //  PoleZeroDiagram = new PoleZeroPlot(GraphWindow, wxID_ANY, wxSize
  //  (GRAPH_WIDTH, GRAPH_HEIGHT));
  PoleZeroDiagram = new PoleZeroPlot(this, wxID_ANY, wxSize(GRAPH_WIDTH, GRAPH_HEIGHT));
  //  PoleZeroDiagram->Show (FALSE);

  HSizer1->Add(MagnitudeGraph, 0, wxALIGN_CENTER | wxALL, 0);

  // A combo box for getting required graph type
  HSizer2->Add(new wxStaticText(this, wxID_ANY, "Graph type : "), 0, wxALIGN_CENTER | wxALL, 0);
  static const wxString GraphChoices[] = {"Magnitude (dB)", "Magnitude (Linear)", "Phase (Wrapped)", "Phase (Unwrapped)",
                                          "Group Delay",    "Impulse Response",   "Step Response",   "Pole-zero (IIR Only)"};
  GraphTypeComboBox = new wxComboBox(this, GRAPH_TYPE_CHANGED, wxEmptyString, wxDefaultPosition, wxDefaultSize, WXSIZEOF(GraphChoices),
                                     GraphChoices, wxCB_READONLY);
  GraphTypeComboBox->SetSelection(0);    // Set the default selection
  HSizer2->Add(GraphTypeComboBox, 0, wxALIGN_CENTER | wxALL, 0);

  GraphTypeComboBox->Enable(FALSE);    // Disable until graph type selected

  GraphType = LOG_MAGNITUDE_GRAPH;    // Initialise graph type

  SetAutoLayout(TRUE);
  SetSizer(VSizer);

  VSizer->SetSizeHints(this);
  VSizer->Fit(this);

  MagnitudeGraph->Show(FALSE);
  PhaseGraph->Show(FALSE);
  GroupDelayGraph->Show(FALSE);
  ImpulseResponseGraph->Show(FALSE);
  StepResponseGraph->Show(FALSE);
  PoleZeroDiagram->Show(FALSE);
}

/*
  Function Name : GraphDisplay::~GraphDisplay
  Description : GraphDisplay destructor
  Notes :
*/

GraphDisplay::~GraphDisplay() {}

/*
  Function Name : GraphDisplay::OnGraphTypeChanged
  Description : Function to process when graph type has changed
  Notes :
*/

void GraphDisplay::OnGraphTypeChanged(wxCommandEvent& WXUNUSED(event))

{
  //  wxMessageBox (_T("Got here"), "GraphDisplay::OnGraphTypeChanged", wxOK |
  //  wxICON_EXCLAMATION, NULL);
  SUF_Log("GraphDisplay::OnGraphTypeChanged\n");

  char DisplayString[40];

  GraphType = GraphTypeComboBox->GetSelection();    // Get the graph type

  if (GraphType == LOG_MAGNITUDE_GRAPH) {
    if (SampleRate >= 1e6) {
      sprintf(DisplayString, "%1.3lf MHz", SampleRate / (SIGLIB_FOUR * 1000000.0));
      MagnitudeGraph->SetCenterLabel(DisplayString);
      sprintf(DisplayString, "%1.3lf MHz", SampleRate / (SIGLIB_TWO * 1000000.0));
      MagnitudeGraph->SetRightLabel(DisplayString);

      MagnitudeGraph->SetStatusBarValueText("Frequency (MHz)");
      MagnitudeGraph->SetStatusBarIndexMultiplier(((SampleRate / SIGLIB_TWO) / 1000000.0) / ((double)GRAPH_WIDTH));
    } else if (SampleRate >= 1e3) {
      sprintf(DisplayString, "%1.3lf KHz", SampleRate / (SIGLIB_FOUR * 1000.0));
      MagnitudeGraph->SetCenterLabel(DisplayString);
      sprintf(DisplayString, "%1.3lf KHz", SampleRate / (SIGLIB_TWO * 1000.0));
      MagnitudeGraph->SetRightLabel(DisplayString);

      MagnitudeGraph->SetStatusBarValueText("Frequency (KHz)");
      MagnitudeGraph->SetStatusBarIndexMultiplier(((SampleRate / SIGLIB_TWO) / 1000.0) / ((double)GRAPH_WIDTH));
    } else {
      sprintf(DisplayString, "%1.3lf Hz", SampleRate / SIGLIB_FOUR);
      MagnitudeGraph->SetCenterLabel(DisplayString);
      sprintf(DisplayString, "%1.3lf Hz", SampleRate / SIGLIB_TWO);
      MagnitudeGraph->SetRightLabel(DisplayString);

      MagnitudeGraph->SetStatusBarValueText("Frequency (Hz)");
      MagnitudeGraph->SetStatusBarIndexMultiplier(((SampleRate / SIGLIB_TWO) / 1.0) / ((double)GRAPH_WIDTH));
    }

    //    MagnitudeGraph->AddDataSet (pGain, GRAPH_SCALE, *wxBLUE, GRAPH_WIDTH);
    MagnitudeGraph->AddDataSet(pGain, LogMagnGraphScale, *wxBLUE, GRAPH_WIDTH);

    PhaseGraph->Show(FALSE);
    GroupDelayGraph->Show(FALSE);
    ImpulseResponseGraph->Show(FALSE);
    StepResponseGraph->Show(FALSE);
    PoleZeroDiagram->Show(FALSE);
    MagnitudeGraph->Show(TRUE);
  }

  else if (GraphType == LIN_MAGNITUDE_GRAPH) {
    if (SampleRate >= 1e6) {
      sprintf(DisplayString, "%1.3lf MHz", SampleRate / (SIGLIB_FOUR * 1000000.0));
      MagnitudeGraph->SetCenterLabel(DisplayString);
      sprintf(DisplayString, "%1.3lf MHz", SampleRate / (SIGLIB_TWO * 1000000.0));
      MagnitudeGraph->SetRightLabel(DisplayString);

      MagnitudeGraph->SetStatusBarValueText("Frequency (MHz)");
      MagnitudeGraph->SetStatusBarIndexMultiplier(((SampleRate / SIGLIB_TWO) / 1000000.0) / ((double)GRAPH_WIDTH));
    } else if (SampleRate >= 1e3) {
      sprintf(DisplayString, "%1.3lf KHz", SampleRate / (SIGLIB_FOUR * 1000.0));
      MagnitudeGraph->SetCenterLabel(DisplayString);
      sprintf(DisplayString, "%1.3lf KHz", SampleRate / (SIGLIB_TWO * 1000.0));
      MagnitudeGraph->SetRightLabel(DisplayString);

      MagnitudeGraph->SetStatusBarValueText("Frequency (KHz)");
      MagnitudeGraph->SetStatusBarIndexMultiplier(((SampleRate / SIGLIB_TWO) / 1000.0) / ((double)GRAPH_WIDTH));
    } else {
      sprintf(DisplayString, "%1.3lf Hz", SampleRate / SIGLIB_FOUR);
      MagnitudeGraph->SetCenterLabel(DisplayString);
      sprintf(DisplayString, "%1.3lf Hz", SampleRate / SIGLIB_TWO);
      MagnitudeGraph->SetRightLabel(DisplayString);

      MagnitudeGraph->SetStatusBarValueText("Frequency (Hz)");
      MagnitudeGraph->SetStatusBarIndexMultiplier(((SampleRate / SIGLIB_TWO) / 1.0) / ((double)GRAPH_WIDTH));
    }

    SLData_t* pTemp;
    pTemp = (SLData_t*)malloc((size_t)(GRAPH_WIDTH * sizeof(SLData_t)));
    short i;
    for (i = 0; i < GRAPH_WIDTH; i++) {
      pTemp[i] = pow(10.0, (pGain[i] / 10.0));
    }
    MagnitudeGraph->AddDataSet(pTemp, GRAPH_SCALE, *wxBLUE, GRAPH_WIDTH);
    free(pTemp);

    PhaseGraph->Show(FALSE);
    GroupDelayGraph->Show(FALSE);
    ImpulseResponseGraph->Show(FALSE);
    StepResponseGraph->Show(FALSE);
    PoleZeroDiagram->Show(FALSE);
    MagnitudeGraph->Show(TRUE);
  }

  else if ((GraphType == PHASE_WRAPPED_GRAPH) || (GraphType == PHASE_UNWRAPPED_GRAPH)) {
    if (GraphType == PHASE_WRAPPED_GRAPH)
      SDA_PhaseWrap(pPhase, pPhase, GRAPH_WIDTH);
    else
      SDA_PhaseUnWrap(pPhase, pPhase, GRAPH_WIDTH);

    if (SampleRate >= 1e6) {
      sprintf(DisplayString, "%1.3lf MHz", SampleRate / (SIGLIB_FOUR * 1000000.0));
      PhaseGraph->SetCenterLabel(DisplayString);
      sprintf(DisplayString, "%1.3lf MHz", SampleRate / (SIGLIB_TWO * 1000000.0));
      PhaseGraph->SetRightLabel(DisplayString);

      PhaseGraph->SetStatusBarValueText("Frequency (MHz)");
      PhaseGraph->SetStatusBarIndexMultiplier(((SampleRate / SIGLIB_TWO) / 1000000.0) / ((double)GRAPH_WIDTH));
    } else if (SampleRate >= 1e3) {
      sprintf(DisplayString, "%1.3lf KHz", SampleRate / (SIGLIB_FOUR * 1000.0));
      PhaseGraph->SetCenterLabel(DisplayString);
      sprintf(DisplayString, "%1.3lf KHz", SampleRate / (SIGLIB_TWO * 1000.0));
      PhaseGraph->SetRightLabel(DisplayString);

      PhaseGraph->SetStatusBarValueText("Frequency (KHz)");
      PhaseGraph->SetStatusBarIndexMultiplier(((SampleRate / SIGLIB_TWO) / 1000.0) / ((double)GRAPH_WIDTH));
    } else {
      sprintf(DisplayString, "%1.3lf Hz", SampleRate / SIGLIB_FOUR);
      PhaseGraph->SetCenterLabel(DisplayString);
      sprintf(DisplayString, "%1.3lf Hz", SampleRate / SIGLIB_TWO);
      PhaseGraph->SetRightLabel(DisplayString);

      PhaseGraph->SetStatusBarValueText("Frequency (Hz)");
      PhaseGraph->SetStatusBarIndexMultiplier(((SampleRate / SIGLIB_TWO) / 1.0) / ((double)GRAPH_WIDTH));
    }

    PhaseGraph->AddDataSet(pPhase, GRAPH_SCALE, *wxBLUE, GRAPH_WIDTH);

    MagnitudeGraph->Show(FALSE);
    GroupDelayGraph->Show(FALSE);
    ImpulseResponseGraph->Show(FALSE);
    StepResponseGraph->Show(FALSE);
    PoleZeroDiagram->Show(FALSE);
    PhaseGraph->Show(TRUE);
  }

  else if (GraphType == GROUP_DELAY_GRAPH) {
    if (SampleRate >= 1e6) {
      sprintf(DisplayString, "%1.3lf MHz", SampleRate / (SIGLIB_FOUR * 1000000.0));
      GroupDelayGraph->SetCenterLabel(DisplayString);
      sprintf(DisplayString, "%1.3lf MHz", SampleRate / (SIGLIB_TWO * 1000000.0));
      GroupDelayGraph->SetRightLabel(DisplayString);

      GroupDelayGraph->SetStatusBarValueText("Frequency (MHz)");
      GroupDelayGraph->SetStatusBarIndexMultiplier(((SampleRate / SIGLIB_TWO) / 1000000.0) / ((double)GRAPH_WIDTH));
    } else if (SampleRate >= 1e3) {
      sprintf(DisplayString, "%1.3lf KHz", SampleRate / (SIGLIB_FOUR * 1000.0));
      GroupDelayGraph->SetCenterLabel(DisplayString);
      sprintf(DisplayString, "%1.3lf KHz", SampleRate / (SIGLIB_TWO * 1000.0));
      GroupDelayGraph->SetRightLabel(DisplayString);

      GroupDelayGraph->SetStatusBarValueText("Frequency (KHz)");
      GroupDelayGraph->SetStatusBarIndexMultiplier(((SampleRate / SIGLIB_TWO) / 1000.0) / ((double)GRAPH_WIDTH));
    } else {
      sprintf(DisplayString, "%1.3lf Hz", SampleRate / SIGLIB_FOUR);
      GroupDelayGraph->SetCenterLabel(DisplayString);
      sprintf(DisplayString, "%1.3lf Hz", SampleRate / SIGLIB_TWO);
      GroupDelayGraph->SetRightLabel(DisplayString);

      GroupDelayGraph->SetStatusBarValueText("Frequency (Hz)");
      GroupDelayGraph->SetStatusBarIndexMultiplier(((SampleRate / SIGLIB_TWO) / 1.0) / ((double)GRAPH_WIDTH));
    }

    GroupDelayGraph->AddDataSet(pGroupDelay, GRAPH_SCALE, *wxBLUE, GRAPH_WIDTH);

    MagnitudeGraph->Show(FALSE);
    PhaseGraph->Show(FALSE);
    ImpulseResponseGraph->Show(FALSE);
    StepResponseGraph->Show(FALSE);
    PoleZeroDiagram->Show(FALSE);
    GroupDelayGraph->Show(TRUE);
  }

  else if (GraphType == IMPULSE_RESPONSE_GRAPH) {
    ImpulseResponseGraph->AddDataSet(pImpulseResponse, GRAPH_SCALE, *wxBLUE, GRAPH_WIDTH);

    ImpulseResponseGraph->SetCenterLabel("Sample\nnumber");
    sprintf(DisplayString, "       %1.3d", GRAPH_WIDTH);
    ImpulseResponseGraph->SetRightLabel(DisplayString);

    MagnitudeGraph->Show(FALSE);
    PhaseGraph->Show(FALSE);
    GroupDelayGraph->Show(FALSE);
    StepResponseGraph->Show(FALSE);
    PoleZeroDiagram->Show(FALSE);
    ImpulseResponseGraph->Show(TRUE);
  }

  else if (GraphType == STEP_RESPONSE_GRAPH) {
    StepResponseGraph->AddDataSet(pStepResponse, GRAPH_SCALE, *wxBLUE, GRAPH_WIDTH);

    StepResponseGraph->SetCenterLabel("Sample\nnumber");
    sprintf(DisplayString, "       %1.3d", GRAPH_WIDTH);
    StepResponseGraph->SetRightLabel(DisplayString);

    MagnitudeGraph->Show(FALSE);
    PhaseGraph->Show(FALSE);
    GroupDelayGraph->Show(FALSE);
    ImpulseResponseGraph->Show(FALSE);
    PoleZeroDiagram->Show(FALSE);
    StepResponseGraph->Show(TRUE);
  }

  else if (GraphType == POLE_ZERO_GRAPH) {
    if (pFilterInfo->IIRFilterOrder > 0) {
      PoleZeroDiagram->AddDataSet(pPolesAndZeros, pFilterInfo->IIRFilterOrder, PZ_CONJUGATE_POLE_ZERO);
      //      PoleZeroDiagram->AddDataSet (pPolesAndZeros,
      //      pFilterInfo->IIRFilterOrder, PZ_COMPLEX_POLE_ZERO);

      MagnitudeGraph->Show(FALSE);
      PhaseGraph->Show(FALSE);
      GroupDelayGraph->Show(FALSE);
      ImpulseResponseGraph->Show(FALSE);
      StepResponseGraph->Show(FALSE);
      PoleZeroDiagram->Show(TRUE);
    }
  }

  //  Refresh (TRUE, NULL);     // Redraw the graph

  Show(FALSE);    // Required for Linux
  Show(TRUE);

  //  EnableGraphTypeComboBox (TRUE);       // Enable graph type selected
  GraphTypeComboBox->Enable(TRUE);    // Enable graph type selected

  //  wxMessageBox (_T("Got here"), "GraphDisplay::OnGraphTypeChanged", wxOK |
  //  wxICON_EXCLAMATION, NULL);
}

void GraphDisplay::SetLogMagnGraphScale(SLData_t gain)
{
  LogMagnGraphScale = gain;
}

void GraphDisplay::SetGainArray(SLData_t* gain)
{
  pGain = gain;
}

void GraphDisplay::SetPhaseArray(SLData_t* phase)
{
  pPhase = phase;
}

void GraphDisplay::SetGroupDelayArray(SLData_t* groupDelay)
{
  pGroupDelay = groupDelay;
}

void GraphDisplay::SetImpulseResponseArray(SLData_t* impulseResponse)
{
  pImpulseResponse = impulseResponse;
}

void GraphDisplay::SetStepResponseArray(SLData_t* stepResponse)
{
  pStepResponse = stepResponse;
}

void GraphDisplay::SetPoleZeroPlotArray(SLComplexRect_s* PoleZeroArray)
{
  pPolesAndZeros = PoleZeroArray;
}

void GraphDisplay::SetFilterInfoStruct(filter_info_t* filterInfo)
{
  pFilterInfo = filterInfo;
}

void GraphDisplay::SetSampleRate(SLData_t sampleRate)
{
  SampleRate = sampleRate;
}

// Resets graph to frequency plot

void GraphDisplay::ResetGraphType(void)
{
  GraphTypeComboBox->SetSelection(0);
}

void GraphDisplay::ShowGraph(bool show)
{
  //  wxMessageBox (_T("Got here"), "GraphDisplay::ShowGraph", wxOK |
  //  wxICON_EXCLAMATION, NULL);
  SUF_Log("GraphDisplay::ShowGraph\n");

  if (GraphType == LOG_MAGNITUDE_GRAPH) {
    MagnitudeGraph->Show(show);
  }

  else if ((GraphType == PHASE_WRAPPED_GRAPH) || (GraphType == PHASE_UNWRAPPED_GRAPH)) {
    PhaseGraph->Show(show);
  }

  else if (GraphType == GROUP_DELAY_GRAPH) {
    GroupDelayGraph->Show(show);
  }

  else if (GraphType == IMPULSE_RESPONSE_GRAPH) {
    ImpulseResponseGraph->Show(show);
  }

  else if (GraphType == STEP_RESPONSE_GRAPH) {
    StepResponseGraph->Show(show);
  }

  else if (GraphType == POLE_ZERO_GRAPH) {
    PoleZeroDiagram->Show(show);
  }
}

void GraphDisplay::EnableGraphTypeComboBox(bool enable)
{
  GraphTypeComboBox->Enable(enable);    // Enable graph type selected
}

BEGIN_EVENT_TABLE(GraphDisplay, wxPanel)
EVT_COMBOBOX(GRAPH_TYPE_CHANGED, GraphDisplay::OnGraphTypeChanged)
END_EVENT_TABLE()
