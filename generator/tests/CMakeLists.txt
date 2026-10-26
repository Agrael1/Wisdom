add_executable(generator-snapshot snapshot.cpp)
set_target_properties(generator-snapshot PROPERTIES CXX_STANDARD 20)
add_dependencies(generator-snapshot generator)

add_executable(generator-documentation-tests documentation.cpp ../documentation.cpp)
set_target_properties(generator-documentation-tests PROPERTIES CXX_STANDARD 20)
target_include_directories(generator-documentation-tests PRIVATE ..)
target_compile_definitions(
  generator-documentation-tests
  PRIVATE "$<TARGET_PROPERTY:generator,COMPILE_DEFINITIONS>")
target_link_libraries(generator-documentation-tests PRIVATE tinyxml2::tinyxml2)
if(WISDOM_USE_FMT)
  target_link_libraries(generator-documentation-tests PRIVATE fmt::fmt-header-only)
endif()

add_test(NAME generator.documentation-words COMMAND generator-documentation-tests)
set_tests_properties(generator.documentation-words PROPERTIES LABELS generator TIMEOUT 60)

add_custom_target(generator-tests DEPENDS generator-snapshot generator-documentation-tests)

set(GENERATOR_TEST_CASES
	core-code
	core-docs
	core-docs-update
	platform-code
	platform-docs
	platform-docs-update
	fixture-code
	fixture-docs
	fixture-docs-update
	fixture-all
	output-options
	snapshot-checker
	invalid-arguments)

foreach(CASE IN LISTS GENERATOR_TEST_CASES)
  add_test(
	NAME generator.${CASE}
	COMMAND
	  ${CMAKE_COMMAND} "-DGENERATOR=$<TARGET_FILE:generator>"
	  "-DSNAPSHOT_CHECKER=$<TARGET_FILE:generator-snapshot>"
		"-DREGISTRY_DIR=${CMAKE_CURRENT_SOURCE_DIR}/../../xml"
	  "-DTEST_BINARY_DIR=${CMAKE_CURRENT_BINARY_DIR}" "-DCASE=${CASE}"
	  -P "${CMAKE_CURRENT_SOURCE_DIR}/run.cmake")
  set_tests_properties(generator.${CASE} PROPERTIES LABELS generator TIMEOUT 60)
endforeach()

add_custom_target(
  update-generator-snapshots
  COMMAND
	${CMAKE_COMMAND} "-DGENERATOR=$<TARGET_FILE:generator>"
	"-DSNAPSHOT_CHECKER=$<TARGET_FILE:generator-snapshot>"
	"-DREGISTRY_DIR=${CMAKE_CURRENT_SOURCE_DIR}/../../xml"
	"-DTEST_BINARY_DIR=${CMAKE_CURRENT_BINARY_DIR}" -DUPDATE_SNAPSHOTS=ON
	-P "${CMAKE_CURRENT_SOURCE_DIR}/run.cmake"
  DEPENDS generator-snapshot
  COMMENT "Updating generator characterization snapshots; review the diff"
  VERBATIM)