// Circular buffer functions
// This software is supplied under the terms of the MIT license:
// https://opensource.org/license/mit/ (C) 2024 Delta Numerix

// Circular buffer functions for integers

typedef struct {
  int* buf;    // Circular buffer
  int size;    // Circular buffer size

  int readIndex;      // Data read index
  int writeIndex;     // Data write index
  int bufferDepth;    // Circular buffer depth
} h_CircularBufferInteger;

void cb_integer_init(h_CircularBufferInteger* cb, int* p_cb, int cb_size)
{
  cb->buf = p_cb;        // Circular buffer location
  cb->size = cb_size;    // Circular buffer size

  cb->readIndex = 0;      // Data read index
  cb->writeIndex = 0;     // Data write index
  cb->bufferDepth = 0;    // Circular buffer depth
}

int cb_integer_read(h_CircularBufferInteger* cb, int* it)
{
  int successFlag = 0;

  if (cb->bufferDepth != 0) {          // Check if circular buffer NOT empty
    *it = *cb->buf + cb->readIndex;    // Read value from circular buffer

    cb->bufferDepth--;    // Decrease circular buffer depth
    cb->readIndex++;      // Increment read index

    if (cb->readIndex == cb->size) {    // Loop if we are at the end of the array
      cb->readIndex = 0;
    }
    successFlag = 1;
  }
  return (successFlag);
}

int cb_integer_write(h_CircularBufferInteger* cb, int it)
{
  int successFlag = 0;

  if (cb->bufferDepth != cb->size) {     // Check if circular buffer NOT full
    *(cb->buf + cb->writeIndex) = it;    // Write value to circular buffer

    cb->bufferDepth++;    // Increase circular buffer depth
    cb->writeIndex++;     // Increment write index

    if (cb->writeIndex == cb->size) {    // Loop if we are at the end of the array
      cb->writeIndex = 0;
    }
    successFlag = 1;
  }
  return (successFlag);
}

// Circular buffer functions for strings

typedef struct {
  int max_string_len;    // Maximum string length
  char* buf;             // Circular buffer
  int size;              // Circular buffer size

  int readIndex;      // Data read index
  int writeIndex;     // Data write index
  int bufferDepth;    // Circular buffer depth
} h_CircularBufferString;

void cb_string_init(h_CircularBufferString* cb, int max_st_len, char* p_cb, int cb_size)
{
  cb->max_string_len = max_st_len;    // Maximum string length
  cb->buf = p_cb;                     // Circular buffer location
  cb->size = cb_size;                 // Circular buffer size

  cb->readIndex = 0;      // Data read index
  cb->writeIndex = 0;     // Data write index
  cb->bufferDepth = 0;    // Circular buffer depth
}

int cb_string_read(h_CircularBufferString* cb, char* st)
{
  int successFlag = 0;

  if (cb->bufferDepth != 0) {    // Check if circular buffer NOT empty
    memcpy(st, cb->buf + cb->readIndex * cb->max_string_len,
           cb->max_string_len);    // Read string from circular buffer

    cb->bufferDepth--;    // Decrease circular buffer depth
    cb->readIndex++;      // Increment read index

    if (cb->readIndex == cb->size) {    // Loop if we are at the end of the array
      cb->readIndex = 0;
    }
    successFlag = 1;
  }
  return (successFlag);
}

int cb_string_write(h_CircularBufferString* cb, char* st)
{
  int successFlag = 0;

  if (cb->bufferDepth != cb->size) {    // Check if circular buffer NOT full
    memcpy(cb->buf + cb->writeIndex * cb->max_string_len, st,
           cb->max_string_len);    // Write string to circular buffer

    cb->bufferDepth++;    // Increase circular buffer depth
    cb->writeIndex++;     // Increment write index

    if (cb->writeIndex == cb->size) {    // Loop if we are at the end of the array
      cb->writeIndex = 0;
    }
    successFlag = 1;
  }
  return (successFlag);
}
