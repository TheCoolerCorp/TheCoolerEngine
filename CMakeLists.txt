cmake_minimum_required(VERSION 3.16)

#---------------------------------------#
#				SETUP					#
#---------------------------------------#
project(TheCoolerEngine)

set(MY_BIN_OUTPUT_DIR "${CMAKE_SOURCE_DIR}/build/${CMAKE_CXX_COMPILER_ID}_$<CONFIG>")
message("[${CMAKE_PROJECT_NAME}]: Output directory: ${MY_BIN_OUTPUT_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${MY_BIN_OUTPUT_DIR}")


#---------------------------------------#
#			SUB DIRECTORY				#
#---------------------------------------#
add_subdirectory(Engine)
set(BUILD_DEBUG_MODE OFF)
set(BUILD_MODE ON)




#---------------------------------------#
#				Libs					#
#---------------------------------------#
