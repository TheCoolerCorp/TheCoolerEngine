
include(FetchContent)

FetchContent_Declare(
 glm
 GIT_REPOSITORY https://github.com/g-truc/glm.git
 GIT_TAG master
 SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/glm
)

FetchContent_Declare(
 googletest
 GIT_REPOSITORY https://github.com/google/googletest.git
 GIT_TAG main
 SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/googletest
)



FetchContent_MakeAvailable(glm)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_subdirectory(UnitTests)


