#ifndef COLORS_H
#define COLORS_H

#include <vector>

struct color { float r, g, b, a; };

class ColorScheme
{
	private:
		std::vector<struct color> m_colors;
		struct color m_black;
		struct color m_white;
		struct color m_none;

	public:
		ColorScheme();
	private:
		void init();
		struct color createColor(float r, float g, float b, float a);
	public:
		const struct color black() const;
		const struct color white() const;
		const struct color none() const;
		const struct color operator[](const unsigned int index) const;
};

#endif
