cmake_minimum_required(VERSION 3.16)

project(EngineUT)


add_subdirectory(glm)

file(GLOB_RECURSE EngineUT_Sources ".cpp")

add_executable(EngineUT ${EngineUT_Sources})

target_link_libraries(EngineUT PRIVATE glm)
