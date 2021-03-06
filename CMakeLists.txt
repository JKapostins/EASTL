#-------------------------------------------------------------------------------------------
# Copyright (C) Electronic Arts Inc.  All rights reserved.
#-------------------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.1)

INCLUDE(${CMAKE_SOURCE_DIR}/Local/Packages/Engine.CMake.Scripts/ModuleMethods.cmake)

InitModule()

AddProject(EASTL)

#-------------------------------------------------------------------------------------------
# Options
#-------------------------------------------------------------------------------------------
option(EASTL_BUILD_BENCHMARK "Enable generation of build files for benchmark" OFF)
option(EASTL_BUILD_TESTS "Enable generation of build files for tests" OFF)

#-------------------------------------------------------------------------------------------
# Sub-projects
#-------------------------------------------------------------------------------------------
add_subdirectory(test/packages/EABase)

if(EASTL_BUILD_TESTS OR EASTL_BUILD_BENCHMARK)
    add_subdirectory(test/packages/EAAssert)
    add_subdirectory(test/packages/EAStdC)
    add_subdirectory(test/packages/EAMain)
    add_subdirectory(test/packages/EATest)
    add_subdirectory(test/packages/EAThread)
endif()

if(EASTL_BUILD_BENCHMARK)
    add_subdirectory(benchmark)
endif()

if(EASTL_BUILD_TESTS)
    add_subdirectory(test)
endif()

#-------------------------------------------------------------------------------------------
# Defines
#-------------------------------------------------------------------------------------------
add_definitions(-D_CHAR16T)
add_definitions(-D_CRT_SECURE_NO_WARNINGS)
add_definitions(-D_SCL_SECURE_NO_WARNINGS)
add_definitions(-DEASTL_OPENSOURCE=1)

#-------------------------------------------------------------------------------------------
# Library definition
#-------------------------------------------------------------------------------------------
file(GLOB EASTL_SOURCES "source/*.cpp" "include/EASTL/*.h")
file(GLOB INTERNAL_HEADERS "include/EASTL/internal/*.h")
FILE(GLOB BONUS_HEADERS "include/EASTL/bonus/*.h")
AddLibrary(EASTL ${EASTL_SOURCES} ${INTERNAL_HEADERS} ${BONUS_HEADERS})
AddSharedDefinitions(EASTL EASTL_DLL EASTL_EXPORT)

#-------------------------------------------------------------------------------------------
# Compiler Flags
#-------------------------------------------------------------------------------------------
set (CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/scripts/CMake")
include(CommonCppFlags)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" AND CMAKE_SIZEOF_VOID_P EQUAL 4 AND
        (CMAKE_BUILD_TYPE MATCHES "Release" OR CMAKE_BUILD_TYPE MATCHES "MinSizeRel"
            OR CMAKE_BUILD_TYPE MATCHES "RelWithDebInfo") AND MINGW)
    message(FATAL_ERROR "FIXME: 32bits mingw-w64 clang crashes when compiling with any optimizations.")
endif()

#-------------------------------------------------------------------------------------------
# Include dirs
#-------------------------------------------------------------------------------------------
AddAdditionalInclude(EASTL PUBLIC include)
#-------------------------------------------------------------------------------------------
# Libraries
#-------------------------------------------------------------------------------------------
AddDependency(EASTL EABase)

#-------------------------------------------------------------------------------------------
# Installation
#-------------------------------------------------------------------------------------------
#install(TARGETS EASTL DESTINATION lib)
#install(DIRECTORY include/EASTL DESTINATION include)
#install(DIRECTORY test/packages/EABase/include/Common/EABase DESTINATION include)

SOURCE_GROUP("Header Files\\internal" FILES ${INTERNAL_HEADERS})
SOURCE_GROUP("Header Files\\bonus" FILES ${BONUS_HEADERS})
SetSolutionFolder(EASTL ${EXTERNAL_LIBRARY_SOLUTION_FOLDER})