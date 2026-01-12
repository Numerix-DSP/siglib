// Circular buffer for integers
// Linux  : gcc circular_buffer_integer_test.c // -lpthread -o circular_buffer_integer_test
// This software is supplied under the terms of the MIT license:
// https://opensource.org/license/mit/.
// Copyright (c) 2026 Delta Numerix All rights reserved.

#include "circular_buffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CIRCULAR_BUFFER_SIZE 8    // Maximum size of buffer

int circularBuffer[CIRCULAR_BUFFER_SIZE];    // Circular buffer

int main(void)
{
  char inputString[20];
  int bufferedInt;

  h_CircularBufferInteger myCircularBuffer;

  cb_integer_init(&myCircularBuffer, circularBuffer,
                  CIRCULAR_BUFFER_SIZE);    // Initialize the circular buffer

  while (1) {
    printf("Enter an integer to push to circular buffer (max length = %d), "
           "\"r\" to read a value, \"x\" to exit\n",
           CIRCULAR_BUFFER_SIZE);

    scanf("%s", &inputString);

    if (strcmp(inputString, "x") == 0) {    // Exit
      exit(0);
    }

    else if (strcmp(inputString, "r") == 0) {    // Read from circular buffer
      if (cb_integer_read(&myCircularBuffer, &bufferedInt) == 0) {
        printf("Circular buffer empty!\n");
      } else {
        printf("Integer : %d\n", bufferedInt);
      }
    }

    else {    // Write to circular buffer
      if (cb_integer_write(&myCircularBuffer, atoi(inputString)) == 0) {
        printf("Circular buffer full!\n");
      } else {
        printf("Integer written to circular buffer\n");
      }
    }
  }
}
