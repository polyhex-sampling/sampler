#ifndef WRITER_FILE_RAW_H
#define WRITER_FILE_RAW_H


#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>

#include "vector.hpp"


class WriterFileRaw
{
	private:
		std::string m_filename;
		std::ofstream m_file;

	public:
		WriterFileRaw(const std::string& fn);
		~WriterFileRaw();

	public:
		inline void sample(const Point& sample)
		{
			m_file << sample.x() << "\t" << sample.y() << std::endl;
		}

		inline void clear()
		{
			m_file.close();
			m_file.open(m_filename.c_str());
			if(!m_file.is_open())
			{
				std::cerr << "<<! Cannot open output file : " << m_filename << std::endl;
				exit(EXIT_FAILURE);
			}
		}
};

#endif
