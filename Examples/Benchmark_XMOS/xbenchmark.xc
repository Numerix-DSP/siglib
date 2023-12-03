// XMOS Benchmarking Functions

// Include files
#include <xs1.h>

timer t;

int xbench_get_time(void)
{
  int time;
  t :> time;
  return (time);
}

int xbench_init(void)
{
  int start_time, end_time;

  start_time = xbench_get_time();
  end_time = xbench_get_time();
  return (end_time - start_time);
}


