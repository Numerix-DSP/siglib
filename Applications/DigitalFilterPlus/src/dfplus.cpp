// Digital filter design program top level functions
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

#include "wx/file.h"
#include "wx/mimetype.h"
#include "wx/splash.h"
#include "wx/stdpaths.h"
#include "wx/string.h"
#include "wx/textfile.h"
#include <wx/spinctrl.h>

#if defined(__WXGTK__) || defined(__WXMOTIF__) || defined(__WXMAC__) || defined(__WXMGL__) || defined(__WXX11__)
#  include "dfplus.xpm"    // the application icon
#endif

#if __APPLE__
#  include <malloc/malloc.h>
#else
#  include <malloc.h>
#endif
#include "mainframe.h"    // mainframe header - must be located after other project headers
#include <math.h>
#include <stdlib.h>
#include <siglib.h>    // SigLib DSP library
#include "dfplus.h"

GraphDisplay* GlobalGraphDisplayWindow;

int global_mValue;    // Global m and n values for Q number format
int global_nValue;
int global_NegateIIRAlphaSign;    // Global NegateIIRAlphaSign for biquads

// #ifdef DEBUG_LOG_FILE
// #undef DEBUG_LOG_FILE
// #define DEBUG_LOG_FILE 1 // Set to 1 to log progress to "debug.log" file
// #endif

/********************************************************
 * Function : df_group_delay ()
 *
 * Parameters  :
 *   SLData_t FreqStep
 *   SLArrayIndex_t FirstCalc
 *   SLData_t ThetaT
 *
 * Return value :
 *   Group delay
 *
 * Description :
 *   Calculates the group delay
 *
 * WARNING - This function is not re-entrant
 *
 ********************************************************/

SLData_t df_group_delay(SLData_t FreqStep, SLArrayIndex_t FirstCalc, SLData_t ThetaT)

{
  static SLData_t GPDP = SIGLIB_ZERO;
  static SLData_t GroupDelayMax = SIGLIB_ZERO;
  static SLData_t GroupDelayMin = SIGLIB_ZERO;
  static SLData_t GPDel = SIGLIB_ZERO;
  static SLData_t ThetaP = SIGLIB_ZERO;

#if DEBUG_LOG_FILE
//  SUF_Debugfprintf ("df_group_delay() : function entered\n");
#endif

  if (FirstCalc == SIGLIB_TRUE) {
    GroupDelayMax = SIGLIB_ZERO;
    GroupDelayMin = SIGLIB_ZERO;
    GPDel = SIGLIB_ZERO;
    ThetaP = ThetaT;
    GPDP = GPDel;
  }

  else {
    if (SDS_Sign(ThetaP) != SDS_Sign(ThetaT)) {
      GPDel = GPDP;
    }

    else {
      GPDel = (ThetaP - ThetaT) / FreqStep;
      //      GPDel = ((ThetaP-ThetaT) / FreqStep) * 1000.0;
    }

    if (GPDel > GroupDelayMax) {
      GroupDelayMax = GPDel;
    }

    if (GPDel < GroupDelayMin) {
      GroupDelayMin = GPDel;
    }

    ThetaP = ThetaT;
    GPDP = GPDel;
  }

  return (GPDel);
} /* End of df_group_delay ()*/

/********************************************************
 * Function : FloatToQFormatInteger ()
 *
 * Parameters  :
 *   double x,
 *   int m,
 *   int n
 *
 * Return value :
 *   FloatToQFormatInteger
 *
 * Description :
 *   Calculates the Q format integer
 *
 ********************************************************/

long long FloatToQFormatInteger(double x, int m, int n)

{
  int i;
  long long y;
  long long mask = (long long)0;

  y = (long long)(x * ((double)(((long long)1) << n)));    // Generate the integer
                                                           // number in m.n format

  for (i = 0; i < (m + n); i++) {    // Calculate the mask to ensure we return the
                                     // correct number of valid bits
    mask = (mask << 1) + 1;
  }
  // printf ("Mask = 0x%llx\n", mask);

  return (y & mask);
}

/*
  Function Name : mainFrame::mainFrame
  Description : Top level frame
  Notes : This should contain the filter designer and graph children
*/

mainFrame::mainFrame(wxString Title, wxPoint Position, wxSize Size)
    : wxFrame((wxFrame*)NULL, -1, Title, Position, Size)
//     : wxFrame ((wxFrame *) NULL, -1, Title, Position, Size, wxCAPTION |
//     wxMINIMIZE_BOX | wxSYSTEM_MENU)

{
  //  wxMessageBox ("Got Here", "main", wxOK | wxICON_EXCLAMATION, NULL);

  wxBitmap bitmap;
  if (bitmap.LoadFile("Splash.bmp", wxBITMAP_TYPE_BMP)) {
    wxSplashScreen* splash = new wxSplashScreen(bitmap, wxSPLASH_CENTRE_ON_SCREEN | wxSPLASH_TIMEOUT, 4000, this, wxID_ANY, wxDefaultPosition,
                                                wxDefaultSize, wxSIMPLE_BORDER | wxSTAY_ON_TOP);
    // 4000, NULL, -1, wxDefaultPosition, wxDefaultSize,
    // wxSIMPLE_BORDER);
    wxYield();    // Note we have to use wxYield, wxSleep and ~wxSplashScreen
                  // otherwise doesn't work with Unregistered Copy wxMessageBox
    wxSleep(4);
    splash->~wxSplashScreen();
  }

  ChildWindowsCreated = FALSE;    // Filter spec and graph windows have NOT been created

  // Get the path to the executable program and
  // make sure that the last character is wxFILE_SEP_PATH
  InstallDirectoryPathString = FindExecutablePath(wxTheApp->argv[0], wxGetCwd());

  if (InstallDirectoryPathString.Last() != wxFILE_SEP_PATH)    // Make sure the last character is a separator
    InstallDirectoryPathString += wxFILE_SEP_PATH;

#if DEBUG_LOG_FILE
  // char *cp = (wxTheApp->argv)[0];
  // SUF_Debugfprintf("mainFrame::mainFrame : wxTheApp->argv[0] = \"%s\"\n", cp);
  SUF_Debugfprintf("mainFrame::mainFrame : wxTheApp->argv[0] = \"%s\"\n", wxTheApp->argv[0]);
  SUF_Debugfprintf("mainFrame::mainFrame : wxGetCwd() = \"%s\"\n", wxGetCwd().GetData());
  SUF_Debugfprintf("mainFrame::mainFrame : FindExecutablePath : "
                   "wxIsAbsolutePath(wxTheApp->argv[0]) = %d\n",
                   (short)wxIsAbsolutePath(wxTheApp->argv[0]));
  SUF_Debugfprintf("mainFrame::mainFrame : wxPathOnly(wxTheApp->argv[0]) = \"%s\"\n", wxPathOnly(wxTheApp->argv[0]).GetData());
  SUF_Debugfprintf("mainFrame::mainFrame : Executable path = \"%s\"\n", InstallDirectoryPathString.GetData());
#endif

  AppIniFile = new IniFile(InstallDirectoryPathString + _T("dfplus.ini"));

  wxString SizeString;    // Get size of parent from .ini file and re-size
                          // Check to see if size has prevoiusly been saved
  if (AppIniFile->ReadIniStringFromFile(_T("SIZE"), &SizeString) == TRUE) {
    // Debug
    //    wxMessageBox (_T("Size KEY detected"), _T("Size"), wxOK |
    //    wxICON_INFORMATION, this);

    int x, y, w, h;
    sscanf(SizeString, "%d, %d, %d, %d", &x, &y, &w, &h);

    int DisplayWidth, DisplayHeight;
    wxDisplaySize(&DisplayWidth, &DisplayHeight);

    // debug
    //     wxString  DebugString;
    //     DebugString.sprintf ("%d, %d", DisplayWidth, DisplayHeight);
    //     wxMessageBox (_T(DebugString), _T("Display Size"), wxOK |
    //     wxICON_INFORMATION, this);

    // Limit width and height
    if (x > ((DisplayWidth / 4) * 3))
      x = ((DisplayWidth / 4) * 3);
    if (y > ((DisplayHeight / 4) * 3))
      y = ((DisplayHeight / 4) * 3);
    if (w > DisplayWidth)
      w = DisplayWidth;
    if (h > DisplayHeight)
      h = DisplayHeight;
    if (x < 0)
      x = 0;
    if (y < 0)
      y = 0;
    if (w < 0)
      w = 0;
    if (h < 0)
      h = 0;

    if ((w < 200) || (h < 200))    // If width of height too small then use defaults
    {
      SetSize(wxRect(DFPLUS_LEFT, DFPLUS_TOP, DFPLUS_WIDTH,
                     DFPLUS_HEIGHT));    // X, Y, W, H - used for a resizeable border
    } else {
      SetSize(x, y, w, h);
    }

    // Debug
    //    wxMessageBox (_T(SizeString), _T("Size"), wxOK | wxICON_INFORMATION,
    //    this);
  } else {
    SetSize(wxRect(DFPLUS_LEFT, DFPLUS_TOP, DFPLUS_WIDTH,
                   DFPLUS_HEIGHT));    // X, Y, W, H - used for a resizeable border
  }

  AppIniFile->WriteIniStringToFile(_T("COPYRIGHT"), _T(DFPLUS_COPYRIGHT_NOTICE));

  SetAutoLayout(TRUE);    // Autolayout the window
#if defined(__WXMSW__)
  SetIcon(wxICON(ApplicationIcon));    // Select the application icon
#endif

  // Create the menubar
  wxMenuBar* menuBar = new wxMenuBar;
  wxMenu* fileMenu = new wxMenu;
  wxMenu* viewMenu = new wxMenu;
  wxMenu* optionsMenu = new wxMenu;
  wxMenu* helpMenu = new wxMenu;

  fileMenu->Append(DFPLUS_GET_FILTER_SPEC, _T("&New Project"));
  fileMenu->AppendSeparator();
  fileMenu->Append(DFPLUS_EXIT, _T("E&xit"));

  viewMenu->Append(DFPLUS_VIEW_COEFFS, _T("View &Filter Coefficients"));
  viewMenu->Append(DFPLUS_VIEW_SPEC, _T("View &Specification"));
  viewMenu->Append(DFPLUS_VIEW_ZPLANE, _T("View &z-Domain Coefficients"));

  optionsMenu->Append(DFPLUS_OPTIONS, _T("&Options"));

  helpMenu->Append(DFPLUS_OPEN_USERS_GUIDE, _T("View &User's Guide"));
  helpMenu->AppendSeparator();
  helpMenu->Append(DFPLUS_ABOUT, _T("&About"));

  menuBar->Append(fileMenu, _T("&File"));
  menuBar->Append(viewMenu, _T("&View"));
  menuBar->Append(optionsMenu, _T("&Options"));
  menuBar->Append(helpMenu, _T("&Help"));
  SetMenuBar(menuBar);

  CreateStatusBar(3);
  SetStatusText(_T("Digital Filter Plus"), 0);
  //  SetStatusText (_T("Status bar text 2"), 1);
  //  SetStatusText (_T("Status bar text 3"), 2);

#if DEBUG_LOG_FILE
  SUF_Debugfprintf("mainFrame::mainFrame : Menus created\n");
#endif

  // Create graph window - allow for the toolbar height
#if defined(__WXGTK__) || defined(__WXMOTIF__)
  SplitterWindow = new wxSplitterWindow(this, -1, wxPoint(0, 0), GetClientSize());
#else
  SplitterWindow = new wxSplitterWindow(this, -1, wxPoint(0, TOOLBAR_HEIGHT), GetClientSize());
#endif

#if DEBUG_LOG_FILE
  SUF_Debugfprintf("mainFrame::mainFrame : Splitter window created\n");
#endif

  // Create filter specification entry window
  FilterSpecWindow = new wxWindow(SplitterWindow, -1);
  FIRWinDesign = new FIRWinFilterDesignWindow(FilterSpecWindow, -1);
  FIRWinDesign->Show(FALSE);
  FIRWinDesign->Enable(FALSE);
  FIRAlgsDesign = new FIRAlgsFilterDesignWindow(FilterSpecWindow, -1);
  FIRAlgsDesign->Show(FALSE);
  FIRAlgsDesign->Enable(FALSE);
  FIRSimpleRemezDesign = new FIRSimpleRemezFilterDesignWindow(FilterSpecWindow, -1);
  FIRSimpleRemezDesign->Show(FALSE);
  FIRSimpleRemezDesign->Enable(FALSE);
  FIRRemezDesign = new FIRRemezFilterDesignWindow(FilterSpecWindow, -1);
  FIRRemezDesign->Show(FALSE);
  FIRRemezDesign->Enable(FALSE);
  IIRPZPDesign = new IIRPZPFilterDesignWindow(FilterSpecWindow, -1);
  IIRPZPDesign->Show(FALSE);
  IIRPZPDesign->Enable(FALSE);
  IIRTraditionalDesign = new IIRTraditionalFilterDesignWindow(FilterSpecWindow, -1);
  IIRTraditionalDesign->Show(FALSE);
  IIRTraditionalDesign->Enable(FALSE);
  IIRAlgsDesign = new IIRAlgsFilterDesignWindow(FilterSpecWindow, -1);
  IIRAlgsDesign->Show(FALSE);
  IIRAlgsDesign->Enable(FALSE);

#if DEBUG_LOG_FILE
  SUF_Debugfprintf("mainFrame::mainFrame : Filter design windows created\n");
#endif

  // Create graph window
  GraphDisplayWindow = new wxWindow(SplitterWindow, -1);
#if DEBUG_LOG_FILE
  SUF_Debugfprintf("mainFrame::mainFrame : GraphDisplayWindow created\n");
#endif

  FilterGraphDisplay = new GraphDisplay(GraphDisplayWindow, -1);
#if DEBUG_LOG_FILE
  SUF_Debugfprintf("mainFrame::mainFrame : FilterGraphDisplay created\n");
#endif

  GlobalGraphDisplayWindow = FilterGraphDisplay;
#if DEBUG_LOG_FILE
  SUF_Debugfprintf("mainFrame::mainFrame : GlobalGraphDisplayWindow initialised\n");
#endif

  // Read the initial graph scale from .ini file
  wxString iniString;
  if (AppIniFile->ReadIniStringFromFile(_T("GRAPH_SCALE"), &iniString) == TRUE) {
    SLData_t GScale;
    sscanf(iniString, "%lf", &GScale);
    FilterGraphDisplay->SetLogMagnGraphScale(GScale);    // Set the graph scale
  } else {
    FilterGraphDisplay->SetLogMagnGraphScale(0.0);    // Set the default graph scale
    AppIniFile->WriteIniStringToFile(_T("GRAPH_SCALE"), "0.0000");
  }

  // Read the initial m.n values from .ini file
  if (AppIniFile->ReadIniStringFromFile(_T("M_VALUE"), &iniString) == TRUE) {
    sscanf(iniString, "%d", &global_mValue);    // Set the global m value
  } else {
    global_mValue = 8;    // Set the default m value
    AppIniFile->WriteIniStringToFile(_T("M_VALUE"), "8");
  }

  if (AppIniFile->ReadIniStringFromFile(_T("N_VALUE"), &iniString) == TRUE) {
    sscanf(iniString, "%d", &global_nValue);    // Set the global n value
  } else {
    global_nValue = 24;    // Set the default n value
    AppIniFile->WriteIniStringToFile(_T("N_VALUE"), "24");
  }

  if (AppIniFile->ReadIniStringFromFile(_T("NEGATE_IIR_ALPHA_SIGN"), &iniString) == TRUE) {
    sscanf(iniString, "%d",
           &global_NegateIIRAlphaSign);    // Set the global NegateIIRAlphaSign value
  } else {
    global_NegateIIRAlphaSign = 0;    // Set the default NegateIIRAlphaSign value
    AppIniFile->WriteIniStringToFile(_T("NEGATE_IIR_ALPHA_SIGN"), "0");
  }

  // Arrange splitter window
  //  SplitterWindow->SetSashSize (0);
  SplitterWindow->SetSashInvisible(true);
  wxSize ClientSize = GetClientSize();
  SplitterWindow->SetMinimumPaneSize((ClientSize.GetWidth() * 3) / 10);
  // Split the screen vertically and allocate the space
  SplitterWindow->SplitVertically(FilterSpecWindow, GraphDisplayWindow, (ClientSize.GetWidth() * 3) / 10);

#if DEBUG_LOG_FILE
  SUF_Debugfprintf("mainFrame::mainFrame : Window split done\n");
#endif

  // Create project dialog box
  FilterSpecDialog = new GetFilterProjectDialog(this, -1, _T("DFPlus Project Options"));

#if DEBUG_LOG_FILE
  SUF_Debugfprintf("mainFrame::mainFrame : New project dialog done\n");
#endif

  // Create the text dialog for displaying the coefficients, specification and
  // z-Domain coefficients
  TextDisplayDialog = new DisplayTextDialog(this, -1, _T("Filter Coefficients"), wxPoint(250, 150), wxSize(600, 450));

#if DEBUG_LOG_FILE
  SUF_Debugfprintf("mainFrame::mainFrame : Coefficient display window created\n");
#endif

  ChildWindowsCreated = TRUE;    // Filter spec and graph windows have been
                                 // created, now we can resize them

#if DEBUG_LOG_FILE
  SUF_Debugfprintf("mainFrame::mainFrame : Child windows created\n");
#endif

  pGain = SUF_VectorArrayAllocate(GRAPH_WIDTH);    // Create data arrays
  pPhase = SUF_VectorArrayAllocate(GRAPH_WIDTH);
  pGroupDelay = SUF_VectorArrayAllocate(GRAPH_WIDTH);
  pImpulseResponse = SUF_VectorArrayAllocate(GRAPH_WIDTH);
  pStepResponse = SUF_VectorArrayAllocate(GRAPH_WIDTH);
  pPolesAndZeros = SUF_ComplexRectArrayAllocate(2 * MAX_BLT_ORDER);    // 2 * Pole / Zero

#if DEBUG_LOG_FILE
  SUF_Debugfprintf("mainFrame::mainFrame : Arrays allocated\n");
#endif

  SDA_Clear(pGain, GRAPH_WIDTH);    // Clear array contents
  SDA_Clear(pPhase, GRAPH_WIDTH);
  SDA_Clear(pGroupDelay, GRAPH_WIDTH);
  SDA_Clear(pImpulseResponse, GRAPH_WIDTH);
  SDA_Clear(pStepResponse, GRAPH_WIDTH);
  SDA_Clear((SLData_t*)pPolesAndZeros, 2 * MAX_BLT_ORDER);

#if DEBUG_LOG_FILE
  SUF_Debugfprintf("mainFrame::mainFrame : Memory cleared\n");
#endif

  FilterGraphDisplay->SetGainArray(pGain);    // Set graph pointers to arrays
  FilterGraphDisplay->SetPhaseArray(pPhase);
  FilterGraphDisplay->SetGroupDelayArray(pGroupDelay);
  FilterGraphDisplay->SetImpulseResponseArray(pImpulseResponse);
  FilterGraphDisplay->SetStepResponseArray(pStepResponse);
  FilterGraphDisplay->SetPoleZeroPlotArray(pPolesAndZeros);
  FilterGraphDisplay->SetFilterInfoStruct(&FilterInfo);

#if DEBUG_LOG_FILE
  SUF_Debugfprintf("mainFrame::mainFrame : FilterGraphDisplay created\n");
#endif

  FIRWinDesign->SetGainArray(pGain);    // Set filter design window pointers to arrays
  FIRWinDesign->SetPhaseArray(pPhase);
  FIRWinDesign->SetGroupDelayArray(pGroupDelay);
  FIRWinDesign->SetImpulseResponseArray(pImpulseResponse);
  FIRWinDesign->SetStepResponseArray(pStepResponse);
  FIRWinDesign->SetFilterInfoStruct(&FilterInfo);

  FIRAlgsDesign->SetGainArray(pGain);
  FIRAlgsDesign->SetPhaseArray(pPhase);
  FIRAlgsDesign->SetGroupDelayArray(pGroupDelay);
  FIRAlgsDesign->SetImpulseResponseArray(pImpulseResponse);
  FIRAlgsDesign->SetStepResponseArray(pStepResponse);
  FIRAlgsDesign->SetFilterInfoStruct(&FilterInfo);

  FIRSimpleRemezDesign->SetGainArray(pGain);
  FIRSimpleRemezDesign->SetPhaseArray(pPhase);
  FIRSimpleRemezDesign->SetGroupDelayArray(pGroupDelay);
  FIRSimpleRemezDesign->SetImpulseResponseArray(pImpulseResponse);
  FIRSimpleRemezDesign->SetStepResponseArray(pStepResponse);
  FIRSimpleRemezDesign->SetFilterInfoStruct(&FilterInfo);

  FIRRemezDesign->SetGainArray(pGain);
  FIRRemezDesign->SetPhaseArray(pPhase);
  FIRRemezDesign->SetGroupDelayArray(pGroupDelay);
  FIRRemezDesign->SetImpulseResponseArray(pImpulseResponse);
  FIRRemezDesign->SetStepResponseArray(pStepResponse);
  FIRRemezDesign->SetFilterInfoStruct(&FilterInfo);

  IIRPZPDesign->SetGainArray(pGain);
  IIRPZPDesign->SetPhaseArray(pPhase);
  IIRPZPDesign->SetGroupDelayArray(pGroupDelay);
  IIRPZPDesign->SetImpulseResponseArray(pImpulseResponse);
  IIRPZPDesign->SetStepResponseArray(pStepResponse);
  IIRPZPDesign->SetPoleZeroPlotArray(pPolesAndZeros);
  IIRPZPDesign->SetFilterInfoStruct(&FilterInfo);

  IIRTraditionalDesign->SetGainArray(pGain);
  IIRTraditionalDesign->SetPhaseArray(pPhase);
  IIRTraditionalDesign->SetGroupDelayArray(pGroupDelay);
  IIRTraditionalDesign->SetImpulseResponseArray(pImpulseResponse);
  IIRTraditionalDesign->SetStepResponseArray(pStepResponse);
  IIRTraditionalDesign->SetPoleZeroPlotArray(pPolesAndZeros);
  IIRTraditionalDesign->SetFilterInfoStruct(&FilterInfo);

  //  SUF_Debugfprintf ("mainFrame::mainFrame : Initilizing IIRAlgsDesign\n");
  IIRAlgsDesign->SetGainArray(pGain);
  IIRAlgsDesign->SetPhaseArray(pPhase);
  IIRAlgsDesign->SetGroupDelayArray(pGroupDelay);
  IIRAlgsDesign->SetImpulseResponseArray(pImpulseResponse);
  IIRAlgsDesign->SetStepResponseArray(pStepResponse);
  IIRAlgsDesign->SetPoleZeroPlotArray(pPolesAndZeros);
  IIRAlgsDesign->SetFilterInfoStruct(&FilterInfo);
  //  SUF_Debugfprintf ("mainFrame::mainFrame : Initilized IIRAlgsDesign\n");

#if DEBUG_LOG_FILE
  SUF_Debugfprintf("mainFrame::mainFrame : Designs created\n");
#endif

  // Open up project dialog on first opening
  // Can not just show the dialog because we
  // need to run OnGetFilterSpec ()
  // Option one - does not have main window in background
  // If you do this and just hit return straight after dialog, app crashes :-(
  //  wxCommandEvent DummyEvent (wxEVT_COMMAND_MENU_SELECTED,
  //  DFPLUS_GET_FILTER_SPEC); OnGetFilterSpec (DummyEvent);

  // Option two - has main window in background
  wxCommandEvent DummyEvent(wxEVT_COMMAND_MENU_SELECTED, DFPLUS_GET_FILTER_SPEC);
  wxPostEvent(this, DummyEvent);

#if DEBUG_LOG_FILE
  SUF_Debugfprintf("mainFrame::mainFrame : Get filter spec. event posted\n");
#endif
}

/*
  Function Name : mainFrame::~mainFrame
  Description : Destructor for top level DFPlus frame
  Notes :
*/

mainFrame::~mainFrame()
{
  wxString SizeString;
  wxRect Rect = GetRect();

  if (Rect.GetHeight() > 24)    // Only save window size if not minimized
  {
    // Save size of parent window so that it can be reloaded on startup
    SizeString.sprintf("%d, %d, %d, %d", Rect.GetX(), Rect.GetY(), Rect.GetWidth(), Rect.GetHeight());

    // Debug
    //  wxMessageBox (_T(SizeString), _T("Size"), wxOK | wxICON_INFORMATION,
    //  this);

    AppIniFile->WriteIniStringToFile(_T("SIZE"), SizeString);
  }

  delete (AppIniFile);    // Delete the .ini file

  free(pGain);
  free(pPhase);
  free(pGroupDelay);
  free(pImpulseResponse);
}

/********************************************************
 * Function : mainFrame::FindExecutablePath ()
 *
 * Parameters :
 *
 * Return value :
 * void
 *
 * Description : Find the directory path of the executable
 *       This function has been taken from the
 *       wxWindows web site and modified slightly
 *       because it does not scan the environment
 *       variable.
 * Find the absolute path where this application has been run from.
 * argv0 is wxTheApp->argv[0]
 * cwd is the current working directory (at startup)
 *
 ********************************************************/

wxString mainFrame::FindExecutablePath(const wxString& argv0, const wxString& cwd)

{
  wxString str;

  if (wxIsAbsolutePath(argv0)) {
    return wxPathOnly(argv0);
  } else {
    // Is it a relative path?
    wxString currentDir(cwd);

    if ((wxPathOnly(argv0) == ".") || (wxPathOnly(argv0) == ""))
      return (currentDir);

    if (currentDir.Last() != wxFILE_SEP_PATH)
      currentDir += wxFILE_SEP_PATH;

#if defined(__WXGTK__) || defined(__WXMOTIF__)
    str = currentDir + argv0;
#else
    str = currentDir + argv0 + wxString(".exe");
#endif
    //  SUF_Debugfprintf ("mainFrame::FindExecutablePath : str = \"%s\"\n",
    //  str);
    if (wxFileExists(str))
      return wxPathOnly(str);
  }

  // OK, it's neither an absolute path nor a relative path.
  // Search PATH.

  wxPathList pathList;
  pathList.AddEnvList(wxT("PATH"));
  str = pathList.FindAbsoluteValidPath(argv0);
  if (!str.IsEmpty())
    return wxPathOnly(str);

  // Failed
  return wxEmptyString;
}

/*
  Function Name : mainFrame::OnGetFilterSpec
  Description : Menu command (File|Project) to call up project dialog
  Notes :
*/

void mainFrame::OnGetFilterSpec(wxCommandEvent& WXUNUSED(event))

{
  //  wxMessageBox (_T("Got here"), "mainFrame::OnGetFilterSpec", wxOK |
  //  wxICON_EXCLAMATION, NULL);
  SUF_Log("mainFrame::OnGetFilterSpec\n");

  FilterGraphDisplay->ShowGraph(FALSE);                  // Hide the graph
  FilterGraphDisplay->EnableGraphTypeComboBox(FALSE);    // Disable until graph type selected

  wxString IniString;    // String from .ini file
                         // Check to see if size has prevoiusly been saved
  if (AppIniFile->ReadIniStringFromFile(_T("SAMPLE_RATE"), &IniString) == TRUE) {
    SLData_t SampleRate;
    sscanf(IniString, "%lf", &SampleRate);
    FilterSpecDialog->SetSampleRate(SampleRate);
  }

  if (AppIniFile->ReadIniStringFromFile(_T("PROJECT_NAME"), &IniString) == TRUE) {
    FilterSpecDialog->SetProjectNameString(IniString);
  }

  if (AppIniFile->ReadIniStringFromFile(_T("FILTER_TYPE"), &IniString) == TRUE) {
    int FilterType;
    sscanf(IniString, "%d", &FilterType);
    FilterSpecDialog->SetFilterType(FilterType);
  }

  FilterSpecDialog->ShowModal();    // Open the project dialog box

  // Gets here when dialog closed
  //  wxMessageBox (_T("Got here"), "mainFrame::OnGetFilterSpec\nThe project
  //  dialog has done", wxOK | wxICON_EXCLAMATION, NULL);

  // Get the filter spec from the dialog
  ProjectNameString = FilterSpecDialog->GetProjectNameString();
  FilterType = FilterSpecDialog->GetFilterType();

  SampleRate = FilterSpecDialog->GetSampleRate();
  char SRString[40];
  sprintf(SRString, "Sample rate = %1.2lf Hz", SampleRate);
  SetStatusText(SRString, 2);

  AppIniFile->WriteIniStringToFile(_T("PROJECT_NAME"), ProjectNameString);
  IniString.sprintf("%d",
                    FilterType);    // Write the new sample rate to the .ini file
  AppIniFile->WriteIniStringToFile(_T("FILTER_TYPE"), IniString);
  IniString.sprintf("%lf",
                    SampleRate);    // Write the new sample rate to the .ini file
  AppIniFile->WriteIniStringToFile(_T("SAMPLE_RATE"), IniString);

  //  Debug - print the project info
  //  wxMessageBox (ProjectNameString, "Project Name", wxOK |
  //  wxICON_EXCLAMATION, NULL); char DebugString[40]; sprintf (DebugString,
  //  "Sample rate = %1.2lf", SampleRate); wxMessageBox (DebugString, "Debug",
  //  wxOK | wxICON_EXCLAMATION, NULL); sprintf (DebugString, "Filter type = %d",
  //  FilterType); wxMessageBox (DebugString, "Debug", wxOK | wxICON_EXCLAMATION,
  //  NULL);

  FilterGraphDisplay->SetSampleRate(SampleRate);    // Set the sample rate to show on the graph

  if (FilterType == FIR_WINDOW_FILTER) {
    SetStatusText("Design Method : FIR, Windowing", 0);

    FIRWinDesign->SetProjectNameString(ProjectNameString);
    FIRWinDesign->SetSampleRate(SampleRate);

    FIRSimpleRemezDesign->Show(FALSE);
    FIRRemezDesign->Show(FALSE);
    FIRAlgsDesign->Show(FALSE);
    IIRPZPDesign->Show(FALSE);
    IIRTraditionalDesign->Show(FALSE);
    IIRAlgsDesign->Show(FALSE);
    FIRWinDesign->Show(TRUE);
    FIRWinDesign->SetFocus();

    FIRSimpleRemezDesign->Enable(FALSE);
    FIRRemezDesign->Enable(FALSE);
    FIRAlgsDesign->Enable(FALSE);
    IIRPZPDesign->Enable(FALSE);
    IIRTraditionalDesign->Enable(FALSE);
    IIRAlgsDesign->Enable(FALSE);
    FIRWinDesign->Enable(TRUE);
    FIRWinDesign->SetFocus();

    GlobalGraphDisplayWindow->ResetGraphType();    // Reset to magnitude graph - ensure no crash if
                                                   // previous was IIR PZP
  }

  else if (FilterType == FIR_SIMPLE_REMEZ_FILTER) {
    SetStatusText("Design Method : FIR, Simple Remez Exchange Algorithm", 0);

    FIRSimpleRemezDesign->SetProjectNameString(ProjectNameString);
    FIRSimpleRemezDesign->SetSampleRate(SampleRate);

    FIRWinDesign->Show(FALSE);
    FIRRemezDesign->Show(FALSE);
    FIRAlgsDesign->Show(FALSE);
    IIRPZPDesign->Show(FALSE);
    IIRTraditionalDesign->Show(FALSE);
    IIRAlgsDesign->Show(FALSE);
    FIRSimpleRemezDesign->Show(TRUE);
    FIRSimpleRemezDesign->SetFocus();

    FIRWinDesign->Enable(FALSE);
    FIRRemezDesign->Enable(FALSE);
    FIRAlgsDesign->Enable(FALSE);
    IIRPZPDesign->Enable(FALSE);
    IIRTraditionalDesign->Enable(FALSE);
    IIRAlgsDesign->Enable(FALSE);
    FIRSimpleRemezDesign->Enable(TRUE);
    FIRSimpleRemezDesign->SetFocus();

    GlobalGraphDisplayWindow->ResetGraphType();    // Reset to magnitude graph - ensure no crash if
                                                   // previous was IIR PZP
  }

  else if (FilterType == FIR_REMEZ_FILTER) {
    SetStatusText("Design Method : FIR, Remez Exchange Algorithm", 0);

    FIRRemezDesign->SetProjectNameString(ProjectNameString);
    FIRRemezDesign->SetSampleRate(SampleRate);

    FIRWinDesign->Show(FALSE);
    FIRSimpleRemezDesign->Show(FALSE);
    FIRAlgsDesign->Show(FALSE);
    IIRPZPDesign->Show(FALSE);
    IIRTraditionalDesign->Show(FALSE);
    IIRAlgsDesign->Show(FALSE);
    FIRRemezDesign->Show(TRUE);
    FIRRemezDesign->SetFocus();

    FIRWinDesign->Enable(FALSE);
    FIRSimpleRemezDesign->Enable(FALSE);
    FIRAlgsDesign->Enable(FALSE);
    IIRPZPDesign->Enable(FALSE);
    IIRTraditionalDesign->Enable(FALSE);
    IIRAlgsDesign->Enable(FALSE);
    FIRRemezDesign->Enable(TRUE);
    FIRRemezDesign->SetFocus();

    GlobalGraphDisplayWindow->ResetGraphType();    // Reset to magnitude graph - ensure no crash if
                                                   // previous was IIR PZP
  }

  else if (FilterType == FIR_ALGS_FILTER) {
    SetStatusText("Design Method : FIR, Deisgn Algorithms", 0);

    FIRAlgsDesign->SetProjectNameString(ProjectNameString);
    FIRAlgsDesign->SetSampleRate(SampleRate);

    FIRWinDesign->Show(FALSE);
    FIRSimpleRemezDesign->Show(FALSE);
    FIRRemezDesign->Show(FALSE);
    IIRPZPDesign->Show(FALSE);
    IIRTraditionalDesign->Show(FALSE);
    IIRAlgsDesign->Show(FALSE);
    FIRAlgsDesign->Show(TRUE);
    FIRAlgsDesign->SetFocus();

    FIRWinDesign->Enable(FALSE);
    FIRSimpleRemezDesign->Enable(FALSE);
    FIRRemezDesign->Enable(FALSE);
    IIRPZPDesign->Enable(FALSE);
    IIRTraditionalDesign->Enable(FALSE);
    IIRAlgsDesign->Enable(FALSE);
    FIRAlgsDesign->Enable(TRUE);
    FIRAlgsDesign->SetFocus();

    GlobalGraphDisplayWindow->ResetGraphType();    // Reset to magnitude graph - ensure no crash if
                                                   // previous was IIR PZP
  }

  else if (FilterType == IIR_PZP_FILTER) {
    SetStatusText("Design Method : IIR, Pole/Zero placement", 0);

    IIRPZPDesign->SetProjectNameString(ProjectNameString);
    IIRPZPDesign->SetSampleRate(SampleRate);

    FIRWinDesign->Show(FALSE);
    FIRSimpleRemezDesign->Show(FALSE);
    FIRRemezDesign->Show(FALSE);
    FIRAlgsDesign->Show(FALSE);
    IIRTraditionalDesign->Show(FALSE);
    IIRAlgsDesign->Show(FALSE);
    IIRPZPDesign->Show(TRUE);
    IIRPZPDesign->SetFocus();

    FIRWinDesign->Enable(FALSE);
    FIRSimpleRemezDesign->Enable(FALSE);
    FIRRemezDesign->Enable(FALSE);
    FIRAlgsDesign->Enable(FALSE);
    IIRTraditionalDesign->Enable(FALSE);
    IIRAlgsDesign->Enable(FALSE);
    IIRPZPDesign->Enable(TRUE);
    IIRPZPDesign->SetFocus();
  }

  else if (FilterType == IIR_FILTER) {
    SetStatusText("Design Method : Traditional IIR Filter", 0);

    IIRTraditionalDesign->SetProjectNameString(ProjectNameString);
    IIRTraditionalDesign->SetSampleRate(SampleRate);

    FIRWinDesign->Show(FALSE);
    FIRSimpleRemezDesign->Show(FALSE);
    FIRRemezDesign->Show(FALSE);
    FIRAlgsDesign->Show(FALSE);
    IIRPZPDesign->Show(FALSE);
    IIRAlgsDesign->Show(FALSE);
    IIRTraditionalDesign->Show(TRUE);
    IIRTraditionalDesign->SetFocus();

    FIRWinDesign->Enable(FALSE);
    FIRSimpleRemezDesign->Enable(FALSE);
    FIRRemezDesign->Enable(FALSE);
    FIRAlgsDesign->Enable(FALSE);
    IIRPZPDesign->Enable(FALSE);
    IIRAlgsDesign->Enable(FALSE);
    IIRTraditionalDesign->Enable(TRUE);
    IIRTraditionalDesign->SetFocus();
  }

  else if (FilterType == IIR_ALGS_FILTER) {
    SetStatusText("Design Method : FIR, Deisgn Algorithms", 0);

    IIRAlgsDesign->SetProjectNameString(ProjectNameString);
    IIRAlgsDesign->SetSampleRate(SampleRate);

    FIRWinDesign->Show(FALSE);
    FIRSimpleRemezDesign->Show(FALSE);
    FIRRemezDesign->Show(FALSE);
    FIRAlgsDesign->Show(FALSE);
    IIRPZPDesign->Show(FALSE);
    IIRTraditionalDesign->Show(FALSE);
    IIRAlgsDesign->Show(TRUE);
    IIRAlgsDesign->SetFocus();

    FIRWinDesign->Enable(FALSE);
    FIRSimpleRemezDesign->Enable(FALSE);
    FIRRemezDesign->Enable(FALSE);
    FIRAlgsDesign->Enable(FALSE);
    IIRPZPDesign->Enable(FALSE);
    IIRTraditionalDesign->Enable(FALSE);
    IIRAlgsDesign->Enable(TRUE);
    IIRAlgsDesign->SetFocus();
  }
}

/*
  Function Name : mainFrame::OnViewCoeffs
  Description : Menu command : View|Coeffs
  Notes :
*/

void mainFrame::OnViewCoeffs(wxCommandEvent& WXUNUSED(event))

{
  //  wxMessageBox (_T("Got here"), "mainFrame::OnViewCoeffs", wxOK |
  //  wxICON_EXCLAMATION, NULL);
  SUF_Log("mainFrame::OnViewCoeffs\n");

  static char FileNameString[500];
  wxString TextString;

  strcpy(FileNameString, ProjectNameString);    // Open and read the file
  strcat(FileNameString, ".dfc");

  wxTextFile file(FileNameString);
  if (file.Open()) {
    //    printf("Number of lines: %u\n", file.GetLineCount());

    for (TextString = file.GetFirstLine(); !file.Eof(); TextString += ("\n" + file.GetNextLine()))
      ;

    TextString += "\n";

    TextDisplayDialog->SetText(TextString);                    // Set the text display text
    TextDisplayDialog->SetTitle(_T("Filter Coefficients"));    // Set the text display title
    TextDisplayDialog->Show(TRUE);                             // Open the text display dialog box

    file.Close();
  } else {
    wxMessageBox("Unable to open project coefficient file\n\nAre you sure you "
                 "have clicked the calculate button ?",
                 "Project file error", wxOK | wxICON_EXCLAMATION, NULL);
  }
}

/*
  Function Name : mainFrame::OnViewSpec
  Description : Menu command : View|Spec
  Notes :
*/

void mainFrame::OnViewSpec(wxCommandEvent& WXUNUSED(event))

{
  //  wxMessageBox (_T("Got here"), "mainFrame::OnViewSpec", wxOK |
  //  wxICON_EXCLAMATION, NULL);
  SUF_Log("mainFrame::OnViewSpec\n");

  static char FileNameString[500];
  wxString TextString;

  strcpy(FileNameString, ProjectNameString);    // Open and read the file
  strcat(FileNameString, ".dfs");

  wxTextFile file(FileNameString);
  if (file.Open()) {
    //    printf("Number of lines: %u\n", file.GetLineCount());

    for (TextString = file.GetFirstLine(); !file.Eof(); TextString += ("\n" + file.GetNextLine()))
      ;

    TextString += "\n";

    TextDisplayDialog->SetText(TextString);                     // Set the text display text
    TextDisplayDialog->SetTitle(_T("Filter Specification"));    // Set the text display title
    TextDisplayDialog->Show(TRUE);                              // Open the text display dialog box

    file.Close();
  } else {
    wxMessageBox("Unable to open project specification file\n\nAre you sure "
                 "you have clicked the calculate button ?",
                 "Project file error", wxOK | wxICON_EXCLAMATION, NULL);
  }
}

/*
  Function Name : mainFrame::OnViewZPlane
  Description : Menu command : View|z-Plane
  Notes :
*/

void mainFrame::OnViewZPlane(wxCommandEvent& WXUNUSED(event))

{
  //  wxMessageBox (_T("Got here"), "mainFrame::OnViewZPlane", wxOK |
  //  wxICON_EXCLAMATION, NULL);
  SUF_Log("mainFrame::OnViewZPlane\n");

  static char FileNameString[500];
  wxString TextString;

  strcpy(FileNameString, ProjectNameString);    // Open and read the file
  strcat(FileNameString, ".dfz");

  wxTextFile file(FileNameString);
  if (file.Open()) {
    //    printf("Number of lines: %u\n", file.GetLineCount());

    for (TextString = file.GetFirstLine(); !file.Eof(); TextString += ("\n" + file.GetNextLine()))
      ;

    TextString += "\n";

    TextDisplayDialog->SetText(TextString);                             // Set the text display text
    TextDisplayDialog->SetTitle(_T("Filter z-Domain Coefficients"));    // Set the text display title
    TextDisplayDialog->Show(TRUE);                                      // Open the text display dialog box

    file.Close();
  } else {
    wxMessageBox("Unable to open project specification file\n\nAre you sure "
                 "you have clicked the calculate button ?",
                 "Project file error", wxOK | wxICON_EXCLAMATION, NULL);
  }
}

/*
  Function Name : mainFrame::OnOptionsMenu
  Description : Menu command : Options| Options
  Notes :
*/

void mainFrame::OnOptionsMenu(wxCommandEvent& WXUNUSED(event))

{
  //  wxMessageBox (_T("Got here"), "mainFrame::OnOptionsMenu", wxOK |
  //  wxICON_EXCLAMATION, NULL);
  SUF_Log("mainFrame::OnOptionsMenu\n");

  GetOptionsDialog* ThisOptionsDialog = new GetOptionsDialog(this, -1, "Digital Filter Plus Options");

  wxString mString;    // Strings from .ini file
  wxString nString;
  wxString GScaleString;
  wxString NegateIIRAlphaSignString;

  // Check to see if negate Biquad alpha coeffs for MAC has previously been
  // saved
  if (AppIniFile->ReadIniStringFromFile(_T("NEGATE_IIR_ALPHA_SIGN"), &NegateIIRAlphaSignString) == TRUE) {
    int nLocalValue;
    sscanf(NegateIIRAlphaSignString, "%d", &nLocalValue);
    ThisOptionsDialog->SetNegateIIRAlphaSign(nLocalValue);
    global_NegateIIRAlphaSign = nLocalValue;
  }

  // Check to see if m has previously been saved
  if (AppIniFile->ReadIniStringFromFile(_T("M_VALUE"), &mString) == TRUE) {
    int mLocalValue;
    sscanf(mString, "%d", &mLocalValue);
    ThisOptionsDialog->SetM(mLocalValue);
  }

  // Check to see if n has previously been saved
  if (AppIniFile->ReadIniStringFromFile(_T("N_VALUE"), &nString) == TRUE) {
    int nLocalValue;
    sscanf(nString, "%d", &nLocalValue);
    ThisOptionsDialog->SetN(nLocalValue);
  }

  // Check to see if graph scale has previously been saved
  if (AppIniFile->ReadIniStringFromFile(_T("GRAPH_SCALE"), &GScaleString) == TRUE) {
    SLData_t GScale;
    sscanf(GScaleString, "%lf", &GScale);
    ThisOptionsDialog->SetScale(GScale);
  }

  if (ThisOptionsDialog->ShowModal() == 1)    // Open the graph options dialog box
  {
    // If options returned, process them
    // NegateIIRAlphaSign
    if (ThisOptionsDialog->GetNegateIIRAlphaSign() == 1) {    // Now get the NegateIIRAlphaSign from the dialog
      global_NegateIIRAlphaSign = 1;                          // Set the global NegateIIRAlphaSign
      AppIniFile->WriteIniStringToFile(_T("NEGATE_IIR_ALPHA_SIGN"), _T("1"));
    } else {
      global_NegateIIRAlphaSign = 0;    // Set the global NegateIIRAlphaSign
      AppIniFile->WriteIniStringToFile(_T("NEGATE_IIR_ALPHA_SIGN"), _T("0"));
    }

    // mValue
    MFmValue = ThisOptionsDialog->GetM();    // Now get the mValue from the dialog
    global_mValue = MFmValue;                // Set the global mValue
    mString.sprintf("%d", MFmValue);         // Write the new m Value to the .ini file
    AppIniFile->WriteIniStringToFile(_T("M_VALUE"), mString);

    // nValue
    MFnValue = ThisOptionsDialog->GetN();    // Now get the nValue from the dialog
    global_nValue = MFnValue;                // Set the global nValue
    nString.sprintf("%d", MFnValue);         // Write the new m Value to the .ini file
    AppIniFile->WriteIniStringToFile(_T("N_VALUE"), nString);

    // Graph scale
    LogMagnGraphScale = ThisOptionsDialog->GetScale();              // Now get the scale from the dialog
    FilterGraphDisplay->SetLogMagnGraphScale(LogMagnGraphScale);    // Set the graph scale
    GScaleString.sprintf("%lf", LogMagnGraphScale);                 // Write the new graph scale to the .ini file
    AppIniFile->WriteIniStringToFile(_T("GRAPH_SCALE"), GScaleString);
  }

  wxCommandEvent* DummyCommand = new wxCommandEvent();    // Post event to say that the graph should be
                                                          // updated
  GlobalGraphDisplayWindow->OnGraphTypeChanged(*DummyCommand);
  delete (DummyCommand);

  delete (ThisOptionsDialog);
}

/*
  Function Name : mainFrame::OnOpenUsersGuide
  Description : Menu command : Help|Users Guide
  Notes :
*/

void mainFrame::OnOpenUsersGuide(wxCommandEvent& WXUNUSED(event))

{
  //  wxMessageBox (_T("Got here"), "mainFrame::OnOpenUsersGuide", wxOK |
  //  wxICON_EXCLAMATION, NULL);
  SUF_Log("mainFrame::OnOpenUsersGuide\n");

  const wxString fname = InstallDirectoryPathString + "DFPUsers.pdf";

#if DEBUG_LOG_FILE
  SUF_Debugfprintf("PDF file path : %s\n", fname.GetData());
#endif

#if defined(__APPLE__)
  // wxString ExeString;
  wxString cmd;
  // if (AppIniFile->ReadIniStringFromFile (_T("LINUX_BROWSER_PATH"),
  // &ExeString) == TRUE) {
  // cmd = "\"" + ExeString + "\"" + "\"" + fname + "\"";
  cmd = "open " + fname;
  wxExecute(cmd.c_str());
  // }

  //#elif defined (__linux__)           // NOT REQUIRED - the standard method
  // works in v3.x
  //  wxString ExeString;
  //  wxString cmd;
  //  if (AppIniFile->ReadIniStringFromFile (wxT("LINUX_PDF_PATH"), &ExeString) == TRUE) {
  //    cmd = ExeString + wxT(" ") + fname;
  //    if ( wxExecute (cmd.c_str()) != 0 ) {
  //      wxMessageBox ("Unable to open User's Guide\nPlease ensure you have a
  //      PDF file reader correctly installed", "DFPlus Warning", wxOK |
  //      wxICON_EXCLAMATION, NULL);
  //    }
  //  }
  //  else {
  //    wxMessageBox ("Unable to open User's Guide\nPlease ensure you have a PDF
  //    file reader correctly installed", "DFPlus Warning", wxOK |
  //    wxICON_EXCLAMATION, NULL);
  //  }

#else
  wxMimeTypesManager g_mimeManager;

  wxString ext = fname.AfterLast(_T('.'));
  wxFileType* ft = g_mimeManager.GetFileTypeFromExtension(ext);
  if (!ft) {
    wxMessageBox("Unable to open User's Guide\nPlease ensure you have a PDF "
                 "file reader correctly installed",
                 "DFPlus Warning", wxOK | wxICON_EXCLAMATION, NULL);
  } else {
    //    wxString desc;
    //    if ( !ft->GetDescription(&desc) )
    //      desc = _T("<no description>");

    wxString cmd;

    if (!ft->GetOpenCommand(&cmd, wxFileType::MessageParameters(fname, _T("")))) {
      wxMessageBox("Unable to open User's Guide\nPlease ensure you have a PDF "
                   "file reader correctly installed",
                   "DFPlus Warning", wxOK | wxICON_EXCLAMATION, NULL);
    } else {
      wxExecute(cmd.c_str());
    }

    delete ft;
  }
#endif
}

/*
  Function Name : mainFrame::About
  Description : Menu command : Help|About
  Notes :
*/

void mainFrame::OnAbout(wxCommandEvent& WXUNUSED(event))

{
  char String[250];
  SUF_Log("mainFrame::OnAbout\n");
  sprintf(String, "Digital Filter Plus Version%s\n", DFPLUS_VERSION);
  wxMessageBox(String, "About Digital Filter Plus", wxOK | wxICON_INFORMATION, NULL);
}

/*
  Function Name : mainFrame::OnExit
  Description : Called from exit menu
  Notes :
*/

void mainFrame::OnExit(wxCommandEvent& WXUNUSED(event))
{
  SUF_Log("mainFrame::OnExit\n");
  Close(TRUE);
}

/*
  Function Name : mainFrame::OnSize
  Description : Called when size changes
  Notes :
*/

void mainFrame::OnSize(wxSizeEvent& event)
{
  // wxMessageBox ("Got Here", "mainFrame::OnSize", wxOK | wxICON_EXCLAMATION,
  // NULL);
  SUF_Log("mainFrame::OnSize\n");

  if (ChildWindowsCreated == TRUE) {    // Only adjust size of child windows, if they have been created
    wxSize ClientSize = GetClientSize();
    SplitterWindow->SetSize(ClientSize);
    SplitterWindow->SetMinimumPaneSize((ClientSize.GetWidth() * 3) / 10);
    // Split the screen vertically and allocate the space
    SplitterWindow->SetSashPosition((ClientSize.GetWidth() * 3) / 10);

    // Debug code
    // FILE *fp;
    // fp = fopen ("debug.log", "a+");
    // fprintf (fp, "Client Size = %d\n", ClientSize.GetWidth());
    // fclose (fp);
  }

  event.Skip();    // Skip original event handler
}

/*
  Function Name : mainFrame::DisplayStatusLineText
  Description : Display text for graph
  Notes :
*/

void mainFrame::DisplayStatusLineText(wxString& string)
{
  //  wxMessageBox ("Got Here", "mainFrame::DisplayGraphText", wxOK |
  //  wxICON_EXCLAMATION, NULL);

  SetStatusText(string,
                1);    // This prints the location and magnitude at the cursor
}

BEGIN_EVENT_TABLE(mainFrame, wxFrame)
EVT_MENU(DFPLUS_GET_FILTER_SPEC, mainFrame::OnGetFilterSpec)
EVT_MENU(DFPLUS_VIEW_COEFFS, mainFrame::OnViewCoeffs)
EVT_MENU(DFPLUS_VIEW_SPEC, mainFrame::OnViewSpec)
EVT_MENU(DFPLUS_VIEW_ZPLANE, mainFrame::OnViewZPlane)
EVT_MENU(DFPLUS_OPTIONS, mainFrame::OnOptionsMenu)
EVT_MENU(DFPLUS_OPEN_USERS_GUIDE, mainFrame::OnOpenUsersGuide)
EVT_MENU(DFPLUS_ABOUT, mainFrame::OnAbout)
EVT_MENU(DFPLUS_EXIT, mainFrame::OnExit)
EVT_TOOL(DFPLUS_TOOLBAR_NEW, mainFrame::OnGetFilterSpec)
EVT_TOOL(DFPLUS_TOOLBAR_VIEW_COEFFS, mainFrame::OnViewCoeffs)
EVT_TOOL(DFPLUS_TOOLBAR_VIEW_SPEC, mainFrame::OnViewSpec)
EVT_TOOL(DFPLUS_TOOLBAR_VIEW_ZPLANE, mainFrame::OnViewZPlane)
EVT_TOOL(DFPLUS_TOOLBAR_VIEW_OPTIONS, mainFrame::OnOptionsMenu)
EVT_TOOL(DFPLUS_TOOLBAR_VIEW_OPEN_USERS_GUIDE, mainFrame::OnOpenUsersGuide)
EVT_TOOL(DFPLUS_TOOLBAR_VIEW_ABOUT, mainFrame::OnAbout)
EVT_TOOL(DFPLUS_TOOLBAR_EXIT, mainFrame::OnExit)
EVT_SIZE(mainFrame::OnSize)
END_EVENT_TABLE()

DisplayTextDialog::DisplayTextDialog(wxFrame* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxDialog(parent, id, title, pos, size, wxCLOSE_BOX | wxCAPTION | wxSYSTEM_MENU | wxRESIZE_BORDER)
{
  //  wxMessageBox ("Got Here", "DisplayTextDialog", wxOK | wxICON_EXCLAMATION,
  //  NULL);

  // Get size for text control
  wxSize TextBoxSize = GetSize();
  TextBoxSize.SetWidth(TextBoxSize.GetWidth() - 7);
  TextBoxSize.SetHeight(TextBoxSize.GetHeight() - 27);

  // Create text control
  TextBox = new wxTextCtrl(this, -1, "", wxDefaultPosition, TextBoxSize, wxTE_MULTILINE | wxTE_READONLY | wxHSCROLL);

  TextBox->SetFont(wxFont(8, wxMODERN, wxNORMAL, wxNORMAL, FALSE, "", wxFONTENCODING_SYSTEM));
}

void DisplayTextDialog::SetText(wxString& string)
{
  //  wxMessageBox ("Got Here", "DisplayTextDialog::SetText", wxOK |
  //  wxICON_EXCLAMATION, NULL);

  TextBox->SetValue(string);
}

void DisplayTextDialog::OnSize(wxSizeEvent& event)
{
  //  wxMessageBox ("Got Here", "DisplayTextDialog::OnSize", wxOK |
  //  wxICON_EXCLAMATION, NULL);
  SUF_Log("mainFrame::OnSize\n");

  // Get size for text control
  wxSize TextBoxSize = GetSize();
  TextBoxSize.SetWidth(TextBoxSize.GetWidth() - 7);
  TextBoxSize.SetHeight(TextBoxSize.GetHeight() - 27);

  // Adjust text control size
  TextBox->SetSize(TextBoxSize);

  event.Skip();    // Skip original event handler
}

BEGIN_EVENT_TABLE(DisplayTextDialog, wxDialog)
EVT_SIZE(DisplayTextDialog::OnSize)
END_EVENT_TABLE()
