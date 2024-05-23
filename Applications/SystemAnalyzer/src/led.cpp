// LED Demo
// This software is provided under the terms of the GNU General Public License
// V2, as published by the Free Software Foundation. Copyright (c) 1999-2024
// Delta Numerix, All rights reserved.

#ifdef __GNUG__
#  pragma implementation "leddemo.cpp"
#  pragma interface "leddemo.cpp"
#endif /* */

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#  include "wx/wx.h"
#endif
#include "wx/image.h"

#include <siglib.h>
#include "led.h"

#include "BlueLEDOn.xpm"
#include "CyanLEDOn.xpm"
#include "GreenLEDOn.xpm"
#include "LEDOff.xpm"
#include "RedLEDOn.xpm"
#include "WhiteLEDOn.xpm"
#include "YellowLEDOn.xpm"

// Start of LED code
// Define LED constructor
LED::LED(wxWindow* parent, wxWindowID id, int LEDNum)
    : wxPanel(parent, id)
{
  LEDNumber = LEDNum;
  switch (LEDNumber) {
  case BLUE_LED:
    LEDOnBitmap = new wxBitmap(BlueLEDOn_xpm);
    break;
  case CYAN_LED:
    LEDOnBitmap = new wxBitmap(CyanLEDOn_xpm);
    break;
  case GREEN_LED:
    LEDOnBitmap = new wxBitmap(GreenLEDOn_xpm);
    break;
  case RED_LED:
    LEDOnBitmap = new wxBitmap(RedLEDOn_xpm);
    break;
  case WHITE_LED:
    LEDOnBitmap = new wxBitmap(WhiteLEDOn_xpm);
    break;
  case YELLOW_LED:
    LEDOnBitmap = new wxBitmap(YellowLEDOn_xpm);
    break;
  default:
    break;
  }
  LEDOffBitmap = new wxBitmap(LEDOff_xpm);
  LEDBitmap = LEDOffBitmap;    // Initialise LED off
  LEDBitmap->SetMask(new wxMask(*LEDBitmap, wxColour(LED_TRANSPARENT_RED, LED_TRANSPARENT_GREEN, LED_TRANSPARENT_BLUE)));
  SetSize(wxSize(LEDBitmap->GetWidth(), LEDBitmap->GetHeight()));

  // Initialise LED state
  LEDOnFlag = TRUE;         // LED permanently on
  LEDFlashFlag = FALSE;     // LED not flashing
  LEDFlashState = FALSE;    // LED flash state off
  canvas = new LEDCanvas(this, this, wxDefaultPosition, GetSize());
  canvas->SetSize(GetSize());
  m_timer = new wxTimer(this, LED_TIMER_ID);
  canvas->Refresh();
}

// LED destructor
LED::~LED()
{
  if (LEDOnBitmap->IsOk()) {
    delete LEDOnBitmap;
    LEDOnBitmap = (wxBitmap*)NULL;
  }
  if (LEDOffBitmap->IsOk()) {
    delete LEDOffBitmap;
    LEDOffBitmap = (wxBitmap*)NULL;
  }
}

void LED::Draw(wxDC& dc, bool WXUNUSED(draw_bitmaps))
{
  if (LEDBitmap->IsOk()) {
    wxMemoryDC memDC;
    memDC.SelectObject(*LEDBitmap);
    // Transparent blitting if there's a mask in the bitmap
    dc.Blit(0, 0, LEDBitmap->GetWidth(), LEDBitmap->GetHeight(), &memDC, 0, 0, wxCOPY, TRUE);
    memDC.SelectObject(wxNullBitmap);
  }
}
void LED::On(bool On)
{
  if (On == TRUE) {
    LEDOnFlag = TRUE;
  }

  else {
    LEDOnFlag = FALSE;
  }
  PowerOn();
}

void LED::Flash(bool Flash)
{
  //  wxMessageBox (_T("Got here"), _T("MyFrame::Flash"), wxOK |
  //  wxICON_INFORMATION, this);
  if (Flash == TRUE) {
    LEDFlashFlag = TRUE;
    m_timer->Start(1000);    // 1 second interval
  }

  else {
    LEDFlashFlag = FALSE;
    m_timer->Stop();    // Stop the timer
  }
  PowerOn();
}

void LED::PowerOn(void)
{
  bool PowerOnFlag;
  if (LEDOnFlag == TRUE)    // LED is On
  {
    if (LEDFlashFlag == TRUE)    // LED is set to flash
    {
      if (LEDFlashState == FALSE) {
        PowerOnFlag = FALSE;
      }

      else {
        PowerOnFlag = TRUE;
      }
    }

    else    // LED is permanently on
    {
      PowerOnFlag = TRUE;
    }
  }

  else    // LED is off
  {
    PowerOnFlag = FALSE;
  }
  if (PowerOnFlag == TRUE) {
    LEDBitmap = LEDOnBitmap;
  }

  else {
    LEDBitmap = LEDOffBitmap;
  }
  if (LEDBitmap->IsOk()) {
    // Set mask for transparent area
    LEDBitmap->SetMask(new wxMask(*LEDBitmap, wxColour(LED_TRANSPARENT_RED, LED_TRANSPARENT_GREEN, LED_TRANSPARENT_BLUE)));
    canvas->Refresh();
  }
}

void LED::OnTimer(wxTimerEvent& WXUNUSED(event))
{
  //  wxMessageBox (_T("Got here"), _T("MyFrame::OnTimer"), wxOK |
  //  wxICON_INFORMATION, this);
  if (LEDFlashState == FALSE)    // If LED is off then turn it on
  {
    LEDFlashState = TRUE;
  }

  else {
    LEDFlashState = FALSE;
  }
  PowerOn();
}

BEGIN_EVENT_TABLE(LED, wxPanel)
EVT_TIMER(LED_TIMER_ID, LED::OnTimer)
END_EVENT_TABLE()
// Define a constructor for my canvas
LEDCanvas::LEDCanvas(LED* pLED, wxWindow* parent, const wxPoint& pos, const wxSize& size)
    : wxWindow(parent, -1, pos, size)
{
  Size = size;
  SetSize(size);    // Set the size of the canvas because it is not done by parent
                    // unless it is a frame
  m_owner = parent;
  m_LED = pLED;
}
LEDCanvas::~LEDCanvas(void) {}

// Define the repainting behaviour
void LEDCanvas::OnPaint(wxPaintEvent& WXUNUSED(event))
{
  wxPaintDC dc(this);
  m_LED->PrepareDC(dc);
  m_LED->Draw(dc, TRUE);
}
BEGIN_EVENT_TABLE(LEDCanvas, wxWindow)
EVT_PAINT(LEDCanvas::OnPaint)
END_EVENT_TABLE()
