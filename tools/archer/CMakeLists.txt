#
# ompt-tsan.cpp -- Archer runtime library, TSan annotations for Archer
#
#
# //===----------------------------------------------------------------------===//
# //
# // Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
# // See tools/archer/LICENSE.txt for details.
# // SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
# //
# //===----------------------------------------------------------------------===//
  
  

include_directories(${LIBOMP_INCLUDE_DIR})

add_library(archer SHARED ompt-tsan.cpp counter.cpp ftsan.c)
add_library(archer_static STATIC ompt-tsan.cpp counter.cpp ftsan.c)

install(TARGETS archer archer_static
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)

add_subdirectory(tests)
