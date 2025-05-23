
/**************************************************************************
File Name               : siglib_host_utils.h   | Author        : JOHN EDWARDS
Siglib Library Version  : 10.50                 |
------------------------------------------------+----------------------------------
Compiler  : Independent                         | Start Date    : 13/09/1992
Options   :                                     | Latest Update : 09/01/2022
---------------------------------------------------------------------------
Support for SigLib is available via Email: support@numerix-dsp.com

This file may be modified, in any way, providing that this header remains
within the file and none of the header contents are removed or modified.

__SIGLIB_LICENSE__

Copyright (c) 2024 Delta Numerix All rights reserved.
---------------------------------------------------------------------------
Description: A subset of conio.h and time.h functions

Calls Microsoft conio.h or implements subset of conio.h functions not
implemented in GCC

****************************************************************************/

#ifndef __SIGLIB_HOST_UTILS_H__
#define __SIGLIB_HOST_UTILS_H__

#include <siglib.h>

#if _MSC_VER
#  include <conio.h>
#  include <time.h>

#  define mssleep Sleep

#else    // If compiler not MSVC then include the function subset

// This kbhit() modified from Morgan McGuire's code here:
// https://www.flipcode.com/archives/_kbhit_for_Linux.shtml

#  include <stdio.h>
#  include <sys/ioctl.h>
#  include <sys/select.h>
#  include <sys/socket.h>
#  include <sys/time.h>
#  include <sys/types.h>
#  include <termios.h>
#  include <unistd.h>

#  define _kbhit kbhit

int kbhit(void);

int kbhit()
{
  static const int STDIN = 0;
  static int initialized = 0;

  if (!initialized) {
    // Use termios to turn off line buffering
    struct termios term;
    tcgetattr(STDIN, &term);
    term.c_lflag &= ~ICANON;
    tcsetattr(STDIN, TCSANOW, &term);
    setbuf(stdin, NULL);
    initialized = 1;
  }

  int bytesWaiting;
  ioctl(STDIN, FIONREAD, &bytesWaiting);
  return bytesWaiting;
}

// These functions copied from here:
// http://stackoverflow.com/questions/3276546/how-to-implement-getch-function-of-c-in-linux

#  define _getch getch
#  define _getche getche

/* reads from keypress, doesn't echo */
int getch(void)
{
  struct termios oldattr, newattr;
  int ch;
  tcgetattr(STDIN_FILENO, &oldattr);
  newattr = oldattr;
  newattr.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newattr);
  ch = getchar();
  tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);
  return ch;
}

/* reads from keypress, echoes */
int getche(void)
{
  struct termios oldattr, newattr;
  int ch;
  tcgetattr(STDIN_FILENO, &oldattr);
  newattr = oldattr;
  newattr.c_lflag &= ~(ICANON);
  tcsetattr(STDIN_FILENO, TCSANOW, &newattr);
  ch = getchar();
  tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);
  return ch;
}

// __unix, __GNUC__ are defined by the appropriate compilers
#  if defined(__unix) || defined(__GNUC__)
#    define mssleep(u) usleep(u * 1000)
#  endif

#endif

#endif    // __SIGLIB_HOST_UTILS_H__
