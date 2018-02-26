$input v_color0, v_texcoord0

#include <bgfx_shader.sh>

SAMPLER2D(u_texture, 0);

// Uniforms
//uniform sampler2D u_texture;

// Inputs
//varying vec4 v_color0;
//varying vec2 v_texcoord0;


float discretize(float f, float d)
{
    return floor(f*d + 0.5)/d;
}

vec2 discretize(vec2 v, float d)
{
    return vec2(discretize(v.x, d), discretize(v.y, d));
}

void main()
{
    vec2 texCoord = discretize(v_texcoord0, 64.0);
    gl_FragColor = texture2D(u_texture, texCoord);
}

 
 
