# CMAKE Lists adapted from SQLiteCPP by Sebastien Rombauts
# 
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT) 

cmake_minimum_required(VERSION 2.8.12)
set(CMAKE_OSX_DEPLOYMENT_TARGET "10.12" CACHE STRING "Minimum OS X deployment version")

project (ScalaTuning)

set(CMAKE_BUILD_TYPE Release)


# MacOS rpath policy
cmake_policy(SET CMP0042 NEW)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

# list of sources files of the library
set(SCALATUNINGCPP_SRC
 ${PROJECT_SOURCE_DIR}/src/ScalaTuning.cpp
 ${PROJECT_SOURCE_DIR}/src/NoteMap.cpp
)
source_group(src FILES ${SCALATUNINGCPP_SRC})

# list of header files of the library
set(SCALATUNINGCPP_INC
 ${PROJECT_SOURCE_DIR}/include/ScalaTuningCPP/ScalaTuning.h
 ${PROJECT_SOURCE_DIR}/include/ScalaTuningCPP/NoteMap.h
)
source_group(inc FILES ${SCALATUNINGCPP_INC})

# list of test files of the library
set(SCALATUNINGCPP_TESTS
 tests/NoteMap_test.cpp
 tests/ScalaTuning_test.cpp
)
source_group(tests FILES ${SCALATUNINGCPP_TESTS})

# list of example files of the library
set(SCALATUNINGCPP_EXAMPLES
 examples/loadScl/main.cpp
)
source_group(loadSclExample FILES ${SCALATUNINGCPP_EXAMPLES})

# list of doc files of the library
set(SCALATUNINGCPP_DOC
 README.md
 LICENSE.txt
)
source_group(doc FILES ${SCALATUNINGCPP_DOC})

# list of script files of the library
#set(SCALATUNINGCPP_SCRIPT
#)
#source_group(scripts FILES ${SCALATUNINGCPP_SCRIPT})

# scala test files
set(SCALATUNINGCPP_SCL
    scala_files/riley_albion.scl
    scala_files/fortune.scl
    scala_files/harm6.scl
)
source_group(scala_files FILES ${SCALATUNINGCPP_SCL})

include_directories("${PROJECT_SOURCE_DIR}/include")
 
add_library(ScalaTuningCpp SHARED ${SCALATUNINGCPP_SRC} ${SCALATUNINGCPP_INC} ${SCALATUNINGCPP_DOC} ${SCALATUNINGCPP_SCRIPT})
add_library(ScalaTuningCppStatic STATIC ${SCALATUNINGCPP_SRC} ${SCALATUNINGCPP_INC} ${SCALATUNINGCPP_DOC} ${SCALATUNINGCPP_SCRIPT})

option(SCALATUNINGCPP_BUILD_EXAMPLES "Build examples." OFF)
if (SCALATUNINGCPP_BUILD_EXAMPLES)
    # add the basic example executable
    add_executable(ScalaTuningCpp_loadSclExample ${SCALATUNINGCPP_EXAMPLES})
    target_link_libraries(ScalaTuningCpp_loadSclExample ScalaTuningCpp)
else (SCALATUNINGCPP_BUILD_EXAMPLES)
    message(STATUS "SCALATUNINGCPP_BUILD_EXAMPLES OFF")
endif (SCALATUNINGCPP_BUILD_EXAMPLES)

option(SCALATUNINGCPP_BUILD_TESTS "Build and run tests." OFF)
if (SCALATUNINGCPP_BUILD_TESTS)
    # deactivate some warnings for compiling the gtest library
    if (NOT MSVC)
        add_compile_options(-Wno-variadic-macros -Wno-long-long -Wno-switch-enum -Wno-float-equal -Wno-conversion-null -Wno-switch-default -Wno-pedantic)
    endif (NOT MSVC)

    # add the subdirectory containing the CMakeLists.txt for the gtest library
    # TODO: under Linux, uses libgtest-dev if found
    # TODO: move to the new googletest Github repository
    if (NOT EXISTS "${PROJECT_SOURCE_DIR}/googletest/CMakeLists.txt")
        message(FATAL_ERROR "Missing 'googletest' submodule! Either use 'git init submodule' and 'git update submodule' to get googletest according to the README, or deactivate unit tests with -DSCALATUNINGCPP_BUILD_TESTS=OFF")
    endif ()
    add_subdirectory(googletest) 
    include_directories("${PROJECT_SOURCE_DIR}/googletest/googletest/include")

    # add the unit test executable
    add_executable(ScalaTuningCpp_tests ${SCALATUNINGCPP_TESTS})
    target_link_libraries(ScalaTuningCpp_tests gtest_main ScalaTuningCpp)
    # Link target with dl for linux
    if (UNIX AND NOT APPLE)
        target_link_libraries(ScalaTuningCpp_tests dl)
    endif ()    

    # add a "test" target:
    enable_testing()

    # does the tests pass?
    add_test(UnitTests ScalaTuningCpp_tests)

    if (SCALATUNINGCPP_BUILD_EXAMPLES)
        # does the example1 runs successfully?
        add_test(LoadSclExampleRun ScalaTuningCpp_loadSclExample)
    endif (SCALATUNINGCPP_BUILD_EXAMPLES)
else (SCALATUNINGCPP_BUILD_TESTS)
    message(STATUS "SCALATUNINGCPP_BUILD_TESTS OFF")
endif (SCALATUNINGCPP_BUILD_TESTS)

