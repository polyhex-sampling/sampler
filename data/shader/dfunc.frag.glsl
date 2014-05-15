#version 330

#define M_PI 3.1415926535897932384626433832795

uniform int u_dfuncIndex;
uniform int u_dfuncMode;
uniform int u_subdivFactor;
uniform float u_spaceSize;
uniform float u_dmin;
uniform float u_dmax;

in vertexData
{
	vec2 pos;
} vertex;

out vec4 f_color;

void main(void)
{
	float c;
	switch(u_dfuncIndex)
	{
		case 0:
			c = 0;
			break;
		case 1:
			c = vertex.pos.x + 0.5;
			break;
		case 2:
			c = pow(vertex.pos.x + 0.5, 2);
			break;
		case 3:
			c = sqrt(pow(vertex.pos.x, 2) + pow(vertex.pos.y, 2)) * 2;
			break;
		case 4:
			c = sin( 100*M_PI * (pow(vertex.pos.x+0.5, 2) + pow(vertex.pos.y+0.5, 2)) )/2. + 0.5;
			break;
		case 5:
			c = (vertex.pos.y+0.5) * sin( 100*M_PI * pow(vertex.pos.x+0.5, 2) )/2. + 0.5;
			break;
		default:
			c = 0;
	}
	switch(u_dfuncMode)
	{
		case 1:
			c = ((u_dmax-u_dmin)*c) + u_dmin;
			c = floor( log( 2*pow(float(u_subdivFactor), 2.)*c / (2.*u_subdivFactor-1)
						* 2.598076212/pow(2*u_spaceSize, 2)
						) / log(float(u_subdivFactor)) ) / 6.;
			break;
		case 3:
			c = ((u_dmax-u_dmin)*c) + u_dmin;
			float lvl = floor( log( 2*pow(float(u_subdivFactor),2.)*c / (2.*u_subdivFactor-1)
						* 2.598076212/pow(2*u_spaceSize, 2)
						) / log(float(u_subdivFactor)) );
			c = floor( 2.598076212 * c / (pow(2.*u_spaceSize, 2.) * pow(float(u_subdivFactor), lvl-1.)) - 0.5 ) / 35.;
			break;
	}
	c = 1. - c*1.0;
	f_color = vec4(c, c, c, 1.);
}
