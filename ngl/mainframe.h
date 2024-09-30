// Digital filter design program mainFrame include file
// Copyright (c) 2024, Delta Numerix, All rights reserved.

#ifndef _MAINFRAME_H
#define _MAINFRAME_H

#include "demo.h"    // Include project header files

/*
    Function Name: mainFrame
    Description: Top level frame
    Notes: This should contain the filter designer and graph children
*/

class mainFrame : public wxFrame
{
    public:
  mainFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
  ~mainFrame();
  void OnAbout(wxCommandEvent& event);
  void OnExit(wxCommandEvent& event);
  void DisplayStatusLineText(wxString& string);

    private:
  GraphDisplay* FilterGraphDisplay;

  double* pData;
  SLComplexRect_s* pPolesAndZeros;

  DECLARE_EVENT_TABLE()
};

enum { DEMO_EXIT = wxID_HIGHEST + 1, DEMO_ABOUT };

#endif    // End of #ifndef _MAINFRAME_H
