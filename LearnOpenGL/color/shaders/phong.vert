#version 330 core
#define CUBE_POSITION_LOCATION 0
#define CUBE_TEXCOORD_LOCATION 1
#define CUBE_NORMAL_LOCATION   2

layout (location = CUBE_POSITION_LOCATION) in vec3 a_position;
layout (location = CUBE_TEXCOORD_LOCATION) in vec2 a_texCoord;
layout (location = CUBE_NORMAL_LOCATION) in vec3 a_normal;

uniform mat4 u_modelMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_projectionMatrix;

out vec2 v_texCoord;
out vec3 v_normal;
out vec3 v_worldPosition;

void main()
{
    v_normal    = a_normal;
    v_texCoord  = a_texCoord;
    v_worldPosition = (u_modelMatrix * vec4(a_position, 1.0)).xyz;
    gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * vec4(a_position, 1.0);
}
