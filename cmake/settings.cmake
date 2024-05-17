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

#TODO: Restore this
if(FALSE AND WTOP AND NOT WISDOM_WIN_STORE)
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
option(WISDOM_BUILD_BINARIES "Build the static lib." ON)
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

set(WISDOM_VERSION "v${PROJECT_VERSION_MAJOR}_${PROJECT_VERSION_MINOR}")
