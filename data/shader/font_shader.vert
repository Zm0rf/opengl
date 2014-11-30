#version 330 core
#extension GL_ARB_explicit_uniform_location : require


// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexColor;
layout(location = 3) in vec2 textureCoordinate;
layout(location = 2) uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

out vec3 fragmentColor;
out vec2 texcoord;
void main(){

    gl_Position.xyz = vertexPosition_modelspace;
    gl_Position.w = 1.0;
	vec4 v = vec4(vertexPosition_modelspace,1); // Transform an homogeneous 4D vector, remember ?
    gl_Position = P*V*M * gl_Position;
 
    // The color of each vertex will be interpolated
    // to produce the color of each fragment
    fragmentColor = vertexColor;

    texcoord = textureCoordinate;
}

