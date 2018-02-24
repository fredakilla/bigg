# use this function to copy resource file to binary dir
function(add_resource file)
  add_custom_command(
      TARGET textured POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy
      ${CMAKE_CURRENT_SOURCE_DIR}/${file}
      ${CMAKE_CURRENT_BINARY_DIR}/${file})
endfunction()

add_subdirectory( cubes )
add_subdirectory( imgui_demo )
add_subdirectory( textured )
