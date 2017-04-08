file(GLOB_RECURSE DLLCLIENT_CPP_SRC_LIST "*.cpp" "*.h")
add_executable(BTRFReader ${DLLCLIENT_CPP_SRC_LIST})
target_link_libraries(BTRFReader BTRFdom)
