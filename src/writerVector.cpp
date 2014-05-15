#include "writerVector.hpp"

WriterVector::WriterVector()
: m_colorization(0)
{
	m_pointset.reserve(1e7);
	m_colors.reserve(1e7);
}

void WriterVector::clear(const float& spaceSize)
{
	m_pointset.clear();
	m_colors.clear();
}

void WriterVector::setColorization(unsigned int colorization)
{
	m_colorization = colorization;
}
