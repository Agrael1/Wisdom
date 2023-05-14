# CMakeList.txt : Top-level CMake project file
cmake_minimum_required (VERSION 3.22)

project ("Wisdom")

# Enable Hot Reload for MSVC compilers if supported.
set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>")


set(SEVERITY_LEVELS "debug;trace;info;warn;error;critical" CACHE INTERNAL "List of possible severities for logging")
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(CURR_LEVEL "debug")
else()
	set(CURR_LEVEL "warn")
endif()

set(WISDOM_LOG_LEVEL ${CURR_LEVEL} CACHE STRING "Wisdom library logging level")
set_property(CACHE WISDOM_LOG_LEVEL PROPERTY STRINGS ${SEVERITY_LEVELS})
list(FIND SEVERITY_LEVELS "${CURR_LEVEL}" SEV_INDEX)
message("[Wisdom] Log Level is set to ${LOG_LEVEL} [${SEV_INDEX}]")


option(WISDOM_RUNTIME_ASSERTS "Turn on/off runtime assertions (turned on might slightly affect performance)" ON)
option(WISDOM_FORCE_VULKAN "Forces the default device to be vulkan, instead of platform specific" OFF)
option(WISDOM_BUILD_EXAMPLE "Build the example project." ON)
option(WISDOM_BUILD_TESTS "Build the tests." ON)




# Additional CMake functions.
include(Misc.cmake)

# Include sub-projects.
add_subdirectory ("plugins")
add_subdirectory ("wisdom")

if(WISDOM_BUILD_EXAMPLE)
	add_subdirectory ("example")
endif()

if(WISDOM_BUILD_TESTS)
	add_subdirectory ("tests")
endif()
