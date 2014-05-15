#version 330

#define PI 3.141592653589793238462643383279
#define DISK_SIZE 32
#define DISK_SIZE_2 66 //DISK_SIZE*2+2

layout (points) in;
layout (triangle_strip) out;
layout (max_vertices = DISK_SIZE_2) out;

uniform mat4 u_projectionMatrix;
uniform float u_diskRadius;

in vertexData
{
	vec4 color;
} vertex[];

out fragmentData
{
	vec4 color;
} frag;

void main()
{
	int i, j;
	float a;
	for(i = 0; i < gl_in.length(); i++)
	{
		a = 0;
		vec4 center = u_projectionMatrix*gl_in[i].gl_Position;

		float radius = u_diskRadius;

		for(j=0; j<=DISK_SIZE; j++)
		{
			a += 2.0*PI/DISK_SIZE;
			vec4 shift = vec4(radius*cos(a), radius*sin(a), 0.0, 0.0);

			frag.color = vertex[0].color;
			gl_Position = u_projectionMatrix*(gl_in[i].gl_Position + shift);
			EmitVertex();

			frag.color = vertex[0].color;
			gl_Position = center;
			EmitVertex();
		}
	}
	EndPrimitive();
}
