// Circular buffer for strings protected by Dekker’s two process mutual exclusion algorithm
// The producer() writes data to a circular buffer at 4x the speed that the consumer() reads it.
// In this implementation, messages are discarded once the fifo overflows
// Cygwin : gcc fifo_mutex_Dekker_test.c -lpthread -o fifo_mutex_Dekker_test.exe
// Linux  : gcc fifo_mutex_Dekker_test.c -lpthread -o fifo_mutex_Dekker_test
// This software is supplied under the terms of the MIT license: https://opensource.org/license/mit/
// (C) 2024 Delta Numerix


#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "circular_buffer.h"
#include "mutex.h"

#define CIRCULAR_BUFFER_SIZE    8                       // Maximum size of buffer
#define MAX_CB_STRING_LENGTH    20                      // Maximum length of any individual string

h_Mutex                 myMutex;
h_CircularBufferString  myCircularBuffer;

char globalStringResource [80];                         // String to pass message from thread #0 to thread #1

char circularBuffer[CIRCULAR_BUFFER_SIZE*MAX_CB_STRING_LENGTH];   // Circular buffer

void* producer_thread(void* arg)
{
  char    inputString[80];
  char    bufferedString[80];
  int     messageNumber = 0;

  cb_string_init (&myCircularBuffer, MAX_CB_STRING_LENGTH, circularBuffer, CIRCULAR_BUFFER_SIZE); // Initialize the circular buffer

  printf("Producer thread started\n");

  while (1) {
    sprintf(inputString, "Message : %d", messageNumber++);          // Create message to send to consumer and increment message number

    if (cb_string_write (&myCircularBuffer, inputString) == 0) {    // If there is space in circular buffer then write to it
      printf("Producer thread Circular buffer full!\n");
    }
    else {
      printf ("Producer thread String written to circular buffer\n");
    }

    if (*globalStringResource == 0) {               // If global resource empty send next pending message in circular buffer
      mutex_dekker_p1_request(&myMutex);

      if (cb_string_read (&myCircularBuffer, bufferedString) == 1) {  // If we have a message in circular buffer send it to consumer
        sprintf(globalStringResource, "%s", bufferedString);
        printf("Producer thread sent message via global resource : %s\n", bufferedString);
      }

      mutex_dekker_p1_release(&myMutex);
    }

    for (unsigned i = 0; i < (0xFFFFFFF); i++)      // Wait
      ;
  }
  return NULL;
}

void* consumer_thread(void* arg)
{

  printf("Consumer thread started\n");

  while (1) {
    mutex_dekker_p2_request(&myMutex);

    if (*globalStringResource != 0) {
      printf("Consumer thread message received : %s\n", globalStringResource);
      *globalStringResource = 0;                        // Write empty string
    }
    else {
      printf("Consumer thread NO NEW message received\n");
    }
    mutex_dekker_p2_release(&myMutex);

    for (unsigned i = 0; i < (0x03FFFFFFF); i++)        // Wait
      ;
  }

  return NULL;
}

int main(void)
{
  pthread_t tid[2];

  mutex_dekker_init (&myMutex);                         // Initialize the mutex
  *globalStringResource = 0;                            // Initialize empty string

  int err = pthread_create(&(tid[0]),
                           NULL,
                           &producer_thread, NULL);
  if (err != 0)
      printf("Producer thread creation error :[%s]", strerror(err));

  err = pthread_create(&(tid[1]),
                       NULL,
                       &consumer_thread, NULL);
  if (err != 0)
    printf("Consumer thread creation error :[%s]", strerror(err));


  pthread_join(tid[0], NULL);
  pthread_join(tid[1], NULL);

  return 0;
}

