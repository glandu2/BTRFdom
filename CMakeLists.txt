project(BTRFdom)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_VERBOSE_MAKEFILE true)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")
#for qtcreator
add_definitions("-D__GXX_EXPERIMENTAL_CXX0X__")

aux_source_directory(. ROOT_SRC_LIST)
aux_source_directory(./TML TML_SRC_LIST)
aux_source_directory(./BTRF BTRF_SRC_LIST)

file(GLOB ROOT_HDR_LIST "*.h" .)
file(GLOB TML_HDR_LIST "*.h" ./TML)
file(GLOB BTRF_HDR_LIST "*.h" ./BTRF)
add_executable(${PROJECT_NAME} ${ROOT_SRC_LIST} ${TML_SRC_LIST} ${BTRF_SRC_LIST} ${ROOT_HDR_LIST} ${TML_HDR_LIST} ${BTRF_HDR_LIST})

