// Two process mutual exclusion functions
// This software is supplied under the terms of the MIT license:
// https://opensource.org/license/mit/.
// Copyright (c) 2026 Delta Numerix All rights reserved.

// These functions do not support resource request while a current request is
// pending If this scenario is a possibility then use
// mutex_peterson_interfaceN_trying() to ensure there is no pending request

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MXFalse 0
#define MXTrue 1

typedef struct {
  int Trying1;
  int Trying2;
  int Turn;
} h_Mutex;

// Peterson’s two process mutual exclusion functions

void mutex_peterson_init(h_Mutex* mutex)    // Initialize the mutex
{
  mutex->Trying1 = MXFalse;
  mutex->Trying2 = MXFalse;
  mutex->Turn = 0;
}

// Interface 1
void mutex_peterson_p1_request(h_Mutex* mutex)    // Request access to mutex
{
  mutex->Trying1 = MXTrue;
  mutex->Turn = 2;

  while ((mutex->Trying2 == MXTrue) && (mutex->Turn == 2))    // Wait until (turn == me) OR other process not trying
    ;
}

void mutex_peterson_p1_release(h_Mutex* mutex)    // Release access to mutex
{
  mutex->Trying1 = MXFalse;    // Release the mutex
}

int mutex_peterson_p1_trying(h_Mutex* mutex)    // Return pending status
{
  return (mutex->Trying1);
}

// Interface 2
void mutex_peterson_p2_request(h_Mutex* mutex)    // Request access to mutex
{
  mutex->Trying2 = MXTrue;
  mutex->Turn = 1;

  while ((mutex->Trying1 == MXTrue) && (mutex->Turn == 1))    // Wait until (turn == me) OR other process not trying
    ;
}

void mutex_peterson_p2_release(h_Mutex* mutex)    // Release access to mutex
{
  mutex->Trying2 = MXFalse;    // Release the mutex
}

int mutex_peterson_p2_trying(h_Mutex* mutex)    // Return pending status
{
  return (mutex->Trying2);
}

// Dekker’s two process mutual exclusion functions

void mutex_dekker_init(h_Mutex* mutex)    // Initialize the mutex
{
  mutex->Trying1 = MXFalse;
  mutex->Trying2 = MXFalse;
  mutex->Turn = 0;
}

// Interface 1
void mutex_dekker_p1_request(h_Mutex* mutex)    // Request access to mutex
{
  mutex->Trying1 = MXTrue;

  while (mutex->Trying2) {
    if (mutex->Turn != 1) {    // If turn != me
      mutex->Trying1 = MXFalse;
      while (mutex->Turn != 1)    // Wait while (turn != me)
        ;
      mutex->Trying1 = MXTrue;
    }
  }
}

void mutex_dekker_p1_release(h_Mutex* mutex)    // Release access to mutex
{
  mutex->Trying1 = MXFalse;    // Release the mutex
  mutex->Turn = 2;             // Swap turns
}

int mutex_dekker_p1_trying(h_Mutex* mutex)    // Return pending status
{
  return (mutex->Trying1);
}

// Interface 2
void mutex_dekker_p2_request(h_Mutex* mutex)    // Request access to mutex
{
  mutex->Trying2 = MXTrue;

  while (mutex->Trying1) {
    if (mutex->Turn != 2) {    // If turn != me
      mutex->Trying2 = MXFalse;
      while (mutex->Turn != 2)    // Wait while (turn != me)
        ;
      mutex->Trying2 = MXTrue;
    }
  }
}

void mutex_dekker_p2_release(h_Mutex* mutex)    // Release access to mutex
{
  mutex->Trying2 = MXFalse;    // Release the mutex
  mutex->Turn = 1;             // Swap turns
}

int mutex_dekker_p2_trying(h_Mutex* mutex)    // Return pending status
{
  return (mutex->Trying2);
}
