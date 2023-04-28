// DSPGraph demo program include file
// Copyright (c) 2023, Alpha Numerix, All rights reserved.

#ifndef     _DEMO_H
#define     _DEMO_H

#include <wx/spinctrl.h>
#include "ngl.h"

#define DEBUG   0                                                   // Set to 0 for release mode


// Constant definitions
#define GRAPH_WIDTH                     500                         // Size of graph itself
#define GRAPH_HEIGHT                    501                         // Odd number to allow for zero line

#define GRAPH_PANEL_WIDTH               600                         // Total size of graph panel area
#define GRAPH_PANEL_HEIGHT              600

#define GRAPH_SINE                      0                           // Data types
#define GRAPH_COSINE                    1
#define GRAPH_POLE_ZERO                 2

enum {
  ID_DFGRAPH_GRAPH_TYPE_CHANGED = wxID_HIGHEST + 100
};


/*
    Name : GraphDisplay
    Description : Child widget that is used to display the graph
    Notes :
*/

class           GraphDisplay:public wxPanel {
public:
  GraphDisplay (
  wxWindow * parent);
  ~GraphDisplay (
    );
  void            SetDataArray (
  double *);
  void            SetPoleZeroPlotArray (
  SLComplexRect_s *);

  void            OnGraphTypeChanged (
  wxCommandEvent & event);

  void            ShowGraph (
  bool show);

protected:
                  wxComboBox * GraphTypeComboBox;

private:
  int             GraphType;
  wxPaintDC      *GraphDC;

  LineGraph      *DataGraph;                                        // Line graph
  PoleZeroPlot   *PoleZeroDiagram;                                  // Pole zero plot

  wxPanel        *GraphPanel;                                       // Panel for plotting graphs

  double         *pData;
  SLComplexRect_s *pPolesAndZeros;

                  DECLARE_EVENT_TABLE (
    )
};



#endif                                                              // End of #ifndef _DEMO_H
