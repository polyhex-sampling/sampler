#version 330

uniform mat4 u_projectionMatrix;
uniform float u_spaceSize;

in vec2 a_position;

out vertexData
{
	vec2 pos;
} vertex;


void main(void)
{
	vertex.pos = a_position/(2*u_spaceSize);
	gl_Position = u_projectionMatrix*vec4(a_position, 0.0, 1.0);
}
