project(wisdom-extensions)

add_library(wisdom-extension-headers INTERFACE)
add_library(wis::extension-headers ALIAS wisdom-extension-headers)
add_library(wisdom-extension INTERFACE)
add_library(wis::extension ALIAS wisdom-extension)

add_subdirectory(debug_info)