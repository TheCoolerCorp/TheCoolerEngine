

file(GLOB_RECURSE ENGINE_MATHS_UT_SOURCES "src/*.cpp")
add_executable(Engine_Maths_UT ${ENGINE_MATHS_UT_SOURCES} "src/Vec3UT.cpp" "include/Vec3UT.h")
target_include_directories(Engine_Maths_UT PUBLIC "include")

target_compile_features(Engine_Maths_UT PUBLIC cxx_std_20)

if(MSVC)
    target_compile_options(Engine_Maths_UT PUBLIC /W4 /WX)
else()
    target_compile_options(Engine_Maths_UT PUBLIC -Wall -Wextra -Wpedantic -Werror)
endif()


target_link_libraries(Engine_Maths_UT PRIVATE Engine)

target_link_libraries(Engine_Maths_UT PRIVATE glm::glm)
target_link_libraries(Engine_Maths_UT PRIVATE   GTest::gtest_main)


add_test(NAME CEngine_Maths_UT COMMAND Engine_Maths_UT)

