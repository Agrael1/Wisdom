# CPack sets this variable to the root of the staging directory right before packaging.
set(STAGING_DIR "${CPACK_TEMPORARY_INSTALL_DIRECTORY}")
set(NUGET_NATIVE_LIB_DIR "${STAGING_DIR}/lib/native/x64")
file(MAKE_DIRECTORY "${NUGET_NATIVE_LIB_DIR}")

message(STATUS "Wisdom CPack: Reorganizing staging directory at ${STAGING_DIR}")

# 1. Strip out the CMake configs
if(EXISTS "${STAGING_DIR}/lib/cmake")
  file(REMOVE_RECURSE "${STAGING_DIR}/lib/cmake")
endif()

# 2. Ensure /lib exists
file(MAKE_DIRECTORY "${STAGING_DIR}/lib")

# 3. Move the DLLs from /bin to /lib
file(GLOB _wisdom_native_dlls "${STAGING_DIR}/bin/*.dll")
foreach(_dll IN LISTS _wisdom_native_dlls)
  get_filename_component(_dll_name "${_dll}" NAME)
  file(RENAME "${_dll}" "${NUGET_NATIVE_LIB_DIR}/${_dll_name}")
endforeach()

# 3. (Optional but recommended) Move your static .libs / import .libs there too!
file(GLOB _wisdom_static_libs "${STAGING_DIR}/lib/*.lib")
foreach(_lib IN LISTS _wisdom_static_libs)
  get_filename_component(_lib_name "${_lib}" NAME)
  file(RENAME "${_lib}" "${NUGET_NATIVE_LIB_DIR}/${_lib_name}")
endforeach()