/**************************************************************************
File Name               : siglib_host_utils.h   | Author        : JOHN EDWARDS
Siglib Library Version  : 10.00                 |
------------------------------------------------+----------------------------------
Compiler  : Independent                         | Start Date    : 13/09/1992
Options   :                                     | Latest Update : 09/01/2022
---------------------------------------------------------------------------
Support for SigLib is available via EMail : support@numerix-dsp.com

This file may be modified, in any way, providing that this header remains
within the file and none of the header contents are removed or modified.

SigLib is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

SigLib is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1335, USA

This sofware is also available with a commercial license, for use in
proprietary, research, government or commercial applications.
Please contact Sigma Numerix Ltd. for further details :
https://www.numerix-dsp.com
support@.numerix-dsp.com

Copyright (c) 2022 Sigma Numerix Ltd. All rights reserved.
---------------------------------------------------------------------------
Description : A subset of conio.h and time.h functions

Calls Microsoft conio.h or implements subset of conio.h functions not implemented in GCC

****************************************************************************/

#ifndef __SIGLIB_HOST_UTILS_H__
#define __SIGLIB_HOST_UTILS_H__

#if _MSC_VER
#include <conio.h>
#include <time.h>

#define mssleep Sleep

#else                                               // If compiler not MSVC then include the function subset

// This kbhit() copied from : http://cboard.cprogramming.com/linux-programming/51531-faq-cached-input-mygetch.html#post357655

#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/time.h>

#define _kbhit kbhit

int kbhit (void);

int kbhit (void)
{
  struct timeval tv;
  fd_set rdfs;

  tv.tv_sec = 0;
  tv.tv_usec = 0;

  FD_ZERO(&rdfs);
  FD_SET (STDIN_FILENO, &rdfs);

  select(STDIN_FILENO+1, &rdfs, NULL, NULL, &tv);
  return FD_ISSET(STDIN_FILENO, &rdfs);
}

// These functions copied from here : http://stackoverflow.com/questions/3276546/how-to-implement-getch-function-of-c-in-linux

#define _getch getch
#define _getche getche

/* reads from keypress, doesn't echo */
int getch(void)
{
    struct termios oldattr, newattr;
    int ch;
    tcgetattr( STDIN_FILENO, &oldattr );
    newattr = oldattr;
    newattr.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
    return ch;
}

/* reads from keypress, echoes */
int getche(void)
{
    struct termios oldattr, newattr;
    int ch;
    tcgetattr( STDIN_FILENO, &oldattr );
    newattr = oldattr;
    newattr.c_lflag &= ~( ICANON );
    tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
    return ch;
}

                            // __unix, __GNUC__ are defined by the appropriate compilers
#if defined (__unix) || defined (__GNUC__)
#define mssleep(u) usleep(u*1000)
#endif

#endif

#endif      // __SIGLIB_HOST_UTILS_H__
