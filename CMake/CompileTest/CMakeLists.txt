cmake_minimum_required(VERSION 2.6)
project(CompileTest)
add_library(compile-test STATIC ${SOURCE})
include_directories(${INCLUDES})
set_source_files_properties(${SOURCE}
  PROPERTIES COMPILE_FLAGS "${COMPILE_FLAGS}")