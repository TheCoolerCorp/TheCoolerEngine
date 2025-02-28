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
#				ENGINE					#
#---------------------------------------#
target_include_directories(Editor PUBLIC "Code/include")

#---------------------------------------#
#				COMPILE MODE			#
#---------------------------------------#

#---------------------------------------#
#				INCLUDE					#
#---------------------------------------#
target_include_directories(Editor PUBLIC "Code/include")
target_link_libraries(Editor PRIVATE Engine)
