add_subdirectory(shaders)

add_example(lut)
add_example(multiview)
add_example(descriptor_storage)
add_example(raytracing)

if(WISDOM_CPP_MODULES_SUPPORTED)
  add_example(modules)
endif()
