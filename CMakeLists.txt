#-----------------ROOT----------------------#

cmake_minimum_required(VERSION 3.29.5)

#---------------------------------------#
#				SETUP					#
#---------------------------------------#
project(TheCoolerEngine)

set(MY_BIN_OUTPUT_DIR "${CMAKE_SOURCE_DIR}/build/${CMAKE_CXX_COMPILERID}$<CONFIG>")
message("[${CMAKE_PROJECT_NAME}]: Output directory: ${MY_BIN_OUTPUT_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${MY_BIN_OUTPUT_DIR}/libs")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${MY_BIN_OUTPUT_DIR}/libs")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${MY_BIN_OUTPUT_DIR}")

enable_testing()

#---------------------------------------#
#			COMPILE OPTION				#
#---------------------------------------#


#---------------------------------------#
#				LIBS					#
#---------------------------------------#
# ADD LIBS LATER


#---------------------------------------#
#			SUBDIRECTORY				#
#---------------------------------------#
add_subdirectory(Engine)
add_subdirectory(Editor)

add_subdirectory(libs/glfw)
target_link_libraries(Engine glfw)
