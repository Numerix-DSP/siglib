// Convert SigLib .sig files to comma delimited files (.csv)
// Copyright (c) 2023 Alpha Numerix All rights reserved.

// Include files
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <siglib.h>


int main (
  int argc,
  char *argv[])
{
  SLData_t        Value;
  FILE           *pInputFile, *pOutputFile;
  char            FileName[256];

  if (argc != 2) {
    printf ("\nUsage error  :\nsig2csv filename (no extension)\n\n");
    exit (-1);                                                      // Exit - usage error
  }

  strcpy (FileName, argv[1]);
  strcat (FileName, ".sig");

  printf ("Source file = %s\n", FileName);

  if ((pInputFile = fopen (FileName, "r")) == NULL) {               // Open i/p file
    printf ("Error opening input file %s\n", FileName);
    exit (-1);                                                      // Exit - file not opened
  }

  strcpy (FileName, argv[1]);
  strcat (FileName, ".csv");

  printf ("Destination file = %s\n", FileName);

  if ((pOutputFile = fopen (FileName, "w")) == NULL) {              // Open o/p file
    printf ("Error opening output file %s\n", FileName);
    fclose (pInputFile);                                            // Close input file
    exit (-1);                                                      // Exit - file not opened
  }

  if (fscanf (pInputFile, "%lf\n", &Value) != EOF) {                // Read first word
    fprintf (pOutputFile, "%lf", Value);                            // Write data
  }
  while (fscanf (pInputFile, "%lf\n", &Value) != EOF) {             // Get data
    fprintf (pOutputFile, ",%lf", Value);                           // Write data
  }
  fprintf (pOutputFile, "\n");                                      // Write data

  fclose (pInputFile);                                              // Close files
  fclose (pOutputFile);

  exit (0);
}
