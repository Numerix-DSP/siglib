// SigLib - DTMF Generation Example
// Copyright (c) 2022 Sigma Numerix Ltd. All rights reserved.

// Freq. (Hz) 1209 1336 1477 1633
//     697     1     2    3    A
//     770     4     5    6    B
//     852     7     8    9    C
//     941     *     0    #    D

// Include files
#include <stdio.h>
#include <string.h>
#include <siglib.h>                                                 // SigLib DSP library

// Define constants
#define MAX_SAMPLE_LENGTH       4000
#define SAMPLE_RATE             8000.
#define SAMPLES_PER_MS          ((SLArrayIndex_t)(SAMPLE_RATE / 1000.0))
#define DETECTION_THRESHOLD     1000.

#define DTMF_SAMPLE_LENGTH      102

// Declare global variables and arrays
static SLData_t *pData;                                             // DTMF data
static SLData_t *pDTMFGenCoeffs;                                    // DTMF generator frequency look up table
static SLWavFileInfo_s wavFileInfo;

static const char TextSrcFilename[] = "dtmf.txt";
static FILE    *fpInputFile;

static const char WavDstFilename[] = "dtmf.wav";
static FILE    *fpOutputFile;

int             read_dtmf_key (
  char *pTone,
  SLFixData_t * pPeriod,
  SLData_t * pMagnitude);


int main (
  void)
{
  char            Tone;
  SLFixData_t     Period;
  SLData_t        Magnitude;
  SLFixData_t     FileLength = 0;
  SLStatus_t      KeyCode;
  SLFixData_t     sampleCount;
  SLStatus_t      PreviousKeyCode = SIGLIB_NO_DTMF_SIGNAL;          // Initialise to no DTMF key detected
  SLFixData_t     KeyCodeLength = 0;                                // Length of key code stream
  SLFixData_t     KeyCodeRegistered = SIGLIB_FALSE;                 // A key code has not been registered

  SLFixData_t     PreviousKeyCodeLength = 0;
  SLData_t        LinearDTMFMagnitude;
  SLData_t        Max = SIGLIB_ZERO;
  SLData_t        NewMax = SIGLIB_ZERO;

  pData = SUF_VectorArrayAllocate (MAX_SAMPLE_LENGTH);
  pDTMFGenCoeffs = SUF_VectorArrayAllocate (SIGLIB_DTMF_FTABLE_LENGTH);

  if ((fpInputFile = fopen (TextSrcFilename, "r")) == NULL) {
    printf ("Can not open input data file %s\n", TextSrcFilename);
    exit (0);
  }

  if ((fpOutputFile = fopen (WavDstFilename, "wb")) == NULL) {
    printf ("Can not open output data file %s\n", WavDstFilename);
    exit (0);
  }

  wavFileInfo = SUF_WavSetInfo ((long) 8000, (long) FileLength, (short) 1, (short) 16, (short) 2, (short) 1);
  SUF_WavWriteHeader (fpOutputFile, wavFileInfo);

  SIF_DtmfGenerate (pDTMFGenCoeffs,                                 // Generator coefficient look up table pointer
                    SAMPLE_RATE);                                   // Sample rate

  while (read_dtmf_key (&Tone, &Period, &Magnitude) == 0) {         // While there are key codes
    LinearDTMFMagnitude = SDS_dBmToLinear (Magnitude, (SLData_t) 32767.0) / SIGLIB_TWO; // Note divide by two necessary for SDA_DtmfGenerate()
    printf ("Tone = %c, Period = %d (ms), Magn. = %lf (dB), Lin. Magn. = %lf\n", Tone, Period, Magnitude, LinearDTMFMagnitude);

    if (Tone == 'S') {
      SDA_Clear (pData,                                             // Pointer to destination array
                 (SLArrayIndex_t) (Period * SAMPLES_PER_MS));       // Array length
    }
    else {
      SDA_DtmfGenerate (pData,                                      // Destination array pointer
                        SUF_AsciiToKeyCode (Tone),                  // Key code
                        LinearDTMFMagnitude,                        // Signal magnitude
                        pDTMFGenCoeffs,                             // Generator coefficient look up table pointer
                        (SLArrayIndex_t) (Period * SAMPLES_PER_MS));  // Array length
    }

    SUF_WavWriteData (pData, fpOutputFile, wavFileInfo, Period * SAMPLES_PER_MS);

    FileLength += Period * SAMPLES_PER_MS;
  }

  printf ("DTMF signal has been generated\nPlease hit <Carriage Return> to decode . . .\n");
  getchar ();

// Now update the header to indicate the filelength
  wavFileInfo = SUF_WavSetInfo ((long) 8000, (long) FileLength, (short) 1, (short) 16, (short) 2, (short) 1);
  rewind (fpOutputFile);
  SUF_WavWriteHeader (fpOutputFile, wavFileInfo);

  fclose (fpInputFile);
  fclose (fpOutputFile);

// We have generated the DTMF file now we will detect the contents
  if ((fpInputFile = fopen (WavDstFilename, "rb")) == NULL) {       // Note this file is binary
    printf ("Error opening input .wav file\n");
    exit (-1);
  }

  wavFileInfo = SUF_WavReadHeader (fpInputFile);
  if (wavFileInfo.NumberOfChannels != 1) {                          // Check how many channels
    printf ("Number of channels in %s = %d\n", WavDstFilename, wavFileInfo.NumberOfChannels);
    printf ("This app requires a mono .wav file\n");
    exit (-1);
  }

  SUF_WavDisplayInfo (wavFileInfo);
  printf ("\n.wav file data. '.' indicates no tone present\n");
  printf ("                '-' indicates signal present but not DTMF\n\n");

  SIF_DtmfDetect (((SLData_t) wavFileInfo.SampleRate),              // Sample rate
                  DTMF_SAMPLE_LENGTH);                              // Array length

  while ((sampleCount = (SLFixData_t) SUF_WavReadData (pData, fpInputFile, wavFileInfo, DTMF_SAMPLE_LENGTH)) == DTMF_SAMPLE_LENGTH) {

    NewMax = SDA_Max (pData, DTMF_SAMPLE_LENGTH);                   // Detect the peak value of the DTMF signal
    if (NewMax > Max)
      Max = NewMax;

    KeyCode = SDA_DtmfDetectAndValidate (pData,                     // Source array pointer
                                         DETECTION_THRESHOLD,       // Threshold for signal energy
                                         &PreviousKeyCode,          // Pointer to previous key code
                                         &KeyCodeLength,            // Pointer to key code run length
                                         &KeyCodeRegistered,        // Pointer to storage for key code registration flag
                                         DTMF_SAMPLE_LENGTH);       // Array length

#if COMMENT
    printf ("Keycode = %d, %x\n", KeyCode, KeyCode);
    SUF_Debugfprintf ("Keycode = %d, %x\n", KeyCode, KeyCode);

    KeyCode = SDA_DtmfDetect (pData,                                // Source array pointer
                              DTMF_SAMPLE_LENGTH);                  // Array length
#endif

    if (KeyCode == SIGLIB_NO_SIGNAL_PRESENT) {
      printf ("(Length %d)\n", (short) PreviousKeyCodeLength);
      printf (". ");
    }

    else if (KeyCode == SIGLIB_NO_DTMF_SIGNAL) {
      printf ("(Length %d)\n", (short) PreviousKeyCodeLength);
      printf ("- ");
    }

    else if (KeyCode != SIGLIB_DTMF_CONTINUATION) {
      printf ("(Length %d)\n", (short) PreviousKeyCodeLength);
      printf ("%c ", (char) SUF_KeyCodeToAscii (KeyCode));
    }

    else {                                                          // KeyCode == SIGLIB_DTMF_CONTINUATION
      PreviousKeyCodeLength = KeyCodeLength;
    }
  }

  printf ("\n\n\n");
  printf ("DTMF signal peak magnitude = %lf\n", Max);
  printf ("\n\n\n");

  SUF_MemoryFree (pData);                                           // Free memory
  SUF_MemoryFree (pDTMFGenCoeffs);

  exit (0);
}


/*
Function : read_dtmf_key()
Read a set of dtmf keys
Return codes :
    0 - success
    1 - end of file
    2 - [START] not found
*/
int read_dtmf_key (
  char *pTone,
  SLFixData_t * pPeriod,
  SLData_t * pMagnitude)
{
  static int      StartFound = 0;
  char            inputString[80];
  char            startString[] = "[START]";

  if (!StartFound) {                                                // Read past header - search for [START] string
    while ((fgets (inputString, 80, fpInputFile) != NULL) && !StartFound) { // Get next string
      if (!strncmp (inputString, startString, strlen (startString))) {
        StartFound = 1;
      }
    }

    if (!StartFound) {
      return (2);                                                   // We have not found the start of the data
    }
  }

  if (fscanf (fpInputFile, "%c, %d, %lf\n", pTone, pPeriod, pMagnitude) != EOF) {
    if (*pPeriod > 500) {
      *pPeriod = 500;
    }
    if (*pMagnitude > SIGLIB_ZERO) {
      *pMagnitude = -*pMagnitude;
    }
    exit (0);
  }

  else {
    return (1);                                                     // Failure
  }
}
