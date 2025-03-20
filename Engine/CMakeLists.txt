#-----------------ENGINE----------------------#

#---------------------------------------#
#				SOURCES					#
#---------------------------------------#
file(GLOB_RECURSE ENGINE_SOURCES "Code/src/*.cpp")

#---------------------------------------#
#			BUILD MODE					#
#---------------------------------------#
add_library(Engine SHARED ${ENGINE_SOURCES})

#---------------------------------------#
#				INCLUDE					#
#---------------------------------------#
target_include_directories(Engine PUBLIC "Code/include")
option(ENGINE_UNIT_TESTS "Option to enable or disable unit tests for engine" OFF)

#---------------------------------------#
#				COMPILE MODE			#
#---------------------------------------#
target_compile_features(Engine PUBLIC cxx_std_20)

if(MSVC)
    target_compile_options(Engine PUBLIC /W3 /WX)
else()
    target_compile_options(Engine PUBLIC -Wall -Wextra -Wpedantic)
endif()

#---------------------------------------#
#		UNIT TESTS CONFIGURATION		#
#---------------------------------------#
if(ENGINE_UNIT_TESTS)
    add_subdirectory(Tests)
endif()
