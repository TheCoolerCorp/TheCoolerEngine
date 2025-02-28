
add_executable(Engine_Maths_UT "main.cpp")

target_link_libraries(Engine_Maths_UT PRIVATE Engine)
target_link_libraries(Engine_Maths_UT PRIVATE glm)

add_test(NAME CEngine_Maths_UT COMMAND Engine_Maths_UT)