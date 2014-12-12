#version 330 core
#extension GL_ARB_explicit_uniform_location : require


// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexColor;
layout(location = 2) uniform mat4 model_coordinate;

out vec3 fragmentColor;

const vec4 scale = vec4(2.0, 2.0, 0.0, 1.0);
const vec4 transl = vec4(-1.0, -1.0, 0.0, 0.0);
void main()
{
    gl_Position   = scale * model_coordinate * vec4(vertexPosition_modelspace, 1.0) + transl;
    fragmentColor = vertexColor;
}

