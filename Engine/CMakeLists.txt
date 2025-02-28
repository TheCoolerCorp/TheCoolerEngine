#-----------------ENGINE----------------------#

#---------------------------------------#
#				SETUP					#
#---------------------------------------#


file(GLOB_RECURSE ENGINE_SOURCES "Code/*.cpp")

add_library(Engine SHARED ${ENGINE_SOURCES})

target_include_directories(Engine PUBLIC "Code/include")

add_subdirectory(Tests)
