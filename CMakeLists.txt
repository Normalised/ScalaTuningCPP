cmake_minimum_required(VERSION 2.8.12)
project (ScalaTuning)

set(CMAKE_BUILD_TYPE Release)

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
 tests/LoadAndParse_test.cpp
)
source_group(tests FILES ${SCALATUNINGCPP_TESTS})

# list of example files of the library
set(SCALATUNINGCPP_EXAMPLES
 examples/loadScl/main.cpp
)
source_group(example1 FILES ${SCALATUNINGCPP_EXAMPLES})

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

include_directories("${PROJECT_SOURCE_DIR}/include")
 
add_library(ScalaTuningCPP SHARED ${SCALATUNINGCPP_SRC} ${SCALATUNINGCPP_INC} ${SCALATUNINGCPP_DOC} ${SCALATUNINGCPP_SCRIPT})