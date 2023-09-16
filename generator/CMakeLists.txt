project(generator)

include(FetchContent)

set(FETCHCONTENT_UPDATES_DISCONNECTED ON)

message("Loading tinyxml2...")
FetchContent_Declare(
	tinyxml2
	GIT_REPOSITORY https://github.com/leethomason/tinyxml2.git
	GIT_TAG origin/master
)
FetchContent_MakeAvailable(tinyxml2)

add_executable(${PROJECT_NAME})
target_sources (${PROJECT_NAME} 
	PRIVATE
		entry_main.cpp 
		generator.h
		generator.cpp
 "string_literal.h" "enums.h")
target_compile_definitions(${PROJECT_NAME} 
	PRIVATE 
		"INPUT_FILE=\"${CMAKE_CURRENT_SOURCE_DIR}/../xml/wis.xml\""
		"OUTPUT_DIR=\"${CMAKE_CURRENT_SOURCE_DIR}/../wisdom/bindings\""
)

target_link_libraries(${PROJECT_NAME} 
	PRIVATE
		tinyxml2::tinyxml2
)

if(WISDOM_USE_FMT)
	target_compile_definitions(${PROJECT_NAME} PUBLIC WISDOM_USE_FMT)
	target_link_libraries(${PROJECT_NAME} 
		PRIVATE
			fmt::fmt
	)
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
	CXX_STANDARD 20
	CXX_STANDARD_REQUIRED ON
)