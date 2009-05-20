cmake_minimum_required(VERSION 2.6)

load_cache(${PARENT_BINARY_DIR})

project(LinkTest)
include_directories (${INCLUDES})
add_executable(link-test ${SOURCE})
set_source_files_properties(${SOURCE}
  PROPERTIES COMPILE_FLAGS "${COMPILE_FLAGS}")

target_link_libraries(link-test
  ${LINK_LIBS})
