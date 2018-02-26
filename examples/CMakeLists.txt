# use this function to copy resource file to binary dir
function(add_resource file)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/${file} ${CMAKE_CURRENT_BINARY_DIR}/${file} COPYONLY)
endfunction()

add_subdirectory( cubes )
add_subdirectory( imgui_demo )
add_subdirectory( textured )
add_subdirectory( renderTarget )
add_subdirectory( runtimeShader )
