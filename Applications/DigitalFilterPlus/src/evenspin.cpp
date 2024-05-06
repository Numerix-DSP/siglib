// Spin control function that supports only even values
// This software is provided under the terms of the GNU General Public License
// V2, as published by the Free Software Foundation. Copyright (c) 1999-2024
// Delta Numerix, All rights reserved.

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

#include "evenspin.h"

/**/
/********************************************************
 * Function : EvenSpinCtrl::EvenSpinCtrl ()
 *
 * Parameters :
 *
 * Return value :
 * void
 *
 * Description : EvenSpinCtrl constructor
 *
 ********************************************************/

EvenSpinCtrl::EvenSpinCtrl(wxWindow* parent, wxWindowID id, const wxString& value, const wxPoint& pos, const wxSize& size, long style, short min,
                           short max, short initial, const wxString& name)
    : wxSpinCtrl(parent, id, value, pos, size, style, min, max, initial, name)

{
  //  wxMessageBox ("Got Here", "EvenSpinCtrl::EvenSpinCtrl", wxOK |
  //  wxICON_EXCLAMATION, NULL);

  PreviousValue = initial;
}

/********************************************************
 * Function : EvenSpinCtrl::OnSpinClicked ()
 *
 * Parameters :
 *
 * Return value :
 * void
 *
 * Description : Ensure that value is always an even number
 *
 ********************************************************/

// void EvenSpinCtrl::OnSpinClicked (wxSpinEvent& event)
void EvenSpinCtrl::OnSpinClicked(wxSpinEvent& WXUNUSED(event))

{
  //  wxMessageBox ("Got Here", "EvenSpinCtrl::OnSpinClicked", wxOK |
  //  wxICON_EXCLAMATION, NULL);

  if (((GetValue() - PreviousValue) > 1) || ((PreviousValue - GetValue()) > 1))
    SetValue(GetValue() & 0x0fffe);

  else if (((GetValue() - PreviousValue) == 1))
    SetValue((GetValue() + 1) & 0x0fffe);

  else if (((GetValue() - PreviousValue) == -1))
    SetValue((GetValue() - 1) & 0x0fffe);

  PreviousValue = GetValue();

  //  event.Skip();
}

#if COMMENTsdfgdfsgd
/********************************************************
 * Function : EvenSpinCtrl::OnValueChanged ()
 *
 * Parameters :
 *
 * Return value :
 * void
 *
 * Description : Ensure that value is always an even number
 *
 ********************************************************/

// void EvenSpinCtrl::OnValueChanged (wxCommandEvent& event)
void EvenSpinCtrl::OnValueChanged(wxCommandEvent& WXUNUSED(event))

{
  wxMessageBox("Got Here", "EvenSpinCtrl::OnValueChanged", wxOK | wxICON_EXCLAMATION, NULL);
  SUF_Log("EvenSpinCtrl::OnValueChanged\n");

  if (((GetValue() - PreviousValue) > 1) || ((PreviousValue - GetValue()) > 1))
    SetValue(GetValue() & 0x0fffe);

  else if (((GetValue() - PreviousValue) == 1))
    SetValue((GetValue() + 1) & 0x0fffe);

  else if (((GetValue() - PreviousValue) == -1))
    SetValue((GetValue() - 1) & 0x0fffe);

  PreviousValue = GetValue();

  //  event.Skip();
}
#endif

// Set the value - if it is odd then increment by 1
void EvenSpinCtrl::SetValue(int value)

{
  //  wxMessageBox ("Got Here", "EvenSpinCtrl::SetValue", wxOK |
  //  wxICON_EXCLAMATION, NULL);

  if (value & 0x1) {
    PreviousValue = value + 1;
    wxSpinCtrl::SetValue(value + 1);
  } else {
    PreviousValue = value;
    wxSpinCtrl::SetValue(value);
  }
}

#ifdef __VISUALC__
int EvenSpinCtrl::GetValue(void)
#else
int EvenSpinCtrl::GetValue(void) const
#endif

{
  //  wxMessageBox ("Got Here", "EvenSpinCtrl::GetValue", wxOK |
  //  wxICON_EXCLAMATION, NULL);
  int value;

  value = wxSpinCtrl::GetValue();    // Ensure value is even number
  if (value & 0x1) {
    value++;
#ifdef __VISUALC__
    SetValue(value);
#else
    wxSpinCtrl::SetValue(value);
#endif
  }
  return (value);
}

BEGIN_EVENT_TABLE(EvenSpinCtrl, wxSpinCtrl)
EVT_SPINCTRL(-1, EvenSpinCtrl::OnSpinClicked)
////  EVT_TEXT    (-1, EvenSpinCtrl::OnValueChanged)
//  EVT_TEXT_ENTER  (-1, EvenSpinCtrl::OnValueChanged)
END_EVENT_TABLE()
