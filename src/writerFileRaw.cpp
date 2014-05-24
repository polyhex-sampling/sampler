#include "writerFileRaw.hpp"


WriterFileRaw::WriterFileRaw(const std::string& fn)
: m_filename(fn)
{
	m_file.open(m_filename.c_str());
	open();
}

WriterFileRaw::~WriterFileRaw()
{
	m_file.close();
}


void WriterFileRaw::clear(const float& spaceSize)
{
	m_file.close();
	open(spaceSize);
}

void WriterFileRaw::open(const float& spaceSize)
{
	m_spaceSize = 2*spaceSize;
	m_file.open(m_filename.c_str());
	if(!m_file.is_open())
	{
		ERR( "Cannot open output file : " << m_filename );
		std::exit(EXIT_FAILURE);
	}
}
