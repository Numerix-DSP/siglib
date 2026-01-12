// .ini file read / write functions

// The .ini file class allows the processing of a .ini file that will allow the
// storing and retrieval of status and configuration information. The
// information is stored in a given file name and has the format : [KeyString]
// InformationString

// Where the KeyString signifies the type of information that is being stored
// and the InformationString contains the specific information being stored.

// InformationStrings can be overwritten with new data and new KeyStrings are
// appended to the end of the file with the appropriate data.

// This software is provided under the terms of the GNU General Public License
// V2, as published by the Free Software Foundation.
// Copyright (c) 1999-2026 Delta Numerix, All rights reserved.

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

#include "inifile.h"
#include <wx/textfile.h>

/********************************************************
 * Function : IniFile::IniFile ()
 *
 * Parameters :
 *
 * Return value :
 * void
 *
 * Description : ini file constructor
 *
 ********************************************************/

IniFile::IniFile(wxString FileNameString)
{
  IniFileName = FileNameString;

  // Debug
  //  wxMessageBox (IniFileName, _T("INI File Name"), wxOK | wxICON_INFORMATION,
  //  NULL);
}

/********************************************************
 * Function : IniFile::~IniFile ()
 *
 * Parameters :
 *
 * Return value :
 * void
 *
 * Description : ini file destructor
 *
 ********************************************************/

IniFile::~IniFile() {}

/********************************************************
 * Function : IniFile::ReadIniStringFromFile ()
 *
 * Parameters :
 *
 * Return value :
 * void
 *
 * Description : Read a configuration parameter from ini file
 *
 ********************************************************/

bool IniFile::ReadIniStringFromFile(wxString Key, wxString* String)

{
  wxString KeySearchString = wxString("[") + Key + wxString("]");

  // Debug
  //  wxMessageBox(_T(KeySearchString), _T("Search string"), wxOK |
  //  wxICON_INFORMATION, NULL);

  // Debug
  //  wxMessageBox (IniFileName, _T("INI File Name"), wxOK | wxICON_INFORMATION,
  //  NULL);

  // Search for required string
  // wxTextFile file (_T(IniFileName));
  wxTextFile file(IniFileName);

  if (!file.Exists()) {
    // Debug
    //    wxMessageBox(_T("Requested .ini file not installed"),
    //      _T("File I/O error"), wxOK | wxICON_INFORMATION, NULL);
    return (FALSE);
  }

  if (file.Open()) {
    wxString s;

    if (file.Eof()) {
      //      wxMessageBox(_T("This .ini file is empty"),
      //        _T("File I/O error"), wxOK | wxICON_INFORMATION, NULL);
      return (FALSE);
    }
    for (s = file.GetFirstLine(); !file.Eof(); s = file.GetNextLine()) {
      // Check each line against Key, if same, return success !

      if (s == KeySearchString) {
        // Debug
        //        wxMessageBox(_T("String match"),
        //        _T("IniFile::ReadIniStringFromFile"), wxOK |
        //        wxICON_INFORMATION, NULL);

        *String = file.GetNextLine();    // Get String

        if (file.Close() == FALSE) {
          wxMessageBox(_T("Can not close .ini file"), _T("File I/O error"), wxOK | wxICON_INFORMATION, NULL);
          return (FALSE);
        }
        return (TRUE);
      }
    }
    // Ordinarily should do last line here but last line can't be a KEY
  } else {
    //    wxMessageBox(_T("Can not open .ini file"),
    //           _T("File I/O error"), wxOK | wxICON_INFORMATION, NULL);
    return (FALSE);
  }

  // Debug
  //  wxMessageBox(_T("Key not found"), _T("IniFile::ReadIniStringFromFile"),
  //  wxOK | wxICON_INFORMATION, NULL);

  // If KEY not present, KEY has not been found

  if (file.Close() == FALSE) {
    wxMessageBox(_T("Can not close .ini file"), _T("File I/O error"), wxOK | wxICON_INFORMATION, NULL);
    return (FALSE);
  }

  return (FALSE);
}

/********************************************************
 * Function : IniFile::WriteIniStringToFile ()
 *
 * Parameters :
 *
 * Return value :
 * void
 *
 * Description : Write a configuration parameter to ini file
 *
 ********************************************************/

bool IniFile::WriteIniStringToFile(wxString Key, wxString String)
{
  wxString KeySearchString = wxString("[") + Key + wxString("]");

  bool KeyPresentFlag = FALSE;    // Flag set if key found

  // Debug
  //  wxMessageBox(_T(KeySearchString), _T("Search string"), wxOK |
  //  wxICON_INFORMATION, NULL);

  // Debug
  //  wxMessageBox (IniFileName, _T("INI File Name"), wxOK | wxICON_INFORMATION,
  //  NULL);

  // Search for required string
  // wxTextFile file(_T(IniFileName));
  wxTextFile file(IniFileName);

  if (!file.Exists()) {
    // The requested .ini file was not installed so we will create it
    if (file.Create() == FALSE) {
      wxMessageBox(_T("Can not create .ini file"), _T("File I/O error"), wxOK | wxICON_INFORMATION, NULL);
      return (FALSE);
    }
  }

  else    // File does exist so search for existing string
  {
    if (file.Open()) {
      wxString s;

      if (!file.Eof())    // Make sure it is nopt an empty file
      {
        for (s = file.GetFirstLine(); !file.Eof(); s = file.GetNextLine()) {
          // Check each line against Key, if same, add new line and delete old
          // one
          if (s == KeySearchString) {
            // If KEY is present, remove old string and update it with new one
            s = file.GetNextLine();
            file.RemoveLine(file.GetCurrentLine());
            file.InsertLine(String, file.GetCurrentLine());

            KeyPresentFlag = TRUE;    // Set flag to say that key was located
          }
        }
        // Ordinarily should do last line here but last line can't be a KEY
      }
    } else {
      wxMessageBox(_T("Can not open .ini file"), _T("File I/O error"), wxOK | wxICON_INFORMATION, NULL);
      return (FALSE);
    }
  }

  // If KEY not present, write KEY and String at the bottom
  if (KeyPresentFlag == FALSE) {
    if (!file.Eof())    // It is not an empty file so jump to last line
    {
      file.GetLastLine();
    }
    file.AddLine(KeySearchString);
    file.AddLine(String);
    file.AddLine("");
  }

  if (file.Write() == FALSE) {
    wxMessageBox(_T("Can not write .ini file"), _T("File I/O error"), wxOK | wxICON_INFORMATION, NULL);
    return (FALSE);
  }

  if (file.Close() == FALSE) {
    wxMessageBox(_T("Can not close .ini file"), _T("File I/O error"), wxOK | wxICON_INFORMATION, NULL);
    return (FALSE);
  }

  return (TRUE);
}
