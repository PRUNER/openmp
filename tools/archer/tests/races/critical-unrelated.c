/*
 * critical-unrelated.c -- Archer testcase
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

int main(int argc, char *argv[]) {
  int var = 0;

#pragma omp parallel num_threads(2) shared(var)
  {
#pragma omp critical
    {
      // Dummy region.
    }

    var++;
  }

  fprintf(stderr, "DONE\n");
}

// CHECK: WARNING: ThreadSanitizer: data race
// CHECK:   Write of size 4
// CHECK: #0 .omp_outlined.
// CHECK:   Previous write of size 4
// CHECK: #0 .omp_outlined.
// CHECK: DONE
