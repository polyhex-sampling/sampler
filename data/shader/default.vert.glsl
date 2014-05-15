#version 330

uniform mat4 u_projectionMatrix;

in vec2 a_position;

void main(void)
{
	gl_Position = u_projectionMatrix*vec4(a_position, 0.0, 1.0);
}
