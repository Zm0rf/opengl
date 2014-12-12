#version 330 core

// Ouput data
out vec4 color;
in vec3 textureCoordinate;
in vec2 texcoord;

uniform sampler2D test_texture;

const vec4 text_color = vec4(1.0, 1.0, 1.0, 1.0);
void main()
{
    vec4 t_color = texture(test_texture, texcoord*vec2(0.1, 1.0));
    if( t_color.b == 1 )
    {
        discard;
    }
    color = text_color;
    //antialiasing
    /* color.a = -t_color.g; */
}
