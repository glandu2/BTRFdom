file(GLOB_RECURSE DLLCLIENT_CPP_SRC_LIST "*.cpp" "*.h")
add_executable(BTRFtest ${DLLCLIENT_CPP_SRC_LIST})
target_link_libraries(BTRFtest BTRFdom gtest_main)

add_test(BTRFtest $<TARGET_FILE:BTRFtest>)
