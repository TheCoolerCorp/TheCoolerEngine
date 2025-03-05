#-----------------EDITOR----------------------#

#---------------------------------------#
#				SOURCES					#
#---------------------------------------#
file(GLOB_RECURSE EDITOR_SOURCES "Code/*.cpp")

#---------------------------------------#
#			BUILD MODE					#
#---------------------------------------#
add_executable(Editor ${EDITOR_SOURCES})

#---------------------------------------#
#				INCLUDE					#
#---------------------------------------#
target_include_directories(Editor PUBLIC "Code/include")

#---------------------------------------#
#				ENGINE					#
#---------------------------------------#
target_link_libraries(Editor PRIVATE Engine)

#---------------------------------------#
#				COMPILE MODE			#
#---------------------------------------#
if(MSVC)
    target_compile_options(Editor PUBLIC /W4 /WX)
elseif(NOT MSVC AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(Editor PUBLIC -Wall -Wextra -Wpedantic -Werror -DDEBUG)
else()
    target_compile_options(Editor PUBLIC -Wall -Wextra -Wpedantic -Werror)
endif()
