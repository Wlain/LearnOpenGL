#version 330 core
in vec3 v_color;
in vec2 v_texCoord;

uniform sampler2D u_texture0;
uniform sampler2D u_texture1;

out vec4 fragColor;
void main()
{
    vec3 texture0 = texture(u_texture0, v_texCoord).rgb;
    vec3 texture1 = texture(u_texture1, v_texCoord).rgb;
    fragColor = vec4(mix(texture0, texture1, 0.2), 1.0);
}
