#==================================================================================================
#
#  Copyright (C) Andreas Fertig - All Rights Reserved
#
#  This file is part of the C++ training by Andreas Fertig. The file may only be used in the
#  context of the C++ training or with explicit agreement by Andreas Fertig.
#
#==================================================================================================

cmake_minimum_required(VERSION 3.19 FATAL_ERROR)
project("C++ Coroutines from scratch" CXX)

# use solution folders in Visual Studio/CLion
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_CXX_STANDARD_REQUIRED ON) # Yes, we require this standard
set(CMAKE_CXX_EXTENSIONS OFF)       # Pure C++ please


if(MSVC)
    # Use sane and nice C++ for MSVC.
    # This makes alternative tokens (not, and, ...) as actual keywords and
    # enables more conforming C++ in general
    add_definitions(/permissive-)


else()


    if(IS_CLANG)
        add_definitions(-fexperimental-library) # std::format for Clang 15 and 16 at least

    else()
    endif()
endif()


enable_testing()



add_executable(coroutineTask1 coroutineTask1.cpp)
set_target_properties(coroutineTask1 PROPERTIES
  FOLDER "coroutineTask1"
  CXX_STANDARD 20
)

add_executable(coroutineChat0 coroutineChat0.cpp)
set_target_properties(coroutineChat0 PROPERTIES
  FOLDER "coroutineChat0"
  CXX_STANDARD 20
)

add_executable(coroutineInterleaved0 coroutineInterleaved0.cpp)
set_target_properties(coroutineInterleaved0 PROPERTIES
  FOLDER "coroutineInterleaved0"
  CXX_STANDARD 20
)

add_executable(coroutineTask0 coroutineTask0.cpp)
set_target_properties(coroutineTask0 PROPERTIES
  FOLDER "coroutineTask0"
  CXX_STANDARD 20
)

