#version 330

in fragmentData
{
	vec4 color;
} frag;

out vec4 f_color;

void main(void)
{
	f_color = frag.color;
}
