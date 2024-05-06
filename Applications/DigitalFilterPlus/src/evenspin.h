// EvenSpinCtrl include file
// This software is provided under the terms of the GNU General Public License
// V2, as published by the Free Software Foundation. Copyright (c) 1999-2024
// Delta Numerix, All rights reserved.

#ifndef _EVENSPIN_H
#define _EVENSPIN_H

#include "wx/spinctrl.h"

class EvenSpinCtrl : public wxSpinCtrl

{
    public:
  EvenSpinCtrl(wxWindow* parent, wxWindowID id = -1, const wxString& value = wxEmptyString, const wxPoint& pos = wxDefaultPosition,
               const wxSize& size = wxDefaultSize, long style = wxSP_ARROW_KEYS, short min = 0, short max = 100, short initial = 0,
               const wxString& name = _T("wxSpinCtrl"));

  void OnSpinClicked(wxSpinEvent& event);
  //  void OnValueChanged (wxCommandEvent& event);

  virtual void SetValue(int value);
#ifdef __VISUALC__
  virtual int GetValue(void);
#else
  virtual int GetValue(void) const;
#endif

  DECLARE_EVENT_TABLE()

    private:
  int PreviousValue;
};

#endif    // End of #ifndef _EVENSPIN_H
