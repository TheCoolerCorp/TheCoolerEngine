#-----------------EDITOR----------------------#

#---------------------------------------#
#				SOURCES					#
#---------------------------------------#
file(GLOB_RECURSE EDITOR_SOURCES "Code/*.cpp")

#---------------------------------------#
#			BUILD MODE					#
#---------------------------------------#
add_executable(Editor ${EDITOR_SOURCES} "Code/Include/LoggerWindow.h" "Code/src/LoggerWindow.cpp")

#---------------------------------------#
#				INCLUDE					#
#---------------------------------------#
target_include_directories(Editor PUBLIC "Code/Include")

#---------------------------------------#
#				ENGINE					#
#---------------------------------------#
target_link_libraries(Editor PRIVATE Engine)

#---------------------------------------#
#				COMPILE MODE			#
#---------------------------------------#
if(MSVC)
    target_compile_options(Editor PUBLIC /W4 /WX)
else()
    target_compile_options(Editor PUBLIC -Wall -Wextra -Wpedantic)
endif()
