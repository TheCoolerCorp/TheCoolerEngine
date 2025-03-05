#-----------------ENGINE----------------------#

#---------------------------------------#
#				SOURCES					#
#---------------------------------------#
file(GLOB_RECURSE ENGINE_SOURCES "Code/*.cpp")

#---------------------------------------#
#			BUILD MODE					#
#---------------------------------------#
add_library(Engine SHARED ${ENGINE_SOURCES})

#---------------------------------------#
#				INCLUDE					#
#---------------------------------------#
target_include_directories(Engine PUBLIC "Code/include")
option(ENGINE_UNIT_TESTS "Option to enable or disable unit tests for engine" ON)

#---------------------------------------#
#				COMPILE MODE			#
#---------------------------------------#
target_compile_features(Engine PUBLIC cxx_std_20)

if(MSVC)
    target_compile_options(Engine PUBLIC /W4 /WX)
elseif(NOT MSVC AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(Editor PUBLIC -Wall -Wextra -Wpedantic -Werror -DDEBUG)
else()
    target_compile_options(Engine PUBLIC -Wall -Wextra -Wpedantic -Werror)
endif()

#---------------------------------------#
#		EXPORTS / IMPORTS FOR DLL		#
#---------------------------------------#
target_compile_definitions(Engine PRIVATE ENGINE_EXPORTS)

#---------------------------------------#
#		UNIT TESTS CONFIGURATION		#
#---------------------------------------#
if(ENGINE_UNIT_TESTS)
    add_subdirectory(Tests)
endif()
