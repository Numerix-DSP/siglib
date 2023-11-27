// Header file to read vibration data from .dat file
// Columns are: Magnitude and Speed

SLArrayIndex_t read_vibration_data (
  SLData_t * bp,
  FILE * fp,
  SLData_t * pSpeed,
  const SLArrayIndex_t array_length)
{
  SLArrayIndex_t  i;
  SLData_t        Speed;

  for (i = 0; ((i < array_length) && (fscanf (fp, "%le %le\n", bp, &Speed) != EOF)); i++) {
    if (i == 0) {                                                   // Return the speed
      *pSpeed = Speed;
    }

    bp++;
  }

  for (; (i < array_length); i++) {                                 // Zero pad the array
    *bp++ = 0.0;
  }

  return (i);
}                                                                   // End of read_vibration_data()
