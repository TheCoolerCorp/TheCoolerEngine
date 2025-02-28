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
#target_include_directories(Editor PUBLIC "Code/include")


#---------------------------------------#
#				INCLUDE					#
#---------------------------------------#
target_include_directories(Editor PUBLIC "Code/include")
target_link_libraries(Editor PRIVATE Engine)


#---------------------------------------#
#				COMPILE MODE			#
#---------------------------------------#
#target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

#if(MSVC)
#	target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
#else()
#	target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
#endif()