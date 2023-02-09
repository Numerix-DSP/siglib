// SigLib Debug Examples
// Copyright (c) 2022 Sigma Numerix Ltd. All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>                                                 // SigLib DSP library

// Declare global variables and arrays
static const SLData_t pArray[] = { 0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0 };


int main (
  void)
{

  SUF_ClearDebugfprintf ();                                         // Clear the debug.log file

  SUF_DebugPrintInfo ();                                            // Print the SigLib version number to the debug file

  SUF_DebugPrintLine ();                                            // Print the current line number to the debug file

  SUF_DebugPrintTime ();                                            // Print the current time to the debug file

  SUF_Debugfprintf ("\nAn array of data :\n");                      // Print a text string to the debug file

  SUF_DebugPrintArray (pArray, 10);                                 // Print the data array to the debug file

  SUF_Debugfprintf ("\nA matrix of data :\n");                      // Print a text string to the debug file

  SUF_DebugPrintMatrix (pArray, 2, 5);                              // Print the 2D matrix to the debug file

  printf ("The debug information has been written to the file : debug.log\n");
  printf ("Here are the contents of debug.log :\n\n");
  system ("more debug.log");

  exit (0);
}
