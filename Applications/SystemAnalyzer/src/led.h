// LED include file
// This functionality supports transparent objects, where the transparent colour is :
// Red : 251, Green : 0, Blue : 251
// Copyright (c) 1999-2020, Sigma Numerix Ltd, All rights reserved

#ifndef     _LED_H
#define     _LED_H

#define NUMBER_OF_LEDS          6

#define LED_TRANSPARENT_RED     251
#define LED_TRANSPARENT_GREEN   0
#define LED_TRANSPARENT_BLUE    251

#define LED_TIMER_ID            201

enum {
  RED_LED,
  GREEN_LED,
  BLUE_LED,
  YELLOW_LED,
  CYAN_LED,
  WHITE_LED
};


class           LEDCanvas;


class           LED:public wxPanel {
public:
  LED (
  wxWindow * parent,
  wxWindowID id,
  int LEDNum);
                 ~LED (
  void);

  void            On (
  bool On = TRUE);
  void            Flash (
  bool Flash = TRUE);
  void            Draw (
  wxDC & dc,
  bool draw_bitmaps = TRUE);
  void            OnTimer (
  wxTimerEvent & event);

private:
  void            PowerOn (
  void);

  int             LEDNumber;
  wxBitmap       *LEDBitmap;
  wxBitmap       *LEDOnBitmap;
  wxBitmap       *LEDOffBitmap;
  LEDCanvas      *canvas;
  bool            LEDOnFlag;
  bool            LEDFlashFlag;
  bool            LEDFlashState;
  wxTimer        *m_timer;

                  DECLARE_EVENT_TABLE (
    )
};



// Define a new canvas which can receive some events
class           LEDCanvas:public wxWindow {
public:
  LEDCanvas (
  LED * pLED,
  wxWindow * parent,
  const wxPoint & pos,
  const wxSize & size);
                 ~LEDCanvas (
  void);

  void            OnPaint (
  wxPaintEvent & event);

private:
                  wxSize Size;
  wxWindow       *m_owner;
  LED            *m_LED;

                  DECLARE_EVENT_TABLE (
    )
};

#endif                                                              // End of #ifndef _LED_H
