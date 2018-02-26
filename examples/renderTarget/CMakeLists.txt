add_executable( renderTarget renderTarget.cpp vs_textured.sc fs_textured.sc varying.def.sc fx_pixelate.frag )
target_link_libraries( renderTarget bigg )
set_target_properties( renderTarget PROPERTIES FOLDER "examples" )

add_shader( vs_textured.sc VERTEX   OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/shaders DX11_MODEL 5_0 GLSL 130 )
add_shader( fs_textured.sc FRAGMENT OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/shaders DX11_MODEL 5_0 GLSL 130 )
add_shader( fx_pixelate.frag FRAGMENT OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/shaders DX11_MODEL 5_0 GLSL 130 )

configure_debugging( renderTarget WORKING_DIR ${CMAKE_CURRENT_BINARY_DIR} )

add_resource(smiley.jpg)
