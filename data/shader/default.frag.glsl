#version 330

uniform vec4 u_color;

out vec4 f_color;

void main(void)
{
	f_color = u_color;
}
