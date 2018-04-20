/*
 * lock-nested-unrelated.c -- Archer testcase
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

  omp_nest_lock_t lock;
  omp_init_nest_lock(&lock);

#pragma omp parallel num_threads(2) shared(var)
  {
    omp_set_nest_lock(&lock);
    omp_set_nest_lock(&lock);
    // Dummy locking.
    omp_unset_nest_lock(&lock);
    omp_unset_nest_lock(&lock);

    var++;
  }

  omp_destroy_nest_lock(&lock);

  fprintf(stderr, "DONE\n");
}

// CHECK: WARNING: ThreadSanitizer: data race
// CHECK:   Write of size 4
// CHECK: #0 .omp_outlined.
// CHECK:   Previous write of size 4
// CHECK: #0 .omp_outlined.
// CHECK: DONE
