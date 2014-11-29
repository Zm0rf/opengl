#version 330 core

// Ouput data
out vec4 color;
in vec3 fragmentColor;
in vec4 gl_FragCoord;
/* in vec2 screen_dimensions; */

void main()
{
    color = vec4(fragmentColor, 0.1);
    color.a=0.1;
    /* if( color.r > color.b ) */
    /*     discard; */
    /* color = vec4(1.0, 0.0, 0.0, 0.5); */
    if( gl_FragCoord.x > 500 )
        discard;
    /* if( gl_FragCoord.x > screen_dimensions.x) */
    /*     discard; */
}
