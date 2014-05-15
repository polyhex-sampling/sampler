#include "densityFunction.hpp"

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>


float dFunc_constant(const Point& pt)
{ return 1.0; }

float dFunc_linear(const Point& pt)
{ return pt.x()+0.5; }

float dFunc_quadra(const Point& pt)
{ return std::pow(pt.x()+0.5, 2); }

float dFunc_radial(const Point& pt)
{ return pt.norm()*2.; }

float dFunc_zoneplate(const Point& pt)
{ return std::sin( 100*M_PI * (std::pow(pt.x()+0.5, 2) + std::pow(pt.y()+0.5, 2)) )/2. + 0.5; }

float dFunc_contrast(const Point& pt)
{ return (pt.y()+0.5) * std::sin( 100*M_PI * std::pow(pt.x()+0.5, 2) )/2. + 0.5; }


// Class Image //
void* Image::m_data = NULL;

Image::Image(const std::string& filename)
{
	load(filename);
}

void Image::load(const std::string& filename)
{
	SDL_Surface *image = IMG_Load(filename.c_str());
	if(!image)
	{
		ERR("Can't load image : " << filename);
		exit(EXIT_FAILURE);
	}
	m_data = image;
}

float Image::dFunc(const Point& pt)
{
	SDL_Surface *image = (SDL_Surface*) m_data;
	int bpp = image->format->BytesPerPixel;
	int px = std::max(std::min(image->w-1, (int)((image->w-1)*(pt.x()+0.5))), 0);
	int py = std::max(std::min(image->h-1, (int)((image->h-1)*(0.5-pt.y()))), 0);
	unsigned char c = ((unsigned char*)image->pixels)[py*image->pitch + px*bpp];
	return 1.0-std::max(std::min(1.0, ((float)c)/255.0), 0.0);
}
