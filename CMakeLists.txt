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


target_link_directories(Engine PUBLIC "libs/Refureku/Refureku/Library/Include")

target_link_directories(Engine PUBLIC "libs/RefurekuGenerator/Bin")
target_link_libraries(Engine PUBLIC Refureku)
target_include_directories(Engine PUBLIC "libs/Refureku/Refureku/Library/Include/Public/Refureku/Generated")

add_custom_target(RunRefurekuGenerator
   WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
   COMMAND libs/RefurekuGenerator/Bin/RefurekuGenerator.exe
           libs/RefurekuGenerator/RefurekuSettings.toml)

add_dependencies(Engine RunRefurekuGenerator)
target_include_directories(Engine PUBLIC "Generated")

target_link_libraries(Engine PUBLIC Vulkan::Vulkan)

#target_link_libraries(Editor PUBLIC Vulkan::Vulkan)
target_link_libraries(Editor PUBLIC imgui)
