if(NOT PLUGINS_LOADED)
    message("Setting up WinRT...")
    load_nuget_dependency(${NUGET_EXE} "Microsoft.Windows.CppWinRT" WinRT ${CMAKE_CURRENT_BINARY_DIR})

    execute_process(COMMAND
        ${WinRT_DIR}/bin/cppwinrt.exe -input sdk -output include
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        RESULT_VARIABLE ret)

    if (NOT ret EQUAL 0)
        message(FATAL_ERROR "Failed to run cppwinrt.exe ${WinRT_DIR} ${ret}")
    endif()
endif()


add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} SYSTEM BEFORE 
    INTERFACE         
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_install_interface(${PROJECT_NAME})
