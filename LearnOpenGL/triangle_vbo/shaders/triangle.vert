#version 330 core
in vec3 a_position;
in vec3 a_color;
out vec3 v_color;
void main()
{
   gl_Position=vec4(a_position,1.);
   v_color = a_color;
}
