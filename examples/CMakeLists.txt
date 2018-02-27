# use this function to copy resource file to binary dir

function(add_resource file)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/${file} ${CMAKE_CURRENT_BINARY_DIR}/${file} COPYONLY)
endfunction()

function(add_shader_src file)
    add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${file}
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/${file} ${CMAKE_CURRENT_BINARY_DIR}/${file}
        MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/${file}
        COMMENT "Copying files: ${file}"
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}

    )
endfunction()

add_subdirectory( cubes )
add_subdirectory( imgui_demo )
add_subdirectory( textured )
add_subdirectory( renderTarget )
add_subdirectory( runtimeShader )
