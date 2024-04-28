// System analyser processing functions
// Copyright (c) 1999-2020, Sigma Numerix Ltd, All rights reserved

#define DEBUG_IO      1                                             // Set to '1' to enable debug file logging, '0' to disable (Only available if using SIGLIB DSP Library
  
// Select desired audio interface - searches for these sub-strings
  
// USB Audio Devices
// #define DESIRED_AUDIO_INPUT_INTERFACE       "Microsoft Sound Mapper - Input"
// #define DESIRED_AUDIO_OUTPUT_INTERFACE      "Microsoft Sound Mapper - Output"
// #define DESIRED_AUDIO_INPUT_INTERFACE       "Line In (BEHRINGER USB WDM"
// #define DESIRED_AUDIO_OUTPUT_INTERFACE      "Speakers (BEHRINGER USB WDM"
#define DESIRED_AUDIO_INPUT_INTERFACE       "Microphone (USB Audio CODEC )"
#define DESIRED_AUDIO_OUTPUT_INTERFACE      "Speakers (USB Audio CODEC )"
// #define DESIRED_AUDIO_INPUT_INTERFACE       "xCORE USB Audio 2.0"
// #define DESIRED_AUDIO_OUTPUT_INTERFACE      "xCORE USB Audio 2.0"
// #define DESIRED_AUDIO_INPUT_INTERFACE       "XMOS XS1-L1"                        // "XMOS XS1-L1"
// #define DESIRED_AUDIO_OUTPUT_INTERFACE      "XMOS XS1-L1"
  
// #define DESIRED_AUDIO_INPUT_INTERFACE       "TUSBAudio ASIO Driver"              // "Thesycon USB BAudio ASIO Driver"
// #define DESIRED_AUDIO_OUTPUT_INTERFACE      "TUSBAudio ASIO Driver"              // "Thesycon USB BAudio ASIO Driver"
// #define DESIRED_AUDIO_INPUT_INTERFACE       "ASIO4ALL v2"                        // UCA202 - ASIO Driver
// #define DESIRED_AUDIO_OUTPUT_INTERFACE      "ASIO4ALL v2"                        // UCA202 - ASIO Driver
  
// Host API
// #define DESIRED_AUDIO_API                   "Windows DirectSound"
#define DESIRED_AUDIO_API                   "MME"
// #define DESIRED_AUDIO_API                   "Windows WASAPI"
// #define DESIRED_AUDIO_API                   "ASIO"
  
// Include files
  
#ifdef __GNUG__
#pragma implementation "saproc.cpp"
#pragma interface "saproc.cpp"
#endif  /*  */
  
// For compilers that support precompilation, includes "wx/wx.h".#include "wx/wxprec.h"
  
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
  
#include "mainframe.h"                                              // mainframe header - must be located after other project headers
#include "wx/spinctrl.h"
#if __APPLE__
#include <malloc/malloc.h>
#else /*  */
#include <malloc.h>
#endif  /*  */
#include <math.h>
#include <stdlib.h>
#include "sa.h"
#include "portaudio.h"
#include "pa_asio.h"
  
#ifndef SIGLIB
#undef DEBUG_IO
#endif  /*  */
  
// Declare Portaudio interface structures
typedef struct IOConfig_s  {
  int            isInputInterleaved;
                  int isOutputInterleaved;
                  int numInputChannels;
                  int numOutputChannels;
                  int framesPerCallback;
               } IOConfig_s;
  typedef struct                                                 // PA I/O data structure
{
  
// Input data
  short          *pCh0InputSamples;                                 // Channel 0 input sample array pointer
  short          *pCh1InputSamples;                                 // Channel 0 input sample array pointer
  long            InputArrayIndex;                                  // Index into input array
                 
// Output data
  short          *pCh0OutputSamples;                                // Channel 0 output sample array pointer
  short          *pCh1OutputSamples;                                // Channel 0 output sample array pointer
  long            OutputArrayIndex;                                 // Index into output array
  long            TotalOutputSamples;                               // End pointer for output sample array
} paIOData;
 
// Define constants
//#define INITIAL_CAL_THRESHOLD       200                   // Initial chirp calibration threshold value
#define INITIAL_CAL_THRESHOLD       770                             // Initial chirp calibration threshold value
#define SAMPLE_RATE                 44100.0                         // Sample rate
#define FRAMES_PER_BUFFER           64L                             // Number of frames per buffer - a frame is one sample per channel
#define PA_SAMPLE_LENGTH            2048L                           // Length of the sample sequence - longer than I/O length to support delays
#define PA_SAMPLING_SAMPLE_LENGTH   (SAMPLE_LENGTH+100L)            // Length of sampling for sample only
#define PREAMBLE_LENGTH             2000
#define POSTAMBLE_LENGTH            20000
  
// Declare functions
long            SampleAudio (
  double *pInputData,
  long DataLength);
long           SampleAndGenerateAudio (
  double *pInputData,
  double *pOutputData,
  long DataLength);
long           SampleAndGenerateAudioWithReference (
  double *pInputData,
  double *pOutputData,
  long DataLength,
  long CalThreshold);
long           CalibrateAudio (
  double *pInputData,
  double *pOutputData,
  long DataLength,
  long CalThreshold);
void           error_terminate (
  PaError err);
  
// Declare global variables
  paIOData IOData;
long           IODelay;
 extern GraphDisplay *GlobalGraphDisplayWindow;
 

/*
Name : SAProcWindow::SAProcWindow
Description : Child widget that is used to get the FIR filter spec.
Notes :
*/ 
SAProcWindow::SAProcWindow (wxWindow * parent, wxWindowID id) :wxPanel (parent, id)  
{
  
#if DEBUG_IO
    SUF_ClearDebugfprintf ();
  SUF_Debugfprintf ("------------   SAProcWindow::SAProcWindow - Initializing   ------------\n");
  
#endif  /*  */
// Top-level layout - a vertical box to contain all the controls
    wxStaticBoxSizer * VSizer = new wxStaticBoxSizer (wxVERTICAL, this, "System Analyzer Controls");
   wxBoxSizer * HSizer_AnalysisType = new wxBoxSizer (wxHORIZONTAL);
  wxBoxSizer * HSizer_TDTitleP = new wxBoxSizer (wxHORIZONTAL);
  wxBoxSizer * HSizer_TDTitle = new wxBoxSizer (wxHORIZONTAL);
  wxBoxSizer * HSizer_TDDataSetSizeP = new wxBoxSizer (wxHORIZONTAL);
  wxBoxSizer * HSizer_TDDataSetSize = new wxBoxSizer (wxHORIZONTAL);
  wxBoxSizer * HSizer_DCRemoveP = new wxBoxSizer (wxHORIZONTAL);
  wxBoxSizer * HSizer_DCRemove = new wxBoxSizer (wxHORIZONTAL);
  wxBoxSizer * HSizer_TimeTrigEnableP = new wxBoxSizer (wxHORIZONTAL);
  wxBoxSizer * HSizer_TimeTrigEnable = new wxBoxSizer (wxHORIZONTAL);
  wxBoxSizer * HSizer_TimeTrigDir = new wxBoxSizer (wxHORIZONTAL);
  wxBoxSizer * HSizer_TimeTrigLevelP = new wxBoxSizer (wxHORIZONTAL);
  wxBoxSizer * HSizer_TimeTrigLevel = new wxBoxSizer (wxHORIZONTAL);
  wxBoxSizer * HSizer_FDTitlePP = new wxBoxSizer (wxHORIZONTAL);
  wxBoxSizer * HSizer_FDTitleP = new wxBoxSizer (wxHORIZONTAL);
  wxBoxSizer * HSizer_FDTitle = new wxBoxSizer (wxHORIZONTAL);
  wxBoxSizer * HSizer_FFTOnePoleAlphaP = new wxBoxSizer (wxHORIZONTAL);
  wxBoxSizer * HSizer_FFTOnePoleAlpha = new wxBoxSizer (wxHORIZONTAL);
  wxBoxSizer * HSizer_FFTPeakHoldAlphaP = new wxBoxSizer (wxHORIZONTAL);
  wxBoxSizer * HSizer_FFTPeakHoldAlpha = new wxBoxSizer (wxHORIZONTAL);
  wxBoxSizer * HSizer_CalcButtonP = new wxBoxSizer (wxHORIZONTAL);
  wxBoxSizer * HSizer_CalcButton = new wxBoxSizer (wxHORIZONTAL);
  wxBoxSizer * HSizer_CalLevelP = new wxBoxSizer (wxHORIZONTAL);
  wxBoxSizer     *HSizer_CalLevelPP = new wxBoxSizer (wxHORIZONTAL);
  wxBoxSizer * HSizer_CalLevel = new wxBoxSizer (wxHORIZONTAL);
  wxBoxSizer * HSizer_CalButtonP = new wxBoxSizer (wxHORIZONTAL);
  wxBoxSizer * HSizer_CalButton = new wxBoxSizer (wxHORIZONTAL);
   VSizer->Add (HSizer_AnalysisType, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add (HSizer_TDTitleP, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add (HSizer_TDTitle, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add (HSizer_TDDataSetSizeP, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add (HSizer_TDDataSetSize, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add (HSizer_DCRemoveP, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add (HSizer_DCRemove, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add (HSizer_TimeTrigEnableP, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add (HSizer_TimeTrigEnable, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add (HSizer_TimeTrigDir, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add (HSizer_TimeTrigLevelP, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add (HSizer_TimeTrigLevel, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add (HSizer_FDTitlePP, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add (HSizer_FDTitleP, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add (HSizer_FDTitle, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add (HSizer_FFTOnePoleAlphaP, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add (HSizer_FFTOnePoleAlpha, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add (HSizer_FFTPeakHoldAlphaP, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add (HSizer_FFTPeakHoldAlpha, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add (HSizer_CalcButtonP, 0, wxALIGN_CENTRE | wxALL, 2);
  VSizer->Add (HSizer_CalcButton, 0, wxALIGN_CENTRE | wxALL, 2);
  VSizer->Add (HSizer_CalLevelP, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add (HSizer_CalLevelPP, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add (HSizer_CalLevel, 0, wxALIGN_LEFT | wxALL, 2);
  VSizer->Add (HSizer_CalButtonP, 0, wxALIGN_CENTRE | wxALL, 2);
  VSizer->Add (HSizer_CalButton, 0, wxALIGN_CENTRE | wxALL, 2);
   
// A combo box for chosing the analysis type
    HSizer_AnalysisType->Add (new wxStaticText (this, -1, "Analysis Type : "), 0, wxALIGN_TOP | wxALL, 0);
  static const wxString FilterChoices[] =
    { "Time - Triggered", "Time - Continuous", "Frequency - Triggered", "Frequency - Continuous", "Chirp - Time", "Chirp - Frequency",
"Chirp - X correlation", "Chirp (+Ref) - Time", "Chirp (+Ref) - Frequency", "Chirp (+Ref) - X correlation", 
  };
  ProcessingTypeComboBox =
    new wxComboBox (this, ID_EVENT_TYPE_CHANGED, wxEmptyString, wxDefaultPosition, wxDefaultSize, WXSIZEOF (FilterChoices), FilterChoices,
                    wxCB_READONLY);
  ProcessingTypeComboBox->SetSelection (0);                        // Set the default selection
  HSizer_AnalysisType->Add (ProcessingTypeComboBox, 0, wxALIGN_TOP | wxALL, 0);
   
// Insert a blank space line
    HSizer_TDTitleP->Add (new wxStaticText (this, -1, ""), 0, wxALIGN_TOP | wxALL, 0);
   
// Insert the time domain title
    HSizer_TDTitle->Add (new wxStaticText (this, -1, "Time Domain Options :"), 0, wxALIGN_TOP | wxALL, 0);
   
// Insert spin control for data set size - for graph
    HSizer_TDDataSetSizeP->Add (new wxStaticText (this, -1, ""), 0, wxALIGN_TOP | wxALL, 0);
  HSizer_TDDataSetSize->Add (new wxStaticText (this, -1, "Graph Zoom Width : "), 0, wxALIGN_TOP | wxALL, 0);
  DataSetSizeSpinCtrl =
    new wxSpinCtrl (this, ID_EVENT_TD_DATASET_SIZE_SPIN_CHANGED, _T (""), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 10,
                    MAXIMUM_GRAPH_WIDTH, 0);
  DataSetSizeSpinCtrl->SetValue (512);
  DataSetSizeSpinCtrl->Enable (TRUE);
  HSizer_TDDataSetSize->Add (DataSetSizeSpinCtrl, 0, wxALIGN_TOP | wxALL, 0);
   
// Insert a CheckBox to enable the D.C. Offset removal
// Insert a blank space line
    HSizer_DCRemoveP->Add (new wxStaticText (this, -1, ""), 0, wxALIGN_TOP | wxALL, 0);
  DCRemoveCheckBox = new wxCheckBox (this, -1, _T (" &Remove D.C. Level"));
  DCRemoveCheckBox->SetValue (FALSE);
  
#if wxUSE_TOOLTIPS
    DCRemoveCheckBox->SetToolTip (_T ("Click here to remove the D.C. level"));
  
#endif                                                              // wxUSE_TOOLTIPS
    HSizer_DCRemove->Add (DCRemoveCheckBox);
   
// Insert a CheckBox to enable the time domain triggering
// Insert a blank space line
    HSizer_TimeTrigEnableP->Add (new wxStaticText (this, -1, ""), 0, wxALIGN_TOP | wxALL, 0);
  TimeTriggerEnableCheckBox = new wxCheckBox (this, -1, _T (" &Enable Trigger           Triggered : "));
  TimeTriggerEnableCheckBox->SetValue (TRUE);
  
#if wxUSE_TOOLTIPS
    TimeTriggerEnableCheckBox->SetToolTip (_T ("Click here to enable the time domain sample trigger"));
  
#endif                                                              // wxUSE_TOOLTIPS
    HSizer_TimeTrigEnable->Add (TimeTriggerEnableCheckBox);
  HSizer_TimeTrigEnable->Add (TriggerLED = new LED (this, -1, GREEN_LED), 0, wxALIGN_TOP | wxALL, 0);  //, 5);
  TriggerLED->On (FALSE);
   
// Insert radio buttons for trigger direction selection
    HSizer_TimeTrigDir->Add (new wxStaticText (this, -1, "Time domain trigger : "), 0, wxALIGN_TOP | wxALL, 0);
  TimeTriggerPositiveRadioButton =
    new wxRadioButton (this, ID_EVENT_TIME_TRIG_POS_RADIOBUTTON, _T ("&Positive"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
  TimeTriggerNegativeRadioButton = new wxRadioButton (this, ID_EVENT_TIME_TRIG_NEG_RADIOBUTTON, _T ("&Negative"), wxDefaultPosition, wxDefaultSize);
  TimeTriggerPositiveRadioButton->SetValue (TRUE);
  TimeTriggerPositiveRadioButton->Enable (TRUE);
  TimeTriggerNegativeRadioButton->Enable (TRUE);
  HSizer_TimeTrigDir->Add (TimeTriggerPositiveRadioButton, 0, wxALIGN_TOP | wxALL, 0);
  HSizer_TimeTrigDir->Add (TimeTriggerNegativeRadioButton, 0, wxALIGN_TOP | wxALL, 0);
   
// Insert a blank space line
    HSizer_TimeTrigLevelP->Add (new wxStaticText (this, -1, ""), 0, wxALIGN_TOP | wxALL, 0);
   
// Insert spin control for trigger level
    HSizer_TimeTrigLevel->Add (new wxStaticText (this, -1, "Time domain trigger level : "), 0, wxALIGN_TOP | wxALL, 0);
  TimeTriggerLevelSpinCtrl =
    new wxSpinCtrl (this, ID_EVENT_TRIG_LEVEL_SPIN_CHANGED, _T (""), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 32, 0);
  TimeTriggerLevelSpinCtrl->SetValue (1);
  TimeTriggerLevelSpinCtrl->Enable (TRUE);
  HSizer_TimeTrigLevel->Add (TimeTriggerLevelSpinCtrl, 0, wxALIGN_TOP | wxALL, 0);
   
// Insert a blank space line
    HSizer_FDTitlePP->Add (new wxStaticText (this, -1, ""), 0, wxALIGN_TOP | wxALL, 0);
  HSizer_FDTitleP->Add (new wxStaticText (this, -1, ""), 0, wxALIGN_TOP | wxALL, 0);
   
// Insert the frequency domain title
    HSizer_FDTitle->Add (new wxStaticText (this, -1, "Frequency Domain Options :"), 0, wxALIGN_TOP | wxALL, 0);
   
// Insert spin control for one pole alpha value
// Insert a blank space line
    HSizer_FFTOnePoleAlphaP->Add (new wxStaticText (this, -1, ""), 0, wxALIGN_TOP | wxALL, 0);
  HSizer_FFTOnePoleAlpha->Add (new wxStaticText (this, -1, "FFT One Pole Alpha Value / 100 : "), 0, wxALIGN_TOP | wxALL, 0);
  FFTOnePoleSpinCtrl =
    new wxSpinCtrl (this, ID_EVENT_ONE_POLE_SPIN_CHANGED, _T (""), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 99, 0);
  FFTOnePoleSpinCtrl->SetValue (0);
  FFTOnePoleSpinCtrl->Enable (FALSE);
  HSizer_FFTOnePoleAlpha->Add (FFTOnePoleSpinCtrl, 0, wxALIGN_TOP | wxALL, 0);
  
#if SIGLIB
    pOnePoleDelay = SUF_VectorArrayAllocate (MAXIMUM_GRAPH_WIDTH);
  if (pOnePoleDelay == NULL) {
    wxMessageBox ("Memory allocation failure", "SAProcWindow::SAProcWindow", wxOK | wxICON_EXCLAMATION, NULL);
    return;
  }
  SDA_Clear (pOnePoleDelay, MAXIMUM_GRAPH_WIDTH);
  
#endif  /*  */
    
// Insert spin control for peak hold alpha value
// Insert a blank space line
    HSizer_FFTPeakHoldAlphaP->Add (new wxStaticText (this, -1, ""), 0, wxALIGN_TOP | wxALL, 0);
  HSizer_FFTPeakHoldAlpha->Add (new wxStaticText (this, -1, "FFT Peak Hold Alpha Value / 100 : "), 0, wxALIGN_TOP | wxALL, 0);
  FFTPeakHoldSpinCtrl =
    new wxSpinCtrl (this, ID_EVENT_PEAK_HOLD_SPIN_CHANGED, _T (""), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 99, 0);
  FFTPeakHoldSpinCtrl->SetValue (0);
  FFTPeakHoldSpinCtrl->Enable (FALSE);
  HSizer_FFTPeakHoldAlpha->Add (FFTPeakHoldSpinCtrl, 0, wxALIGN_TOP | wxALL, 0);
  
#if SIGLIB
    pPeakHoldDelay = SUF_VectorArrayAllocate (MAXIMUM_GRAPH_WIDTH);
  if (pPeakHoldDelay == NULL) {
    wxMessageBox ("Memory allocation failure", "SAProcWindow::SAProcWindow", wxOK | wxICON_EXCLAMATION, NULL);
    return;
  }
  SDA_Clear (pPeakHoldDelay, MAXIMUM_GRAPH_WIDTH);
  
#endif  /*  */
    
// Insert a blank space line
    HSizer_CalcButtonP->Add (new wxStaticText (this, -1, ""), 0, wxALIGN_TOP | wxALL, 0);
   
// This button samples the data and displays the graph
    wxButton * CalculateButton = new wxButton (this, ID_EVENT_PROCESS, "Calculate");
  HSizer_CalcButton->Add (CalculateButton, 0, wxALIGN_TOP | wxALL, 0);
  CalculateButton->SetDefault ();                                  // Set the calculate button as the default
  
// Insert a blank space line
    HSizer_CalLevelP->Add (new wxStaticText (this, -1, ""), 0, wxALIGN_TOP | wxALL, 0);
   HSizer_CalLevel->Add (new wxStaticText (this, -1, "Calibration threshold : "), 0, wxALIGN_TOP | wxALL, 0);
  CalTholdSpinCtrl = new wxSpinCtrl (this, -1, _T (""), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10000, 0);
  CalTholdSpinCtrl->SetValue (INITIAL_CAL_THRESHOLD);
  HSizer_CalLevel->Add (CalTholdSpinCtrl, 0, wxALIGN_TOP | wxALL, 0);
   
// This button calibrates the I/O
    wxButton * CalibrateButton = new wxButton (this, ID_EVENT_CALIBRATE, "Calibrate");
  HSizer_CalButton->Add (CalibrateButton, 0, wxALIGN_TOP | wxALL, 0);
   SetAutoLayout (TRUE);                                          // Configure the layout for the controls
  SetSizer (VSizer);
   VSizer->SetSizeHints (this);
  VSizer->Fit (this);
    
#if SIGLIB
    pFDPRealData = SUF_VectorArrayAllocate (FFT_SIZE);              // Allocate data processing arrays
  pFDPImagData = SUF_VectorArrayAllocate (FFT_SIZE);
  pFDPWindow = SUF_VectorArrayAllocate (FFT_SIZE);
  pFDPFFTCoeffs = SUF_FftCoefficientAllocate (FFT_SIZE);
   if ((pFDPRealData == NULL) || (pFDPImagData == NULL) || (pFDPWindow == NULL) || (pFDPFFTCoeffs == NULL)) {
    wxMessageBox ("Memory allocation failure", "SAProcWindow::SAProcWindow", wxOK | wxICON_EXCLAMATION, NULL);
    return;
  }
   SIF_Window (pFDPWindow, SIGLIB_HANNING, SIGLIB_ZERO, FFT_SIZE);  // Generate Hanning window table
  SIF_Fft (pFDPFFTCoeffs, SIGLIB_NULL_FIX_DATA_PTR, FFT_SIZE);      // Initialise FFT
#endif  /*  */
  
// Initialize data acquisition
    IOData.pCh0InputSamples = (short *) malloc ((1024 + PREAMBLE_LENGTH + PA_SAMPLE_LENGTH + POSTAMBLE_LENGTH) * sizeof (short));
  IOData.pCh0OutputSamples = (short *) malloc ((1024 + PREAMBLE_LENGTH + PA_SAMPLE_LENGTH + POSTAMBLE_LENGTH) * sizeof (short));
  IOData.pCh1InputSamples = (short *) malloc ((1024 + PREAMBLE_LENGTH + PA_SAMPLE_LENGTH + POSTAMBLE_LENGTH) * sizeof (short));
  IOData.pCh1OutputSamples = (short *) malloc ((1024 + PREAMBLE_LENGTH + PA_SAMPLE_LENGTH + POSTAMBLE_LENGTH) * sizeof (short));
  if ((IOData.pCh0InputSamples == NULL) || (IOData.pCh0OutputSamples == NULL) || 
       (IOData.pCh1InputSamples == NULL) || (IOData.pCh1OutputSamples == NULL)) {
    wxMessageBox ("Memory allocation failure", "PortAudio::SAProcWindow", wxOK | wxICON_EXCLAMATION, NULL);
    return;
  }
   
#if DEBUG_IO
    SUF_Debugfprintf ("SAProcWindow::SAProcWindow - Memory allocated\n");
  
#endif  /*  */
}

  SAProcWindow::~SAProcWindow ()  
{
  
#if DEBUG_IO
    SUF_Debugfprintf ("SAProcWindow::~SAProcWindow\n");
  
#endif  /*  */
    
#if SIGLIB
    SUF_MemoryFree (pFDPRealData);                                  // Free data processing arrays
  SUF_MemoryFree (pFDPImagData);
  SUF_MemoryFree (pFDPWindow);
  SUF_MemoryFree (pFDPFFTCoeffs);
  SUF_MemoryFree (pOnePoleDelay);
  
#endif  /*  */
    free (IOData.pCh0InputSamples);                                // Free data acquisition memory
  free (IOData.pCh0OutputSamples);
}

 

/*
Name : SAProcWindow::OnDataSetSizeSpinControlChange
Description : OnDataSetSizeSpinControlChange
Notes :
*/ 
void SAProcWindow::OnDataSetSizeSpinControlChange (
  wxSpinEvent & WXUNUSED (event))  
{
  
//  wxMessageBox ("Got here", "SAProcWindow::OnDataSetSizeSpinControlChange", wxOK | wxICON_EXCLAMATION, NULL);
    if (DataSetSizeSpinCtrl->GetValue () > MAXIMUM_GRAPH_WIDTH)
    DataSetSizeSpinCtrl->SetValue (MAXIMUM_GRAPH_WIDTH);
   
  else if (DataSetSizeSpinCtrl->GetValue () < MINIMUM_GRAPH_WIDTH)
    DataSetSizeSpinCtrl->SetValue (MINIMUM_GRAPH_WIDTH);
  
  else
    DataSetSizeSpinCtrl->SetValue (DataSetSizeSpinCtrl->GetValue ());
   
//  if (RunningFlag == SIGLIB_TRUE)
//  {
//      GlobalGraphDisplayWindow->SetDataSetSize ((int)DataSetSizeSpinCtrl->GetValue ());
//  }
}

  

/*
Name : SAProcWindow::OnTriggerLevelSpinControlChange
Description : OnTriggerLevelSpinControlChange
Notes :
*/ 
void SAProcWindow::OnTriggerLevelSpinControlChange (
  wxSpinEvent & WXUNUSED (event))  
{
  
//  wxMessageBox ("Got here", "SAProcWindow::OnTriggerLevelSpinControlChange", wxOK | wxICON_EXCLAMATION, NULL);
    if (TimeTriggerLevelSpinCtrl->GetValue () > 32)
    TimeTriggerLevelSpinCtrl->SetValue (32);
  
  else
    TimeTriggerLevelSpinCtrl->SetValue (TimeTriggerLevelSpinCtrl->GetValue ());
}

  

/*
Name : SAProcWindow::OnPeakHoldSpinControlChange
Description : OnPeakHoldSpinControlChange
Notes :
*/ 
void SAProcWindow::OnOnePoleFilterSpinControlChange (
  wxSpinEvent & WXUNUSED (event))  
{
  
//  wxMessageBox ("Got here", "SAProcWindow::OnOnePoleFilterSpinControlChange", wxOK | wxICON_EXCLAMATION, NULL);
    if (FFTOnePoleSpinCtrl->GetValue () > 99)
    FFTOnePoleSpinCtrl->SetValue (99);
  
  else
    FFTOnePoleSpinCtrl->SetValue (FFTOnePoleSpinCtrl->GetValue ());
}

  

/*
Name : SAProcWindow::OnPeakHoldSpinControlChange
Description : OnPeakHoldSpinControlChange
Notes :
*/ 
void SAProcWindow::OnPeakHoldSpinControlChange (
  wxSpinEvent & WXUNUSED (event))  
{
  
//  wxMessageBox ("Got here", "SAProcWindow::OnPeakHoldSpinControlChange", wxOK | wxICON_EXCLAMATION, NULL);
    if (FFTPeakHoldSpinCtrl->GetValue () > 99)
    FFTPeakHoldSpinCtrl->SetValue (99);
  
  else
    FFTPeakHoldSpinCtrl->SetValue (FFTPeakHoldSpinCtrl->GetValue ());
}

  

/*
Name : SAProcWindow::OnTypeChanged
Description : Called if processing type changed
Notes :
*/ 
void SAProcWindow::OnTypeChanged (
  wxCommandEvent & WXUNUSED (event))  
{
   if ((ProcessingTypeComboBox->GetSelection () == ID_PROC_TIME) || (ProcessingTypeComboBox->GetSelection () == ID_PROC_TIME_CONT)) {
    DataSetSizeSpinCtrl->Enable (TRUE);
    TimeTriggerEnableCheckBox->Enable (TRUE);
    TimeTriggerLevelSpinCtrl->Enable (TRUE);
    TimeTriggerPositiveRadioButton->Enable (TRUE);
    TimeTriggerNegativeRadioButton->Enable (TRUE);
     FFTOnePoleSpinCtrl->Enable (FALSE);
    FFTPeakHoldSpinCtrl->Enable (FALSE);
  }
   
  else if ((ProcessingTypeComboBox->GetSelection () == ID_PROC_FREQ) || (ProcessingTypeComboBox->GetSelection () == ID_PROC_FREQ_CONT)) {
    DataSetSizeSpinCtrl->Enable (FALSE);
    TimeTriggerEnableCheckBox->Enable (FALSE);
    TimeTriggerLevelSpinCtrl->Enable (FALSE);
    TimeTriggerPositiveRadioButton->Enable (FALSE);
    TimeTriggerNegativeRadioButton->Enable (FALSE);
     FFTOnePoleSpinCtrl->Enable (TRUE);
    FFTPeakHoldSpinCtrl->Enable (TRUE);
  }
   
  else {                                                            // ID_PROC_CHIRP_TIME, ID_PROC_CHIRP_FREQ, ID_PROC_CHIRP_XCORR ID_PROC_CHIRP_REF_TIME, ID_PROC_CHIRP_REF_FREQ, ID_PROC_CHIRP_REF_XCORR
    DataSetSizeSpinCtrl->Enable (TRUE);
    TimeTriggerEnableCheckBox->Enable (FALSE);
    TimeTriggerLevelSpinCtrl->Enable (FALSE);
    TimeTriggerPositiveRadioButton->Enable (FALSE);
    TimeTriggerNegativeRadioButton->Enable (FALSE);
     FFTOnePoleSpinCtrl->Enable (FALSE);
    FFTPeakHoldSpinCtrl->Enable (FALSE);
  }
}

  

/*
Name : SAProcWindow::OnProcessData
Description : Process the data
Notes :
*/ 
void SAProcWindow::OnProcessData (
  wxCommandEvent & WXUNUSED (event))  
{
  
//  wxMessageBox ("Got Here", "SAProcWindow::OnProcessData", wxOK | wxICON_EXCLAMATION, NULL);
    
// Process data here :
// ID_PROC_TIME & ID_PROC_TIME_CONT,
// ID_PROC_FREQ & ID_PROC_FREQ_CONT,
// ID_PROC_CHIRP_TIME,
// ID_PROC_CHIRP_FREQ,
// ID_PROC_CHIRP_XCORR,
// ID_PROC_CHIRP_REF_TIME,
// ID_PROC_CHIRP_REF_FREQ,
// ID_PROC_CHIRP_REF_XCORR,
    if ((ProcessingTypeComboBox->GetSelection () == ID_PROC_TIME) || (ProcessingTypeComboBox->GetSelection () == ID_PROC_TIME_CONT)) {
    SampleAudio (pFDPRealData, SAMPLE_LENGTH);                     // Sample analog input data
    
#if SIGLIB
      TriggerLocation = ((SLArrayIndex_t) 0);                       // Set initial trigger location
    if (TimeTriggerEnableCheckBox->GetValue () == TRUE) {
      if (TimeTriggerPositiveRadioButton->GetValue () == TRUE) {   // Positive slope
        ZXPrevSign = SIGLIB_POSITIVE;                               // Only processing individual arrays
        TriggerLocation =
          SDA_Trigger (pFDPRealData, &ZXPrevSign, 1000.0 * (SLData_t) TimeTriggerLevelSpinCtrl->GetValue (), SIGLIB_POSITIVE_LEVEL_CROSS,
                       SAMPLE_LENGTH);
      }
      
      else {                                                        // Negative slope
        ZXPrevSign = SIGLIB_NEGATIVE;                               // Only processing individual arrays
        TriggerLocation =
          SDA_Trigger (pFDPRealData, &ZXPrevSign, 1000.0 * (SLData_t) TimeTriggerLevelSpinCtrl->GetValue (), SIGLIB_NEGATIVE_LEVEL_CROSS,
                       SAMPLE_LENGTH);
      }
      if (TriggerLocation > (SAMPLE_LENGTH - MAXIMUM_GRAPH_WIDTH)) { // If trigger location too late then set to beginning
        TriggerLocation = ((SLArrayIndex_t) 0);
        TriggerLED->On (FALSE);
      }
       
      else {
        TriggerLED->On (TRUE);
      }
     }
    
    else {
      TriggerLED->On (FALSE);
    }
     SDA_Copy (pFDPRealData + TriggerLocation, pData, MAXIMUM_GRAPH_WIDTH); // Copy from start to display array
    if (DCRemoveCheckBox->GetValue () == TRUE) {                   // Remove D.C. term
      SDA_SubtractMean (pData, pData, SIGLIB_ONE / (SLData_t) MAXIMUM_GRAPH_WIDTH, MAXIMUM_GRAPH_WIDTH);
    }
    
#else /*  */
    {                                                               // When SigLib is not being used, copy the input data so that it can be displayed
      int i;
      for (i = 0; i < MAXIMUM_GRAPH_WIDTH; i++)
        pData[i] = pFDPRealData[i];
    }
    
#endif  /*  */
  }
   
  else if ((ProcessingTypeComboBox->GetSelection () == ID_PROC_FREQ) || (ProcessingTypeComboBox->GetSelection () == ID_PROC_FREQ_CONT))
     {
    
#if SIGLIB
      SampleAudio (pFDPRealData, SAMPLE_LENGTH);                    // Sample analog input data
    if (DCRemoveCheckBox->GetValue () == TRUE) {
      SDA_SubtractMean (pFDPRealData, pFDPRealData, SIGLIB_ONE / (SLData_t) SAMPLE_LENGTH, SAMPLE_LENGTH);
    }
     SDA_Window (pFDPRealData, pFDPRealData, pFDPWindow, FFT_SIZE); // Apply window to real data
    SDA_Rfft (pFDPRealData, pFDPImagData, pFDPFFTCoeffs, SIGLIB_NULL_FIX_DATA_PTR, FFT_SIZE, LOG2_FFT_SIZE);  // Perform FFT
    SDA_LogMagnitude (pFDPRealData, pFDPImagData, pData, MAXIMUM_GRAPH_WIDTH);  // Calc real power fm complex
    SLData_t OnePoleFilterAlpha = FFTOnePoleSpinCtrl->GetValue () / SIGLIB_ONE_HUNDRED;
    SDA_OnePolePerSample (pData, pData, pOnePoleDelay, OnePoleFilterAlpha, MAXIMUM_GRAPH_WIDTH);
     SLData_t PeakHoldAlpha = FFTPeakHoldSpinCtrl->GetValue () / SIGLIB_ONE_HUNDRED;
    SDA_PeakHoldPerSample (pData, pPeakHoldDelay, PeakHoldAlpha, MAXIMUM_GRAPH_WIDTH);
    SDA_Copy (pPeakHoldDelay, pData, MAXIMUM_GRAPH_WIDTH);
     SDA_Offset (pData, -138.0, pData, MAXIMUM_GRAPH_WIDTH);      // Offset graph results
#else /*  */
      wxMessageBox ("This functionality is only available when SigLib is installed\nFor more details, please see http://www.numerix-dsp.com",
                    "System Analyzer", wxOK | wxICON_EXCLAMATION, NULL);
    
#endif  /*  */
    }
   
  else if (ProcessingTypeComboBox->GetSelection () == ID_PROC_CHIRP_TIME)
     {
    
#if SIGLIB
    double         *pChirpData;
    pChirpData = SUF_VectorArrayAllocate (SAMPLE_LENGTH);
     if (pChirpData == NULL) {
      wxMessageBox ("Memory allocation failure", "SAProcWindow::OnGetProcessDetails", wxOK | wxICON_EXCLAMATION, NULL);
      return;
    }
     
// Generate chirp of SAMPLE_LENGTH from 0.001 to 0.499 - normalised to 1Hz
      SDA_Clear (pChirpData, SAMPLE_LENGTH);
    double ChirpPhase = SIGLIB_ZERO;
    double ChirpValue = SIGLIB_ZERO;
    SDA_SignalGenerate (pChirpData, SIGLIB_CHIRP_LIN, 32767.0, SIGLIB_FILL, 0.001, 0.0, 
                         (0.498 / ((double) CHIRP_LENGTH)), 0.5, &ChirpPhase, &ChirpValue, CHIRP_LENGTH);
     SampleAndGenerateAudio (pFDPRealData, pChirpData, SAMPLE_LENGTH);  // Sample and generate analog data
    if (DCRemoveCheckBox->GetValue () == TRUE) {
      SDA_SubtractMean (pFDPRealData, pFDPRealData, SIGLIB_ONE / (SLData_t) SAMPLE_LENGTH, SAMPLE_LENGTH);
    }
    SDA_Copy (pFDPRealData, pData, MAXIMUM_GRAPH_WIDTH);           // Copy input data to display buffer
    SUF_MemoryFree (pChirpData);                                   // Free memory
#else /*  */
      wxMessageBox ("This functionality is only available when SigLib is installed\nFor more details, please see http://www.numerix-dsp.com",
                    "System Analyzer", wxOK | wxICON_EXCLAMATION, NULL);
    
#endif  /*  */
    }
   
  else if (ProcessingTypeComboBox->GetSelection () == ID_PROC_CHIRP_FREQ)
     {
    
#if SIGLIB
    double         *pChirpData;
    pChirpData = SUF_VectorArrayAllocate (SAMPLE_LENGTH);
    if (pChirpData == NULL) {
      wxMessageBox ("Memory allocation failure", "SAProcWindow::OnGetProcessDetails", wxOK | wxICON_EXCLAMATION, NULL);
      return;
    }
     
// Generate chirp of SAMPLE_LENGTH from 0.001 to 0.499 - normalised to 1Hz
      SDA_Clear (pChirpData, SAMPLE_LENGTH);
    double ChirpPhase = SIGLIB_ZERO;
    double ChirpValue = SIGLIB_ZERO;
    SDA_SignalGenerate (pChirpData, SIGLIB_CHIRP_LIN, 32767.0, SIGLIB_FILL, 0.001, 0.0, 
                         (0.498 / ((double) CHIRP_LENGTH)), 0.5, &ChirpPhase, &ChirpValue, CHIRP_LENGTH);
     SampleAndGenerateAudio (pFDPRealData, pChirpData, SAMPLE_LENGTH);  // Sample and generate analog data
    if (DCRemoveCheckBox->GetValue () == TRUE) {
      SDA_SubtractMean (pFDPRealData, pFDPRealData, SIGLIB_ONE / (SLData_t) SAMPLE_LENGTH, SAMPLE_LENGTH);
    }
    SDA_Divide (pFDPRealData, 32767.0, pFDPRealData, SAMPLE_LENGTH); // Re-scale results
    SDA_Rfft (pFDPRealData, pFDPImagData, pFDPFFTCoeffs, SIGLIB_NULL_FIX_DATA_PTR, FFT_SIZE, LOG2_FFT_SIZE); // Perform FFT
    SDA_LogMagnitude (pFDPRealData, pFDPImagData, pData, MAXIMUM_GRAPH_WIDTH);  // Calc real power fm complex
    SDA_Offset (pData, -28.0, pData, MAXIMUM_GRAPH_WIDTH);          // Offset graph results
    SUF_MemoryFree (pChirpData);                                   // Free memory
#else /*  */
      wxMessageBox ("This functionality is only available when SigLib is installed\nFor more details, please see http://www.numerix-dsp.com",
                    "System Analyzer", wxOK | wxICON_EXCLAMATION, NULL);
    
#endif  /*  */
    }
   
  else if (ProcessingTypeComboBox->GetSelection () == ID_PROC_CHIRP_XCORR)
     {
    
#if SIGLIB
    double         *pFDPRealData, *pFDPImagData, *pFDPChirpRealData, *pFDPChirpImagData;
    double        *pFDPFFTCoeffs;
     pFDPRealData = SUF_VectorArrayAllocate (XCORR_FFT_SIZE);
    pFDPImagData = SUF_VectorArrayAllocate (XCORR_FFT_SIZE);
    pFDPChirpRealData = SUF_VectorArrayAllocate (XCORR_FFT_SIZE);
    pFDPChirpImagData = SUF_VectorArrayAllocate (XCORR_FFT_SIZE);
    pFDPFFTCoeffs = SUF_FftCoefficientAllocate (XCORR_FFT_SIZE);
     if ((pFDPRealData == NULL) || (pFDPImagData == NULL) || 
           (pFDPChirpRealData == NULL) || (pFDPChirpImagData == NULL) || (pFDPFFTCoeffs == NULL)) {
      wxMessageBox ("Memory allocation failure", "SAProcWindow::OnGetProcessDetails", wxOK | wxICON_EXCLAMATION, NULL);
      return;
    }
     SDA_Clear (pFDPRealData, XCORR_FFT_SIZE);
    SDA_Clear (pFDPImagData, XCORR_FFT_SIZE);
    SDA_Clear (pFDPChirpRealData, XCORR_FFT_SIZE);
    SDA_Clear (pFDPChirpImagData, XCORR_FFT_SIZE);
     SIF_Fft (pFDPFFTCoeffs, SIGLIB_NULL_FIX_DATA_PTR, XCORR_FFT_SIZE); // Initialise FFT
    
// Generate chirp of SAMPLE_LENGTH from 0.001 to 0.499 - normalised to 1Hz
    double ChirpPhase = SIGLIB_ZERO;
    double ChirpValue = SIGLIB_ZERO;
    SDA_SignalGenerate (pFDPChirpRealData, SIGLIB_CHIRP_LIN, 32767.0, SIGLIB_FILL, 0.001, 0.0, 
                         (0.498 / ((double) CHIRP_LENGTH)), 0.5, &ChirpPhase, &ChirpValue, CHIRP_LENGTH);
     SampleAndGenerateAudio (pFDPRealData, pFDPChirpRealData, SAMPLE_LENGTH); // Sample and generate analog data
    if (DCRemoveCheckBox->GetValue () == TRUE) {
      SDA_SubtractMean (pFDPRealData, pFDPRealData, SIGLIB_ONE / (SLData_t) SAMPLE_LENGTH, SAMPLE_LENGTH);
    }
    SDA_Divide (pFDPRealData, 32767.0, pFDPRealData, SAMPLE_LENGTH); // Re-scale results
    SDA_Divide (pFDPChirpRealData, 32767.0, pFDPChirpRealData, SAMPLE_LENGTH);  // Re-scale results
     SDA_Rfft (pFDPChirpRealData, pFDPChirpImagData, pFDPFFTCoeffs, SIGLIB_NULL_FIX_DATA_PTR, XCORR_FFT_SIZE, LOG2_XCORR_FFT_SIZE); // Perform FFT
    SDA_Negate (pFDPChirpImagData, pFDPChirpImagData, XCORR_FFT_SIZE);  // Conjugate the data
    SDA_Rfft (pFDPRealData, pFDPImagData, pFDPFFTCoeffs, SIGLIB_NULL_FIX_DATA_PTR, XCORR_FFT_SIZE, LOG2_XCORR_FFT_SIZE); // Perform FFT
    SDA_ComplexMultiply2 (pFDPRealData, pFDPImagData, pFDPChirpRealData, pFDPChirpImagData, pFDPRealData, pFDPImagData, XCORR_FFT_SIZE);
    SDA_Cifft (pFDPRealData, pFDPImagData, pFDPFFTCoeffs, SIGLIB_NULL_FIX_DATA_PTR, XCORR_FFT_SIZE, LOG2_XCORR_FFT_SIZE);  // Perform inverse FFT
    SDA_Magnitude (pFDPRealData, pFDPImagData, pData, MAXIMUM_GRAPH_WIDTH);  // Calc real power fm complex
    SUF_MemoryFree (pFDPRealData);                                 // Free memory
    SUF_MemoryFree (pFDPImagData);
    SUF_MemoryFree (pFDPChirpRealData);
    SUF_MemoryFree (pFDPChirpImagData);
    SUF_MemoryFree (pFDPFFTCoeffs);
    
#else /*  */
      wxMessageBox ("This functionality is only available when SigLib is installed\nFor more details, please see http://www.numerix-dsp.com",
                    "System Analyzer", wxOK | wxICON_EXCLAMATION, NULL);
    
#endif  /*  */
    }
   
  else if (ProcessingTypeComboBox->GetSelection () == ID_PROC_CHIRP_REF_TIME)
     {
    
#if SIGLIB
    double         *pChirpData;
    pChirpData = SUF_VectorArrayAllocate (SAMPLE_LENGTH);
     if (pChirpData == NULL) {
      wxMessageBox ("Memory allocation failure", "SAProcWindow::OnGetProcessDetails", wxOK | wxICON_EXCLAMATION, NULL);
      return;
    }
     
// Generate chirp of SAMPLE_LENGTH from 0.001 to 0.499 - normalised to 1Hz
      SDA_Clear (pChirpData, SAMPLE_LENGTH);
    double ChirpPhase = SIGLIB_ZERO;
    double ChirpValue = SIGLIB_ZERO;
    SDA_SignalGenerate (pChirpData, SIGLIB_CHIRP_LIN, 32767.0, SIGLIB_FILL, 0.001, 0.0, 
                         (0.498 / ((double) CHIRP_LENGTH)), 0.5, &ChirpPhase, &ChirpValue, CHIRP_LENGTH);
     SampleAndGenerateAudioWithReference (pFDPRealData, pChirpData, SAMPLE_LENGTH, CalTholdSpinCtrl->GetValue ());  // Sample and generate analog data
    if (DCRemoveCheckBox->GetValue () == TRUE) {
      SDA_SubtractMean (pFDPRealData, pFDPRealData, SIGLIB_ONE / (SLData_t) SAMPLE_LENGTH, SAMPLE_LENGTH);
    }
    SDA_Copy (pFDPRealData, pData, MAXIMUM_GRAPH_WIDTH);           // Copy input data to display buffer
    SUF_MemoryFree (pChirpData);                                   // Free memory
#else /*  */
      wxMessageBox ("This functionality is only available when SigLib is installed\nFor more details, please see http://www.numerix-dsp.com",
                    "System Analyzer", wxOK | wxICON_EXCLAMATION, NULL);
    
#endif  /*  */
    }
   
  else if (ProcessingTypeComboBox->GetSelection () == ID_PROC_CHIRP_REF_FREQ)
     {
    
#if SIGLIB
    double         *pChirpData;
    pChirpData = SUF_VectorArrayAllocate (SAMPLE_LENGTH);
    if (pChirpData == NULL) {
      wxMessageBox ("Memory allocation failure", "SAProcWindow::OnGetProcessDetails", wxOK | wxICON_EXCLAMATION, NULL);
      return;
    }
     
// Generate chirp of SAMPLE_LENGTH from 0.001 to 0.499 - normalised to 1Hz
      SDA_Clear (pChirpData, SAMPLE_LENGTH);
    double ChirpPhase = SIGLIB_ZERO;
    double ChirpValue = SIGLIB_ZERO;
    SDA_SignalGenerate (pChirpData, SIGLIB_CHIRP_LIN, 32767.0, SIGLIB_FILL, 0.001, 0.0, 
                         (0.498 / ((double) CHIRP_LENGTH)), 0.5, &ChirpPhase, &ChirpValue, CHIRP_LENGTH);
     SampleAndGenerateAudioWithReference (pFDPRealData, pChirpData, SAMPLE_LENGTH, CalTholdSpinCtrl->GetValue ());  // Sample and generate analog data
    if (DCRemoveCheckBox->GetValue () == TRUE) {
      SDA_SubtractMean (pFDPRealData, pFDPRealData, SIGLIB_ONE / (SLData_t) SAMPLE_LENGTH, SAMPLE_LENGTH);
    }
    SDA_Divide (pFDPRealData, 32767.0, pFDPRealData, SAMPLE_LENGTH); // Re-scale results
    SDA_Rfft (pFDPRealData, pFDPImagData, pFDPFFTCoeffs, SIGLIB_NULL_FIX_DATA_PTR, FFT_SIZE, LOG2_FFT_SIZE); // Perform FFT
    SDA_LogMagnitude (pFDPRealData, pFDPImagData, pData, MAXIMUM_GRAPH_WIDTH);  // Calc real power fm complex
    SDA_Offset (pData, -28.0, pData, MAXIMUM_GRAPH_WIDTH);          // Offset graph results
    SUF_MemoryFree (pChirpData);                                   // Free memory
#else /*  */
      wxMessageBox ("This functionality is only available when SigLib is installed\nFor more details, please see http://www.numerix-dsp.com",
                    "System Analyzer", wxOK | wxICON_EXCLAMATION, NULL);
    
#endif  /*  */
    }
   
  else if (ProcessingTypeComboBox->GetSelection () == ID_PROC_CHIRP_REF_XCORR)
     {
    
#if SIGLIB
    double         *pFDPRealData, *pFDPImagData, *pFDPChirpRealData, *pFDPChirpImagData;
    double        *pFDPFFTCoeffs;
     pFDPRealData = SUF_VectorArrayAllocate (XCORR_FFT_SIZE);
    pFDPImagData = SUF_VectorArrayAllocate (XCORR_FFT_SIZE);
    pFDPChirpRealData = SUF_VectorArrayAllocate (XCORR_FFT_SIZE);
    pFDPChirpImagData = SUF_VectorArrayAllocate (XCORR_FFT_SIZE);
    pFDPFFTCoeffs = SUF_FftCoefficientAllocate (XCORR_FFT_SIZE);
     if ((pFDPRealData == NULL) || (pFDPImagData == NULL) || 
           (pFDPChirpRealData == NULL) || (pFDPChirpImagData == NULL) || (pFDPFFTCoeffs == NULL)) {
      wxMessageBox ("Memory allocation failure", "SAProcWindow::OnGetProcessDetails", wxOK | wxICON_EXCLAMATION, NULL);
      return;
    }
     SDA_Clear (pFDPRealData, XCORR_FFT_SIZE);
    SDA_Clear (pFDPImagData, XCORR_FFT_SIZE);
    SDA_Clear (pFDPChirpRealData, XCORR_FFT_SIZE);
    SDA_Clear (pFDPChirpImagData, XCORR_FFT_SIZE);
     SIF_Fft (pFDPFFTCoeffs, SIGLIB_NULL_FIX_DATA_PTR, XCORR_FFT_SIZE); // Initialise FFT
    
// Generate chirp of SAMPLE_LENGTH from 0.001 to 0.499 - normalised to 1Hz
    double ChirpPhase = SIGLIB_ZERO;
    double ChirpValue = SIGLIB_ZERO;
    SDA_SignalGenerate (pFDPChirpRealData, SIGLIB_CHIRP_LIN, 32767.0, SIGLIB_FILL, 0.001, 0.0, 
                         (0.498 / ((double) CHIRP_LENGTH)), 0.5, &ChirpPhase, &ChirpValue, CHIRP_LENGTH);
     SampleAndGenerateAudioWithReference (pFDPRealData, pFDPChirpRealData, SAMPLE_LENGTH, CalTholdSpinCtrl->GetValue ()); // Sample and generate analog data
    if (DCRemoveCheckBox->GetValue () == TRUE) {
      SDA_SubtractMean (pFDPRealData, pFDPRealData, SIGLIB_ONE / (SLData_t) SAMPLE_LENGTH, SAMPLE_LENGTH);
    }
    SDA_Divide (pFDPRealData, 32767.0, pFDPRealData, SAMPLE_LENGTH); // Re-scale results
    SDA_Divide (pFDPChirpRealData, 32767.0, pFDPChirpRealData, SAMPLE_LENGTH);  // Re-scale results
     SDA_Rfft (pFDPChirpRealData, pFDPChirpImagData, pFDPFFTCoeffs, SIGLIB_NULL_FIX_DATA_PTR, XCORR_FFT_SIZE, LOG2_XCORR_FFT_SIZE); // Perform FFT
    SDA_Negate (pFDPChirpImagData, pFDPChirpImagData, XCORR_FFT_SIZE);  // Conjugate the data
    SDA_Rfft (pFDPRealData, pFDPImagData, pFDPFFTCoeffs, SIGLIB_NULL_FIX_DATA_PTR, XCORR_FFT_SIZE, LOG2_XCORR_FFT_SIZE); // Perform FFT
    SDA_ComplexMultiply2 (pFDPRealData, pFDPImagData, pFDPChirpRealData, pFDPChirpImagData, pFDPRealData, pFDPImagData, XCORR_FFT_SIZE);
    SDA_Cifft (pFDPRealData, pFDPImagData, pFDPFFTCoeffs, SIGLIB_NULL_FIX_DATA_PTR, XCORR_FFT_SIZE, LOG2_XCORR_FFT_SIZE);  // Perform inverse FFT
    SDA_Magnitude (pFDPRealData, pFDPImagData, pData, MAXIMUM_GRAPH_WIDTH);  // Calc real power fm complex
    SUF_MemoryFree (pFDPRealData);                                 // Free memory
    SUF_MemoryFree (pFDPImagData);
    SUF_MemoryFree (pFDPChirpRealData);
    SUF_MemoryFree (pFDPChirpImagData);
    SUF_MemoryFree (pFDPFFTCoeffs);
    
#else /*  */
      wxMessageBox ("This functionality is only available when SigLib is installed\nFor more details, please see http://www.numerix-dsp.com",
                    "System Analyzer", wxOK | wxICON_EXCLAMATION, NULL);
    
#endif  /*  */
    }
   
// Now we have processed the data we need to
// configure the graph
    if ((ProcessingTypeComboBox->GetSelection () == ID_PROC_TIME) || 
        (ProcessingTypeComboBox->GetSelection () == ID_PROC_TIME_CONT) || 
        (ProcessingTypeComboBox->GetSelection () == ID_PROC_CHIRP_TIME) || (ProcessingTypeComboBox->GetSelection () == ID_PROC_CHIRP_REF_TIME)) {
    GlobalGraphDisplayWindow->SetScaling (32767.0);                // Set graph scaling
    GlobalGraphDisplayWindow->SetGraphType (ID_TIME_DOMAIN_GRAPH);  // Set the graph type
    GlobalGraphDisplayWindow->SetDataSetSize ((int) DataSetSizeSpinCtrl->GetValue ());
  }
   
  else if ((ProcessingTypeComboBox->GetSelection () == ID_PROC_CHIRP_XCORR) || (ProcessingTypeComboBox->GetSelection () == ID_PROC_CHIRP_REF_XCORR)) {
    GlobalGraphDisplayWindow->SetScaling (0.0);                    // Set graph scaling
    GlobalGraphDisplayWindow->SetGraphType (ID_TIME_DOMAIN_GRAPH);  // Set the graph type
    GlobalGraphDisplayWindow->SetDataSetSize ((int) DataSetSizeSpinCtrl->GetValue ());
  }
   
  else {                                                            // Frequency domain graph
    GlobalGraphDisplayWindow->SetScaling (100.0);                   // Set graph scaling
    GlobalGraphDisplayWindow->SetGraphType (ID_FREQ_DOMAIN_GRAPH);  // Set the graph type
    GlobalGraphDisplayWindow->SetDataSetSize (512);
  }
   wxCommandEvent * DummyCommand = new wxCommandEvent ();         // Post event to say that data has been updated
  GlobalGraphDisplayWindow->OnGraphTypeChanged (*DummyCommand);
  delete (DummyCommand);
 }                                                                // End of SAProcWindow::OnProcessData ()

 

/*
Name : SAProcWindow::OnCalibrateIO
Description : Calibrate the I/O delay
Notes :
*/ 
void SAProcWindow::OnCalibrateIO (
  wxCommandEvent & WXUNUSED (event))  
{
  
#if SIGLIB
  double         *pChirpData;
  pChirpData = SUF_VectorArrayAllocate (SAMPLE_LENGTH);
   if (pChirpData == NULL) {
    wxMessageBox ("Memory allocation failure", "SAProcWindow::OnGetProcessDetails", wxOK | wxICON_EXCLAMATION, NULL);
    return;
  }
   
// Generate chirp of SAMPLE_LENGTH from 0.001 to 0.499 - normalised to 1Hz
    SDA_Clear (pChirpData, SAMPLE_LENGTH);
  double ChirpPhase = SIGLIB_ZERO;
  double ChirpValue = SIGLIB_ZERO;
  SDA_SignalGenerate (pChirpData, SIGLIB_CHIRP_LIN, 32767.0, SIGLIB_FILL, 0.001, 0.0, 
                       (0.498 / ((double) CHIRP_LENGTH)), 0.5, &ChirpPhase, &ChirpValue, CHIRP_LENGTH);
   CalibrateAudio (pFDPRealData, pChirpData, SAMPLE_LENGTH, CalTholdSpinCtrl->GetValue ()); // Sample and generate analog data
  SUF_MemoryFree (pChirpData);                                     // Free memory
#else /*  */
    wxMessageBox ("This functionality is only available when SigLib is installed\nFor more details, please see http://www.numerix-dsp.com",
                  "System Analyzer", wxOK | wxICON_EXCLAMATION, NULL);
  
#endif  /*  */
}                                                                  // End of SAProcWindow::OnCalibrate I/O ()

 void SAProcWindow::SetSampleRate (
  double sampleRate)  { SampleRate = sampleRate;
}   void SAProcWindow::SetDataArray (
  double *pDataArray)  { pData = pDataArray;
}  void SAProcWindow::OnIdle (

  wxIdleEvent & WXUNUSED (event))  
{
  
//  wxMessageBox ("Got Here", "SAProcWindow::OnIdle", wxOK | wxICON_EXCLAMATION, NULL);
    if ((ProcessingTypeComboBox->GetSelection () == ID_PROC_TIME_CONT) || (ProcessingTypeComboBox->GetSelection () == ID_PROC_FREQ_CONT)) {
    wxCommandEvent * DummyCommand = new wxCommandEvent ();         // Post event to process data
    OnProcessData (*DummyCommand);
    delete (DummyCommand);
  }
}

  BEGIN_EVENT_TABLE (SAProcWindow, wxPanel) 
EVT_SPINCTRL (ID_EVENT_TD_DATASET_SIZE_SPIN_CHANGED, SAProcWindow::OnDataSetSizeSpinControlChange) 
EVT_BUTTON (ID_EVENT_PROCESS, SAProcWindow::OnProcessData) 
EVT_BUTTON (ID_EVENT_CALIBRATE, SAProcWindow::OnCalibrateIO) 
EVT_COMBOBOX (ID_EVENT_TYPE_CHANGED, SAProcWindow::OnTypeChanged) 
EVT_SPINCTRL (ID_EVENT_TRIG_LEVEL_SPIN_CHANGED, SAProcWindow::OnTriggerLevelSpinControlChange) 
EVT_SPINCTRL (ID_EVENT_ONE_POLE_SPIN_CHANGED, SAProcWindow::OnOnePoleFilterSpinControlChange) 
EVT_SPINCTRL (ID_EVENT_PEAK_HOLD_SPIN_CHANGED, SAProcWindow::OnPeakHoldSpinControlChange) 
EVT_IDLE (SAProcWindow::OnIdle)  END_EVENT_TABLE ()    
// Portaudio I/O driver interface functions
// This routine will be called by the PortAudio engine when audio is needed.
// It may be called at interrupt level on some machines so don't do anything
// that could mess up the system like calling malloc() or free().
static int PlayRecMonoCallback (
  const void *inputBuffer,
  void *outputBuffer,
  unsigned long framesPerBuffer,                                   // Number of frames (samples) to write to or read from driver
  const PaStreamCallbackTimeInfo * timeInfo,
  PaStreamCallbackFlags statusFlags,
  void *userData)  { paIOData * IOData = (paIOData *) userData;
  short         *pCh0Input = IOData->pCh0InputSamples + IOData->InputArrayIndex;
  short         *pCh0Output = IOData->pCh0OutputSamples + IOData->OutputArrayIndex;
  long i;
  short FinishedFlag = 0;
  long OutputSamplesLeft = IOData->TotalOutputSamples - IOData->OutputArrayIndex;
  short Dummy;
   const PaStreamCallbackTimeInfo *DummytimeInfo = timeInfo;      // Dummy locals just to prevent compiler warnings
  PaStreamCallbackFlags DummystatusFlags = statusFlags;
   short       *pInputBuffer = (short *) inputBuffer;
  short         *pOutputBuffer = (short *) outputBuffer;
   if (OutputSamplesLeft < ((long) framesPerBuffer)) {            // Number of samples to write out is less than driver buffer size
    for (i = 0; i < OutputSamplesLeft; i++) {                       // Process input and output data
      *pCh0Input++ = *pInputBuffer++;                               // Input channel 0
      *pOutputBuffer++ = *pCh0Output++;                             // Output channel 0
    }
     for (i = OutputSamplesLeft; i < ((long) framesPerBuffer); i++) { // Output zeros
      Dummy = *pInputBuffer++;                                      // Input channel 0
      *pOutputBuffer++ = 0;                                         // Output channel 0
    }  FinishedFlag = 1;                                          // Set flag to say that we have finished
  }
   
  else {                                                            // Process one buffer full of samples
    for (i = 0; i < ((long) framesPerBuffer); i++) {                // Process input and output data
      *pCh0Input++ = *pInputBuffer++;                               // Input channel 0
      *pOutputBuffer++ = *pCh0Output++;                             // Output channel 0
  } }  IOData->InputArrayIndex += ((long) framesPerBuffer);      // Update array index
  IOData->OutputArrayIndex += ((long) framesPerBuffer);
   return (FinishedFlag);
}

  static int PlayRecStereoCallback (
  const void *inputBuffer,
  void *outputBuffer,
  unsigned long framesPerBuffer,                                   // Number of frames (samples) to write to or read from driver
  const PaStreamCallbackTimeInfo * timeInfo,
  PaStreamCallbackFlags statusFlags,
  void *userData)  { paIOData * IOData = (paIOData *) userData;
  short         *pCh0Input = IOData->pCh0InputSamples + IOData->InputArrayIndex;
  short         *pCh0Output = IOData->pCh0OutputSamples + IOData->OutputArrayIndex;
  short         *pCh1Input = IOData->pCh1InputSamples + IOData->InputArrayIndex;
  short         *pCh1Output = IOData->pCh1OutputSamples + IOData->OutputArrayIndex;
  long i;
  short FinishedFlag = 0;
  long OutputSamplesLeft = IOData->TotalOutputSamples - IOData->OutputArrayIndex;
  short Dummy;
   const PaStreamCallbackTimeInfo *DummytimeInfo = timeInfo;      // Dummy locals just to prevent compiler warnings
  PaStreamCallbackFlags DummystatusFlags = statusFlags;
   short       *pInputBuffer = (short *) inputBuffer;
  short         *pOutputBuffer = (short *) outputBuffer;
   if (OutputSamplesLeft < ((long) framesPerBuffer)) {            // Number of samples to write out is less than driver buffer size
    for (i = 0; i < OutputSamplesLeft; i++) {                       // Process input and output data
      *pCh0Input++ = *pInputBuffer++;                               // Input channel 0
      *pCh1Input++ = *pInputBuffer++;                               // Input channel 1
      *pOutputBuffer++ = *pCh0Output++;                             // Output channel 0
      *pOutputBuffer++ = *pCh1Output++;                             // Output channel 1
    }
     for (i = OutputSamplesLeft; i < ((long) framesPerBuffer); i++) { // Output zeros
      Dummy = *pInputBuffer++;                                      // Input channel 0
      *pOutputBuffer++ = 0;                                         // Output channel 0
      *pOutputBuffer++ = 0;                                         // Output channel 1
    }  FinishedFlag = 1;                                          // Set flag to say that we have finished
  }
   
  else {                                                            // Process one buffer full of samples
    for (i = 0; i < ((long) framesPerBuffer); i++) {                // Process input and output data
      *pCh0Input++ = *pInputBuffer++;                               // Input channel 0
      *pCh1Input++ = *pInputBuffer++;                               // Input channel 1
      *pOutputBuffer++ = *pCh0Output++;                             // Output channel 0
      *pOutputBuffer++ = *pCh1Output++;                             // Output channel 1
  } }  IOData->InputArrayIndex += ((long) framesPerBuffer);      // Update array index
  IOData->OutputArrayIndex += ((long) framesPerBuffer);
   return (FinishedFlag);
}

  

/*******************************************************************/ 
long SampleAudio (
  double *pInputData,
  long DataLength)  
{
  PaStreamParameters inputParameters, outputParameters;
  PaStream * stream;
  PaError err;
  long i;
  int numDevices;
  const PaDeviceInfo *deviceInfo;
  int InputDeviceNo = -1;
  int OutputDeviceNo = -1;
   
#if DEBUG_IO
    SUF_Debugfprintf ("--  SampleAudio  --\n");
  
#endif  /*  */
    IOData.InputArrayIndex = IOData.OutputArrayIndex = 0;          // Initialise array indices
  IOData.TotalOutputSamples = PA_SAMPLING_SAMPLE_LENGTH;            // Amount of data to sample
  for (i = 0; i < PA_SAMPLING_SAMPLE_LENGTH; i++) {
    IOData.pCh0InputSamples[i] = 0;                                // Clear input data arrays
    IOData.pCh0OutputSamples[i] = 0;                                // Output silence
  }
   
#if DEBUG_IO
    SUF_Debugfprintf ("SampleAudio - Arrays initialised\n");
  
#endif  /*  */
    err = Pa_Initialize ();
  if (err != paNoError) {
    error_terminate (err);
    return (1);
  }
   
#if DEBUG_IO
    SUF_Debugfprintf ("SampleAudio - PA initialised\n");
  
#endif  /*  */
    numDevices = Pa_GetDeviceCount ();
  if (numDevices < 0) {
    err = numDevices;
    error_terminate (err);
    return (1);
  }
   for (i = 0; i < numDevices; i++) {
    deviceInfo = Pa_GetDeviceInfo (i);
     
#if DEBUG_IO
      SUF_Debugfprintf ("SampleAudio - Device %d = %s, inputs = %d, outputs = %d, Host API = %s\n", (int) i, deviceInfo->name,
                        (int) deviceInfo->maxInputChannels, (int) deviceInfo->maxOutputChannels, Pa_GetHostApiInfo (deviceInfo->hostApi)->name);
    
#endif  /*  */
      if ((strstr (deviceInfo->name, DESIRED_AUDIO_INPUT_INTERFACE) != 0) && 
           (strcmp (Pa_GetHostApiInfo (deviceInfo->hostApi)->name, DESIRED_AUDIO_API) == 0) && (deviceInfo->maxInputChannels != 0)) {
       InputDeviceNo = i;
    }
    if ((strstr (deviceInfo->name, DESIRED_AUDIO_OUTPUT_INTERFACE) != 0) && 
         (strcmp (Pa_GetHostApiInfo (deviceInfo->hostApi)->name, DESIRED_AUDIO_API) == 0) && (deviceInfo->maxOutputChannels != 0)) {
       OutputDeviceNo = i;
    }
  }
   
#if DEBUG_IO
    SUF_Debugfprintf ("SampleAudio - Desired audio input interface = %s\n", DESIRED_AUDIO_INPUT_INTERFACE);
  SUF_Debugfprintf ("SampleAudio - Desired audio output interface = %s\n", DESIRED_AUDIO_OUTPUT_INTERFACE);
  SUF_Debugfprintf ("SampleAudio - Selected input device = %d\n", InputDeviceNo);
  SUF_Debugfprintf ("SampleAudio - Selected output device = %d\n", OutputDeviceNo);
  SUF_Debugfprintf ("SampleAudio - Selected host API = %s\n", DESIRED_AUDIO_API);
  SUF_Debugfprintf ("SampleAudio - PA initialised\n");
  
#endif  /*  */
    if (InputDeviceNo == -1) {                                     // If desired interface not detected, use default device
    inputParameters.device = Pa_GetDefaultInputDevice ();           // Default input device
  }
  
  else {
    inputParameters.device = InputDeviceNo;                        // Select input device
  }
  inputParameters.channelCount = 1;                                // Mono input
  inputParameters.sampleFormat = paInt16;
  inputParameters.suggestedLatency = Pa_GetDeviceInfo (inputParameters.device)->defaultLowInputLatency;
  inputParameters.hostApiSpecificStreamInfo = NULL;
   if (OutputDeviceNo == -1) {                                    // If desired interface not detected, use default device
    outputParameters.device = Pa_GetDefaultOutputDevice ();         // Default output device
  }
  
  else {
    outputParameters.device = OutputDeviceNo;                      // Select output device
  }
  outputParameters.channelCount = 1;                               // Mono output
  outputParameters.sampleFormat = paInt16;
  outputParameters.suggestedLatency = Pa_GetDeviceInfo (outputParameters.device)->defaultLowOutputLatency;
  outputParameters.hostApiSpecificStreamInfo = NULL;
   
#if DEBUG_IO
    SUF_Debugfprintf ("SampleAudio - Printing device details to status line\n");
  
#endif  /*  */
  {                                                                 // Print the selected device to the status line
    deviceInfo = Pa_GetDeviceInfo (inputParameters.device);
    wxString devString = "I/P = ";
    devString += deviceInfo->name;
    deviceInfo = Pa_GetDeviceInfo (outputParameters.device);
    devString += ", O/P = ";
    devString += deviceInfo->name;
    mainFrame * frame = (mainFrame *) (wxTheApp->GetTopWindow ());
    frame->DisplayStatusLineText2 (devString);
    
#if DEBUG_IO
      SUF_Debugfprintf ("SampleAudio - I/O Devices : %s : %s\n", devString.mb_str ().data (), Pa_GetHostApiInfo (deviceInfo->hostApi)->name);
    
#endif  /*  */
  }
   
#if DEBUG_IO
    SUF_Debugfprintf ("SampleAudio - PA params set\n");
  
#endif  /*  */
    err = Pa_OpenStream (&stream, &inputParameters, &outputParameters, SAMPLE_RATE, FRAMES_PER_BUFFER, paClipOff,  // We won't output out of range samples so don't bother clipping them
                           PlayRecMonoCallback, &IOData);
  
//              (void *)&IOData );
    if (err != paNoError) {
    
#if DEBUG_IO
      SUF_Debugfprintf ("SampleAudio - Pa_OpenStream() error_terminate : %d\n", err);
    SUF_Debugfprintf ("SampleAudio - Pa_OpenStream() error : %s\n", Pa_GetErrorText (err));
    
#endif  /*  */
      error_terminate (err);
    return (1);
  }
   
#if DEBUG_IO
    SUF_Debugfprintf ("SampleAudio - Stream opened\n");
  
#endif  /*  */
    err = Pa_StartStream (stream);
  if (err != paNoError) {
    
#if DEBUG_IO
      SUF_Debugfprintf ("SampleAudio - Pa_StartStream() error_terminate : %d\n", err);
    SUF_Debugfprintf ("SampleAudio - Pa_StartStream() error : %s\n", Pa_GetErrorText (err));
    
#endif  /*  */
      error_terminate (err);
    return (1);
  }
  
#if DEBUG_IO
    SUF_Debugfprintf ("SampleAudio - Stream started\n");
  
#endif  /*  */
    while (Pa_IsStreamActive (stream)) {
    wxMilliSleep (10);                                             // Wait for 10 ms
  }
   
#if DEBUG_IO
    SUF_Debugfprintf ("SampleAudio - Stream streamed\n");
  
#endif  /*  */
    err = Pa_StopStream (stream);
  if (strcmp (Pa_GetHostApiInfo (deviceInfo->hostApi)->name, "Windows WASAPI") != 0) {
    if (err != paNoError) {
      
#if DEBUG_IO
        SUF_Debugfprintf ("CalibrateAudio - Error stopping stream\n");
      
#endif  /*  */
        error_terminate (err);
      return (1);
    }
  }
   
#if DEBUG_IO
    SUF_Debugfprintf ("SampleAudio - Stream stopped\n");
  
#endif  /*  */
    err = Pa_CloseStream (stream);
  if (err != paNoError) {
    error_terminate (err);
    return (1);
  }
   
#if DEBUG_IO
    SUF_Debugfprintf ("SampleAudio - Stream closed\n");
  
#endif  /*  */
    for (i = 0; i < SAMPLE_LENGTH; i++) {                          // Save input data for calling function
    *(pInputData + i) = (double) IOData.pCh0InputSamples[i + 100];  // Offset of 100 is used to allow for latency through ADC
  }  
#if DEBUG_IO
    SUF_Debugfprintf ("SampleAudio - Data copied\n");
  
#endif  /*  */
    Pa_Terminate ();
   
#if DEBUG_IO
    SUF_Debugfprintf ("SampleAudio - Stream terminated\n");
  
#endif  /*  */
    return (0);
}

  long SampleAndGenerateAudio (
  double *pInputData,
  double *pOutputData,
  long DataLength)  
{
  PaStreamParameters inputParameters, outputParameters;
  PaStream * stream;
  PaError err;
  long i;
  int numDevices;
  const PaDeviceInfo *deviceInfo;
  int InputDeviceNo = -1;
  int OutputDeviceNo = -1;
   
#if DEBUG_IO
    SUF_Debugfprintf ("--  SampleAndGenerateAudio  --\n");
  
#endif  /*  */
    IOData.InputArrayIndex = IOData.OutputArrayIndex = 0;          // Initialise array indices
  IOData.TotalOutputSamples = PREAMBLE_LENGTH + PA_SAMPLE_LENGTH + POSTAMBLE_LENGTH;  // Amount of data to generate and sample
  for (i = 0; i < PREAMBLE_LENGTH; i++) {                          // Clear pre-amble
    IOData.pCh0InputSamples[i] = 0;                                 // Clear input data array
    IOData.pCh0OutputSamples[i] = 0;                                // Clear output data pre-amble
  }
   for (i = 0; i < PA_SAMPLE_LENGTH; i++) {
    IOData.pCh0InputSamples[i + PREAMBLE_LENGTH] = 0;              // Clear input data array
    if (i < DataLength) {
      IOData.pCh0OutputSamples[i + PREAMBLE_LENGTH] = (short) pOutputData[i];  // Copy output data
    }
    
    else {
      IOData.pCh0OutputSamples[i + PREAMBLE_LENGTH] = 0;           // Zero pad
    }
  }
   for (i = 0; i < POSTAMBLE_LENGTH; i++) {                       // Clear post-amble
    IOData.pCh0InputSamples[i + PREAMBLE_LENGTH + PA_SAMPLE_LENGTH] = 0;  // Clear input data array
    IOData.pCh0OutputSamples[i + PREAMBLE_LENGTH + PA_SAMPLE_LENGTH] = 0; // Clear output data pre-amble
  }
    IOData.InputArrayIndex = IOData.OutputArrayIndex = 0;        // Initialise array indices
  
#if DEBUG_IO
    SUF_Debugfprintf ("SampleAndGenerateAudio - Arrays initialised\n");
  
#endif  /*  */
    err = Pa_Initialize ();
  if (err != paNoError) {
    error_terminate (err);
    return (1);
  }
   
#if DEBUG_IO
    SUF_Debugfprintf ("SampleAndGenerateAudio - PA initialised\n");
  
#endif  /*  */
    numDevices = Pa_GetDeviceCount ();
  if (numDevices < 0) {
    err = numDevices;
    error_terminate (err);
    return (1);
  }
   for (i = 0; i < numDevices; i++) {
    deviceInfo = Pa_GetDeviceInfo (i);
     
#if DEBUG_IO
      SUF_Debugfprintf ("SampleAndGenerateAudio - Device %d = %s, inputs = %d, outputs = %d, Host API = %s\n", (int) i, deviceInfo->name,
                        (int) deviceInfo->maxInputChannels, (int) deviceInfo->maxOutputChannels, Pa_GetHostApiInfo (deviceInfo->hostApi)->name);
    
#endif  /*  */
      if ((strstr (deviceInfo->name, DESIRED_AUDIO_INPUT_INTERFACE) != 0) && 
           (strcmp (Pa_GetHostApiInfo (deviceInfo->hostApi)->name, DESIRED_AUDIO_API) == 0) && (deviceInfo->maxInputChannels != 0)) {
       InputDeviceNo = i;
    }
    if ((strstr (deviceInfo->name, DESIRED_AUDIO_OUTPUT_INTERFACE) != 0) && 
         (strcmp (Pa_GetHostApiInfo (deviceInfo->hostApi)->name, DESIRED_AUDIO_API) == 0) && (deviceInfo->maxOutputChannels != 0)) {
       OutputDeviceNo = i;
    }
  }
   
#if DEBUG_IO
    SUF_Debugfprintf ("SampleAndGenerateAudio - Desired audio input interface = %s\n", DESIRED_AUDIO_INPUT_INTERFACE);
  SUF_Debugfprintf ("SampleAndGenerateAudio - Desired audio output interface = %s\n", DESIRED_AUDIO_OUTPUT_INTERFACE);
  SUF_Debugfprintf ("SampleAndGenerateAudio - Selected input device = %d\n", InputDeviceNo);
  SUF_Debugfprintf ("SampleAndGenerateAudio - Selected output device = %d\n", OutputDeviceNo);
  SUF_Debugfprintf ("SampleAndGenerateAudio - Selected host API = %s\n", DESIRED_AUDIO_API);
  SUF_Debugfprintf ("SampleAndGenerateAudio - PA initialised\n");
  
#endif  /*  */
    if (InputDeviceNo == -1) {                                     // If desired interface not detected, use default device
    inputParameters.device = Pa_GetDefaultInputDevice ();           // Default input device
  }
  
  else {
    inputParameters.device = InputDeviceNo;                        // Select input device
  }
  inputParameters.channelCount = 1;                                // Mono input
  inputParameters.sampleFormat = paInt16;
  inputParameters.suggestedLatency = Pa_GetDeviceInfo (inputParameters.device)->defaultLowInputLatency;
  inputParameters.hostApiSpecificStreamInfo = NULL;
   if (OutputDeviceNo == -1) {                                    // If desired interface not detected, use default device
    outputParameters.device = Pa_GetDefaultOutputDevice ();         // Default output device
  }
  
  else {
    outputParameters.device = OutputDeviceNo;                      // Select output device
  }
  outputParameters.channelCount = 1;                               // Mono output
  outputParameters.sampleFormat = paInt16;
  outputParameters.suggestedLatency = Pa_GetDeviceInfo (outputParameters.device)->defaultLowOutputLatency;
  outputParameters.hostApiSpecificStreamInfo = NULL;
   
#if DEBUG_IO
    SUF_Debugfprintf ("SampleAndGenerateAudio - Printing device details to status line\n");
  
#endif  /*  */
  {                                                                 // Print the selected device to the status line
    deviceInfo = Pa_GetDeviceInfo (inputParameters.device);
    wxString devString = "I/P = ";
    devString += deviceInfo->name;
    deviceInfo = Pa_GetDeviceInfo (outputParameters.device);
    devString += ", O/P = ";
    devString += deviceInfo->name;
    mainFrame * frame = (mainFrame *) (wxTheApp->GetTopWindow ());
    frame->DisplayStatusLineText2 (devString);
    
#if DEBUG_IO
      SUF_Debugfprintf ("SampleAndGenerateAudio - I/O Devices : %s : %s\n", devString.mb_str ().data (),
                        Pa_GetHostApiInfo (deviceInfo->hostApi)->name);
    
#endif  /*  */
  }
   
#if DEBUG_IO
    SUF_Debugfprintf ("SampleAndGenerateAudio - PA params set\n");
  
#endif  /*  */
    err = Pa_OpenStream (&stream, &inputParameters, &outputParameters, SAMPLE_RATE, FRAMES_PER_BUFFER, paClipOff,  // We won't output out of range samples so don't bother clipping them
                           PlayRecMonoCallback, &IOData);
   if (err != paNoError) {
    
#if DEBUG_IO
      SUF_Debugfprintf ("SampleAndGenerateAudio - Pa_OpenStream() error_terminate : %d\n", err);
    SUF_Debugfprintf ("SampleAndGenerateAudio - Pa_OpenStream() error : %s\n", Pa_GetErrorText (err));
    
#endif  /*  */
      error_terminate (err);
    return (1);
  }
   
#if DEBUG_IO
    SUF_Debugfprintf ("SampleAndGenerateAudio - Stream opened\n");
  
#endif  /*  */
    err = Pa_StartStream (stream);
  if (err != paNoError) {
    
#if DEBUG_IO
      SUF_Debugfprintf ("SampleAndGenerateAudio - Pa_StartStream() error_terminate : %d\n", err);
    SUF_Debugfprintf ("SampleAndGenerateAudio - Pa_StartStream() error : %s\n", Pa_GetErrorText (err));
    
#endif  /*  */
      error_terminate (err);
    return (1);
  }
  
#if DEBUG_IO
    SUF_Debugfprintf ("SampleAndGenerateAudio - Stream started\n");
  
#endif  /*  */
    while (Pa_IsStreamActive (stream)) {
    wxMilliSleep (10);                                             // Wait for 10 ms
  }
   
#if DEBUG_IO
    SUF_Debugfprintf ("SampleAndGenerateAudio - Stream streamed\n");
  
#endif  /*  */
    err = Pa_StopStream (stream);
  if (strcmp (Pa_GetHostApiInfo (deviceInfo->hostApi)->name, "Windows WASAPI") != 0) {
    if (err != paNoError) {
      
#if DEBUG_IO
        SUF_Debugfprintf ("SampleAndGenerateAudio - Error stopping stream\n");
      
#endif  /*  */
        error_terminate (err);
      return (1);
    }
  }
   err = Pa_CloseStream (stream);
  if (err != paNoError) {
    error_terminate (err);
    return (1);
  }
   for (i = 0; i < DataLength; i++) {                             // Save input data for calling function
    *(pInputData + i) = (double) IOData.pCh0InputSamples[i + IODelay];
  }  Pa_Terminate ();
   return (0);
}

  long SampleAndGenerateAudioWithReference (
  double *pInputData,
  double *pOutputData,
  long DataLength,
  long CalThreshold)  
{
  PaStreamParameters inputParameters, outputParameters;
  PaStream * stream;
  PaError err;
  long i;
  int numDevices;
  const PaDeviceInfo *deviceInfo;
  int InputDeviceNo = -1;
  int OutputDeviceNo = -1;
   
#if DEBUG_IO
    SUF_Debugfprintf ("--  SampleAndGenerateAudioWithReference  --\n");
  
#endif  /*  */
    IOData.InputArrayIndex = IOData.OutputArrayIndex = 0;          // Initialise array indices
  IOData.TotalOutputSamples = PREAMBLE_LENGTH + PA_SAMPLE_LENGTH + POSTAMBLE_LENGTH;  // Amount of data to generate and sample
  for (i = 0; i < PREAMBLE_LENGTH; i++) {                          // Clear pre-amble
    IOData.pCh0InputSamples[i] = 0;                                 // Clear input data array
    IOData.pCh0OutputSamples[i] = 0;                                // Clear output data pre-amble
    IOData.pCh1InputSamples[i] = 0;                                 // Clear input data array
    IOData.pCh1OutputSamples[i] = 0;                                // Clear output data pre-amble
  }
   for (i = 0; i < PA_SAMPLE_LENGTH; i++) {
    IOData.pCh0InputSamples[i + PREAMBLE_LENGTH] = 0;              // Clear input data array
    IOData.pCh1InputSamples[i + PREAMBLE_LENGTH] = 0;               // Clear input data array
    if (i < DataLength) {
      IOData.pCh0OutputSamples[i + PREAMBLE_LENGTH] = (short) pOutputData[i];  // Copy output data
      IOData.pCh1OutputSamples[i + PREAMBLE_LENGTH] = (short) pOutputData[i]; // Copy output data
    }
    
    else {
      IOData.pCh0OutputSamples[i + PREAMBLE_LENGTH] = 0;           // Zero pad
      IOData.pCh1OutputSamples[i + PREAMBLE_LENGTH] = 0;            // Zero pad
    }
  }
   for (i = 0; i < POSTAMBLE_LENGTH; i++) {                       // Clear post-amble
    IOData.pCh0InputSamples[i + PREAMBLE_LENGTH + PA_SAMPLE_LENGTH] = 0;  // Clear input data array
    IOData.pCh0OutputSamples[i + PREAMBLE_LENGTH + PA_SAMPLE_LENGTH] = 0; // Clear output data pre-amble
    IOData.pCh1InputSamples[i + PREAMBLE_LENGTH + PA_SAMPLE_LENGTH] = 0;  // Clear input data array
    IOData.pCh1OutputSamples[i + PREAMBLE_LENGTH + PA_SAMPLE_LENGTH] = 0; // Clear output data pre-amble
  }
    IOData.InputArrayIndex = IOData.OutputArrayIndex = 0;        // Initialise array indices
  
#if DEBUG_IO
    SUF_Debugfprintf ("SampleAndGenerateAudioWithReference - Arrays initialised\n");
  
#endif  /*  */
    err = Pa_Initialize ();
  if (err != paNoError) {
    error_terminate (err);
    return (1);
  }
   
#if DEBUG_IO
    SUF_Debugfprintf ("SampleAndGenerateAudioWithReference - PA initialised\n");
  
#endif  /*  */
    numDevices = Pa_GetDeviceCount ();
  if (numDevices < 0) {
    err = numDevices;
    error_terminate (err);
    return (1);
  }
   for (i = 0; i < numDevices; i++) {
    deviceInfo = Pa_GetDeviceInfo (i);
     
#if DEBUG_IO
      SUF_Debugfprintf ("SampleAndGenerateAudioWithReference - Device %d = %s, inputs = %d, outputs = %d, Host API = %s\n", (int) i,
                        deviceInfo->name, (int) deviceInfo->maxInputChannels, (int) deviceInfo->maxOutputChannels,
                        Pa_GetHostApiInfo (deviceInfo->hostApi)->name);
    
#endif  /*  */
      if ((strstr (deviceInfo->name, DESIRED_AUDIO_INPUT_INTERFACE) != 0) && 
           (strcmp (Pa_GetHostApiInfo (deviceInfo->hostApi)->name, DESIRED_AUDIO_API) == 0) && (deviceInfo->maxInputChannels != 0)) {
       InputDeviceNo = i;
    }
    if ((strstr (deviceInfo->name, DESIRED_AUDIO_OUTPUT_INTERFACE) != 0) && 
         (strcmp (Pa_GetHostApiInfo (deviceInfo->hostApi)->name, DESIRED_AUDIO_API) == 0) && (deviceInfo->maxOutputChannels != 0)) {
       OutputDeviceNo = i;
    }
  }
   
#if DEBUG_IO
    SUF_Debugfprintf ("SampleAndGenerateAudioWithReference - Desired audio input interface = %s\n", DESIRED_AUDIO_INPUT_INTERFACE);
  SUF_Debugfprintf ("SampleAndGenerateAudioWithReference - Desired audio output interface = %s\n", DESIRED_AUDIO_OUTPUT_INTERFACE);
  SUF_Debugfprintf ("SampleAndGenerateAudioWithReference - Selected input device = %d\n", InputDeviceNo);
  SUF_Debugfprintf ("SampleAndGenerateAudioWithReference - Selected output device = %d\n", OutputDeviceNo);
  SUF_Debugfprintf ("SampleAndGenerateAudioWithReference - Selected host API = %s\n", DESIRED_AUDIO_API);
  SUF_Debugfprintf ("SampleAndGenerateAudioWithReference - PA initialised\n");
  
#endif  /*  */
    if (InputDeviceNo == -1) {                                     // If desired interface not detected, use default device
    inputParameters.device = Pa_GetDefaultInputDevice ();           // Default input device
  }
  
  else {
    inputParameters.device = InputDeviceNo;                        // Select input device
  }
  inputParameters.channelCount = 2;                                // Mono input
  inputParameters.sampleFormat = paInt16;
  inputParameters.suggestedLatency = Pa_GetDeviceInfo (inputParameters.device)->defaultLowInputLatency;
  inputParameters.hostApiSpecificStreamInfo = NULL;
   if (OutputDeviceNo == -1) {                                    // If desired interface not detected, use default device
    outputParameters.device = Pa_GetDefaultOutputDevice ();         // Default output device
  }
  
  else {
    outputParameters.device = OutputDeviceNo;                      // Select output device
  }
  outputParameters.channelCount = 2;                               // Mono output
  outputParameters.sampleFormat = paInt16;
  outputParameters.suggestedLatency = Pa_GetDeviceInfo (outputParameters.device)->defaultLowOutputLatency;
  outputParameters.hostApiSpecificStreamInfo = NULL;
   
#if DEBUG_IO
    SUF_Debugfprintf ("SampleAndGenerateAudioWithReference - Printing device details to status line\n");
  
#endif  /*  */
  {                                                                 // Print the selected device to the status line
    deviceInfo = Pa_GetDeviceInfo (inputParameters.device);
    wxString devString = "I/P = ";
    devString += deviceInfo->name;
    deviceInfo = Pa_GetDeviceInfo (outputParameters.device);
    devString += ", O/P = ";
    devString += deviceInfo->name;
    mainFrame * frame = (mainFrame *) (wxTheApp->GetTopWindow ());
    frame->DisplayStatusLineText2 (devString);
    
#if DEBUG_IO
      SUF_Debugfprintf ("SampleAndGenerateAudioWithReference - I/O Devices : %s : %s\n", devString.mb_str ().data (),
                        Pa_GetHostApiInfo (deviceInfo->hostApi)->name);
    
#endif  /*  */
  }
   
#if DEBUG_IO
    SUF_Debugfprintf ("SampleAndGenerateAudioWithReference - PA params set\n");
  
#endif  /*  */
    err = Pa_OpenStream (&stream, &inputParameters, &outputParameters, SAMPLE_RATE, FRAMES_PER_BUFFER, paClipOff,  // We won't output out of range samples so don't bother clipping them
                           PlayRecStereoCallback, &IOData);
   if (err != paNoError) {
    
#if DEBUG_IO
      SUF_Debugfprintf ("SampleAndGenerateAudioWithReference - Pa_OpenStream() error_terminate : %d\n", err);
    SUF_Debugfprintf ("SampleAndGenerateAudioWithReference - Pa_OpenStream() error : %s\n", Pa_GetErrorText (err));
    
#endif  /*  */
      error_terminate (err);
    return (1);
  }
   
#if DEBUG_IO
    SUF_Debugfprintf ("SampleAndGenerateAudioWithReference - Stream opened\n");
  
#endif  /*  */
    err = Pa_StartStream (stream);
  if (err != paNoError) {
    
#if DEBUG_IO
      SUF_Debugfprintf ("SampleAndGenerateAudioWithReference - Pa_StartStream() error_terminate : %d\n", err);
    SUF_Debugfprintf ("SampleAndGenerateAudioWithReference - Pa_StartStream() error : %s\n", Pa_GetErrorText (err));
    
#endif  /*  */
      error_terminate (err);
    return (1);
  }
  
#if DEBUG_IO
    SUF_Debugfprintf ("SampleAndGenerateAudioWithReference - Stream started\n");
  
#endif  /*  */
    while (Pa_IsStreamActive (stream)) {
    wxMilliSleep (10);                                             // Wait for 10 ms
  }
   
#if DEBUG_IO
    SUF_Debugfprintf ("SampleAndGenerateAudioWithReference - Stream streamed\n");
  
#endif  /*  */
    err = Pa_StopStream (stream);
  if (strcmp (Pa_GetHostApiInfo (deviceInfo->hostApi)->name, "Windows WASAPI") != 0) {
    if (err != paNoError) {
      
#if DEBUG_IO
        SUF_Debugfprintf ("SampleAndGenerateAudioWithReference - Error stopping stream\n");
      
#endif  /*  */
        error_terminate (err);
      return (1);
    }
  }
   err = Pa_CloseStream (stream);
  if (err != paNoError) {
    error_terminate (err);
    return (1);
  }
   
// Calculate I/O delay - Using output to input delay on channel 1
    for (i = 0; i < PREAMBLE_LENGTH + PA_SAMPLE_LENGTH + POSTAMBLE_LENGTH; i++) {
    if (abs (IOData.pCh1InputSamples[i]) > CalThreshold) {
      IODelay = i - 7;
      break;
    }
  }
  
#if DEBUG_IO
    SUF_Debugfprintf ("SampleAndGenerateAudioWithReference - Estimated Delay = %d samples\n", (int) IODelay);
  
#endif  /*  */
     for (i = 0; i < DataLength; i++) {                           // Save input data for calling function
    *(pInputData + i) = (double) IOData.pCh0InputSamples[i + IODelay];
  }  Pa_Terminate ();
   return (0);
}

  long CalibrateAudio (
  double *pInputData,
  double *pOutputData,
  long DataLength,
  long CalThreshold)  
{
  PaStreamParameters inputParameters, outputParameters;
  PaStream * stream;
  PaError err;
  long i;
  double        *DummypInputData = pInputData;                     // Dummy locals just to prevent compiler warnings
  int numDevices;
  const PaDeviceInfo *deviceInfo;
  int InputDeviceNo = -1;
  int OutputDeviceNo = -1;
   
#if DEBUG_IO
    SUF_Debugfprintf ("--  CalibrateAudio  --\n");
  
#endif  /*  */
    IOData.TotalOutputSamples = PREAMBLE_LENGTH + PA_SAMPLE_LENGTH + POSTAMBLE_LENGTH; // Amount of data to generate and sample
  for (i = 0; i < PREAMBLE_LENGTH; i++) {                          // Clear pre-amble
    IOData.pCh0InputSamples[i] = 0;                                 // Clear input data array
    IOData.pCh0OutputSamples[i] = 0;                                // Clear output data pre-amble
  }
   for (i = 0; i < PA_SAMPLE_LENGTH; i++) {
    IOData.pCh0InputSamples[i + PREAMBLE_LENGTH] = 0;              // Clear input data array
    if (i < DataLength)
      IOData.pCh0OutputSamples[i + PREAMBLE_LENGTH] = (short) pOutputData[i];  // Copy output data
    else
      IOData.pCh0OutputSamples[i + PREAMBLE_LENGTH] = 0;           // Zero pad
  }
   for (i = 0; i < POSTAMBLE_LENGTH; i++) {                       // Clear post-amble
    IOData.pCh0InputSamples[i + PREAMBLE_LENGTH + PA_SAMPLE_LENGTH] = 0;  // Clear input data array
    IOData.pCh0OutputSamples[i + PREAMBLE_LENGTH + PA_SAMPLE_LENGTH] = 0; // Clear output data pre-amble
  }
    IOData.InputArrayIndex = IOData.OutputArrayIndex = 0;        // Initialise array indices
  
#if DEBUG_IO
    SUF_Debugfprintf ("CalibrateAudio - Arrays initialised\n");
  
#endif  /*  */
    err = Pa_Initialize ();
  if (err != paNoError) {
    error_terminate (err);
    return (1);
  }
   
#if DEBUG_IO
    SUF_Debugfprintf ("CalibrateAudio - PA initialised\n");
  
#endif  /*  */
    numDevices = Pa_GetDeviceCount ();
  if (numDevices < 0) {
    err = numDevices;
    error_terminate (err);
    return (1);
  }
   for (i = 0; i < numDevices; i++) {
    deviceInfo = Pa_GetDeviceInfo (i);
     
#if DEBUG_IO
      SUF_Debugfprintf ("CalibrateAudio - Device %d = %s, inputs = %d, outputs = %d, Host API = %s\n", (int) i, deviceInfo->name,
                        (int) deviceInfo->maxInputChannels, (int) deviceInfo->maxOutputChannels, Pa_GetHostApiInfo (deviceInfo->hostApi)->name);
    
#endif  /*  */
      if ((strstr (deviceInfo->name, DESIRED_AUDIO_INPUT_INTERFACE) != 0) && 
           (strcmp (Pa_GetHostApiInfo (deviceInfo->hostApi)->name, DESIRED_AUDIO_API) == 0) && (deviceInfo->maxInputChannels != 0)) {
       InputDeviceNo = i;
    }
    if ((strstr (deviceInfo->name, DESIRED_AUDIO_OUTPUT_INTERFACE) != 0) && 
         (strcmp (Pa_GetHostApiInfo (deviceInfo->hostApi)->name, DESIRED_AUDIO_API) == 0) && (deviceInfo->maxOutputChannels != 0)) {
       OutputDeviceNo = i;
    }
  }
   
#if DEBUG_IO
    SUF_Debugfprintf ("CalibrateAudio - Desired audio input interface = %s\n", DESIRED_AUDIO_INPUT_INTERFACE);
  SUF_Debugfprintf ("CalibrateAudio - Desired audio output interface = %s\n", DESIRED_AUDIO_OUTPUT_INTERFACE);
  SUF_Debugfprintf ("CalibrateAudio - Selected input device = %d\n", InputDeviceNo);
  SUF_Debugfprintf ("CalibrateAudio - Selected output device = %d\n", OutputDeviceNo);
  SUF_Debugfprintf ("CalibrateAudio - Selected host API = %s\n", DESIRED_AUDIO_API);
  SUF_Debugfprintf ("CalibrateAudio - PA initialised\n");
  
#endif  /*  */
    if (InputDeviceNo == -1) {                                     // If desired interface not detected, use default device
    inputParameters.device = Pa_GetDefaultInputDevice ();           // Default input device
  }
  
  else {
    inputParameters.device = InputDeviceNo;                        // Select input device
  }
  inputParameters.channelCount = 1;                                // Mono input
  inputParameters.sampleFormat = paInt16;
  inputParameters.suggestedLatency = Pa_GetDeviceInfo (inputParameters.device)->defaultLowInputLatency;
  inputParameters.hostApiSpecificStreamInfo = NULL;
   if (OutputDeviceNo == -1) {                                    // If desired interface not detected, use default device
    outputParameters.device = Pa_GetDefaultOutputDevice ();         // Default output device
  }
  
  else {
    outputParameters.device = OutputDeviceNo;                      // Select output device
  }
  outputParameters.channelCount = 1;                               // Mono output
  outputParameters.sampleFormat = paInt16;
  outputParameters.suggestedLatency = Pa_GetDeviceInfo (outputParameters.device)->defaultLowOutputLatency;
  outputParameters.hostApiSpecificStreamInfo = NULL;
   
#if DEBUG_IO
    SUF_Debugfprintf ("CalibrateAudio - Printing device details to status line\n");
  
#endif  /*  */
  {                                                                 // Print the selected device to the status line
    deviceInfo = Pa_GetDeviceInfo (inputParameters.device);
    wxString devString = "I/P = ";
    devString += deviceInfo->name;
    deviceInfo = Pa_GetDeviceInfo (outputParameters.device);
    devString += ", O/P = ";
    devString += deviceInfo->name;
    mainFrame * frame = (mainFrame *) (wxTheApp->GetTopWindow ());
    frame->DisplayStatusLineText2 (devString);
    
#if DEBUG_IO
      SUF_Debugfprintf ("CalibrateAudio - I/O Devices : %s : %s\n", devString.mb_str ().data (), Pa_GetHostApiInfo (deviceInfo->hostApi)->name);
    
#endif  /*  */
  }
   
#if DEBUG_IO
    SUF_Debugfprintf ("CalibrateAudio - PA params set\n");
  
#endif  /*  */
    err = Pa_OpenStream (&stream, &inputParameters, &outputParameters, SAMPLE_RATE, FRAMES_PER_BUFFER, paClipOff,  // We won't output out of range samples so don't bother clipping them
                           PlayRecMonoCallback, &IOData);
  
//              (void *)&IOData );
    if (err != paNoError) {
    
#if DEBUG_IO
      SUF_Debugfprintf ("CalibrateAudio - Pa_OpenStream() error_terminate : %d\n", err);
    SUF_Debugfprintf ("CalibrateAudio - Pa_OpenStream() error : %s\n", Pa_GetErrorText (err));
    
#endif  /*  */
      error_terminate (err);
    return (1);
  }
   
#if DEBUG_IO
    SUF_Debugfprintf ("CalibrateAudio - Stream opened\n");
  
#endif  /*  */
    err = Pa_StartStream (stream);
  if (err != paNoError) {
    
#if DEBUG_IO
      SUF_Debugfprintf ("CalibrateAudio - Pa_StartStream() error_terminate : %d\n", err);
    SUF_Debugfprintf ("CalibrateAudio - Pa_StartStream() error : %s\n", Pa_GetErrorText (err));
    
#endif  /*  */
      error_terminate (err);
    return (1);
  }
  
#if DEBUG_IO
    SUF_Debugfprintf ("CalibrateAudio - Stream started\n");
  
#endif  /*  */
     while (Pa_IsStreamActive (stream)) {
    wxMilliSleep (10);                                             // Wait for 10 ms
  }
   
#if DEBUG_IO
    SUF_Debugfprintf ("CalibrateAudio - Stream streamed\n");
  
#endif  /*  */
    err = Pa_StopStream (stream);
  if (strcmp (Pa_GetHostApiInfo (deviceInfo->hostApi)->name, "Windows WASAPI") != 0) {
    if (err != paNoError) {
      
#if DEBUG_IO
        SUF_Debugfprintf ("CalibrateAudio - Error stopping stream\n");
      
#endif  /*  */
        error_terminate (err);
      return (1);
    }
  }
   err = Pa_CloseStream (stream);
  if (err != paNoError) {
    
#if DEBUG_IO
      SUF_Debugfprintf ("CalibrateAudio - Error closing stream\n");
    
#endif  /*  */
      error_terminate (err);
    return (1);
  }
   
#if DEBUG_IO
    SUF_Debugfprintf ("CalibrateAudio - Calculating Delay\n");
  
#endif  /*  */
    
// Calculate I/O delay
    for (i = 0; i < PREAMBLE_LENGTH + PA_SAMPLE_LENGTH + POSTAMBLE_LENGTH; i++) {
    if (abs (IOData.pCh0InputSamples[i]) > CalThreshold) {
      IODelay = i;
      break;
    }
  }
   
#if DEBUG_IO
    SUF_Debugfprintf ("CalibrateAudio - Display delay dialog\n");
  
#endif  /*  */
  char String[100];
  sprintf (String, "I/O Delay: %ld", IODelay);
  wxMessageBox (String, "System Analyzer I/O Delay", wxOK | wxICON_EXCLAMATION, NULL);
   
#if DEBUG_IO
    SUF_Debugfprintf ("CalibrateAudio - Terminating portaudio\n");
  
#endif  /*  */
    Pa_Terminate ();
   
#if DEBUG_IO
    SUF_Debugfprintf ("CalibrateAudio - Exiting\n");
  
#endif  /*  */
    return (0);
}

  void error_terminate (
  PaError err)  { Pa_Terminate ();
   char String[100];
   sprintf (String, "An error occured while using the portaudio stream\nError number: %d\nError message: %s", err, Pa_GetErrorText (err));
}  
