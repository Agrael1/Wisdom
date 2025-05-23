# Enable Hot Reload for MSVC compilers if supported.
if(MSVC)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT
      "$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>")
endif()

# Check if cpp modules are supported by cmake
set(WISDOM_CPP_MODULES_SUPPORTED OFF)
if (WISDOM_EXPERIMENTAL_CPP_MODULES)
if (CMAKE_VERSION VERSION_LESS "3.28")
  message("[Wisdom] C++ Modules are not supported")
else()
  set(WISDOM_CPP_MODULES_SUPPORTED ON)
  message("[Wisdom] C++ Modules are supported")
endif()
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(CURR_LEVEL "debug")
  set(WDEBUG ON)
else()
  set(CURR_LEVEL "warn")
  set(WDEBUG OFF)
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
option(WISDOM_BUILD_BINDINGS "Build the C bindings library." OFF)
option(WISDOM_SHARED_LIB "Build shared library for C bindings" ON)

option(WISDOM_GENERATE_FUNCTIONS "Generate files for generation Vulkan functions" OFF)
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
