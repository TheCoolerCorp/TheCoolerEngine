# STB
add_library(stb INTERFACE)
target_include_directories(stb INTERFACE "stb/")

# ASSIMP
add_subdirectory(assimp)

# GLFW
add_subdirectory(glfw)

# Refureku
add_subdirectory(Refureku)

# IMGUI
set(IMGUI_FILES_LOCATION "imgui")
file(GLOB IMGUI_SOURCES_FILES "${IMGUI_FILES_LOCATION}/*.cpp")
file(GLOB IMGUI_SOURCES_FILES "${IMGUI_FILES_LOCATION}/*.cpp" "${IMGUI_FILES_LOCATION}/backends/vulkan/imgui_impl_vulkan.cpp")
add_library(imgui STATIC ${IMGUI_SOURCES_FILES})
target_include_directories(imgui PUBLIC ${IMGUI_FILES_LOCATION})