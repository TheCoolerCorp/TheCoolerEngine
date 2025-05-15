#-----------------EDITOR----------------------#

#---------------------------------------#
#				SOURCES					#
#---------------------------------------#
file(GLOB_RECURSE EDITOR_SOURCES "Code/*.cpp")

#---------------------------------------#
#			BUILD MODE					#
#---------------------------------------#
add_executable(Editor ${EDITOR_SOURCES} "Code/Include/LoggerWindow.h" "Code/src/LoggerWindow.cpp" "Code/Include/InspectorComponent/UiRigidbodyComponent.h" "Code/src/InspectorComponent/UiRigidbodyComponent.cpp" "Code/Include/FileExplorerWindow.h" "Code/src/FileExplorerWindow.cpp" "Code/Include/Resources/ImGuiTexture.h" "Code/src/Resources/ImGuiTexture.cpp" "Code/src/Layer.cpp" "Code/Include/InspectorComponent/UiLightComponent.h" "Code/src/InspectorComponent/UiLightComponent.cpp" "Code/Include/Components.h/ComponentRegistry.h" "Code/Include/Components.h/CustomComponents/PressurePlateComponent.h")

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
    target_compile_options(Editor PUBLIC /W3 /WX)
else()
    target_compile_options(Editor PUBLIC -Wall -Wextra -Wpedantic)
endif()
