add_executable( textured main.cpp fs_textured.sc vs_textured.sc varying.def.sc )
target_link_libraries( textured bigg )
set_target_properties( textured PROPERTIES FOLDER "examples" )

add_shader( vs_textured.sc VERTEX   OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/shaders DX11_MODEL 5_0 GLSL 130 )
add_shader( fs_textured.sc FRAGMENT OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/shaders DX11_MODEL 5_0 GLSL 130 )

configure_debugging( textured WORKING_DIR ${CMAKE_CURRENT_BINARY_DIR} )

add_resource(fieldstone-rgba.dds)
add_resource(smiley.png)
