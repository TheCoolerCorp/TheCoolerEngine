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
#			LIBS FETCH					#
#---------------------------------------#
add_subdirectory(libs)

find_package(Vulkan REQUIRED)
#find_package(Vulkan REQUIRED )
#---------------------------------------#
#			SUBDIRECTORY				#
#---------------------------------------#
add_subdirectory(Engine)
add_subdirectory(Editor)


#---------------------------------------#
#			LIBS LINK					#
#---------------------------------------#
target_link_libraries(Engine PUBLIC stb)

target_link_libraries(Engine PUBLIC assimp::assimp)

target_link_libraries(Engine PUBLIC glfw)

target_link_libraries(Engine PUBLIC Jolt)

target_link_libraries(Engine PUBLIC Vulkan::Vulkan)
target_link_libraries(Engine PUBLIC spir-v)
target_link_libraries(Engine PUBLIC meta)
target_link_libraries(Engine PUBLIC nlohmann_json)
target_link_libraries(Editor PUBLIC imgui)
target_link_libraries(Editor PUBLIC imguizmo)
target_link_libraries(Engine PUBLIC miniaudio)
