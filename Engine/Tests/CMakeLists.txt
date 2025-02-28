
include(FetchContent)

if(NOT TARGET glm)
	if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/libs/glm/CMakeLists.txt")
		add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/libs/glm"
		"${CMAKE_CURRENT_BINARY_DIR}/libs/glm")
	else()
		FetchContent_Declare(
		glm
		GIT_REPOSITORY https://github.com/g-truc/glm.git
		GIT_TAG master
		SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/glm)
		FetchContent_MakeAvailable(glm)
	endif()
endif()

if(NOT TARGET googletest)
	if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/libs/googletest/CMakeLists.txt")
		add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/libs/googletest"
		"${CMAKE_CURRENT_BINARY_DIR}/libs/googletest")
	else()
		set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
		FetchContent_Declare(
		 googletest
		 GIT_REPOSITORY https://github.com/google/googletest.git
		 GIT_TAG v1.16.0
		 SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/googletest
		)
		FetchContent_MakeAvailable(googletest)
	endif()
endif()

add_subdirectory(UnitTests)


