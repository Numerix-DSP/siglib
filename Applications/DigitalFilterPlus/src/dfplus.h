// Digital filter design program include file
// This software is provided under the terms of the GNU General Public License
// V2, as published by the Free Software Foundation. Copyright (c) 1999-2024
// Delta Numerix, All rights reserved.

#ifndef _DFPLUS_H
#define _DFPLUS_H

#include "evenspin.h"
#include "inifile.h"
#include <wx/spinctrl.h>
#include <wx/splitter.h>

#include "ngl.h"       // NGL header file - should always be included after siglib.h, if SigLib is being used
#include <siglib.h>    // SigLib header file

#define DEBUG_LOG_FILE 0    // Set to 1 to log progress to "debug.log" file

/* Constant definitions */

#define DFPLUS_COPYRIGHT_NOTICE "Digital Filter Plus (C) 2002 to 2024 Delta Numerix, All rights reserved."
#define DFPLUS_ID "22/04/2024 V3.00"
#define DFPLUS_VERSION "3.00"

#define MAX_FIR_LENGTH 5000    // FIR filter length limit
#define MAX_FIR_BANDS 5
#define MAX_FIR_PP_BANKS 1000    // Maximum number of poly-phase filter banks

#define MAX_PZP_BIQUADS 5        // Max number of biquads in pole zero placement
#define MAX_BLT_ORDER 32         // Max filter order for bilinear transform
#define MAX_BESSEL_ORDER 10      // Max filter order for Bessel filter
#define SIMPLE_REMEZ_BANDS 3     // Number of frequency bands for simple Remez algo.
#define COMPLEX_REMEZ_BANDS 4    // Number of frequency bands for complex Remez algo.

// Declare global functions

SLData_t df_group_delay(SLData_t FreqStep, SLArrayIndex_t FirstCalc, SLData_t ThetaT);
long long FloatToQFormatInteger(double x, int m, int n);

// Declare enums

enum    // Filter design methods
{
  FIR_WINDOW_FILTER,
  FIR_SIMPLE_REMEZ_FILTER,
  FIR_REMEZ_FILTER,
  FIR_ALGS_FILTER,
  IIR_PZP_FILTER,
  IIR_FILTER,
  IIR_ALGS_FILTER,
  NOT_VALID_FILTER
};

enum    // FIR filter design algorithms
{
  FIR_HILBERT_TRANSFORM,
  FIR_RAISED_COSINE,
  FIR_ROOT_RAISED_COSINE,
  FIR_GAUSSIAN
};

enum    // FIR Remez filter design type
{
  FIR_REMEZ_STANDARD_FILTER,
  FIR_REMEZ_DIFFERENTIATOR,
  FIR_REMEZ_HILBERT
};

enum    // FIR Remez filter type
{
  FIR_REMEZ_LOW_PASS,
  FIR_REMEZ_HIGH_PASS,
  FIR_REMEZ_BAND_PASS,
  FIR_REMEZ_BAND_STOP
};

enum    // Graph display types
{
  LOG_MAGNITUDE_GRAPH,
  LIN_MAGNITUDE_GRAPH,
  PHASE_WRAPPED_GRAPH,
  PHASE_UNWRAPPED_GRAPH,
  GROUP_DELAY_GRAPH,
  IMPULSE_RESPONSE_GRAPH,
  STEP_RESPONSE_GRAPH,
  POLE_ZERO_GRAPH
};

enum    // IIR filter band type
{
  IIR_LPF,
  IIR_HPF,
  IIR_BPF,
  IIR_BSF
};

enum    // FIR filter design algorithms
{
  IIR_NOTCH_FILTER
};

enum    // IIR bilinear transform filter design type
{
  IIR_BUTTERWORTH,
  IIR_CHEBYSCHEV,
  IIR_INVERSE_CHEBYSCHEV,
  IIR_ELLIPTIC,
  IIR_BESSEL
};

enum    // IIR S to z translation type
{
  IIR_BILINEAR,
  IIR_MATCHED_Z
};

#define DFPLUS_LEFT 100    // DFPlus layout
#define DFPLUS_TOP 20
#define DFPLUS_WIDTH 900
#define DFPLUS_HEIGHT 680

#define TOOLBAR_HEIGHT 27    // Height of toolbar

// #define FILTER_ENTRY_PANEL_WIDTH    240     // Panel layout
#define FILTER_ENTRY_PANEL_WIDTH 300    // Panel layout
#define FILTER_ENTRY_PANEL_HEIGHT 600

#define GRAPH_PANEL_WIDTH 600    // Total panel size
#define GRAPH_PANEL_HEIGHT 600

#define GRAPH_WIDTH 512     // Graph size
#define GRAPH_HEIGHT 501    // Odd number allows for zero !!

#define FFT_SIZE 1024       // FFT Size
#define LOG2_FFT_SIZE 10    // Log 2 FFT Size

#define ID_GAUSSIAN_RADIOBUTTON_1 1
#define ID_GAUSSIAN_RADIOBUTTON_2 2

#define DEFAULT_M_VALUE 8    // Default m.n for Q number format
#define DEFAULT_N_VALUE 24

enum {
  FIRWIN_GET_FILTER_SPEC = wxID_HIGHEST + 100,

  FIRSIMPLEREMEZ_GET_FILTER_SPEC,
  FIRSIMPLEREMEZ_ESTIMATE_ORDER,
  FIRSIMPLEREMEZ_FILTER_BAND_COMBO_BOX,

  FIRREMEZ_GET_FILTER_SPEC,

  FIRALGS_FILTER_TYPE_CHANGED,
  FIRALGS_GET_FILTER_SPEC,

  IIRPZP_GET_FILTER_SPEC,
  IIRPZP_FILTER_ORDER_SPINCTRL,

  IIRBLT_FILTER_BAND_COMBO_BOX,
  IIRBLT_FILTER_TYPE_COMBO_BOX,
  IIRBLT_GET_FILTER_SPEC,
  IIRBLT_FILTER_ORDER_SPINCTRL,
  IIRBLT_BE1,
  IIRBLT_BE2,
  IIRBLT_BE3,
  IIRBLT_BE4,
  IIRBLT_PBR,
  IIRBLT_SBA,

  IIRMZT_FILTER_BAND_COMBO_BOX,
  IIRMZT_FILTER_TYPE_COMBO_BOX,
  IIRBLT_TRANSLATION_TYPE_COMBO_BOX,
  IIRMZT_GET_FILTER_SPEC,
  IIRMZT_FILTER_ORDER_SPINCTRL,

  IIRALGS_FILTER_TYPE_CHANGED,
  IIRALGS_GET_FILTER_SPEC,

  GRAPH_TYPE_CHANGED,

  DIALOG_GET_FILTER_SPEC,

  STRING_DIALOG_OK,
  STRING_DIALOG_CANCEL,

  GRAPH_OPTIONS_IIR_ALPHA_SIGN_CHECK,
  GRAPH_OPTIONS_AUTO_SCALE_CHECK,
  OPTIONS_DIALOG_OK,
  OPTIONS_DIALOG_CANCEL,

};

typedef struct {
  SLArrayIndex_t Order;      /* Filter order */
  SLArrayIndex_t BandSpec;   /* Filter type (lp, hp, bp, bs) */
  SLArrayIndex_t DesignAlgo; /* Approximation (bw, ch, ic, el) */
  SLData_t Ripple_dB;        /* Ripple in dB (+ve number) */
  SLData_t Attenuation_dB;   /* Attenuation in dB (+ve number) */
  SLData_t OhmegaZero;       /* W0 */
  SLData_t LowerBandEdge;    /* Lower frequency band edge */
  SLData_t UpperBandEdge;    /* Upper frequency band edge */
  SLComplexRect_s* pPoles;   /* Dynamic allocated array of poles */
  SLComplexRect_s* pZeros;   /* Dynamic allocated array of zeros */
} filter_t;

typedef struct {
  SLArrayIndex_t FIRFilterOrder; /* FIR filter order */
  SLArrayIndex_t IIRFilterOrder; /* IIR filter order */
} filter_info_t;

/*
  Class Name : FIRWinFilterDesignWindow
  Description : Child widget that is used to design the filter spec.
  Notes :
*/

class FIRWinFilterDesignWindow : public wxPanel
{
    public:
  FIRWinFilterDesignWindow(wxWindow* parent, wxWindowID id);
  void SetProjectNameString(wxString);
  void SetSampleRate(SLData_t);
  void SetGainArray(SLData_t*);
  void SetPhaseArray(SLData_t*);
  void SetGroupDelayArray(SLData_t*);
  void SetImpulseResponseArray(SLData_t*);
  void SetStepResponseArray(SLData_t*);
  void SetFilterInfoStruct(filter_info_t*);

  void OnGetFilterSpec(wxCommandEvent& event);

    private:
  wxTextCtrl *FilterBand1FLowLine, *FilterBand1FHighLine, *FilterBand1GainLine;
  wxTextCtrl *FilterBand2FLowLine, *FilterBand2FHighLine, *FilterBand2GainLine;
  wxTextCtrl *FilterBand3FLowLine, *FilterBand3FHighLine, *FilterBand3GainLine;
  wxTextCtrl *FilterBand4FLowLine, *FilterBand4FHighLine, *FilterBand4GainLine;
  wxTextCtrl *FilterBand5FLowLine, *FilterBand5FHighLine, *FilterBand5GainLine;

  wxSpinCtrl* PolyPhaseSpinControl;
  wxSpinCtrl* NumCoeffsSpinControl;

  wxComboBox* WindowTypeComboBox;

  SLArrayIndex_t NumberOfFilterCoeffs;
  SLArrayIndex_t NumberOfFilterBands;
  SLArrayIndex_t NumberOfPPFilterBanks;

  wxString ProjectNameString;
  SLData_t SampleRate;

  short WindowType;

  SLData_t pFIRCoeffs[MAX_FIR_LENGTH];

  SLData_t pFIRWLow[MAX_FIR_BANDS],
      pFIRWHigh[MAX_FIR_BANDS];    // Ohmega high and low
  SLData_t pFIRGain[MAX_FIR_BANDS];

  SLData_t* pGain;
  SLData_t* pPhase;
  SLData_t* pGroupDelay;
  SLData_t* pImpulseResponse;
  SLData_t* pStepResponse;

  filter_info_t* pFilterInfo;

  DECLARE_EVENT_TABLE()
};

/*
  Class Name : FIRSimpleRemezFilterDesignWindow
  Description : Child widget that is used to design the Simple Remez exchange
  algorithm filter spec. Notes :
*/

class FIRSimpleRemezFilterDesignWindow : public wxPanel
{
    public:
  FIRSimpleRemezFilterDesignWindow(wxWindow* parent, wxWindowID id);
  void SetProjectNameString(wxString);
  void SetSampleRate(SLData_t);
  void SetGainArray(SLData_t*);
  void SetPhaseArray(SLData_t*);
  void SetGroupDelayArray(SLData_t*);
  void SetImpulseResponseArray(SLData_t*);
  void SetStepResponseArray(SLData_t*);
  void SetPoleZeroPlotArray(SLComplexRect_s*);
  void SetFilterInfoStruct(filter_info_t*);

  void OnGetFilterSpec(wxCommandEvent& event);
  void OnEstimateOrder(wxCommandEvent& event);
  void OnFilterSpecChanged(wxCommandEvent& event);

    private:
  wxComboBox* FilterBandComboBox;
  //  wxComboBox    *DesignMethodComboBox;

  wxTextCtrl *PassBandFc1Line, *PassBandFc2Line;
  wxTextCtrl* TransitionBWLine;
  wxTextCtrl *PassBandRippleLine, *StopBandAttenuationLine;

  wxSpinCtrl* PolyPhaseSpinControl;
  wxSpinCtrl* NumCoeffsSpinControl;

  short RemezFilterBandType;

  SLData_t pFIRCoeffs[MAX_FIR_LENGTH];

  SLArrayIndex_t NumberOfFilterCoeffs;
  SLArrayIndex_t NumberOfPPFilterBanks;

  wxString ProjectNameString;
  SLData_t SampleRate;

  SLData_t* pGain;
  SLData_t* pPhase;
  SLData_t* pGroupDelay;
  SLData_t* pImpulseResponse;
  SLData_t* pStepResponse;
  //  SLComplexRect_s *pPolesAndZeros;

  filter_info_t* pFilterInfo;

  DECLARE_EVENT_TABLE()
};

/*
  Class Name : FIRRemezFilterDesignWindow
  Description : Child widget that is used to design the Remez exchange algorithm
  filter spec. Notes :
*/

class FIRRemezFilterDesignWindow : public wxPanel
{
    public:
  FIRRemezFilterDesignWindow(wxWindow* parent, wxWindowID id);
  void SetProjectNameString(wxString);
  void SetSampleRate(SLData_t);
  void SetGainArray(SLData_t*);
  void SetPhaseArray(SLData_t*);
  void SetGroupDelayArray(SLData_t*);
  void SetImpulseResponseArray(SLData_t*);
  void SetStepResponseArray(SLData_t*);
  void SetPoleZeroPlotArray(SLComplexRect_s*);
  void SetFilterInfoStruct(filter_info_t*);

  //  void    OnFilterSpecChanged (wxCommandEvent & event);
  void OnGetFilterSpec(wxCommandEvent& event);

    private:
  //  wxComboBox  *FilterBandComboBox;
  //  wxComboBox  *DesignMethodComboBox;

  wxTextCtrl *FilterBand1FLowLine, *FilterBand1FHighLine, *FilterBand1GainLine, *FilterBand1RippleWLine;
  wxTextCtrl *FilterBand2FLowLine, *FilterBand2FHighLine, *FilterBand2GainLine, *FilterBand2RippleWLine;
  wxTextCtrl *FilterBand3FLowLine, *FilterBand3FHighLine, *FilterBand3GainLine, *FilterBand3RippleWLine;
  wxTextCtrl *FilterBand4FLowLine, *FilterBand4FHighLine, *FilterBand4GainLine, *FilterBand4RippleWLine;

  wxSpinCtrl* PolyPhaseSpinControl;
  wxSpinCtrl* NumCoeffsSpinControl;

  short RemezFilterBandType;

  SLData_t pFIRCoeffs[MAX_FIR_LENGTH];

  SLArrayIndex_t NumberOfFilterCoeffs;
  SLArrayIndex_t NumberOfFilterBands;
  SLArrayIndex_t NumberOfPPFilterBanks;

  wxString ProjectNameString;
  SLData_t SampleRate;

  SLData_t pFIRWLow[MAX_FIR_BANDS],
      pFIRWHigh[MAX_FIR_BANDS];    // Ohmega high and low
  SLData_t pFIRGain[MAX_FIR_BANDS],
      pFIRRippleW[MAX_FIR_BANDS];    // Gain and ripple weight

  SLData_t* pGain;
  SLData_t* pPhase;
  SLData_t* pGroupDelay;
  SLData_t* pImpulseResponse;
  SLData_t* pStepResponse;
  //  SLComplexRect_s *pPolesAndZeros;

  filter_info_t* pFilterInfo;

  DECLARE_EVENT_TABLE()
};

/*
  Class Name : FIRAlgsFilterDesignWindow
  Description : Child widget that is used to design the filter spec.
  Notes :
*/

class FIRAlgsFilterDesignWindow : public wxPanel
{
    public:
  FIRAlgsFilterDesignWindow(wxWindow* parent, wxWindowID id);
  void SetProjectNameString(wxString);
  void SetSampleRate(SLData_t);
  void SetGainArray(SLData_t*);
  void SetPhaseArray(SLData_t*);
  void SetGroupDelayArray(SLData_t*);
  void SetImpulseResponseArray(SLData_t*);
  void SetStepResponseArray(SLData_t*);
  void SetFilterInfoStruct(filter_info_t*);

  void OnGetFilterSpec(wxCommandEvent& event);
  void OnFilterTypeComboBoxChanged(wxCommandEvent& event);

    private:
  wxComboBox* FilterTypeComboBox;

  wxTextCtrl* RaisedCosineAlphaLine;
  wxTextCtrl* RaisedCosineSymRateLine;

  wxRadioButton* GaussianRadioButton1;
  wxRadioButton* GaussianRadioButton2;
  wxTextCtrl* GaussianAlphaLine;

  wxSpinCtrl* NumCoeffsSpinControl;

  short FilterType;
  SLData_t RaisedCosineAlpha;
  SLData_t RaisedCosineSymRate;
  SLData_t GaussianAlpha;
  SLArrayIndex_t NumberOfFilterCoeffs;

  wxString ProjectNameString;
  SLData_t SampleRate;

  SLData_t pFIRCoeffs[MAX_FIR_LENGTH];

  SLData_t* pGain;
  SLData_t* pPhase;
  SLData_t* pGroupDelay;
  SLData_t* pImpulseResponse;
  SLData_t* pStepResponse;

  filter_info_t* pFilterInfo;

  DECLARE_EVENT_TABLE()
};

/*
  Class Name : IIRPZPFilterDesignWindow
  Description : Child widget that is used to design the IIR pole-zero position
  filter spec. Notes :
*/

class IIRPZPFilterDesignWindow : public wxPanel
{
    public:
  IIRPZPFilterDesignWindow(wxWindow* parent, wxWindowID id);
  void SetProjectNameString(wxString);
  void SetSampleRate(SLData_t);
  void SetGainArray(SLData_t*);
  void SetPhaseArray(SLData_t*);
  void SetGroupDelayArray(SLData_t*);
  void SetImpulseResponseArray(SLData_t*);
  void SetStepResponseArray(SLData_t*);
  void SetPoleZeroPlotArray(SLComplexRect_s*);
  void SetFilterInfoStruct(filter_info_t*);

  void OnGetFilterSpec(wxCommandEvent& event);

    private:
  wxTextCtrl *Pole1Magn, *Pole1Angle, *Zero1Magn, *Zero1Angle;
  wxTextCtrl *Pole2Magn, *Pole2Angle, *Zero2Magn, *Zero2Angle;
  wxTextCtrl *Pole3Magn, *Pole3Angle, *Zero3Magn, *Zero3Angle;
  wxTextCtrl *Pole4Magn, *Pole4Angle, *Zero4Magn, *Zero4Angle;
  wxTextCtrl *Pole5Magn, *Pole5Angle, *Zero5Magn, *Zero5Angle;

  SLData_t pIIRCoeffs[5 * MAX_PZP_BIQUADS];    // 5 * Max Biquads

  SLData_t pDP[MAX_PZP_BIQUADS], pDZ[MAX_PZP_BIQUADS], pAP[MAX_PZP_BIQUADS], pAZ[MAX_PZP_BIQUADS];

  wxString ProjectNameString;
  SLData_t SampleRate;

  SLArrayIndex_t NumberOfBiquads;

  SLData_t* pGain;
  SLData_t* pPhase;
  SLData_t* pGroupDelay;
  SLData_t* pImpulseResponse;
  SLData_t* pStepResponse;
  SLComplexRect_s* pPolesAndZeros;

  filter_info_t* pFilterInfo;

  DECLARE_EVENT_TABLE()
};

/*
  Class Name : IIRTraditionalFilterDesignWindow
  Description : Child widget that is used to design the IIR bilinear transform
  filter spec. Notes :
*/

class IIRTraditionalFilterDesignWindow : public wxPanel
{
    public:
  IIRTraditionalFilterDesignWindow(wxWindow* parent, wxWindowID id);
  void SetProjectNameString(wxString);
  void SetSampleRate(SLData_t);
  void SetGainArray(SLData_t*);
  void SetPhaseArray(SLData_t*);
  void SetGroupDelayArray(SLData_t*);
  void SetImpulseResponseArray(SLData_t*);
  void SetStepResponseArray(SLData_t*);
  void SetPoleZeroPlotArray(SLComplexRect_s*);
  void SetFilterInfoStruct(filter_info_t*);

  void OnFilterSpecChanged(wxCommandEvent& event);
  void OnGetFilterSpec(wxCommandEvent& event);

    private:
  wxComboBox* FilterBandComboBox;
  wxComboBox* DesignMethodComboBox;
  wxComboBox* TranslationMethodComboBox;

  wxTextCtrl *BandEdge1Line, *BandEdge2Line, *BandEdge3Line, *BandEdge4Line;
  wxTextCtrl *PassBandRippleLine, *StopBandAttenLine;

  //  NewSpinCtrl   *FilterOrderSpinBox;
  wxSpinCtrl* FilterOrderSpinBox;

  wxCheckBox* ScaleGainCheckBox;
  wxCheckBox* PreWarpingCheckBox;

  SLData_t pIIRCoeffs[5 * MAX_BLT_ORDER];    // 5 * Max Biquads

  short NumberOfBiquads;

  wxString ProjectNameString;
  SLData_t SampleRate;

  SLData_t* pGain;
  SLData_t* pPhase;
  SLData_t* pGroupDelay;
  SLData_t* pImpulseResponse;
  SLData_t* pStepResponse;
  SLComplexRect_s* pPolesAndZeros;

  filter_info_t* pFilterInfo;

  DECLARE_EVENT_TABLE()
};

/*
  Class Name : IIRAlgsFilterDesignWindow
  Description : Child widget that is used to design the filter spec.
  Notes :
*/

class IIRAlgsFilterDesignWindow : public wxPanel
{
    public:
  IIRAlgsFilterDesignWindow(wxWindow* parent, wxWindowID id);
  void SetProjectNameString(wxString);
  void SetSampleRate(SLData_t);
  void SetGainArray(SLData_t*);
  void SetPhaseArray(SLData_t*);
  void SetGroupDelayArray(SLData_t*);
  void SetImpulseResponseArray(SLData_t*);
  void SetStepResponseArray(SLData_t*);
  void SetPoleZeroPlotArray(SLComplexRect_s*);
  void SetFilterInfoStruct(filter_info_t*);

  void OnGetFilterSpec(wxCommandEvent& event);
  void OnFilterTypeComboBoxChanged(wxCommandEvent& event);

    private:
  wxComboBox* FilterTypeComboBox;

  wxTextCtrl* CentreFrequencyLine;
  wxTextCtrl* PoleMagnitudeLine;

  wxSpinCtrl* FilterOrderSpinBox;
  //  EvenSpinCtrl  *FilterOrderSpinBox;

  wxCheckBox* ScaleGainCheckBox;

  short FilterType;
  SLArrayIndex_t FilterOrder;
  SLData_t CentreFrequency;
  SLData_t PoleMagnitude;

  wxString ProjectNameString;
  SLData_t SampleRate;

  SLData_t pIIRCoeffs[5 * MAX_BLT_ORDER];    // 5 * Max Biquads

  short NumberOfBiquads;

  SLData_t* pGain;
  SLData_t* pPhase;
  SLData_t* pGroupDelay;
  SLData_t* pImpulseResponse;
  SLData_t* pStepResponse;
  SLComplexRect_s* pPolesAndZeros;

  filter_info_t* pFilterInfo;

  DECLARE_EVENT_TABLE()
};

/*
  Class Name : GraphDisplay
  Description : Child widget that is used to display the graph
  Notes :
*/

class GraphDisplay : public wxPanel
{
    public:
  GraphDisplay(wxWindow* parent, wxWindowID id);
  ~GraphDisplay();
  void SetLogMagnGraphScale(SLData_t);
  void SetGainArray(SLData_t*);
  void SetPhaseArray(SLData_t*);
  void SetGroupDelayArray(SLData_t*);
  void SetImpulseResponseArray(SLData_t*);
  void SetStepResponseArray(SLData_t*);
  void SetPoleZeroPlotArray(SLComplexRect_s*);
  void SetSampleRate(SLData_t);
  void SetFilterInfoStruct(filter_info_t*);

  void OnGraphTypeChanged(wxCommandEvent& event);

  void ResetGraphType(void);    // Resets graph to frequency plot

  void ShowGraph(bool show);
  void EnableGraphTypeComboBox(bool enable);

    protected:
  wxComboBox* GraphTypeComboBox;

    private:
  short GraphType;
  wxPaintDC* GraphDC;

  SLData_t SampleRate;

  SLData_t LogMagnGraphScale;

  LineGraph* MagnitudeGraph;
  LineGraph* PhaseGraph;
  LineGraph* GroupDelayGraph;
  LineGraph* ImpulseResponseGraph;
  LineGraph* StepResponseGraph;
  PoleZeroPlot* PoleZeroDiagram;

  SLData_t* pGain;
  SLData_t* pPhase;
  SLData_t* pGroupDelay;
  SLData_t* pImpulseResponse;
  SLData_t* pStepResponse;
  SLComplexRect_s* pPolesAndZeros;

  filter_info_t* pFilterInfo;

  DECLARE_EVENT_TABLE()
};

/*
  Class Name : GetFilterProjectWidget
  Description : Get filter project details
  Notes :
*/

class GetFilterProjectDialog : public wxDialog
{
    public:
  GetFilterProjectDialog(wxFrame* parent, wxWindowID id, const wxString& title);
  //  GetFilterProjectDialog (wxWindow *parent, wxWindowID id, const wxString
  //  &title,
  //       const wxPoint& pos = wxDefaultPosition, const wxSize& size =
  //       wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE);
  wxString GetProjectNameString(void);
  void SetProjectNameString(wxString&);
  short GetFilterType(void);
  void SetFilterType(short);
  SLData_t GetSampleRate(void);
  void SetSampleRate(SLData_t);

    protected:
  wxTextCtrl* ProjectLineEdit;
  wxTextCtrl* SampleRateLineEdit;
  wxComboBox* FilterTypeComboBox;

  void OnGetFilterSpec(wxCommandEvent& event);

    private:
  void OnClose(wxCloseEvent& event);

  wxString ProjectNameString;
  short FilterType;
  SLData_t SampleRate;

  DECLARE_EVENT_TABLE()
};

class GetStringDialog : public wxDialog
{
    public:
  GetStringDialog(wxFrame* parent, wxWindowID id, const wxString& title);
  wxString GetString(void);

    protected:
  wxTextCtrl* StringLineEdit;

  void OnOk(wxCommandEvent& event);
  void OnCancel(wxCommandEvent& event);

    private:
  wxString String;

  DECLARE_EVENT_TABLE()
};

class GetOptionsDialog : public wxDialog
{
    public:
  GetOptionsDialog(wxFrame* parent, wxWindowID id, const wxString& title);
  short GetNegateIIRAlphaSign(void);
  void SetNegateIIRAlphaSign(short);
  short GetM(void);
  void SetM(short);
  short GetN(void);
  void SetN(short);
  SLData_t GetScale(void);
  void SetScale(SLData_t);

    protected:
  wxSpinCtrl* mSpinControl;
  wxSpinCtrl* nSpinControl;
  wxTextCtrl* ScaleLineEdit;

  void OnAutoGraphScale(wxCommandEvent& event);
  void OnOk(wxCommandEvent& event);
  void OnCancel(wxCommandEvent& event);

    private:
  //  wxString  String;
  short mValue;
  short nValue;
  wxCheckBox* NegateIIRAlphaSignCheckBox;
  wxCheckBox* ScaleGainCheckBox;
  SLData_t Scale;

  DECLARE_EVENT_TABLE()
};

class DisplayTextDialog : public wxDialog
{
    public:
  DisplayTextDialog(wxFrame* parent, wxWindowID id, const wxString& title, const wxPoint& pos = wxDefaultPosition,
                    const wxSize& size = wxDefaultSize);

  void SetText(wxString& string);
  void OnSize(wxSizeEvent& event);

    private:
  wxTextCtrl* TextBox;

  DECLARE_EVENT_TABLE()
};

#endif    // End of #ifndef _DFPLUS_H
