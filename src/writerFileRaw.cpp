#include "writerFileRaw.hpp"


WriterFileRaw::WriterFileRaw(const std::string& fn)
: m_filename(fn)
{
	m_file.open(m_filename.c_str());
	if(!m_file.is_open())
	{
		ERR( "Cannot open output file : " << m_filename );
		std::exit(EXIT_FAILURE);
	}
}

WriterFileRaw::~WriterFileRaw()
{
	m_file.close();
}
