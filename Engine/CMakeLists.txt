#-----------------ENGINE----------------------#

#---------------------------------------#
#				SOURCES					#
#---------------------------------------#
file(GLOB_RECURSE ENGINE_SOURCES "Code/*.cpp")

#---------------------------------------#
#			BUILD MODE					#
#---------------------------------------#
add_library(Engine SHARED ${ENGINE_SOURCES})

#---------------------------------------#
#				INCLUDE					#
#---------------------------------------#
target_include_directories(Engine PUBLIC "Code/include")
option(ENGINE_UNIT_TESTS "Option to enable or disable unit tests for engine" OFF)
#target_include_directories(spir-v PUBLIC ${SPIRV_FILES_LOCATION})

#---------------------------------------#
#				COMPILE MODE			#
#---------------------------------------#
target_compile_features(Engine PUBLIC cxx_std_20)

if(MSVC)
    target_compile_options(Engine PUBLIC /W3 /WX)
else()
    target_compile_options(Engine PUBLIC -Wall -Wextra -Wpedantic)
endif()

#---------------------------------------#
#		UNIT TESTS CONFIGURATION		#
#---------------------------------------#
if(ENGINE_UNIT_TESTS)
    add_subdirectory(Tests)
endif()

#---------------------------------------#
#           COPY ASSETS FOLDER          #
#---------------------------------------#
add_custom_command(
        TARGET Engine
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Assets
        $<TARGET_FILE_DIR:Engine>/Assets
)

#add_custom_command(TARGET Engine POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy_if_different
#        "${CMAKE_BINARY_DIR}/libs/$<CONFIG>/Refurekud.dll"
#        "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Refurekud.dll"
#    COMMAND ${CMAKE_COMMAND} -E copy_if_different
#        "${CMAKE_BINARY_DIR}/libs/$<CONFIG>/Refurekud.pdb"
#        "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Refurekud.pdb"
#    COMMAND ${CMAKE_COMMAND} -E copy_if_different
#        "${CMAKE_BINARY_DIR}/libs/$<CONFIG>/Refurekud.lib"
#        "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/libs/Refurekud.lib"
#    COMMAND ${CMAKE_COMMAND} -E copy_if_different
#        "${CMAKE_BINARY_DIR}/libs/$<CONFIG>/Refurekud.exp"
#        "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/libs/Refurekud.exp"
#)
