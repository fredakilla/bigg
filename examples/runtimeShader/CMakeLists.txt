add_executable( runtimeShader runtimeShader.cpp vs_cubes.sc fs_cubes.sc varying.def.sc)
target_link_libraries( runtimeShader bigg )
set_target_properties( runtimeShader PROPERTIES FOLDER "examples" )

# copy shaders src to build dir
add_shader_src(vs_cubes.sc)
add_shader_src(fs_cubes.sc)
add_shader_src(varying.def.sc)

# copy 'common' folder to build dir
file(COPY common DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

configure_debugging( runtimeShader WORKING_DIR ${CMAKE_CURRENT_BINARY_DIR} )
