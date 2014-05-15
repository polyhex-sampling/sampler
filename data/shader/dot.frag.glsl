#version 330

in vertexData
{
	vec4 color;
} vertex;

out vec4 f_color;

void main(void)
{
	f_color = vertex.color;
}
