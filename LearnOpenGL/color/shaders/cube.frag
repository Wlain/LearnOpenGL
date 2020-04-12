#version 330 core
in vec2 v_texCoord;

uniform sampler2D u_texture0;
uniform sampler2D u_texture1;
uniform vec3 u_objectColor;
uniform vec3 u_lightColor;

out vec4 fragColor;
void main()
{
    vec3 texture0 = texture(u_texture0, v_texCoord).rgb;
    vec3 texture1 = texture(u_texture1, v_texCoord).rgb;
    fragColor = vec4(u_objectColor * u_lightColor, 1.0);
}
