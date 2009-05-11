cmake_minimum_required(VERSION 2.6)

project(LinkTest)
include_directories (${INCLUDES})
add_executable(link-test ${SOURCE})
set_source_files_properties(${SOURCE}
  PROPERTIES COMPILE_FLAGS "${COMPILE_FLAGS}")