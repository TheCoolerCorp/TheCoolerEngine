# STB
add_library(stb INTERFACE)
target_include_directories(stb INTERFACE "stb/")

# ASSIMP
add_subdirectory(assimp)

# GLFW
add_subdirectory(glfw)

# Refureku
#add_subdirectory(Refureku/Refureku/Library)

# IMGUI
add_library(imgui INTERFACE)
target_sources(imgui INTERFACE
        "imgui/imgui.cpp"
        "imgui/imgui_draw.cpp"
        "imgui/imgui_widgets.cpp"
        "imgui/imgui_tables.cpp"
        "imgui/backends/imgui_impl_glfw.cpp"
        "imgui/backends/imgui_impl_vulkan.cpp"
)
target_include_directories(imgui INTERFACE imgui)