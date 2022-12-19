/******************************************************************************
Numerix Graphics Library for wxWidgets
Functions to display a graphical array

SigLib is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License V2, as
published by the Free Software Foundation.

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

#include "wx/graphics.h"

#if __APPLE__
#include <malloc/malloc.h>
#else
#include <malloc.h>
#endif
#include <stdlib.h>
#include "ngl.h"
#include "mainframe.h"          // This is the top level application header file
                                // It is used for the mouse event handler
                                // The graph values are pushed to the application for displaying on mouse move

// Math function
double AbsMax (const double *, const long);


// Define constants

#define SL_UNIT_CIRCLE_RADIUS           100
#define SL_UNIT_CIRCLE_RADIUS_FLOAT     100.0

#define ALWAYS                          1



/**/
/********************************************************
* Function: LineGraph::LineGraph
*
* Parameters:
*   const long BufferSize
*
* Return value:
*   long    ErrorCode - Memory allocation error value
*
* Description: Initialise display
*
********************************************************/

LineGraph::LineGraph (wxWindow *parent, wxWindowID id, const wxSize graphSize)
//  : wxPanel (parent, id, wxDefaultPosition, graphSize)
    : wxPanel (parent, id, wxDefaultPosition, wxDefaultSize)

{
//  wxMessageBox (_T("Got here"), _T("LineGraph::LineGraph"), wxOK | wxICON_EXCLAMATION, NULL);

    DataSetSize = 0;                    // Initialise to zero for first time through

    GraphSize = graphSize;              // This saves the physical size of the graph area
    QuantizeXAxisFlag = false;          // Don't quantize the x-axis

                        // Make the top-level layout; a vertical box to contain all widgets and sub-layouts.
    wxBoxSizer *VSizer = new wxBoxSizer (wxVERTICAL);

    wxBoxSizer *HSizer0 = new wxBoxSizer (wxHORIZONTAL); VSizer->Add (HSizer0, 0, wxALIGN_CENTER | wxALL, 0);   // Graph + left labels
    wxBoxSizer *HSizer1 = new wxBoxSizer (wxHORIZONTAL); VSizer->Add (HSizer1, 0, wxALIGN_CENTER | wxALL, 0);   // Bottom labels

    wxBoxSizer *VSizer0 = new wxBoxSizer (wxVERTICAL); HSizer0->Add (VSizer0, 0, wxALIGN_CENTER | wxALL, 0);    // Left labels
    wxBoxSizer *VSizer1 = new wxBoxSizer (wxVERTICAL); HSizer0->Add (VSizer1, 0, wxALIGN_CENTER | wxALL, 0);    // Graph

    VSizer0->SetMinSize (-1, graphSize.GetHeight()+20);     // Set min height for vertical text

                        // Vertical axis labels
    VSizer0->Add (TopLabel = new wxStaticText  (this, -1, "Max"), 0, wxALIGN_LEFT | wxALL, 5);
    VSizer0->Add (0, 0, 1);                                 // This line pads out the vertical space
    VSizer0->Add (MiddleLabel = new wxStaticText  (this, -1, "Middle"), 0, wxALIGN_LEFT | wxALL, 5);
    VSizer0->Add (0, 0, 1);                                 // This line pads out the vertical space
    VSizer0->Add (BottomLabel = new wxStaticText  (this, -1, "-Max"), 0, wxALIGN_LEFT | wxALL, 5);

    HSizer1->SetMinSize (graphSize.GetWidth()+80, -1);      // Set min width for horizontal text

                        // Horizontal axis labels
    HSizer1->Add (new wxStaticText  (this, -1, "           "), 0, wxALIGN_CENTER | wxALL, 5);
    HSizer1->Add (LeftLabel = new wxStaticText  (this, -1, "0"), 0, wxALIGN_CENTER | wxALL, 0);
    HSizer1->Add (0, 0, 1);                                 // This line pads out the horizontal space
    HSizer1->Add (CenterLabel = new wxStaticText  (this, -1, "N/2"), 0, wxALIGN_CENTER | wxALL, 0);
    HSizer1->Add (0, 0, 1);                                 // This line pads out the horizontal space
    HSizer1->Add (RightLabel = new wxStaticText  (this, -1, "N          "), 0, wxALIGN_CENTER | wxALL, 0);

                        // Graph
    wxPanel *DrawPanel = new wxPanel (this, wxID_ANY, wxDefaultPosition, GraphSize);
    VSizer1->Add (DrawPanel, 0, wxALIGN_CENTER | wxALL, 5);

    GraphCanvas = new LineGraphCanvas (this, DrawPanel, wxDefaultPosition, GraphSize);
//  GraphCanvas->SetSize (GraphSize);

    SetAutoLayout (TRUE);
    SetSizer (VSizer);

    VSizer->SetSizeHints (this);
    VSizer->Fit (this);

    pDataSet = (double *)malloc (MAXIMUM_GRAPH_WIDTH * sizeof (double));    // Allocate memory
    if (pDataSet == NULL) {
        wxMessageBox (_T("Memory Allocation Error"), _T("LineGraph::LineGraph"), wxOK | wxICON_EXCLAMATION, NULL);
    }
}       // End of LineGraph::LineGraph()



/*
    Function Name : LineGraph::~LineGraph
    Description : LineGraph destructor
    Notes :
*/

LineGraph::~LineGraph()
{
    free (pDataSet);                            // Free the memory
}


/**/
/********************************************************
* Function: LineGraph::Draw
*
* Parameters:
*
*
* Return value:
*
*
* Description: Draw the graph
*
********************************************************/

void LineGraph::Draw (wxDC& GraphPaintDC, bool WXUNUSED (draw_bitmaps))

{
//  wxMessageBox (_T("Got here"), _T("LineGraph::Draw"), wxOK | wxICON_EXCLAMATION, NULL);

    long i;

    wxRect r = wxRect (wxDefaultPosition, GraphSize);

    GraphPaintDC.SetBrush (* wxWHITE_BRUSH);
    GraphPaintDC.SetPen (* wxBLACK_PEN);

#ifdef __WXMSW__
    GraphPaintDC.DrawRectangle (r.GetLeft()+1, r.GetTop()+1, r.GetRight()+2, r.GetBottom()+2);      // X, Y, W, H
#else
    GraphPaintDC.DrawRectangle (r.GetLeft()+2, r.GetTop()+2, r.GetRight()+1, r.GetBottom()+1);      // X, Y, W, H
#endif

                // Draw axis
    GraphPaintDC.SetPen (*wxBLACK_PEN);

    long Height = (long)GraphSize.GetHeight();
    long Width = (long)GraphSize.GetWidth();

#define MAJOR_MARK_SIZE     8
#define MINOR_MARK_SIZE     4

                                    // Horizontal dividers
#define VERT_NUMBER_OF_DIVISIONS    8       // Split top and bottom into quarters
#define VERT_MARKS_PER_DIVISION     4
#define VERT_TOTAL_NUMBER_OF_MARKS  (VERT_NUMBER_OF_DIVISIONS * VERT_MARKS_PER_DIVISION)

    for (i = 1; i < VERT_TOTAL_NUMBER_OF_MARKS; i++)
    {
                                    // Full length - only on even indices
        if ((i & 3) == 0)
        {
            GraphPaintDC.DrawLine (0, (int)((i * Height) / VERT_TOTAL_NUMBER_OF_MARKS),
                                   Width + 1, (int)((i * Height) / VERT_TOTAL_NUMBER_OF_MARKS));
        }

                                    // Major mark - only on odd indices
        else if ((i & 1) == 0)
        {
                                    // Left hand side
            GraphPaintDC.DrawLine (0, (int)((i * Height) / VERT_TOTAL_NUMBER_OF_MARKS),
                                   MAJOR_MARK_SIZE, (int)((i * Height) / VERT_TOTAL_NUMBER_OF_MARKS));
                                    // Right hand side
            GraphPaintDC.DrawLine (Width - MAJOR_MARK_SIZE, (int)((i * Height) / VERT_TOTAL_NUMBER_OF_MARKS),
                                   Width, (int)((i * Height) / VERT_TOTAL_NUMBER_OF_MARKS));
        }
                                    // Minor mark
        else
        {
                                    // Left hand side
            GraphPaintDC.DrawLine (0, (int)((i * Height) / VERT_TOTAL_NUMBER_OF_MARKS),
                                   MINOR_MARK_SIZE, (int)((i * Height) / VERT_TOTAL_NUMBER_OF_MARKS));
                                    // Right hand side
            GraphPaintDC.DrawLine (Width - MINOR_MARK_SIZE, (int)((i * Height) / VERT_TOTAL_NUMBER_OF_MARKS),
                                   Width, (int)((i * Height) / VERT_TOTAL_NUMBER_OF_MARKS));
        }
    }
//  wxMessageBox (_T("Got here"), _T("LineGraph::Draw"), wxOK | wxICON_EXCLAMATION, NULL);

                                    // Vertical dividers
    for (i = 1; i < 40; i++)
    {
                                    // Full length - only on even indices
        if ((i & 3) == 0)
        {
            GraphPaintDC.DrawLine ((int)((i * Width) / 40), 0, (int)((i * Width) / 40), Height);
        }

                                    // Major mark - only on odd indices
        else if ((i & 1) == 0)
        {
                                    // Top
            GraphPaintDC.DrawLine ((int)((i * Width) / 40), 0, (int)((i * Width) / 40), MAJOR_MARK_SIZE);
                                    // Middle
            GraphPaintDC.DrawLine ((int)((i * Width) / 40), (Height >> 1) - (MAJOR_MARK_SIZE - 1),
                                        (int)((i * Width) / 40), (Height >> 1) + MAJOR_MARK_SIZE);
                                    // Bottom
            GraphPaintDC.DrawLine ((int)((i * Width) / 40), Height - MAJOR_MARK_SIZE, (int)((i * Width) / 40), Height);
        }
                                    // Minor mark
        else
        {
                                    // Top
            GraphPaintDC.DrawLine ((int)((i * Width) / 40), 0, (int)((i * Width) / 40), MINOR_MARK_SIZE);
                                    // Middle
            GraphPaintDC.DrawLine ((int)((i * Width) / 40), (Height >> 1) - (MINOR_MARK_SIZE - 1),
                                        (int)((i * Width) / 40), (Height >> 1) + MINOR_MARK_SIZE);
                                    // Bottom
            GraphPaintDC.DrawLine ((int)((i * Width) / 40), Height - MINOR_MARK_SIZE, (int)((i * Width) / 40), Height);
        }
    }
//  wxMessageBox (_T("Got here"), _T("LineGraph::Draw"), wxOK | wxICON_EXCLAMATION, NULL);


    if (DataSetSize)                    // Only plot the graph if the data set exists
    {
        wxPen * DrawPen = new wxPen (LineColour, 1, wxSOLID);
        GraphPaintDC.SetPen (* DrawPen);

        double xP = 0.0;

        PreviousX = 0;
        PreviousY = GraphSize.GetHeight() >> 1;        // Initialise graph points

        for  (int x = 0; x < DataSetSize; x++ )
        {
            xP += ((double)GraphSize.GetWidth() ) / ((double)DataSetSize);

            int Y;
            double  GraphOffset;
                                        // Plot graph and scale to required max
            GraphOffset = ((double)(GraphSize.GetHeight() >> 1)) -
                        ((((double)(GraphSize.GetHeight() >> 1)) / Max) * pDataSet[x]);

            if (GraphOffset < 0.0)
                GraphOffset = 0.0;

            if (GraphOffset > ((double)GraphSize.GetHeight()))
                GraphOffset = ((double)GraphSize.GetHeight());

            Y = (int)GraphOffset;

            GraphPaintDC.DrawLine (PreviousX, PreviousY, (int)xP, Y);
            PreviousX = (int)xP;
            PreviousY = Y;
        }
        delete (DrawPen);
    }
//  wxMessageBox (_T("Got here"), _T("LineGraph::Draw"), wxOK | wxICON_EXCLAMATION, NULL);
}



/**/
/********************************************************
* Function: LineGraph::AddDataSet
*
* Parameters:
*   const double *BPtr      - array pointer
*   const double ScaleFlag  - Is graph scaled
*   const short Colour      - Graph colour
*   const long BufferSize   - Buffer size
*
* Return value:
*   void
*
* Description: Display a array of double fmt data
*   and join the samples with a line, the graph can be
*   added to an existing one, or the previous graph can
*   be removed first.
*
********************************************************/

void LineGraph::AddDataSet (const double *pSrc, const double scaleFlag, const wxColour colour, const int size)

{
//  wxMessageBox (_T("Got here"), _T("LineGraph::AddDataSet"), wxOK | wxICON_EXCLAMATION, NULL);

    LineColour = colour;

    if (size <= MAXIMUM_GRAPH_WIDTH)
    {
        DataSetSize = size;                     // Set size variable
    }
    else
    {
        DataSetSize = MAXIMUM_GRAPH_WIDTH;      // Set size variable
    }

    for  (int i = 0; i < DataSetSize; i++ )     // Copy data set to local array
    {
        pDataSet [i] = pSrc [i];
    }

    if (scaleFlag == GRAPH_SCALE)               // Auto scale graph
    {
        Max = AbsMax (pDataSet, (long)DataSetSize);
        if (Max == 0.0)                         // Do not divide by 0
        {
            Max = 1.0;
        }
    }

    else                                        // Scale graph to fit predefined max
    {
        Max = scaleFlag;
    }

    char LabelString[40];
    if (Max >= 1e3)
    {
        sprintf (LabelString, "%1.1lf", Max);
        SetTopLabel (wxString (LabelString));
        SetMiddleLabel ("0.0");
        sprintf (LabelString, "-%1.1lf", Max);
        SetBottomLabel (wxString (LabelString));
    }
    else
    {
        sprintf (LabelString, "%1.3lf", Max);
        SetTopLabel (wxString (LabelString));
        SetMiddleLabel ("0.0");
        sprintf (LabelString, "-%1.3lf", Max);
        SetBottomLabel (wxString (LabelString));
    }

    Refresh();          // Refresh graph window

}       // End of LineGraph::AddDataSet()



void LineGraph::SetTopLabel (wxString String)

{
    TopLabel->SetLabel (String);
}       // End of LineGraph::SetTopLabel()


void LineGraph::SetMiddleLabel (wxString String)

{
    MiddleLabel->SetLabel (String);
}       // End of LineGraph::SetMiddleLabel()


void LineGraph::SetBottomLabel (wxString String)

{
    BottomLabel->SetLabel (String);
}       // End of LineGraph::SetBottomLabel()


void LineGraph::SetLeftLabel (wxString String)

{
    LeftLabel->SetLabel (String);
}       // End of LineGraph::SetLeftLabel()


void LineGraph::SetCenterLabel (wxString String)

{
    CenterLabel->SetLabel (String);
}       // End of LineGraph::SetCenterLabel()


void LineGraph::SetRightLabel (wxString String)

{
    RightLabel->SetLabel (String);
}       // End of LineGraph::SetRightLabel()


void LineGraph::SetStatusBarValueText (wxString String)

{
    pStatusBarValueText = String;
}       // End of LineGraph::SetStatusBarValueText()


wxString LineGraph::GetStatusBarValueText (void)

{
    return (pStatusBarValueText);
}       // End of LineGraph::GetStatusBarValueText()


void LineGraph::SetQuantizeXAxisFlag (bool flag)

{
    QuantizeXAxisFlag = flag;
}       // End of LineGraph::SetStatusBarValueText()


bool LineGraph::GetQuantizeXAxisFlag (void)

{
    return (QuantizeXAxisFlag);
}       // End of LineGraph::GetStatusBarValueText()


void LineGraph::SetStatusBarIndexMultiplier (double Multiplier)

{
    StatusBarIndexMultiplier = Multiplier;
}       // End of LineGraph::SetStatusBarIndexMultiplier()


double LineGraph::GetStatusBarIndexMultiplier (void)

{
    return (StatusBarIndexMultiplier);
}       // End of LineGraph::GetStatusBarIndexMultiplier()


/**/
/********************************************************
* Function: LineGraph::AbsMax
*
* Parameters:
*   const double *pSrc,
*   const long SampleLength
*
* Return value:
*   Maximum absolute value in an array.
*
* Description:
*   Return the maximum absolute value in an array.
*
********************************************************/

double LineGraph::AbsMax (const double *pSrc,
    const long SampleLength)

{
    long   i;
    double Tmp, Max;

    Tmp = *pSrc++;                      // Initial value
    if (Tmp >= 0.)
    {
        Max = Tmp;
    }

    else
    {
        Max = -Tmp;
    }

    for (i = 1; i < SampleLength; i++)
    {
        Tmp = *pSrc++;

        if (Tmp >= 0.)                  // Positive data value
        {
            if (Tmp > Max)
            {
                Max = Tmp;
            }
        }

        else                            // Negative data value
        {
            if ((-Tmp) > Max)
            {
                Max = -Tmp;
            }
        }
    }

    return (Max);

}       // End of LineGraph::AbsMax()


/*
    Function Name : LineGraph::GetValue
    Description : Returns the value at the given location
    Notes :
*/

double LineGraph::GetValue (int Location)

{
    return pDataSet[Location];
}



BEGIN_EVENT_TABLE (LineGraph, wxPanel)
//  EVT_PAINT (LineGraph::OnPaint)
END_EVENT_TABLE()



// Define a constructor for the line graph canvas
LineGraphCanvas::LineGraphCanvas (LineGraph *pLineGraph, wxPanel *parent, wxPoint GraphPoint, wxSize graphSize):
//LineGraphCanvas::LineGraphCanvas (wxPanel *parent, wxPoint GraphPoint, wxSize graphSize):
    wxWindow (parent, wxID_ANY, GraphPoint, graphSize)

{
//  wxMessageBox (_T("Got here"), _T("LineGraphCanvas::LineGraphCanvas"), wxOK | wxICON_EXCLAMATION, NULL);

    GraphSize = graphSize;
    SetSize (GraphSize);            // Set the size of the canvas because it is not done by parent unless it is a frame
    m_owner = parent;
    m_LineGraph = pLineGraph;
//  m_LineGraph = parent;
}

LineGraphCanvas::~LineGraphCanvas()

{
}


// Define the repainting behaviour
void LineGraphCanvas::OnPaint (wxPaintEvent & WXUNUSED(event))
{
//  wxMessageBox (_T("Got here"), _T("LineGraphCanvas::OnPaint"), wxOK | wxICON_EXCLAMATION, NULL);

    wxPaintDC GraphPaintDC (this);
//  m_LineGraph->PrepareDC (GraphPaintDC);
    PrepareDC (GraphPaintDC);
    m_LineGraph->Draw (GraphPaintDC, TRUE);

}


// Define the mouse move behaviour
void LineGraphCanvas::OnMouseMove (wxMouseEvent & event)
{
//  wxMessageBox (_T("Got here"), _T("LineGraphCanvas::OnMouseMove"), wxOK | wxICON_EXCLAMATION, NULL);

    wxString    String;
//  int         ArrayIndex = (int)(((double)event.GetX()) * m_LineGraph->GetStatusBarIndexMultiplier());
    int         ArrayIndex = event.GetX();
    if (ArrayIndex >= MAXIMUM_GRAPH_WIDTH)
        ArrayIndex = MAXIMUM_GRAPH_WIDTH-1;

    if (m_LineGraph->GetQuantizeXAxisFlag() == true)
    {
        String.sprintf ("%s : %1.3lf, Value : %1.3lf          ",
                        m_LineGraph->GetStatusBarValueText().GetData(),
                        (double)ArrayIndex,
                        m_LineGraph->GetValue (ArrayIndex));
    }
    else
    {
        String.sprintf ("%s : %1.3lf, Value : %1.3lf          ",
                        m_LineGraph->GetStatusBarValueText().GetData(),
                        (((double)event.GetX()) * m_LineGraph->GetStatusBarIndexMultiplier()),
                        m_LineGraph->GetValue (ArrayIndex));
    }

    mainFrame *frame = (mainFrame*)(wxTheApp->GetTopWindow());
    frame->DisplayStatusLineText (String);
}


BEGIN_EVENT_TABLE(LineGraphCanvas, wxWindow)
    EVT_PAINT (LineGraphCanvas::OnPaint)
    EVT_MOTION (LineGraphCanvas::OnMouseMove)
END_EVENT_TABLE()



/**/
/********************************************************
* Function: PoleZeroPlot::PoleZeroPlot()
*
* Parameters:
*
* Return value:
*   void
*
* Description: Initialise pole zero plot
*
********************************************************/

PoleZeroPlot::PoleZeroPlot (wxWindow *parent, wxWindowID id, wxSize plotSize)
    : wxPanel (parent, id)

{
//  wxMessageBox (_T("Start"), _T("PoleZeroPlot::PoleZeroPlot"), wxOK | wxICON_EXCLAMATION, NULL);

    DataSetSize = 0;                    // Initialise to zero for first time through

    PlotSize = plotSize;                // This saves the physical size of the graph area

                        // Make the top-level layout; a vertical box to contain all widgets and sub-layouts.
    wxBoxSizer *VSizer = new wxBoxSizer (wxVERTICAL);

                        // Plot
    wxPanel *PlotPanel = new wxPanel (this, -1, wxDefaultPosition, PlotSize);
    // VSizer->Add (PlotPanel, 0, wxALIGN_RIGHT | wxALL, 5);
    VSizer->Add (PlotPanel, 0, wxALIGN_CENTER | wxALL, 5);
    PlotCanvas = new PoleZeroPlotCanvas (this, PlotPanel, wxPoint (55,22), PlotSize);
    PlotCanvas->SetSize (PlotSize);

    SetAutoLayout (TRUE);
    SetSizer (VSizer);

    VSizer->SetSizeHints (this);
    VSizer->Fit (this);

    pDataSet = (SLComplexRect_s *)malloc (2 * MAXIMUM_PZ_SIZE * sizeof (SLComplexRect_s));  // Allocate memory - Real and Imag
    if (pDataSet == NULL)
    {
        wxMessageBox (_T("Memory Allocation Error"), _T("PoleZeroPlot::PoleZeroPlot"), wxOK | wxICON_EXCLAMATION, NULL);
    }
}



/*
    Function Name : PoleZeroPlot::~PoleZeroPlot
    Description : PoleZeroPlot destructor
    Notes :
*/

PoleZeroPlot::~PoleZeroPlot()
{
    free (pDataSet);                            // Free the memory
}


/**/
/********************************************************
* Function: PoleZeroPlot::Draw()
*
* Parameters:
*
* Return value:
*   void
*
* Description: Plot a pole zero set
*
********************************************************/

void PoleZeroPlot::Draw (wxDC& GraphPaintDC, bool WXUNUSED (draw_bitmaps))

{
    int     i;
    double  XPos, YPos;

    // wxMessageBox (_T("Got here - Start"), _T("PoleZeroPlot::Draw"), wxOK | wxICON_EXCLAMATION, NULL);

    wxRect r = wxRect (wxDefaultPosition, PlotSize);

    GraphPaintDC.SetBrush (* wxWHITE_BRUSH);
    GraphPaintDC.SetPen (* wxBLACK_PEN);

    GraphPaintDC.DrawRectangle (r.GetLeft()+1, r.GetTop()+1, r.GetRight()+2, r.GetBottom()+2);      // X, Y, W, H

    int     XCenter = r.GetRight() >> 1;
    int     YCenter = r.GetBottom() >> 1;

    int     Radius = 3 * (r.GetBottom() >> 3);

                        // Draw axis
    GraphPaintDC.SetPen (* wxBLACK_PEN);

    GraphPaintDC.DrawEllipse (XCenter-Radius, YCenter-Radius, Radius<<1, Radius<<1);
    GraphPaintDC.DrawLine (XCenter-(Radius+5), YCenter, XCenter+Radius+5, YCenter);     // Horiz
    GraphPaintDC.DrawLine (XCenter, YCenter-(Radius+5), XCenter, YCenter+Radius+5);     // Virt


    if (DataSetSize)                    // Only plot the graph if the data set exists
    {
        switch (PlotType)
        {
            case PZ_COMPLEX_POLE_ZERO:
                for (i = 0; i < DataSetSize; i++)
                {
                    GraphPaintDC.SetPen (* wxRED_PEN);

                    XPos = pDataSet[(2 * i) + 0].real;
                    YPos = pDataSet[(2 * i) + 0].imag;

                                    // Plot poles - Above
                    GraphPaintDC.DrawLine (XCenter+((int)(XPos * ((double)Radius))) - 4, YCenter-((int)(YPos * ((double)Radius))) - 4,
                            XCenter+((int)(XPos * ((double)Radius))) + 5, YCenter-((int)(YPos * ((double)Radius))) + 5);
                    GraphPaintDC.DrawLine (XCenter+((int)(XPos * ((double)Radius))) - 4, YCenter-((int)(YPos * ((double)Radius))) + 5,
                            XCenter+((int)(XPos * ((double)Radius))) + 5, YCenter-((int)(YPos * ((double)Radius))) - 4);

                    GraphPaintDC.SetPen (* wxBLUE_PEN);
                    GraphPaintDC.SetBrush (* wxTRANSPARENT_BRUSH);

                    XPos = pDataSet[(2 * i) + 1].real;
                    YPos = pDataSet[(2 * i) + 1].imag;

                                    // Plot zeros - Above
                    GraphPaintDC.DrawEllipse (XCenter+((int)(XPos * ((double)Radius))) - 5,
                            YCenter-((int)(YPos * ((double)Radius))) - 5, 10, 10);
                }
                break;
            case PZ_CONJUGATE_POLE_ZERO:
                for (i = 0; i < DataSetSize; i++)
                {

                    GraphPaintDC.SetPen (* wxRED_PEN);

                    XPos = pDataSet[(2 * i) + 0].real;
                    YPos = pDataSet[(2 * i) + 0].imag;

                                    // Plot poles - Above
                    GraphPaintDC.DrawLine (XCenter+((int)(XPos * ((double)Radius))) - 4, YCenter-((int)(YPos * ((double)Radius))) - 4,
                            XCenter+((int)(XPos * ((double)Radius))) + 5, YCenter-((int)(YPos * ((double)Radius))) + 5);
                    GraphPaintDC.DrawLine (XCenter+((int)(XPos * ((double)Radius))) - 4, YCenter-((int)(YPos * ((double)Radius))) + 5,
                            XCenter+((int)(XPos * ((double)Radius))) + 5, YCenter-((int)(YPos * ((double)Radius))) - 4);

                                    // Plot poles - Below
                    GraphPaintDC.DrawLine (XCenter+((int)(XPos * ((double)Radius))) - 4, YCenter+((int)(YPos * ((double)Radius))) - 4,
                            XCenter+((int)(XPos * ((double)Radius))) + 5, YCenter+((int)(YPos * ((double)Radius))) + 5);
                    GraphPaintDC.DrawLine (XCenter+((int)(XPos * ((double)Radius))) - 4, YCenter+((int)(YPos * ((double)Radius))) + 5,
                            XCenter+((int)(XPos * ((double)Radius))) + 5, YCenter+((int)(YPos * ((double)Radius))) - 4);

                    GraphPaintDC.SetPen (* wxBLUE_PEN);
                    GraphPaintDC.SetBrush (* wxTRANSPARENT_BRUSH);

                    XPos = pDataSet[(2 * i) + 1].real;
                    YPos = pDataSet[(2 * i) + 1].imag;

                                    // Plot zeros - Above
                    GraphPaintDC.DrawEllipse (XCenter+((int)(XPos * ((double)Radius))) - 5,
                            YCenter-((int)(YPos * ((double)Radius))) - 5, 10, 10);

                                    // Plot zeros - Below
                    GraphPaintDC.DrawEllipse (XCenter+((int)(XPos * ((double)Radius))) - 5,
                            YCenter+((int)(YPos * ((double)Radius))) - 5, 10, 10);
                }
                break;
            case PZ_COMPLEX_ZERO:
                for (i = 0; i < DataSetSize; i++)
                {
                    GraphPaintDC.SetPen (* wxBLUE_PEN);
                    GraphPaintDC.SetBrush (* wxTRANSPARENT_BRUSH);

                    XPos = pDataSet[i].real;
                    YPos = pDataSet[i].imag;

                                    // Plot zeros - Above
                    GraphPaintDC.DrawEllipse (XCenter+((int)(XPos * ((double)Radius))) - 5,
                            YCenter-((int)(YPos * ((double)Radius))) - 5, 10, 10);
                }
                break;
            case PZ_CONJUGATE_ZERO:
                for (i = 0; i < DataSetSize; i++)
                {
                    GraphPaintDC.SetPen (* wxBLUE_PEN);
                    GraphPaintDC.SetBrush (* wxTRANSPARENT_BRUSH);

                    XPos = pDataSet[i].real;
                    YPos = pDataSet[i].imag;

                                    // Plot zeros - Above
                    GraphPaintDC.DrawEllipse (XCenter+((int)(XPos * ((double)Radius))) - 5,
                            YCenter-((int)(YPos * ((double)Radius))) - 5, 10, 10);

                                    // Plot zeros - Below
                    GraphPaintDC.DrawEllipse (XCenter+((int)(XPos * ((double)Radius))) - 5,
                            YCenter+((int)(YPos * ((double)Radius))) - 5, 10, 10);
                }
                break;
            case PZ_COMPLEX_POLE:
                for (i = 0; i < DataSetSize; i++)
                {
                    GraphPaintDC.SetPen (* wxRED_PEN);

                    XPos = pDataSet[i].real;
                    YPos = pDataSet[i].imag;

                                    // Plot poles - Above
                    GraphPaintDC.DrawLine (XCenter+((int)(XPos * ((double)Radius))) - 4, YCenter-((int)(YPos * ((double)Radius))) - 4,
                            XCenter+((int)(XPos * ((double)Radius))) + 5, YCenter-((int)(YPos * ((double)Radius))) + 5);
                    GraphPaintDC.DrawLine (XCenter+((int)(XPos * ((double)Radius))) - 4, YCenter-((int)(YPos * ((double)Radius))) + 5,
                            XCenter+((int)(XPos * ((double)Radius))) + 5, YCenter-((int)(YPos * ((double)Radius))) - 4);
                }
                break;
            case PZ_CONJUGATE_POLE:
                for (i = 0; i < DataSetSize; i++)
                {
                    GraphPaintDC.SetPen (* wxRED_PEN);

                    XPos = pDataSet[i].real;
                    YPos = pDataSet[i].imag;

                                    // Plot poles - Above
                    GraphPaintDC.DrawLine (XCenter+((int)(XPos * ((double)Radius))) - 4, YCenter-((int)(YPos * ((double)Radius))) - 4,
                            XCenter+((int)(XPos * ((double)Radius))) + 5, YCenter-((int)(YPos * ((double)Radius))) + 5);
                    GraphPaintDC.DrawLine (XCenter+((int)(XPos * ((double)Radius))) - 4, YCenter-((int)(YPos * ((double)Radius))) + 5,
                            XCenter+((int)(XPos * ((double)Radius))) + 5, YCenter-((int)(YPos * ((double)Radius))) - 4);

                                    // Plot poles - Below
                    GraphPaintDC.DrawLine (XCenter+((int)(XPos * ((double)Radius))) - 4, YCenter+((int)(YPos * ((double)Radius))) - 4,
                            XCenter+((int)(XPos * ((double)Radius))) + 5, YCenter+((int)(YPos * ((double)Radius))) + 5);
                    GraphPaintDC.DrawLine (XCenter+((int)(XPos * ((double)Radius))) - 4, YCenter+((int)(YPos * ((double)Radius))) + 5,
                            XCenter+((int)(XPos * ((double)Radius))) + 5, YCenter+((int)(YPos * ((double)Radius))) - 4);
                }
                break;
            default:
                break;
        }

    }

    // wxMessageBox (_T("Got here - End"), _T("PoleZeroPlot::Draw"), wxOK | wxICON_EXCLAMATION, NULL);
}



/**/
/********************************************************
* Function: PoleZeroPlot::AddDataSet()
*
* Parameters:
*
* Return value:
*   void
*
* Description: Plot a pole zero set
*
********************************************************/

void PoleZeroPlot::AddDataSet (const SLComplexRect_s *pSrc, int size, int plotType)

{
    // wxMessageBox (_T("Got here : Start"), _T("PoleZeroPlot::AddDataSet"), wxOK | wxICON_EXCLAMATION, NULL);

    if (size <= MAXIMUM_PZ_SIZE)
    {
        DataSetSize = size;                     // Set size variable
    }
    else
    {
        DataSetSize = MAXIMUM_PZ_SIZE;          // Set size variable
    }

    PlotType = plotType;                        // This saves the plot type

    for  (int i = 0; i < (2 * DataSetSize); i++ )       // Copy data set to local array - Real and Imag
    {
        pDataSet [i] = pSrc [i];
    }

//  Max = AbsMax (pDataSet, (long)DataSetSize);
//  if (Max == 0.0)                                 // Do not divide by 0
//  {
//      Max = 1.0;
//  }


//  char LabelString[40];
//  sprintf (LabelString, "%lf ", Max);
//  SetTopLabel (wxString (LabelString));
//  SetMiddleLabel ("0.0");
//  sprintf (LabelString, "-%lf ", Max);
//  SetBottomLabel (wxString (LabelString));

    Refresh();          // Refresh graph window

    // wxMessageBox (_T("Got here : End"), _T("PoleZeroPlot::AddDataSet"), wxOK | wxICON_EXCLAMATION, NULL);

}       // End of PoleZeroPlot::AddDataSet()



BEGIN_EVENT_TABLE (PoleZeroPlot, wxPanel)
//    EVT_PAINT  (PoleZeroPlot::OnPaint)
END_EVENT_TABLE()


// Define a constructor for the pole zero plot canvas
PoleZeroPlotCanvas::PoleZeroPlotCanvas (PoleZeroPlot *pPoleZeroPlot, wxPanel *parent, wxPoint PlotPoint, wxSize plotSize):
 wxWindow (parent, -1, PlotPoint, plotSize)

{
    PlotSize = plotSize;
    SetSize (PlotSize);         // Set the size of the canvas because it is not done by parent unless it is a frame
    m_owner = parent;
    m_PoleZeroPlot = pPoleZeroPlot;
}

PoleZeroPlotCanvas::~PoleZeroPlotCanvas()

{
}


// Define the repainting behaviour
void PoleZeroPlotCanvas::OnPaint (wxPaintEvent& WXUNUSED(event))
{
//  wxMessageBox (_T("Got here"), _T("PoleZeroPlotCanvas::OnPaint"), wxOK | wxICON_EXCLAMATION, NULL);

    wxPaintDC GraphPaintDC (this);
    m_PoleZeroPlot->PrepareDC (GraphPaintDC);
    m_PoleZeroPlot->Draw (GraphPaintDC, TRUE);
}


BEGIN_EVENT_TABLE(PoleZeroPlotCanvas, wxWindow)
    EVT_PAINT (PoleZeroPlotCanvas::OnPaint)
END_EVENT_TABLE()


