#ifndef WRITER_EMPTY_H
#define WRITER_EMPTY_H


#include "tileState.hpp"
#include "vector.hpp"


class WriterEmpty
{
	public:
		inline void sample(const Point& sample, const TileState& tilestate) {}
		inline void clear(const float& a=1.) const {}
};

#endif
