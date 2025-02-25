cmake_minimum_required(VERSION 3.16)

#---------------------------------------#
#				SETUP					#
#---------------------------------------#
project(Editor)

set(MY_BIN_OUTPUT_DIR "${CMAKE_SOURCE_DIR}/build/${CMAKE_CXX_COMPILER_ID}_$<CONFIG>")
message("[${CMAKE_PROJECT_NAME}]: Output directory: ${MY_BIN_OUTPUT_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${MY_BIN_OUTPUT_DIR}")


#---------------------------------------#
#				SOURCES					#
#---------------------------------------#
file(GLOB_RECURSE Sources "Code/*.cpp")


#---------------------------------------#
#			BUILD MODE					#
#---------------------------------------#
add_executable(${PROJECT_NAME} ${Sources})


#---------------------------------------#
#				ENGINE					#
#---------------------------------------#
target_link_libraries(${PROJECT_NAME} PRIVATE Engine)


#---------------------------------------#
#				INCLUDE					#
#---------------------------------------#
target_include_directories(${PROJECT_NAME} PUBLIC "Code/include")


#---------------------------------------#
#				COMPILE MODE			#
#---------------------------------------#
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

if(MSVC)
	target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
else()
	target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()