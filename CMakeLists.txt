project(BTRFdom)
cmake_minimum_required(VERSION 2.8)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

aux_source_directory(. ROOT_SRC_LIST)
aux_source_directory(./TML TML_SRC_LIST)
aux_source_directory(./BTRF BTRF_SRC_LIST)
add_executable(${PROJECT_NAME} ${ROOT_SRC_LIST} ${TML_SRC_LIST} ${BTRF_SRC_LIST})

