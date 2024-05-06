// .ini file handler header
// This software is provided under the terms of the GNU General Public License
// V2, as published by the Free Software Foundation. Copyright (c) 1999-2024
// Delta Numerix, All rights reserved.

#ifndef INIFILE_H
#define INIFILE_H

class IniFile
{
    public:
  IniFile(wxString FileNameString);
  ~IniFile();

  bool ReadIniStringFromFile(wxString Key, wxString* String);
  bool WriteIniStringToFile(wxString Key, wxString String);

    private:
  wxString IniFileName;
};

#endif
