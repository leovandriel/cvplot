cmake_minimum_required(VERSION 3.0)

project (cvplot)

find_package(OpenCV REQUIRED)

include_directories(include)
include_directories(${OpenCV_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fPIC ")
set(CVPLOT_LIB cvplot)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")
set(GOOGLE_TEST_VERSION "1.11.0")

find_program(CLANG_TIDY_EXISTS clang-tidy)
if(CLANG_TIDY_EXISTS)
  set(CMAKE_CXX_CLANG_TIDY clang-tidy)
endif()

if (DEFINED CVPLOT_LIB)
  file(GLOB LIB_SOURCES "${PROJECT_SOURCE_DIR}/src/cvplot/*.cc")
  add_library(${CVPLOT_LIB} ${LIB_SOURCES})
endif()

if (${CVPLOT_DEMO})
  file(GLOB BIN_SOURCES "${PROJECT_SOURCE_DIR}/src/demo/*.cc")
  foreach(filename ${BIN_SOURCES})
    get_filename_component(name ${filename} NAME_WE)
    add_executable(${name} ${filename})
    target_link_libraries(${name} ${CVPLOT_LIB} ${OpenCV_LIBS})
  endforeach()
endif()

target_include_directories ( cvplot PUBLIC ../include )
target_include_directories ( cvplot PRIVATE ../src )

install ( TARGETS cvplot
          LIBRARY DESTINATION lib
          ARCHIVE DESTINATION lib
          RUNTIME DESTINATION bin)

install ( DIRECTORY include DESTINATION .
          PATTERN CMakeLists.txt EXCLUDE )

if (${CVPLOT_TEST})
  include(FetchContent)
  FetchContent_Declare(googletest URL "https://github.com/google/googletest/archive/refs/tags/release-${GOOGLE_TEST_VERSION}.zip")
  FetchContent_MakeAvailable(googletest)
  enable_testing()
  include(GoogleTest)

  file(GLOB TEST_SOURCES "${PROJECT_SOURCE_DIR}/test/cvplot/*_test.cc")
  foreach(filename ${TEST_SOURCES})
    get_filename_component(name ${filename} NAME_WE)
    add_executable(${name} ${filename})
    target_link_libraries(${name} gtest_main ${CVPLOT_LIB} ${OpenCV_LIBS})
    gtest_discover_tests(${name})
  endforeach()
endif()
