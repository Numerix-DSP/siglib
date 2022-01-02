/******************************************************************************
Numerix Graphics Library for wxWidgets
Top level graph functions

SigLib is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

SigLib is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1335, USA

This sofware is also available with a commercial license, for use in
proprietary, research, government or commercial applications.
Please contact Sigma Numerix Ltd. for further details :
https://www.numerix-dsp.com
support@.numerix-dsp.com

Copyright (c) 2022, Sigma Numerix Ltd, All rights reserved.
****************************************************************************/

// Include files
// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#if wxUSE_TOOLTIPS
    #include "wx/tooltip.h"
#endif

#include <math.h>
#include <stdlib.h>
#if __APPLE__
#include <malloc/malloc.h>
#else
#include <malloc.h>
#endif
#include "ngl.h"
#include "demo.h"


/*
    Name : GraphDisplay::GraphDisplay
    Description : Child widget that is used to display the filter graphs
    Notes :
*/

GraphDisplay::GraphDisplay (wxWindow *parent)
    : wxPanel (parent, wxID_ANY, wxDefaultPosition, wxDefaultSize)

{
//  wxMessageBox ("Got here", "GraphDisplay::GraphDisplay", wxOK | wxICON_EXCLAMATION, NULL);

                        // Make the top-level layout; a vertical box to contain all widgets and sub-layouts.
    wxBoxSizer *VSizer = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer *HSizer1 = new wxBoxSizer(wxHORIZONTAL); VSizer->Add(HSizer1, 0, wxALIGN_CENTER | wxALL, 0);
    wxBoxSizer *HSizer2 = new wxBoxSizer(wxHORIZONTAL); VSizer->Add(HSizer2, 0, wxALIGN_CENTER | wxALL, 0);
    wxBoxSizer *HSizer2p = new wxBoxSizer(wxHORIZONTAL); VSizer->Add(HSizer2p, 0, wxALIGN_CENTER | wxALL, 0);

//  GraphPanel = new wxPanel (this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
    GraphPanel = new wxPanel (this, wxID_ANY, wxDefaultPosition, wxSize (0,GRAPH_HEIGHT+80));
    HSizer1->Add (GraphPanel, 0, wxALIGN_CENTER | wxALL, 0);

                                            // Declare the graphs here
                                            // The size is the size of the graph client area, where the actual graph is drawn
//  DataGraph = new LineGraph (GraphPanel, wxID_ANY, wxSize (GRAPH_WIDTH, GRAPH_HEIGHT));
    DataGraph = new LineGraph (this, wxID_ANY, wxSize (GRAPH_WIDTH, GRAPH_HEIGHT));
    HSizer1->Add (DataGraph, 0, wxALIGN_CENTER | wxALL, 0);

    char DisplayString[20];
    DataGraph->SetLeftLabel ("  0");
    sprintf (DisplayString, "      %d", GRAPH_WIDTH / 2);
    DataGraph->SetCenterLabel (DisplayString);
    sprintf (DisplayString, "      %d", GRAPH_WIDTH);
    DataGraph->SetRightLabel (DisplayString);

    PoleZeroDiagram = new PoleZeroPlot(this, wxID_ANY, wxSize (GRAPH_WIDTH, GRAPH_HEIGHT));

//  GraphPanel->Show (TRUE);                // Hide the panel because we do not have any graphs showing


                        // A combo box for getting required data type
    HSizer2->Add (new wxStaticText (this, -1, "Graph type : "), 0, wxALIGN_CENTER | wxALL, 0);
    static const wxString GraphChoices[] =
    {
        _T("Sine"),
        _T("Cosine"),
        _T("Pole-zero")
    };
    GraphTypeComboBox = new wxComboBox (this, ID_DFGRAPH_GRAPH_TYPE_CHANGED, "Sine", wxDefaultPosition,
                                 wxSize (160, -1), WXSIZEOF(GraphChoices), GraphChoices, wxCB_READONLY);
    HSizer2->Add (GraphTypeComboBox, 0, wxALIGN_CENTER | wxALL, 0);

    HSizer2p->AddSpacer(20);                    // Put in a blank spacer line

    GraphType = GRAPH_SINE;                     // Initialise graph type

    SetAutoLayout (TRUE);
    SetSizer (VSizer);

    VSizer->SetSizeHints (this);

// Debug
//  char DebugString[80];
//  wxSize DebugSize;
//  DebugSize = HSizer1->GetSize();
//  sprintf (DebugString, "Height = %d, Width = %d", DebugSize.GetHeight(), DebugSize.GetWidth());
//  wxMessageBox (DebugString, _T("GraphDisplay::GraphDisplay"), wxOK | wxICON_EXCLAMATION, NULL);

    VSizer->Fit (this);

    DataGraph->Show (FALSE);
    PoleZeroDiagram->Show (FALSE);
}


/*
    Function Name : GraphDisplay::~GraphDisplay
    Description : GraphDisplay destructor
    Notes :
*/

GraphDisplay::~GraphDisplay()
{


}


/*
    Function Name : GraphDisplay::OnGraphTypeChanged
    Description : Function to process when graph type has changed
    Notes :
*/

void GraphDisplay::OnGraphTypeChanged (wxCommandEvent & WXUNUSED(event))

{
//  wxMessageBox (_T("Got here"), _T("GraphDisplay::OnGraphTypeChanged"), wxOK | wxICON_EXCLAMATION, NULL);

    char DisplayString[40];

    GraphType = GraphTypeComboBox->GetSelection();      // Get the graph type

// Debug
//  char string [80];
//  sprintf (string, "Graph type = %d", GraphType);
//  wxMessageBox (_T(string), _T("LineGraphCanvas::LineGraphCanvas"), wxOK | wxICON_EXCLAMATION, NULL);

    if (GraphType == GRAPH_SINE)
    {
        double *pTemp;
        pTemp = (double *)malloc ((size_t)(GRAPH_WIDTH * sizeof (double)));
        int i;
        for (i = 0; i < GRAPH_WIDTH; i++)
        {
            pTemp[i] = sin ((TWO_PI * ((double)i)) / 512.);
        }
        DataGraph->AddDataSet (pTemp, GRAPH_SCALE, *wxBLUE, GRAPH_WIDTH);
        free (pTemp);

        sprintf (DisplayString, "      %d", GRAPH_WIDTH / 2);
        DataGraph->SetCenterLabel (DisplayString);
        sprintf (DisplayString, "      %d", GRAPH_WIDTH);
        DataGraph->SetRightLabel (DisplayString);
        DataGraph->SetStatusBarValueText ("Sample number");
        DataGraph->SetStatusBarIndexMultiplier (1.0);

        PoleZeroDiagram->Show (FALSE);
        DataGraph->Show (TRUE);

//      GraphPanel->Show (TRUE);                // Unhide the panel because we now have graphs showing
    }

    else if (GraphType == GRAPH_COSINE)
    {
        double *pTemp;
        pTemp = (double *)malloc ((size_t)(GRAPH_WIDTH * sizeof (double)));
        int i;
        for (i = 0; i < GRAPH_WIDTH; i++)
        {
            pTemp[i] = cos ((TWO_PI * ((double)i)) / 512.);
        }
        DataGraph->AddDataSet (pTemp, GRAPH_SCALE, *wxBLUE, GRAPH_WIDTH);
        free (pTemp);

        sprintf (DisplayString, "      %d", GRAPH_WIDTH / 2);
        DataGraph->SetCenterLabel (DisplayString);
        sprintf (DisplayString, "      %d", GRAPH_WIDTH);
        DataGraph->SetRightLabel (DisplayString);
        DataGraph->SetStatusBarValueText ("Sample number");
        DataGraph->SetStatusBarIndexMultiplier (1.0);

        PoleZeroDiagram->Show (FALSE);
        DataGraph->Show (TRUE);

//      GraphPanel->Show (TRUE);                // Unhide the panel because we now have graphs showing
    }

    else if (GraphType == GRAPH_POLE_ZERO)
    {
        pPolesAndZeros [0].real = 0.900000; pPolesAndZeros [0].imag = 0.000000; // 0.9, > 0.0
        pPolesAndZeros [1].real = 0.984808; pPolesAndZeros [1].imag = 0.173648; // 1.0, > 10.0
        pPolesAndZeros [2].real = 0.845723; pPolesAndZeros [2].imag = 0.307818; // 0.9, > 20.0
        pPolesAndZeros [3].real = 0.866025; pPolesAndZeros [3].imag = 0.500000; // 1.0, > 30.0
        pPolesAndZeros [4].real = 0.689440; pPolesAndZeros [4].imag = 0.578509; // 0.9, > 40.0
        pPolesAndZeros [5].real = 0.642788; pPolesAndZeros [5].imag = 0.766044; // 1.0, > 50.0
        pPolesAndZeros [6].real = 0.450000; pPolesAndZeros [6].imag = 0.779423; // 0.9, > 60.0
        pPolesAndZeros [7].real = 0.342020; pPolesAndZeros [7].imag = 0.939693; // 1.0, > 70.0
        pPolesAndZeros [8].real = 0.156283; pPolesAndZeros [8].imag = 0.886327; // 0.9, > 80.0
        pPolesAndZeros [9].real = 0.000000; pPolesAndZeros [9].imag = 1.000000; // 1.0, > 90.0

        PoleZeroDiagram->AddDataSet (pPolesAndZeros, 5, PZ_CONJUGATE_POLE_ZERO);
//      PoleZeroDiagram->AddDataSet (pPolesAndZeros, 5, PZ_COMPLEX_POLE_ZERO);
//      PoleZeroDiagram->AddDataSet (pPolesAndZeros, 5, PZ_COMPLEX_ZERO);
//      PoleZeroDiagram->AddDataSet (pPolesAndZeros, 5, PZ_CONJUGATE_ZERO);
//      PoleZeroDiagram->AddDataSet (pPolesAndZeros, 5, PZ_COMPLEX_POLE);
//      PoleZeroDiagram->AddDataSet (pPolesAndZeros, 5, PZ_CONJUGATE_POLE);

    // wxMessageBox (_T("Got here - PoleZeroDiagram->AddDataSet()"), _T("GraphDisplay::OnGraphTypeChanged"), wxOK | wxICON_EXCLAMATION, NULL);
        DataGraph->Show (FALSE);
    // wxMessageBox (_T("Got here - DataGraph->Show (FALSE)"), _T("GraphDisplay::OnGraphTypeChanged"), wxOK | wxICON_EXCLAMATION, NULL);
        PoleZeroDiagram->Show (TRUE);
    // wxMessageBox (_T("Got here - PoleZeroDiagram->Show (TRUE)"), _T("GraphDisplay::OnGraphTypeChanged"), wxOK | wxICON_EXCLAMATION, NULL);

//      GraphPanel->Show (TRUE);                // Unhide the panel because we now have graphs showing
    }

    // Show (FALSE);                    // Required for Linux
    // wxMessageBox (_T("Got here - Show (FALSE)"), _T("GraphDisplay::OnGraphTypeChanged"), wxOK | wxICON_EXCLAMATION, NULL);
    // Show (TRUE);
    // wxMessageBox (_T("Got here - Show (TRUE)"), _T("GraphDisplay::OnGraphTypeChanged"), wxOK | wxICON_EXCLAMATION, NULL);

    // wxMessageBox (_T("Got here - End"), _T("GraphDisplay::OnGraphTypeChanged"), wxOK | wxICON_EXCLAMATION, NULL);
}


void GraphDisplay::SetDataArray (double * data)
{
    pData = data;
}

void GraphDisplay::SetPoleZeroPlotArray (SLComplexRect_s * PoleZeroArray)
{
    pPolesAndZeros = PoleZeroArray;
}


void GraphDisplay::ShowGraph (bool show)
{
    if (GraphType == GRAPH_SINE)
    {
        DataGraph->Show (show);
    }

    else if (GraphType == GRAPH_COSINE)
    {
        DataGraph->Show (show);
    }

    else if (GraphType == GRAPH_POLE_ZERO)
    {
        PoleZeroDiagram->Show (show);
    }
}


BEGIN_EVENT_TABLE (GraphDisplay, wxPanel)
    EVT_COMBOBOX (ID_DFGRAPH_GRAPH_TYPE_CHANGED, GraphDisplay::OnGraphTypeChanged)
END_EVENT_TABLE()


