project(multiview-${POSTFIX})

set(HEADERS )
set(SOURCES entry_main.cpp)

add_executable(${PROJECT_NAME} STATIC ${HEADERS} ${SOURCES})
target_link_libraries(${PROJECT_NAME} PUBLIC example-helper-${POSTFIX})
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 20)