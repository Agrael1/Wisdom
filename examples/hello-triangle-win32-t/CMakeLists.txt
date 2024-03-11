# CMakeList.txt : CMake project for Wisdom, include source and define project
# specific logic here.
#
project(Example)

find_package(glm QUIET)
if(NOT glm_FOUND)
    FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm
        GIT_TAG        master
    )
    FetchContent_MakeAvailable(glm)
endif()

include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/Functions.cmake)
include(deps.cmake)

# Add source to this project's executable.
add_executable(${PROJECT_NAME} "resources/WinD3D.rc" "include/example/util.h")

wis_install_deps(${PROJECT_NAME})

set(SOURCES "src/entry_main.cpp" "src/app.cpp" "src/keyboard.cpp"
            "src/mouse.cpp" "src/window.cpp" "src/util.cpp")
set(HEADERS
    "include/example/app.h"
    "include/example/keyboard.h"
    "include/example/menu.h"
    "include/example/mouse.h"
    "include/example/window.h"
    "include/resource.h"
    "include/example/util.h")

target_sources(
  ${PROJECT_NAME}
  PRIVATE ${SOURCES}
  PUBLIC ${HEADERS})

target_link_libraries(${PROJECT_NAME} PUBLIC Wisdom Wil glm::glm)

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 20)

target_include_directories(${PROJECT_NAME}
                           PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_definitions(${PROJECT_NAME} PUBLIC SHADER_DIR="${SHADER_DIR}")

add_dependencies(${PROJECT_NAME} compile_shaders_kd)
