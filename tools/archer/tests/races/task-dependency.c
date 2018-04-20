/*
 * task-deoendency.c -- Archer testcase
 */

//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is licensed under the LLNS/LLNL "BSD-Like" license.
// See tools/archer/LICENSE.txt for details.
//
//===----------------------------------------------------------------------===//


// RUN: %libarcher-compile-and-run-race | FileCheck %s
#include <omp.h>
#include <stdio.h>
#include <unistd.h>
#include "ompt/ompt-signal.h"

int main(int argc, char *argv[]) {
  int var = 0, a = 0;

#pragma omp parallel num_threads(2) shared(var, a)
#pragma omp master
  {
#pragma omp task shared(var, a) depend(out : var)
    {
      OMPT_SIGNAL(a);
      var++;
    }

#pragma omp task shared(a) depend(in : var)
    {
      OMPT_SIGNAL(a);
      OMPT_WAIT(a, 3);
    }

#pragma omp task shared(var) // depend(in: var) is missing here!
    {
      var++;
      OMPT_SIGNAL(a);
    }

    // Give other thread time to steal the task.
    OMPT_WAIT(a, 2);
  }

  int error = (var != 2);
  fprintf(stderr, "DONE\n");
  return error;
}

// CHECK: WARNING: ThreadSanitizer: data race
// CHECK:   Write of size 4
// CHECK: #0 .omp_outlined.
// CHECK: #1 .omp_task_entry.
// CHECK:   Previous write of size 4
// CHECK: #0 .omp_outlined.
// CHECK: #1 .omp_task_entry.
// CHECK: DONE
