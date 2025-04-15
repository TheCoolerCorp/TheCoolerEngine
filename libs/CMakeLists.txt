#-----------------LIBS----------------------#

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
set(IMGUI_FILES_LOCATION "imgui")
file(GLOB IMGUI_SOURCES_FILES "${IMGUI_FILES_LOCATION}/*.cpp")
file(GLOB IMGUI_SOURCES_FILES "${IMGUI_FILES_LOCATION}/*.cpp" "${IMGUI_FILES_LOCATION}/backends/vulkan/imgui_impl_vulkan.cpp")
add_library(imgui STATIC ${IMGUI_SOURCES_FILES})
target_include_directories(imgui PUBLIC ${IMGUI_FILES_LOCATION})

#SPIR-V
#add_subdirectory(spir-v)
#set(SPIRV_FILES_LOCATION "spir-v")
#file(GLOB SPIRV_SOURCES_FILES "{SPIRV_FILES_LOCATION/spirv-reflect.cpp")

set(SPIRV_FILES_LOCATION "spir-v")
add_library(spir-v STATIC ${SPIRV_FILES_LOCATION}/spirv_reflect.c)
target_include_directories(spir-v PUBLIC ${SPIRV_FILES_LOCATION})
