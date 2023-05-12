# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.22)

project ("Wisdom")

# Enable Hot Reload for MSVC compilers if supported.
set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>")

include(Misc.cmake)

# Include sub-projects.
add_subdirectory ("Plugins")
add_subdirectory ("Wisdom")
add_subdirectory ("Example")
