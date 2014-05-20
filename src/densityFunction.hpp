#ifndef DENSITY_FUNCTION_H
#define DENSITY_FUNCTION_H

#include "debug.hpp"
#include "vector.hpp"

/****  [-0.5, 0.5]^2 ---dFunc--> [0, 1]  ****/
float dFunc_constant(const Point& pt);
float dFunc_linear(const Point& pt);
float dFunc_quadra(const Point& pt);
float dFunc_radial(const Point& pt);
float dFunc_zoneplate(const Point& pt);
float dFunc_contrast(const Point& pt);

class Image
{
	private:
		static void* m_data;

	public:
		Image();
		Image(const std::string& filename);
		void load(const std::string& filename);

		static float dFunc(const Point& pt);

		bool isNull();
};

#endif
