project(hello-triangle-modules)

FetchContent_Declare(
	kdgui
	GIT_REPOSITORY https://github.com/KDAB/KDUtils.git
	GIT_TAG        master
)
set(BUILD_SHARED_LIBS OFF)
set(KDUTILS_BUILD_TESTS OFF)
FetchContent_MakeAvailable(kdgui)



add_executable(${PROJECT_NAME}
	entry_main.cpp 
 )

target_sources(${PROJECT_NAME}
PUBLIC FILE_SET CXX_MODULES 
	FILES "window.ixx" "app.ixx")

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
        CXX_STANDARD 23
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
