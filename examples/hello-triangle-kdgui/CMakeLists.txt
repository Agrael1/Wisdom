project(hello-triangle-kdgui)

#set(FETCHCONTENT_UPDATES_DISCONNECTED ON)
include(FetchContent)

FetchContent_Declare(
	kdgui
	GIT_REPOSITORY https://github.com/KDAB/KDUtils.git
	GIT_TAG        master
)
set(BUILD_SHARED_LIBS OFF)
set(KDUTILS_BUILD_TESTS OFF)
#set(SPDLOG_FMT_EXTERNAL ON)
FetchContent_MakeAvailable(kdgui)



add_executable(${PROJECT_NAME}
	entry_main.cpp "window.h"
 "window.cpp" "app.h" "app.cpp")

if(WIN32)
	include(${CMAKE_CURRENT_SOURCE_DIR}/../../cmake/Functions.cmake)
	wis_install_dx(${PROJECT_NAME})
endif()

target_link_libraries(
	${PROJECT_NAME}
		PUBLIC Wisdom PRIVATE KDFoundation KDGui
)
set_target_properties(${PROJECT_NAME}
    PROPERTIES
        CXX_STANDARD 23
)

#target_include_directories(${PROJECT_NAME} PUBLIC
#	${CMAKE_CURRENT_SOURCE_DIR}/include
#)
target_compile_definitions(${PROJECT_NAME} PUBLIC SHADER_DIR="${SHADER_DIR}")

add_dependencies(${PROJECT_NAME}
    compile_shaders_kd
)
