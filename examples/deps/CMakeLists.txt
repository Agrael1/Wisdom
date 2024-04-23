
if(WISDOM_WINDOWS)
	find_program(
	  NUGET_EXE
	  NAMES nuget
	  PATHS ${CMAKE_CURRENT_SOURCE_DIR}/NuGet)
	if(NOT NUGET_EXE)
	  message("NUGET.EXE not found.")
	  message(FATAL_ERROR "Please install this executable, and run CMake again.")
	endif()
endif()

include(KDGui.cmake)