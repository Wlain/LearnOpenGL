#version 330 core
#define LIGHT_POSITION_LOCATION 0
#define LIGHT_TEXCOORD_LOCATION 1

layout (location = LIGHT_POSITION_LOCATION) in vec3 a_position;

uniform mat4 u_modelMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_projectionMatrix;

void main()
{
    gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * vec4(a_position, 1.0);
}
