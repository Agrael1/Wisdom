add_library(wisdom-shared INTERFACE)
target_sources(wisdom-shared 
    INTERFACE FILE_SET HEADERS
	BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include
	FILES
        "include/wisdom/global/definitions.h"
        "include/wisdom/global/internal.h"
        "include/wisdom/util/com_ptr.h"
        "include/wisdom/wisdom.hpp"
        "include/wisdom/util/log_layer.h"
        "include/wisdom/util/flags.h"
        "include/wisdom/bridge/generator.h"
        "include/wisdom/util/exception.h"
        "include/wisdom/util/shader_compiler.h"
        "include/wisdom/util/profile.h"
        "include/wisdom/util/misc.h"
        "include/wisdom/util/small_allocator.h"
        "include/wisdom/bridge/format.h"
        "include/wisdom/bridge/source_location.h"
        "include/wisdom/util/error_messages.h")

target_include_directories(wisdom-shared 
    INTERFACE 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_compile_definitions(
  wisdom-shared
  INTERFACE
  DEBUG_MODE=$<IF:$<CONFIG:Debug>,1,0>
  DEBUG_LAYER=$<BOOL:${WISDOM_DEBUG_LAYER}>
  WISDOM_VERSION=${WISDOM_VERSION}
  WISDOM_LOG_LEVEL=${SEV_INDEX}
  NOMINMAX
  _CRT_SECURE_NO_WARNINGS)

install (TARGETS wisdom-shared
	EXPORT wisdom-targets
    FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})