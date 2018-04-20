/*
 * parallel-reduction.c -- Archer testcase
 */

//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is licensed under the LLNS/LLNL "BSD-Like" license.
// See tools/archer/LICENSE.txt for details.
//
//===----------------------------------------------------------------------===//


// RUN: %libarcher-compile-and-run| FileCheck %s
#include <omp.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
  int var = 0;

// Number of threads is empirical: We need enough threads so that
// the reduction is really performed hierarchically in the barrier!
#pragma omp parallel num_threads(5) reduction(+ : var)
  { var = 1; }

  fprintf(stderr, "DONE\n");
  int error = (var != 5);
  return error;
}

// CHECK-NOT: ThreadSanitizer: data race
// CHECK-NOT: ThreadSanitizer: reported
// CHECK: DONE
