project(LutBenchmark2)

FetchContent_Declare(
	fpng 
	GIT_REPOSITORY https://github.com/richgel999/fpng
	GIT_TAG main
)
set(SSE ON)
FetchContent_MakeAvailable(fpng)

find_package(glm CONFIG REQUIRED)


add_executable(${PROJECT_NAME} entry_main.cpp app.h app.cpp lut_loader.h lut_loader.cpp
window.h
window.cpp
keyboard.h
keyboard.cpp
mouse.h
mouse.cpp
menu.h
util.h
util.cpp
)

wis_install_deps(${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} PRIVATE 
	Wisdom 
	glm::glm
	Wil
)
set_target_properties(${PROJECT_NAME} 
	PROPERTIES
		CXX_STANDARD 20
)
target_include_directories(${PROJECT_NAME} PRIVATE ${fpng_SOURCE_DIR}/src)

wis_compile_shader(TARGET ${PROJECT_NAME} SHADER ${CMAKE_CURRENT_SOURCE_DIR}/lut.vs.hlsl)
wis_compile_shader(TARGET ${PROJECT_NAME} SHADER ${CMAKE_CURRENT_SOURCE_DIR}/lut.ps.hlsl)
wis_compile_shader(TARGET ${PROJECT_NAME} 
	SHADER ${CMAKE_CURRENT_SOURCE_DIR}/lut.ps.hlsl 
	DEFINITIONS "TETRA" 
	OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/lut_tetra.ps
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
		${CMAKE_CURRENT_SOURCE_DIR}/lut.cube
		$<TARGET_FILE_DIR:${PROJECT_NAME}>/lut.cube
)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
		${CMAKE_CURRENT_SOURCE_DIR}/image.png
		$<TARGET_FILE_DIR:${PROJECT_NAME}>/image.png
)