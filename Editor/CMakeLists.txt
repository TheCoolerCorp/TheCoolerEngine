#-----------------EDITOR----------------------#

#---------------------------------------#
#				SOURCES					#
#---------------------------------------#
file(GLOB_RECURSE EDITOR_SOURCES "Code/*.cpp")

#---------------------------------------#
#			BUILD MODE					#
#---------------------------------------#
add_executable(Editor ${EDITOR_SOURCES} "Code/Include/ImGuiLayer.h" "Code/src/ImGuiLayer.cpp" "Code/Include/SubWindow.h" "Code/src/SubWindow.cpp" "Code/Include/MainWindow.h" "Code/src/MainWindow.cpp" "Code/Include/RHIImGuiRenderer.h" "Code/src/RHIImGuiRenderer.cpp" "Code/Include/VulkanImGuiRenderer.h" "Code/src/VulkanImGuiRenderer.cpp")

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
else()
    target_compile_options(Editor PUBLIC -Wall -Wextra -Wpedantic)
endif()
