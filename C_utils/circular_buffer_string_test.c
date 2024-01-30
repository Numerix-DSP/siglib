// Circular buffer for strings
// Cygwin : gcc circular_buffer_string_test.c -lpthread -o circular_buffer_string_test.exe
// Linux  : gcc circular_buffer_string_test.c -lpthread -o circular_buffer_string_test
// This software is supplied under the terms of the MIT license: https://opensource.org/license/mit/
// (C) 2024 Delta Numerix

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "circular_buffer.h"

#define CIRCULAR_BUFFER_SIZE    8                       // Maximum size of buffer
#define MAX_CB_STRING_LENGTH    20                      // Maximum length of any individual string

char circularBuffer[CIRCULAR_BUFFER_SIZE*MAX_CB_STRING_LENGTH];   // Circular buffer

int main(void)
{
  char inputString[MAX_CB_STRING_LENGTH];
  char bufferedString[MAX_CB_STRING_LENGTH];

  h_CircularBufferString myCircularBuffer;

  cb_string_init (&myCircularBuffer, MAX_CB_STRING_LENGTH, circularBuffer, CIRCULAR_BUFFER_SIZE); // Initialize the circular buffer

  while (1) {
    printf ("Enter a sting to push to circular buffer (max length = %d), \"r\" to read a value, \"x\" to exit\n", CIRCULAR_BUFFER_SIZE);

    scanf ("%s", &inputString);

    if (strcmp (inputString, "x") == 0) {               // Exit
      exit(0);
    }

    else if (strcmp (inputString, "r") == 0) {          // Read from circular buffer
      if (cb_string_read (&myCircularBuffer, bufferedString) == 0) {
        printf("Circular buffer empty!\n");
      }
      else {
        printf("String : %s\n", bufferedString);
      }
    }

    else {                                              // Write to circular buffer
      if (cb_string_write (&myCircularBuffer, inputString) == 0) {
        printf("Circular buffer full!\n");
      }
      else {
        printf ("String written to circular buffer\n");
      }
    }
  }
}

