#version 330 core
in vec3 v_color;
in vec2 v_texCoord;

uniform sampler2D u_texture;

out vec4 fragColor;
void main()
{
    vec4 texture = texture(u_texture, v_texCoord);
    fragColor=texture;
}
