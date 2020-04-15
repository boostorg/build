cmake_minimum_required(VERSION 3.10)

project(boost_build LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11 CACHE STRING "C++ standard version to use (default is 11)")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(b2)

target_sources(b2
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/builtins.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/class.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/color.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/command.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/compile.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/constants.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/cwd.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/debug.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/debugger.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/execcmd.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/filesys.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/frames.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/function.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/glob.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/hash.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/hcache.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/hdrmacro.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/headers.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/jam.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/jambase.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/jamgram.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/lists.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/make.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/make1.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/md5.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/mem.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/modules.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/native.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/object.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/option.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/output.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/parse.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/pathsys.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/regexp.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/rules.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/scan.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/search.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/jam_strings.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/subst.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/sysinfo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/timestamp.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/variable.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/w32_getreg.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/modules/order.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/modules/path.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/modules/property-set.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/modules/regex.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/modules/sequence.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/modules/set.cpp
    $<$<PLATFORM_ID:Windows>:${CMAKE_CURRENT_SOURCE_DIR}/src/engine/execnt.cpp>
    $<$<PLATFORM_ID:Windows>:${CMAKE_CURRENT_SOURCE_DIR}/src/engine/filent.cpp>
    $<$<PLATFORM_ID:Windows>:${CMAKE_CURRENT_SOURCE_DIR}/src/engine/pathnt.cpp>
    $<$<NOT:$<PLATFORM_ID:Windows>>:${CMAKE_CURRENT_SOURCE_DIR}/src/engine/execunix.cpp>
    $<$<NOT:$<PLATFORM_ID:Windows>>:${CMAKE_CURRENT_SOURCE_DIR}/src/engine/fileunix.cpp>
    $<$<NOT:$<PLATFORM_ID:Windows>>:${CMAKE_CURRENT_SOURCE_DIR}/src/engine/pathunix.cpp>)

target_compile_definitions(b2
  PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:_CRT_NONSTDC_NO_DEPRECATE>
    $<$<CXX_COMPILER_ID:MSVC>:_SCL_SECURE_NO_DEPRECATE>
    $<$<CXX_COMPILER_ID:MSVC>:_CRT_SECURE_NO_WARNINGS>
    $<$<CXX_COMPILER_ID:MSVC>:NOMINMAX>)

target_compile_features(b2 PRIVATE cxx_decltype)
