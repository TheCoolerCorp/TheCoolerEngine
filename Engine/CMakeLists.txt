cmake_minimum_required(VERSION 3.16)

project(Engine)

set(MY_BIN_OUTPUT_DIR "${CMAKE_SOURCE_DIR}/build/${CMAKE_CXX_COMPILER_ID}_$<CONFIG>")
message("[${CMAKE_PROJECT_NAME}]: Output directory: ${MY_BIN_OUTPUT_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${MY_BIN_OUTPUT_DIR}")

file(GLOB_RECURSE Sources "Code/*.cpp")

add_executable(Engine ${Sources})

target_include_directories(Engine PUBLIC "Code/include/Maths")

target_compile_features(Engine PRIVATE cxx_std_17)

if(MSVC)
	target_compile_options(Engine PRIVATE /W4 /WX)
else()
	target_compile_options(Engine PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()