#include "writerFilePts.hpp"


WriterFilePts::WriterFilePts(const std::string& fn)
: m_filename(fn)
{
	m_file.open(m_filename.c_str());
	open();
}

WriterFilePts::~WriterFilePts()
{
	m_file.close();
}

void WriterFilePts::clear(const float& spaceSize)
{
	m_file.close();
	open(spaceSize);
}

void WriterFilePts::open(const float& spaceSize)
{
	m_file.open(m_filename.c_str());
	if(!m_file.is_open())
	{
		ERR( "Cannot open output file : " << m_filename );
		exit(EXIT_FAILURE);
	}
	header(spaceSize);
}

void WriterFilePts::header(const float& spaceSize)
{
	m_file << "pts2\t2\tn\trect" << std::endl;
	m_file << -spaceSize << "\t" << spaceSize << "\t" << -spaceSize << "\t" << spaceSize << std::endl;
}
