/*
 * task-taskgroup-nested.c -- Archer testcase
 */

//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is licensed under the LLNS/LLNL "BSD-Like" license.
// See tools/archer/LICENSE.txt for details.
//
//===----------------------------------------------------------------------===//


// RUN: %libarcher-compile-and-run | FileCheck %s
#include <omp.h>
#include <stdio.h>
#include <unistd.h>
#include "ompt-signal.h"

int main(int argc, char *argv[]) {
  int var = 0, a = 0;

#pragma omp parallel num_threads(2) shared(var, a)
#pragma omp master
  {
#pragma omp taskgroup
    {
#pragma omp task
      {
#pragma omp task shared(var, a)
        {
          var++;
          OMPT_SIGNAL(a);
        }
      }

      // Give other thread time to steal the task and execute its child.
      OMPT_WAIT(a, 1);
    }

    var++;
  }

  fprintf(stderr, "DONE\n");
  int error = (var != 2);
  return error;
}

// CHECK-NOT: ThreadSanitizer: data race
// CHECK-NOT: ThreadSanitizer: reported
// CHECK: DONE
