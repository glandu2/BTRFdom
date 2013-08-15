cmake_minimum_required(VERSION 2.8)
project(BTRFdom)

set(CMAKE_VERBOSE_MAKEFILE true)
set(CMAKE_SHARED_LIBRARY_PREFIX "")

set(SHARED_LIB_DIR shared-lib)
set(DLLCLIENT_CPP_LIB_DIR "dll-client-cpp")

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_LIST_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_LIST_DIR}/bin)

set(SHARED_LIB_PATH "${CMAKE_CURRENT_LIST_DIR}/${SHARED_LIB_DIR}")
set(DLLCLIENT_CPP_LIB_PATH "${CMAKE_CURRENT_LIST_DIR}/${DLLCLIENT_CPP_LIB_DIR}")

set(SHARED_LIB_NAME "${PROJECT_NAME}")
set(DLLCLIENT_CPP_NAME "BTRFReader")

include_directories("${SHARED_LIB_PATH}/common")
include_directories("${SHARED_LIB_PATH}/interfaces")

if(MINGW)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11 -Wall -static-libgcc -static-libstdc++")
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -static-libgcc -static-libstdc++ -Wl,--add-stdcall-alias")

  #for qtcreator
  add_definitions("-D__GXX_EXPERIMENTAL_CXX0X__")
else()
  set(CMAKE_EXE_LINKER_FLAGS "/MANIFEST:NO /INCREMENTAL:NO")
  set(CMAKE_MODULE_LINKER_FLAGS "/MANIFEST:NO /INCREMENTAL:NO")
  set(CMAKE_SHARED_LINKER_FLAGS "/MANIFEST:NO /INCREMENTAL:NO")

  add_definitions("-D_CRT_SECURE_NO_WARNINGS")
endif()

add_subdirectory("${SHARED_LIB_PATH}" ${SHARED_LIB_DIR})
add_subdirectory("${DLLCLIENT_CPP_LIB_PATH}" ${DLLCLIENT_CPP_LIB_DIR})
