set(TARGET_NAME "BTRFdom")

file(GLOB_RECURSE IMPL_SRC_LIST "impl/*.cpp" "impl/*.h")
file(GLOB_RECURSE COMMON_SRC_LIST "common/*.cpp" "common/*.h")
file(GLOB_RECURSE INTERFACES_SRC_LIST "interfaces/*.cpp" "interfaces/*.h")

add_library(${TARGET_NAME} ${IMPL_SRC_LIST} ${COMMON_SRC_LIST} ${INTERFACES_SRC_LIST})
target_include_directories(${TARGET_NAME} PUBLIC "interfaces" "common" PRIVATE "impl")
if(BUILD_SHARED_LIBS)
	target_compile_definitions(${TARGET_NAME} PRIVATE BUILDING_BTRF)
else()
	target_compile_definitions(${TARGET_NAME} PUBLIC BUILDING_BTRF_STATIC)
endif()
