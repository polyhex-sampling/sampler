#ifndef WRITER_FILE_PTS_H
#define WRITER_FILE_PTS_H


#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>

#include "tileState.hpp"
#include "vector.hpp"


class WriterFilePts
{
	private:
		std::string m_filename;
		std::ofstream m_file;

	public:
		WriterFilePts(const std::string& fn);
		~WriterFilePts();

		void open(const float& spaceSize=1.);
		void header(const float& spaceSize=1.);

	public:
		inline void sample(const Point& sample, const TileState& tilestate)
		{
			m_file << sample.x() << "\t" << sample.y() << std::endl;
		}

		void clear(const float& spaceSize=1.);
};

#endif
