project(hello-triangle-kdgui)

set(BUILD_SHARED_LIBS OFF)
set(KDUTILS_BUILD_TESTS OFF)

find_package(KDUtils CONFIG)
if(NOT KDUtils_FOUND)
    FetchContent_Declare(
        KDUtils
        GIT_REPOSITORY https://github.com/KDAB/KDUtils.git
        GIT_TAG        master
    )
    FetchContent_MakeAvailable(KDUtils)
endif()

find_package(KDFoundation CONFIG)
find_package(KDGui CONFIG)

add_executable(${PROJECT_NAME}
	entry_main.cpp "window.h"
 "window.cpp" "app.h" "app.cpp")

if(WIN32)
	include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/Functions.cmake)
	wis_install_dx(${PROJECT_NAME})
endif()

target_link_libraries(
	${PROJECT_NAME}
		PUBLIC Wisdom glm::glm PRIVATE KDFoundation KDGui
)
set_target_properties(${PROJECT_NAME}
    PROPERTIES
        CXX_STANDARD 20
)
target_compile_definitions(${PROJECT_NAME} 
	PUBLIC 
		SHADER_DIR="${SHADER_DIR}"
		GLM_FORCE_SIMD_AVX2
		GLM_FORCE_ALIGNED_GENTYPES
)

if(UNIX)
	target_link_options(${PROJECT_NAME} 
		PRIVATE 
			"-lm"
	)
endif()

add_dependencies(${PROJECT_NAME}
    compile_shaders_kd
)
