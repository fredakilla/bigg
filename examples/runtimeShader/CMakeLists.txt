add_executable( runtimeShader runtimeShader.cpp fs_cubes.sc vs_cubes.sc )
target_link_libraries( runtimeShader bigg )
set_target_properties( runtimeShader PROPERTIES FOLDER "examples" )

add_shader( vs_cubes.sc VERTEX   OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/shaders DX11_MODEL 5_0 GLSL 130 )
add_shader( fs_cubes.sc FRAGMENT OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/shaders DX11_MODEL 5_0 GLSL 130 )


add_resource(vs_cubes.sc)
add_resource(fs_cubes.sc)

configure_debugging( runtimeShader WORKING_DIR ${CMAKE_CURRENT_BINARY_DIR} )
