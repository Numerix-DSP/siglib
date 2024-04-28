// System analyzer project top level graph functions
// Copyright (c) 1999-2020, Sigma Numerix Ltd, All rights reserved

// Include files
#ifdef __GNUG__
#pragma implementation "dfgraph.cpp"
#pragma interface "dfgraph.cpp"
#endif  /*  */
  
// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"
  
// These pragmas prevents the compiler from precompiling the #defines below so that
// changing them doesn't "take place" later!
#ifdef __VISUALC__
#pragma hdrstop
#endif  /*  */
  
#ifdef __BORLANDC__
#pragma hdrstop
#endif  /*  */
  
// for all others, include the necessary headers
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif  /*  */
  
#include <math.h>
#include <stdlib.h>
#include "sa.h"
   

/*
    Name : GraphDisplay::GraphDisplay
    Description : Child widget that is used to display the filter graphs
    Notes :
*/ 
GraphDisplay::GraphDisplay (wxWindow * parent, wxWindowID id) :wxPanel (parent, id)  
{
  
//  wxMessageBox (_T("Got here"), "GraphDisplay::GraphDisplay", wxOK | wxICON_EXCLAMATION, NULL);
  char DisplayString[40];
   
// Make the top-level layout; a vertical box to contain all widgets and sub-layouts.
    wxBoxSizer * VSizer = new wxBoxSizer (wxVERTICAL);
   wxBoxSizer * HSizer1 = new wxBoxSizer (wxHORIZONTAL);
  wxBoxSizer * HSizer2 = new wxBoxSizer (wxHORIZONTAL);
   VSizer->Add (HSizer1, 0, wxALIGN_CENTER | wxALL, 0);
  VSizer->Add (HSizer2, 0, wxALIGN_CENTER | wxALL, 0);
   GraphPanel = new wxPanel (this, -1, wxDefaultPosition, wxDefaultSize);
   
// Declare the graphs here
// The size is the size of the graph client area, where the actual graph is drawn
    DataGraph = new LineGraph (GraphPanel, -1, wxSize (MAXIMUM_GRAPH_WIDTH, MAXIMUM_GRAPH_HEIGHT));
   DataGraph->SetLeftLabel ("0 Hz");
  sprintf (DisplayString, "%1.1lf Hz", SampleRate / 4.0);
  DataGraph->SetCenterLabel (DisplayString);
  sprintf (DisplayString, "%1.1lf Hz", SampleRate / 2.0);
  DataGraph->SetRightLabel (DisplayString);
   HSizer1->Add (GraphPanel, 0, wxALIGN_CENTER | wxALL, 0);
   SetAutoLayout (TRUE);
  SetSizer (VSizer);
   VSizer->SetSizeHints (this);
  
//  VSizer->Fit (this);
    DataGraph->Show (FALSE);
   DataSetSize = MAXIMUM_GRAPH_WIDTH;                             // Set initial graph width
}   

/*
    Function Name : GraphDisplay::~GraphDisplay
    Description : GraphDisplay destructor
    Notes :
*/ 
GraphDisplay::~GraphDisplay ()  {   
}   

/*
    Function Name : GraphDisplay::OnGraphTypeChanged
    Description : Function to process when graph type has changed
    Notes :
*/ 
  void GraphDisplay::OnGraphTypeChanged (
  wxCommandEvent & WXUNUSED (event))  { 
//  wxMessageBox (_T("Got here"), "GraphDisplay::OnGraphTypeChanged", wxOK | wxICON_EXCLAMATION, NULL);
    char DisplayString[40];
   if (GraphType == ID_TIME_DOMAIN_GRAPH)                         // Time domain graph
  {
    DataGraph->SetQuantizeXAxisFlag (true);                        // Set x-axis quantization ON
    sprintf (DisplayString, "0       ");
    DataGraph->SetLeftLabel (DisplayString);
    sprintf (DisplayString, "      %d", DataSetSize / 2);
    DataGraph->SetCenterLabel (DisplayString);
    sprintf (DisplayString, "      %d", DataSetSize);
    DataGraph->SetRightLabel (DisplayString);
    DataGraph->SetStatusBarValueText ("Sample number");
    DataGraph->SetStatusBarIndexMultiplier (1.0);
    DataGraph->SetStatusBarIndexMultiplier (((double) DataSetSize) / ((double) MAXIMUM_GRAPH_WIDTH));
  }
   
  else                                                              // Frequency domain graph
  {
    DataGraph->SetQuantizeXAxisFlag (false);                       // Set x-axis quantization OFF
    if (SampleRate >= 1e6)
       {
      sprintf (DisplayString, "0 Hz    ");
      DataGraph->SetLeftLabel (DisplayString);
      sprintf (DisplayString, "%1.1lf MHz", SampleRate / (4.0 * 1000000.0));
      DataGraph->SetCenterLabel (DisplayString);
      sprintf (DisplayString, "%1.1lf MHz", SampleRate / (2.0 * 1000000.0));
      DataGraph->SetRightLabel (DisplayString);
      DataGraph->SetStatusBarValueText ("Frequency (MHz)");
      DataGraph->SetStatusBarIndexMultiplier (((SampleRate / 2.0) / ((double) MAXIMUM_GRAPH_WIDTH)) / 1000000.0);
      }
    
    else if (SampleRate >= 1e3)
       {
      sprintf (DisplayString, "0 Hz    ");
      DataGraph->SetLeftLabel (DisplayString);
      sprintf (DisplayString, "%1.1lf KHz", SampleRate / (4.0 * 1000.0));
      DataGraph->SetCenterLabel (DisplayString);
      sprintf (DisplayString, "%1.1lf KHz", SampleRate / (2.0 * 1000.0));
      DataGraph->SetRightLabel (DisplayString);
      DataGraph->SetStatusBarValueText ("Frequency (KHz)");
      DataGraph->SetStatusBarIndexMultiplier (((SampleRate / 2.0) / ((double) MAXIMUM_GRAPH_WIDTH)) / 1000.0);
      }
    
    else
       {
      sprintf (DisplayString, "0 Hz    ");
      DataGraph->SetLeftLabel (DisplayString);
      sprintf (DisplayString, "%1.1lf Hz", SampleRate / 4.0);
      DataGraph->SetCenterLabel (DisplayString);
      sprintf (DisplayString, "%1.1lf Hz", SampleRate / 2.0);
      DataGraph->SetRightLabel (DisplayString);
      DataGraph->SetStatusBarValueText ("Frequency (Hz)");
      DataGraph->SetStatusBarIndexMultiplier ((SampleRate / 2.0) / ((double) MAXIMUM_GRAPH_WIDTH));
  } }  DataGraph->AddDataSet (pData, Scaling, *wxBLUE, DataSetSize);
   DataGraph->Show (TRUE);
   Show (FALSE);                                                  // Required for Linux
  Show (TRUE);
}   void GraphDisplay::SetDataArray (
  double *gain)  { pData = gain;
}  void GraphDisplay::SetScaling (
  double scaling)  { Scaling = scaling;
}  void GraphDisplay::SetGraphType (
  int graphType)  { GraphType = graphType;
}  void GraphDisplay::SetSampleRate (
  double sampleRate)  { SampleRate = sampleRate;
}   void GraphDisplay::SetDataSetSize (
  int size)  { 
//  wxMessageBox (_T("Got here"), "GraphDisplay::SetDataSetSize", wxOK | wxICON_EXCLAMATION, NULL);
    DataSetSize = size;
  DataGraph->SetStatusBarIndexMultiplier (((double) DataSetSize) / ((double) MAXIMUM_GRAPH_WIDTH));
}   

// Resets graph to frequency plot
void GraphDisplay::ResetGraphType (
  void)  { 
//  GraphTypeComboBox ->SetSelection (0);
}   void GraphDisplay::ShowGraph (
  bool show)  { DataGraph->Show (show);
}   void GraphDisplay::EnableGraphTypeComboBox (
  bool enable)  { 
//  GraphTypeComboBox->Enable (enable);             // Enable graph type selected
    bool DummyBool = enable;
}   BEGIN_EVENT_TABLE (GraphDisplay, wxPanel)  EVT_COMBOBOX (ID_TIME_DOMAIN_GRAPH, GraphDisplay::OnGraphTypeChanged)  END_EVENT_TABLE ()   
