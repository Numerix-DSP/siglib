// Siglib Synch / Asynch Converter Example
// Copyright (c) 2022 Sigma Numerix Ltd. All rights reserved.

// Include files
#include <stdio.h>
#include <siglib.h>                                 // SigLib DSP library
#include "dpchar.h"

// Define constants
#define NUMBER_OF_SAMPLES               40
#define NUMBER_OF_ASYNCH_BITS_PER_BYTE  8
#define NUMBER_OF_ASYNCH_STOP_BITS      1
#define ASYNCH_PARITY_TYPE              SIGLIB_EVEN_PARITY

// Declare global variables and arrays
// Test sequences - uncomment which ever one you would like to use

static const unsigned char SyncSequence[] = "Hello World - abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
//static const unsigned char SyncSequence[] = {
//  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
//  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
//  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
//  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
//  };
//static const unsigned char SyncSequence[] = {
//  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
//  };
//static const unsigned char SyncSequence[] = {
//  0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
//  0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f,
//  0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f,
//  0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f
//  };
//static const unsigned char InputSequence[] = {             // Bit reversed
//  0x00, 0x80, 0x40, 0xc0, 0x20, 0xa0, 0x60, 0xe0, 0x10, 0x90, 0x50, 0xd0, 0x30, 0xb0, 0x70, 0xf0,
//  0x08, 0x88, 0x48, 0xc8, 0x28, 0xa8, 0x68, 0xe8, 0x18, 0x98, 0x58, 0xd8, 0x38, 0xb8, 0x78, 0xf8,
//  0x04, 0x84, 0x44, 0xc4, 0x24, 0xa4, 0x64, 0xe4, 0x14, 0x94, 0x54, 0xd4, 0x34, 0xb4, 0x74, 0xf4,
//  0x0c, 0x8c, 0x4c, 0xcc, 0x2c, 0xac, 0x6c, 0xec, 0x1c, 0x9c, 0x5c, 0xdc, 0x3c, 0xbc, 0x7c, 0xfc
//  };


static unsigned char AsyncSequence[80];
static unsigned char ReSyncSequence[80];

static SLArrayIndex_t  StopBitRemoveCounter;


int main (void)
{
    SLArrayIndex_t  NumberOfBytesConverted;
    SLArrayIndex_t  i;
    SLArrayIndex_t  ParityErrorFlag;
    SLArrayIndex_t  NumberOfAsyncBitsPerByte =
                        SUF_AsyncCharacterLength (NUMBER_OF_ASYNCH_BITS_PER_BYTE,   // Number of bits in the data word
                                                  ASYNCH_PARITY_TYPE,               // Parity type
                                                  NUMBER_OF_ASYNCH_STOP_BITS);      // Number of stop bits

    SUF_ClearDebugfprintf();

    printf ("Number of bits per character = %d\n", (int)NUMBER_OF_ASYNCH_BITS_PER_BYTE);
    printf ("Number of stop bits per character = %d\n", (int)NUMBER_OF_ASYNCH_STOP_BITS);
    SUF_Debugfprintf ("Number of bits per character = %d\n", (int)NUMBER_OF_ASYNCH_BITS_PER_BYTE);
    SUF_Debugfprintf ("Number of stop bits per character = %d\n", (int)NUMBER_OF_ASYNCH_STOP_BITS);
#if (ASYNCH_PARITY_TYPE == SIGLIB_NO_PARITY)
    printf ("No parity\n");
    SUF_Debugfprintf ("No parity\n");
#elif (ASYNCH_PARITY_TYPE == SIGLIB_EVEN_PARITY)
    printf ("Even parity\n");
    SUF_Debugfprintf ("Even parity\n");
#elif (ASYNCH_PARITY_TYPE == SIGLIB_ODD_PARITY)
    printf ("Odd parity\n");
    SUF_Debugfprintf ("Odd parity\n");
#endif
    printf ("Number of asynchronous bits per character = %d\n\n", (int)NumberOfAsyncBitsPerByte);
    SUF_Debugfprintf ("Number of asynchronous bits per character = %d\n\n", (int)NumberOfAsyncBitsPerByte);

    // Test with a bunch of 1111s at the start before the sequence

    NumberOfBytesConverted =
        SDA_SyncToAsyncConverter (SyncSequence,                     // Pointer to source data
                                  AsyncSequence,                    // Pointer to destination data
                                  NUMBER_OF_ASYNCH_BITS_PER_BYTE,   // Number of bits in the data word
                                  ASYNCH_PARITY_TYPE,               // Parity type
                                  NUMBER_OF_ASYNCH_STOP_BITS,       // Number of stop bits
                                  NUMBER_OF_SAMPLES);               // Source array length

    printf ("SDA_SyncToAsyncConverter - number of bytes converted = %d\n", NumberOfBytesConverted);

    for (i = 0; i < NumberOfBytesConverted; i++) {
        SUF_Debugfprintf ("AsyncSequence[%d]", i);
        dpchar (AsyncSequence[i]);
    }

// If the sequence "0x00, 0x01, 0x02 ..." is used then the next line can be uncommented to test
// Parity error checking. This will convert the 3rd character from 0x03 to 0x07
// This works with 8 data bits, 1 stop bit and both even and odd parity
//  AsyncSequence[4] = 0x1d;

    NumberOfBytesConverted =
        SDA_AsyncToSyncConverter (AsyncSequence,                    // Pointer to source data
                                  ReSyncSequence,                   // Pointer to destination data
                                  NUMBER_OF_ASYNCH_BITS_PER_BYTE,   // Number of bits in the data word
                                  ASYNCH_PARITY_TYPE,               // Parity type
                                  &ParityErrorFlag,                 // Number of stop bits
                                  NumberOfBytesConverted);          // Source array length

    printf ("SDA_AsyncToSyncConverter - number of bytes converted = %d\n", NumberOfBytesConverted);
    printf ("SDA_AsyncToSyncConverter - ParityErrorFlag = %d\n", ParityErrorFlag);

    SUF_Debugfprintf ("\n", i);

    for (i = 0; i < NumberOfBytesConverted; i++) {
        SUF_Debugfprintf ("ReSyncSequence[%d]", i);
        dpchar (ReSyncSequence[i]);
    }

    ReSyncSequence[NumberOfBytesConverted] = '\0';                  // Add trailing NULL to end of string
    printf ("ReAsync string = %s\n", ReSyncSequence);


    SUF_Debugfprintf ("\n", i);

    for (i = 0; i < 20; i++) {
        SUF_Debugfprintf ("Input to stop bit remove[%d]", i);
        dpchar (ReSyncSequence[i]);
    }

    SIF_AsyncAddRemoveStopBits (&StopBitRemoveCounter);             // Pointer to stop bits removed counter

    NumberOfBytesConverted =
        SDA_AsyncRemoveStopBits (ReSyncSequence,                    // Pointer to source data
                                 AsyncSequence,                     // Pointer to destination data
                                 NUMBER_OF_ASYNCH_BITS_PER_BYTE,    // Number of bits in the data word
                                 ASYNCH_PARITY_TYPE,                // Parity type
                                 4,                                 // Ratio of stop bits removed
                                 &StopBitRemoveCounter,             // Pointer to stop bits removed counter
                                 20);                               // Source array length

    printf ("SDA_AsyncRemoveStopBits - number of bytes converted = %d\n", NumberOfBytesConverted);

    SUF_Debugfprintf ("\n", i);

    for (i = 0; i < NumberOfBytesConverted; i++) {
        SUF_Debugfprintf ("Stop bit removed sequence[%d]", i);
        dpchar (AsyncSequence[i]);
    }


    SIF_AsyncAddRemoveStopBits (&StopBitRemoveCounter);         // Pointer to stop bits added counter

    NumberOfBytesConverted =
        SDA_AsyncAddStopBits (AsyncSequence,                    // Pointer to source data
                              ReSyncSequence,                   // Pointer to destination data
                              NUMBER_OF_ASYNCH_BITS_PER_BYTE,   // Number of bits in the data word
                              ASYNCH_PARITY_TYPE,               // Parity type
                              3,                                // Ratio of stop bits added
                              &StopBitRemoveCounter,            // Pointer to stop bits added counter
                              20);                              // Source array length

    printf ("SDA_AsyncAddStopBits - number of bytes converted = %d\n", NumberOfBytesConverted);

    SUF_Debugfprintf ("\n", i);

    for (i = 0; i < NumberOfBytesConverted; i++) {
        SUF_Debugfprintf ("Stop bit added sequence[%d]", i);
        dpchar (ReSyncSequence[i]);
    }

    exit(0);
}
