/*
 * ordered.c -- Archer testcase
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

#define NUM_THREADS 2

int main(int argc, char *argv[]) {
  int var = 0;
  int i;

#pragma omp parallel for ordered num_threads(NUM_THREADS) shared(var)
  for (i = 0; i < NUM_THREADS; i++) {
#pragma omp ordered
    { var++; }
  }

  fprintf(stderr, "DONE\n");
  int error = (var != 2);
  return error;
}

// CHECK-NOT: ThreadSanitizer: data race
// CHECK-NOT: ThreadSanitizer: reported
// CHECK: DONE
