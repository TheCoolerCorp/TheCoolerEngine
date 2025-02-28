#---------------------------------------#
#				SETUP					#
#---------------------------------------#

 #project(Engine)

file(GLOB_RECURSE MY_ENGINE_SOURCES "Code/*.cpp")

add_library(Engine STATIC ${MY_ENGINE_SOURCES})

target_include_directories(Engine PUBLIC "Code/include")

add_subdirectory(Tests)
