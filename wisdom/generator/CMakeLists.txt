project(binding-generator)

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME}
	PRIVATE
		entry_main.cpp
)
target_link_libraries(${PROJECT_NAME} PRIVATE Wisdom)

set_target_properties(${PROJECT_NAME} PROPERTIES
	CXX_STANDARD 20
	CXX_STANDARD_REQUIRED ON
)