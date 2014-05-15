#include "colors.hpp"

ColorScheme::ColorScheme()
{
	init();
}

struct color ColorScheme::createColor(float r, float g, float b, float a)
{
	struct color c;
	c.r = r;
	c.g = g;
	c.b = b;
	c.a = a;
	return c;
}

const struct color ColorScheme::black() const
{
	return m_black;
}

const struct color ColorScheme::white() const
{
	return m_white;
}

const struct color ColorScheme::none() const
{
	return m_none;
}

const struct color ColorScheme::operator[](const unsigned int index) const
{
	return m_colors[index % m_colors.size()];
}

void ColorScheme::init()
{
	m_black = createColor(0,0,0,1);
	m_white = createColor(1,1,1,1);
	m_none = createColor(1,1,1,0);

	m_colors.reserve(13);
	m_colors.push_back(createColor(0.000,0.121,0.246,1.000)); //navy
	m_colors.push_back(createColor(0.000,0.453,0.848,1.000)); //blue
	m_colors.push_back(createColor(0.496,0.855,0.996,1.000)); //aqua
	m_colors.push_back(createColor(0.223,0.797,0.797,1.000)); //teal
	m_colors.push_back(createColor(0.238,0.598,0.438,1.000)); //olive
	m_colors.push_back(createColor(0.180,0.797,0.250,1.000)); //green
	m_colors.push_back(createColor(0.004,0.996,0.438,1.000)); //lime
	m_colors.push_back(createColor(0.996,0.859,0.000,1.000)); //yellow
	m_colors.push_back(createColor(0.996,0.520,0.105,1.000)); //orange
	m_colors.push_back(createColor(0.996,0.254,0.211,1.000)); //red
	m_colors.push_back(createColor(0.520,0.078,0.293,1.000)); //marroon
	m_colors.push_back(createColor(0.938,0.070,0.742,1.000)); //fuchsia
	m_colors.push_back(createColor(0.691,0.051,0.785,1.000)); //purple
}
