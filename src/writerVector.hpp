#ifndef WRITER_VECTOR_H
#define WRITER_VECTOR_H


#include <vector>

#include "colors.hpp"
#include "tileState.hpp"
#include "vector.hpp"

class WriterVector
{
	private:
		std::vector<Point> m_pointset;
		std::vector<struct color> m_colors;
		ColorScheme m_colorScheme;
		unsigned int m_colorization;


	public:
		WriterVector();

		inline void sample(const Point& sample)
		{
			m_pointset.push_back(sample);
		}

		inline void sample(const Point& sample, const TileState& tile)
		{
			m_pointset.push_back(sample);
			switch(m_colorization)
			{
				case 0: //none
					m_colors.push_back(m_colorScheme.black());
					break;
				case 1: //subdiv level
					m_colors.push_back(m_colorScheme[
							tile.lvl()//*4+2
							]);
					break;
				case 2: //parent
					m_colors.push_back(m_colorScheme[
							tile.offsetId()%37
							]);
					break;
				case 3: //rank
					m_colors.push_back(m_colorScheme[
							tile.rank()
							]);
					break;
				default: //default
					m_colors.push_back(m_colorScheme.black());
			}
		}

		void clear(const float& spaceSize=0.21);
		void setColorization(unsigned int colorization);

		const std::vector<Point>& pts() const { return m_pointset; }
		const std::vector<color>& colors() const { return m_colors; }
};

#endif
