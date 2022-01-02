/* Include file to print a character in different formats
// This function can be very useful when debugging synchronization problems in comms
// functions because it allows the data to be printed out in different formats.
// To use this function, just include the header at the top of your source file.
*/

#include <stdio.h>

void dpchar (char ch)
{
  int     i;

  SUF_Debugfprintf (" : c = %c, x = %x, b = ", ch, ch);
  for (i = 0; i < 8; i++)
  {
    SUF_Debugfprintf ("%d", (ch >> (7-i)) & 0x1);
  }
  SUF_Debugfprintf ("\n");
}

