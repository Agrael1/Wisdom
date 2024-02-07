# Enable Hot Reload for MSVC compilers if supported.
if(MSVC)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT
      "$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(CURR_LEVEL "debug")
else()
  set(CURR_LEVEL "warn")
endif()

# Build type
set(DEF_BUILD
    "static"
    CACHE INTERNAL "Default build type for library")
set(BUILD_TYPES
    "headers;static;modules"
    CACHE INTERNAL "List of possible build types for library")
set(WISDOM_BUILD_TYPE
    ${DEF_BUILD}
    CACHE STRING "Wisdom library build type")
set_property(CACHE WISDOM_BUILD_TYPE PROPERTY STRINGS ${BUILD_TYPES})
list(FIND BUILD_TYPES ${WISDOM_BUILD_TYPE} BUILD_INDEX)
message("[Wisdom] Build is ${WISDOM_BUILD_TYPE} [${BUILD_INDEX}]")

set(WISDOM_${WISDOM_BUILD_TYPE} TRUE)
if(WISDOM_headers)
    set(WISDOM_BTYPE INTERFACE)
    set(WISDOM_PUBLIC INTERFACE)
    set(WISDOM_PRIVATE INTERFACE)
else()# static or modules
	set(WISDOM_BTYPE STATIC)
	set(WISDOM_PUBLIC PUBLIC)
	set(WISDOM_PRIVATE PRIVATE)
endif()



# Log level
set(WISDOM_LOG_LEVEL
    ${CURR_LEVEL}
    CACHE STRING "Wisdom library logging level")
set(SEVERITY_LEVELS
    "debug;trace;info;warn;error;critical"
    CACHE INTERNAL "List of possible severities for logging")
set_property(CACHE WISDOM_LOG_LEVEL PROPERTY STRINGS ${SEVERITY_LEVELS})
list(FIND SEVERITY_LEVELS ${WISDOM_LOG_LEVEL} SEV_INDEX)
message("[Wisdom] Log Level is set to ${WISDOM_LOG_LEVEL} [${SEV_INDEX}]")

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  set(WTOP ON)
else()
  set(WTOP OFF)
endif()

if(WTOP AND NOT WISDOM_WIN_STORE)
  set(GEN ON)
endif()

option(
  WISDOM_DEBUG_LAYER
  "Turn on/off runtime debug messages (turned on might slightly affect performance)"
  OFF)
option(WISDOM_FORCE_VULKAN
       "Forces the default device to be vulkan, instead of platform specific"
       OFF)
option(WISDOM_BUILD_EXAMPLES "Build the example project." ${WTOP})
option(WISDOM_BUILD_TESTS "Build the tests." ${WTOP})
option(WISDOM_BUILD_DOCS "Build the documentation." ${WTOP})
option(WISDOM_GENERATE_API "Generate the API" ${GEN})
option(WISDOM_USE_SYSTEM_DXC "Use dxc from PATH" OFF)

if(WISDOM_FORCE_VULKAN)
  message("[Wisdom] Forcing vulkan as default device")
else()
  message("[Wisdom] Using platform specific default device")
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU"
   AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "13"
   OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang"
   AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "16")
  option(WISDOM_USE_FMT "Build Wisdom with fmtlib" ON)
else()
  option(WISDOM_USE_FMT "Build Wisdom with fmtlib" OFF)
endif()

if(WISDOM_modules)
  if(CMAKE_MINOR_VERSION LESS 27)
    message(
      WARNING
        "WARNING: modules are at beta support, use this build mode at your own risk."
    )
  endif()
  if(CMAKE_MINOR_VERSION EQUAL 25)
    set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API
        "3c375311-a3c9-4396-a187-3227ef642046")
  else()
    set(CMAKE_EXPERIMENTAL_CXX_MODULE_CMAKE_API
        "2182bf5c-ef0d-489a-91da-49dbc3090d2a")
  endif()
  set(CMAKE_EXPERIMENTAL_CXX_MODULE_DYNDEP 1)
endif()

set(WISDOM_VERSION "v${PROJECT_VERSION_MAJOR}_${PROJECT_VERSION_MINOR}")

