# STB
add_library(stb INTERFACE)
target_include_directories(stb INTERFACE "stb/")

# ASSIMP
add_subdirectory(assimp)

# GLFW
add_subdirectory(glfw)

# Refureku
#add_subdirectory(Refureku/Refureku/Library)

#Jolt
set(PHYSICS_REPO_ROOT ${CMAKE_SOURCE_DIR}/libs/JoltPhysics)

set(JPH_BUILD_DEMOS OFF CACHE BOOL "" FORCE)

set(JPH_ENABLE_ASSERTS ON CACHE BOOL "" FORCE)

include(${PHYSICS_REPO_ROOT}/Jolt/Jolt.cmake)

target_compile_definitions(Jolt PUBLIC "$<$<CONFIG:RelWithDebInfo>:JPH_PROFILE_ENABLED;JPH_DEBUG_RENDERER>")
target_compile_definitions(Jolt PUBLIC "$<$<CONFIG:Debug>:JPH_PROFILE_ENABLED;JPH_DEBUG_RENDERER>")

option(FLOATING_POINT_EXCEPTIONS_ENABLED "Enable FPE in Jolt" OFF)
if (FLOATING_POINT_EXCEPTIONS_ENABLED AND "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    target_compile_definitions(Jolt PUBLIC "$<$<CONFIG:RelWithDebInfo>:JPH_FLOATING_POINT_EXCEPTIONS_ENABLED>")
    target_compile_definitions(Jolt PUBLIC "$<$<CONFIG:Debug>:JPH_FLOATING_POINT_EXCEPTIONS_ENABLED>")
endif()

# IMGUI
set(IMGUI_FILES_LOCATION "imgui")
file(GLOB IMGUI_SOURCES_FILES "${IMGUI_FILES_LOCATION}/*.cpp")
file(GLOB IMGUI_SOURCES_FILES "${IMGUI_FILES_LOCATION}/*.cpp" "${IMGUI_FILES_LOCATION}/backends/imgui_impl_vulkan.cpp" "${IMGUI_FILES_LOCATION}/backends/imgui_impl_glfw.cpp")
add_library(imgui INTERFACE)
target_sources(imgui INTERFACE ${IMGUI_SOURCES_FILES})
target_include_directories(imgui INTERFACE ${IMGUI_FILES_LOCATION})