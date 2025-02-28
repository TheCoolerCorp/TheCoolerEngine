
include(FetchContent)

FetchContent_Declare(
 glm
 GIT_REPOSITORY https://github.com/g-truc/glm.git
 GIT_TAG master
 SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/glm
)

FetchContent_MakeAvailable(glm)

add_subdirectory(UnitTests)


