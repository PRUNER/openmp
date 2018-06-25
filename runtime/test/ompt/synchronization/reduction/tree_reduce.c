// RUN: %libomp-compile-and-run | %sort-threads | FileCheck %s
// RUN: %libomp-compile -DNOWAIT && %libomp-run | %sort-threads | FileCheck %s
// REQUIRES: ompt
#include "callback.h"
#include <omp.h>

#ifdef NOWAIT
  #define FOR_CLAUSE nowait
#else
  #define FOR_CLAUSE
#endif

int main()
{
  int sum = 0;
  int i;
  #pragma omp parallel num_threads(5)
  #pragma omp for reduction(+:sum) FOR_CLAUSE
  for(i = 0; i < 10000; i++)
  {
    sum += i;
  }

  // CHECK: 0: NULL_POINTER=[[NULL:.*$]]

  // CHECK: {{^}}[[MASTER_ID:[0-9]+]]: ompt_event_parallel_begin:
  // CHECK-SAME: parallel_id=[[PARALLEL_ID:[0-9]+]]
  // CHECK: {{^}}[[MASTER_ID]]: ompt_event_implicit_task_begin: parallel_id=[[PARALLEL_ID]], task_id=[[TASK_ID:[0-9]+]]

  // CHECK: {{^}}[[MASTER_ID]]: ompt_event_reduction_begin: parallel_id=[[PARALLEL_ID]], task_id=[[TASK_ID]], codeptr_ra=
  // CHECK: {{^}}[[MASTER_ID]]: ompt_event_reduction_end: parallel_id=[[PARALLEL_ID]], task_id=[[TASK_ID]], codeptr_ra=

  return 0;
}
