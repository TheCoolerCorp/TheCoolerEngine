#-----------------EDITOR----------------------#

#---------------------------------------#
#				SOURCES					#
#---------------------------------------#
file(GLOB_RECURSE EDITOR_SOURCES "Code/*.cpp")

#---------------------------------------#
#			BUILD MODE					#
#---------------------------------------#
add_executable(Editor ${EDITOR_SOURCES}  "Code/Inlude/VulkanImGui.h" "Code/src/VulkanImGui.cpp" "Code/Inlude/ImGuiLayer.h" "Code/src/ImGuiLayer.cpp" "Code/Inlude/RHIImGui.h" "Code/src/RHIImGui.cpp" "Code/Inlude/UiWindow.h" "Code/Inlude/SceneGraphWindow.h" "Code/src/SceneGraphWindow.cpp" "Code/Inlude/InspectorWindow.h" "Code/Inlude/InspectorComponent/UiInspectorComponent.h" "Code/Inlude/InspectorComponent/UiTransformComponent.h" "Code/src/InspectorComponent/UiTransformComponent.cpp" "Code/src/InspectorWindow.cpp")

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
