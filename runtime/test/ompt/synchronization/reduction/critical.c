// RUN: %libomp-compile-and-run | FileCheck %s
// REQUIRES: ompt
#include "callback.h"
#include <omp.h>

int main()
{
  int sum = 0;
  int i;
  #pragma omp parallel num_threads(2)
  #pragma omp for reduction(+:sum)
  for(i = 0; i < 10000; i++)
  {
    sum += i;
  }

  // CHECK: 0: NULL_POINTER=[[NULL:.*$]]

  return 0;
}
