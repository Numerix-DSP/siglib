// SA include file
// Copyright (c) 1999-2024 Delta Numerix, All rights reserved.

#ifndef     _SA_H
#define     _SA_H

#include <wx/spinctrl.h>
#include <wx/splitter.h>

#include <siglib.h>                                                 // SigLib header file - comment out this line if SigLib is not available

#include "ngl.h"                                                    // NGL header file - should always be included after siglib.h, if SigLib is being used
#include "led.h"


#define DEBUG   0                                                   // Set to 0 for release mode


// Constant definitions

#define SA_COPYRIGHT_NOTICE             "SA Version 2.00. (c) 2002 to 2019 Numerix Ltd."
#define SA_ID                           "12/05/2019 V2.00"
#define SA_VERSION                      "2.00"


#define SA_LEFT                         100                         // SA layout
#define SA_TOP                          20
#define SA_WIDTH                        880
#define SA_HEIGHT                       664

#define FILTER_ENTRY_PANEL_WIDTH        240                         // Panel layout
#define FILTER_ENTRY_PANEL_HEIGHT       600

#define GRAPH_PANEL_WIDTH               600                         // Total panel size
#define GRAPH_PANEL_HEIGHT              600

#define MAXIMUM_GRAPH_WIDTH             512                         // Maximum graph width
#define MINIMUM_GRAPH_WIDTH             10                          // Minimum graph width
#define MAXIMUM_GRAPH_HEIGHT            501                         // Odd number allows for zero !!

#define SAMPLE_LENGTH                   ((SLArrayIndex_t)1024)      // Number of samples to take

#define FFT_SIZE                        ((SLArrayIndex_t)1024)      // FFT Size
#define LOG2_FFT_SIZE                   ((SLArrayIndex_t)10)        // Log 2 FFT Size

#define XCORR_FFT_SIZE                  ((SLArrayIndex_t)(FFT_SIZE * 2))  // Cross correlation FFT Size
#define LOG2_XCORR_FFT_SIZE             ((SLArrayIndex_t)(LOG2_FFT_SIZE + 1)) // Cross correlation Log 2 FFT Size

#define CHIRP_LENGTH                    ((SLArrayIndex_t)512)       // Length of chirp signal in samples

enum {
  ID_EVENT_PROCESS = wxID_HIGHEST + 100,
  ID_EVENT_TD_DATASET_SIZE_SPIN_CHANGED,
  ID_EVENT_TRIG_LEVEL_SPIN_CHANGED,
  ID_EVENT_ONE_POLE_SPIN_CHANGED,
  ID_EVENT_PEAK_HOLD_SPIN_CHANGED,
  ID_EVENT_TYPE_CHANGED,
  ID_EVENT_TIME_TRIG_POS_RADIOBUTTON,
  ID_EVENT_TIME_TRIG_NEG_RADIOBUTTON,
  ID_EVENT_CALIBRATE,
};

enum                                                                // Types of processing that can be performed
{
  ID_PROC_TIME,
  ID_PROC_TIME_CONT,
  ID_PROC_FREQ,
  ID_PROC_FREQ_CONT,
  ID_PROC_CHIRP_TIME,
  ID_PROC_CHIRP_FREQ,
  ID_PROC_CHIRP_XCORR,
  ID_PROC_CHIRP_REF_TIME,
  ID_PROC_CHIRP_REF_FREQ,
  ID_PROC_CHIRP_REF_XCORR
};

enum                                                                // Types of graph
{
  ID_TIME_DOMAIN_GRAPH,
  ID_FREQ_DOMAIN_GRAPH
};



/*
    Name : Function ProcessWindow
    Description : Child widget to process the data
    Notes :
*/

class           SAProcWindow:public wxPanel {
public:
  SAProcWindow (
  wxWindow * parent,
  wxWindowID id);
  ~SAProcWindow (
    );

  LEDCanvas      *canvas;
  LED            *TriggerLED;

  void            SetProjectNameString (
  wxString);
  void            SetSampleRate (
  double);
  void            SetDataArray (
  double *);

  void            OnProcessData (
  wxCommandEvent & event);
  void            OnCalibrateIO (
  wxCommandEvent & event);
  void            OnIdle (
  wxIdleEvent & event);

private:
  void            OnDataSetSizeSpinControlChange (
  wxSpinEvent & event);
  void            OnTriggerLevelSpinControlChange (
  wxSpinEvent & event);
  void            OnOnePoleFilterSpinControlChange (
  wxSpinEvent & event);
  void            OnPeakHoldSpinControlChange (
  wxSpinEvent & event);
  void            OnTypeChanged (
  wxCommandEvent & event);

  double         *pFDPRealData, *pFDPImagData, *pFDPWindow, *pFDPFFTCoeffs;

  wxSpinCtrl     *DataSetSizeSpinCtrl;
  wxCheckBox     *TimeTriggerEnableCheckBox;
  wxSpinCtrl     *TimeTriggerLevelSpinCtrl;
  wxRadioButton  *TimeTriggerPositiveRadioButton, *TimeTriggerNegativeRadioButton;
  wxCheckBox     *DCRemoveCheckBox;

  wxSpinCtrl     *FFTOnePoleSpinCtrl;
  wxSpinCtrl     *FFTPeakHoldSpinCtrl;

  wxComboBox     *ProcessingTypeComboBox;

  wxSpinCtrl     *PolyPhaseSpinControl;
  wxSpinCtrl     *NumCoeffsSpinControl;

  wxSpinCtrl     *CalTholdSpinCtrl;

  double          SampleRate;

  double         *pData;
  double         *pOnePoleDelay;
  double         *pPeakHoldDelay;

  double          ZXPrevSign;                                       // Previous value for trigger detect
  long            TriggerLocation;                                  // Location of trigger in source array

                  DECLARE_EVENT_TABLE (
    )
};



/*
    Name : GraphDisplay
    Description : Child widget that is used to display the graph
    Notes :
*/

class           GraphDisplay:public wxPanel {
public:
  GraphDisplay (
  wxWindow * parent,
  wxWindowID id);
  ~GraphDisplay (
    );

  void            SetDataArray (
  double *);
  void            SetScaling (
  double);
  void            SetSampleRate (
  double);
  void            SetDataSetSize (
  int);
  void            SetGraphType (
  int);

  void            OnGraphTypeChanged (
  wxCommandEvent & event);

  void            ResetGraphType (
  void);                                                            // Resets graph to frequency plot

  void            ShowGraph (
  bool show);
  void            EnableGraphTypeComboBox (
  bool enable);

protected:

private:
  int             GraphType;
  wxPaintDC      *GraphDC;

  int             DataSetSize;
  double          SampleRate;
  double          Scaling;

  LineGraph      *DataGraph;

  wxPanel        *GraphPanel;                                       // Panel for plotting graphs

  double         *pData;

                  DECLARE_EVENT_TABLE (
    )
};



class           DisplayTextDialog:public wxDialog {
public:
  DisplayTextDialog (
  wxFrame * parent,
  wxWindowID id,
  const wxString & title,
  const wxPoint & pos = wxDefaultPosition, const wxSize & size = wxDefaultSize);

  void            SetText (
  wxString & string);
  void            OnSize (
  wxSizeEvent & event);

private:
                  wxTextCtrl * TextBox;

                  DECLARE_EVENT_TABLE (
    )
};


#endif                                                              // End of #ifndef _SA_H
