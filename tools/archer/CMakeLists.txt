#
# ompt-tsan.cpp -- Archer runtime library, TSan annotations for Archer
#
#
# //===----------------------------------------------------------------------===//
# //
# //                     The LLVM Compiler Infrastructure
# //
# // This file is licensed under the LLNS/LLNL "BSD-Like" license.
# // See tools/archer/LICENSE.txt for details.
# //
# //===----------------------------------------------------------------------===//
  
  

include_directories(${LIBOMP_INCLUDE_DIR})

add_library(archer SHARED ompt-tsan.cpp counter.cpp ftsan.c)
add_library(archer_static STATIC ompt-tsan.cpp counter.cpp ftsan.c)

install(TARGETS archer archer_static
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib)

add_subdirectory(tests)
