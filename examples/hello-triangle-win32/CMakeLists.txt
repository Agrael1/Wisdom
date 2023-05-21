# CMakeList.txt : CMake project for Wisdom, include source and define
# project specific logic here.
#
project(Example)

include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/Functions.cmake)


# Add source to this project's executable.
add_executable(${PROJECT_NAME} "resources/WinD3D.rc")

wis_install_dx(${PROJECT_NAME})

set(SOURCES "src/entry_main.cpp" "src/app.cpp" "src/keyboard.cpp" "src/mouse.cpp" "src/window.cpp")
set(HEADERS "include/example/app.h" "include/example/keyboard.h" "include/example/menu.h" "include/example/mouse.h" "include/example/window.h" "include/resource.h")

target_sources(${PROJECT_NAME}
	PRIVATE 
		${SOURCES}
	PUBLIC
		${HEADERS}
)

target_link_libraries(
	${PROJECT_NAME}
		PUBLIC Wisdom Wil
)

set_target_properties(${PROJECT_NAME}
    PROPERTIES
        CXX_STANDARD 23
)

target_include_directories(${PROJECT_NAME} PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_compile_definitions(${PROJECT_NAME} PUBLIC SHADER_DIR="${SHADER_DIR}")

add_dependencies(${PROJECT_NAME}
    compile_shaders_kd
)
