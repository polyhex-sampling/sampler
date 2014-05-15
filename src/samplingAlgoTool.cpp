#include "samplingAlgoTool.hpp"

#include <algorithm>
#include <cmath>


unsigned short int computeLvl(const float& density, const float& spaceSize, const unsigned short int& subdivFactor)
{
	return std::floor( std::log(
				subdivFactor * density
				* 2.598076212/* 3*hex area */ /std::pow(2*spaceSize, 2)
				) / std::log(subdivFactor) );
}

unsigned short int computeLvlMinusHalfRank(const float& density, const float& spaceSize, const unsigned short int& subdivFactor)
{
	return std::floor( std::log(
				2*std::pow(subdivFactor,2)*density / (2*subdivFactor-1)
				* 2.598076212/* 3*hex area */ /std::pow(2*spaceSize, 2)
				) / std::log(subdivFactor) );
}

unsigned short int computeLvlMinusOneRank(const float& density, const float& spaceSize, const unsigned short int& subdivFactor)
{
	return std::floor( std::log(
				std::pow(subdivFactor,2)*density / (subdivFactor-1)
				* 2.598076212/* 3*hex area */ /std::pow(2*spaceSize, 2)
				) / std::log(subdivFactor) );
}

short int computeRank(const float& density, const float& spaceSize, const unsigned short int& lvl, const unsigned short int& subdivFactor)
{
	return std::floor( (2.598076212/* 3*hex area */ * density/std::pow(2*spaceSize, 2))
				/ std::pow(subdivFactor, lvl-1) - 0.5 );
}

float computeRankCont(const float& density, const float& spaceSize, const unsigned short int& lvl, const unsigned short int& subdivFactor)
{
	return std::fmod(
			(2.598076212/* 3*hex area */ * density/std::pow(2*spaceSize, 2))
			 / std::pow(subdivFactor, lvl-1)
			, subdivFactor-1);
}

float computeDensity(const unsigned short int& lvl, const unsigned short int& rank, const float& spaceSize, const unsigned short int& subdivFactor)
{
	return std::pow(subdivFactor, lvl-1) * (rank+1)
		* std::pow(2*spaceSize, 2) / 2.598076212/* 3*hex area */ + 0.5;
}
