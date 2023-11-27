
/*
Program to take source files and split them into smaller files.
The files are split using the file separator character - 0x1c
Note, functions that use global data - e.g. some of the modulation /
demodulation functions must not be split.
The primary reason for wanting to split the source files is because
this allows the linker in some older compiler toolchains to discard
functions that are not actually required by the applicaiton.
The output filenames are given a number, starting at 0.
This program will also generate the makefile to build the library.
The makefile is generated from the prototype makefile, which has
a place holder character - 0x1c located at the position that
stores the object files.
The source files to be split should be placed in the file "srcfiles.txt"
Please ensure that the directory "SplitFiles" exists within
the current directory.
*/

#if defined (__GNUC__)
#define _GNU_SOURCE
#elif defined (_MSC_VER)
#pragma warning (disable : 4996)
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG       0                                               // Set to '1' to enable debug

int main (
  int argc,
  char **argv)
{
  FILE           *fpInputFileNames, *fpInput, *fpOutput, *fpMakeFileInput, *fpMakeFileOutput;
  char            InputSrcFileName[20];
  char            InputFileName[20];
  char            OutputFileName[30];
  int             InputChar;
  short           OutputFileNameCounter = 0;

  if (argc != 3) {
    printf ("Useage error: splitfile <makefile prototype> <object file extension>\n");
    return (1);
  }

  if ((fpInputFileNames = fopen ("srcfiles.txt", "rb")) == NULL) {
    printf ("The source file list file \"srcfiles.txt\" could not be opened for reading\n");
    fcloseall ();
    return (1);
  }

  if ((fpMakeFileInput = fopen (argv[1], "rb")) == NULL) {
    printf ("The prototype makefile \"%s\" could not be opened for reading\n", argv[1]);
    fcloseall ();
    return (1);
  }

#if defined (__unix)
  if ((fpMakeFileOutput = fopen ("SplitFiles/makefile", "wb")) == NULL) {
#else
  if ((fpMakeFileOutput = fopen ("SplitFiles\\makefile", "wb")) == NULL) {
#endif
    printf ("The output makefile could not be opened for writing\n\n");
    printf ("The output files are written to a directory called \"SplitFiles\"\n");
    printf ("Please ensure that the directory exists within the current directory\n");
    fcloseall ();
    return (1);
  }

  while ((InputChar = getc (fpMakeFileInput)) != 0x1c) {            // Read start from makefile prototype
    if (InputChar == EOF) {
      printf ("The prototype makefile did not contain the place holder character - 0x1c\n");
      fcloseall ();
      return (1);
    }
    putc (InputChar, fpMakeFileOutput);
  }

  while (fscanf (fpInputFileNames, "%s", InputFileName) != EOF) {
    strcpy (InputSrcFileName, InputFileName);                       // Copy source file name
    strcat (InputSrcFileName, ".c");                                // Add .c extention

#if DEBUG
    printf ("Input file name = %s\n", InputSrcFileName);
#endif

    if ((fpInput = fopen (InputSrcFileName, "rb")) == NULL) {
      printf ("The input source file \"%s\" could not be opened\n", InputSrcFileName);
      fcloseall ();
      return (1);
    }

    if (OutputFileNameCounter != 0) {
      fprintf (fpMakeFileOutput, " \\\n");
    }

// Write the object file name to the makefile
    fprintf (fpMakeFileOutput, "\t%d.%s", OutputFileNameCounter, argv[2]);

#if defined (__unix)
    sprintf (OutputFileName, "SplitFiles/%d.c", OutputFileNameCounter++);
#else
    sprintf (OutputFileName, "SplitFiles\\%d.c", OutputFileNameCounter++);
#endif

#if DEBUG
    printf ("Output file name = %s\n", OutputFileName);
#endif

    fpOutput = fopen (OutputFileName, "wb");

    while ((InputChar = getc (fpInput)) != EOF) {
      if (InputChar != 0x1c) {
        putc (InputChar, fpOutput);
      }
      else {
        while ((InputChar = getc (fpInput)) != '/') {               // Read to '/'
          putc (InputChar, fpOutput);
        }
        putc (InputChar, fpOutput);                                 // Put final '/'

        fprintf (fpOutput, "\n\n");
        fclose (fpOutput);                                          // Close old output file

// Write the object file name to the makefile
        fprintf (fpMakeFileOutput, " \\\n\t%d.%s", OutputFileNameCounter, argv[2]);

// Open new output file
#if defined (__unix)
        sprintf (OutputFileName, "SplitFiles/%d.c", OutputFileNameCounter++);
#else
        sprintf (OutputFileName, "SplitFiles\\%d.c", OutputFileNameCounter++);
#endif

#if DEBUG
        printf ("Output file name = %s\n", OutputFileName);
#endif

        fpOutput = fopen (OutputFileName, "wb");

        fprintf (fpOutput, "#include <siglib.h>\n\n");
      }
    }
    fclose (fpInput);
    fclose (fpOutput);
#if DEBUG
    printf ("Files closed\n");
#endif
  }

  while ((InputChar = getc (fpMakeFileInput)) != EOF) {             // Read to end of makefile prototype
    putc (InputChar, fpMakeFileOutput);
  }

  fcloseall ();
  return (0);

}
