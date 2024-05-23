// Convert SigLib .sig files to comma delimited files (.csv)
// Copyright (c) 2023 Delta Numerix All rights reserved.

// Include files
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <siglib.h>

int main(int argc, char* argv[])
{
  SLData_t Value;
  FILE *fpInputFile, *fpOutputFile;
  char FileName[256];

  if (argc != 2) {
    printf("\nUsage:\nsig2csv filename (no extension)\n\n");
    exit(-1);    // Exit - usage error
  }

  strcpy(FileName, argv[1]);
  strcat(FileName, ".sig");

  printf("Source file = %s\n", FileName);

  if ((fpInputFile = fopen(FileName, "r")) == NULL) {    // Open i/p file
    printf("Error opening input file %s\n", FileName);
    exit(-1);    // Exit - file not opened
  }

  strcpy(FileName, argv[1]);
  strcat(FileName, ".csv");

  printf("Destination file = %s\n", FileName);

  if ((fpOutputFile = fopen(FileName, "w")) == NULL) {    // Open o/p file
    printf("Error opening output file %s\n", FileName);
    fclose(fpInputFile);    // Close input file
    exit(-1);               // Exit - file not opened
  }

  if (fscanf(fpInputFile, "%lf\n", &Value) != EOF) {    // Read first word
    fprintf(fpOutputFile, "%lf", Value);                // Write data
  }
  while (fscanf(fpInputFile, "%lf\n", &Value) != EOF) {    // Get data
    fprintf(fpOutputFile, ",%lf", Value);                  // Write data
  }
  fprintf(fpOutputFile, "\n");    // Write data

  fclose(fpInputFile);    // Close files
  fclose(fpOutputFile);

  return (0);
}
