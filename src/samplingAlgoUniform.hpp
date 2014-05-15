#ifndef SAMPLING_ALGO_UNIFORM_H
#define SAMPLING_ALGO_UNIFORM_H


#include "debug.hpp"
#include "tile.hpp"
#include "tileState.hpp"
#include "type.hpp"
#include "vector.hpp"

template <class WRITER>
class UniformSampling
{
	private:
		Tiling* m_tiling;
		data_T* m_data;
		char m_lut_choice;
		WRITER& m_write;

	public:
		UniformSampling(Tiling* tiling, data_T* data, const short int& ilut, WRITER& write);
	
	private:
		Point tile2refPt(const TileState& tileState) const;
		Point tile2bary(const TileState& tileState) const;
		Point tile2offset(const TileState& tileState, const unsigned short int& rank) const;
		Point tile2sample(const TileState& tileState, const unsigned short int& rank) const;

		const bool crop(const Point& pt, const float& limit) const;
		const bool cropTile(const TileState& tileState, const float& spaceSize) const;

	public:
		const unsigned int generate(const float& density, const unsigned short int& seed=0, const float& spaceSize=1);
		const unsigned int generate(const unsigned short int& finalLvl, const unsigned short int& finalRank, const unsigned short int& seed=0, const float& spaceSize=0.21);

		const unsigned int generateSquareDomain(const float& density, const unsigned short int& seed=0, const float& spaceSize=0.21);
		const unsigned int generateSquareDomain(const unsigned short int& finalLvl, const unsigned short int& finalRank, const unsigned short int& seed=0, const float& spaceSize=0.21);

		const unsigned int generateTileDomain(const float& density, const unsigned short int& seed=0, const float& spaceSize=0.21);
		const unsigned int generateTileDomain(const unsigned short int& finalLvl, const unsigned short int& finalRank, const unsigned short int& seed=0, const float& spaceSize=0.21);
};


#include "samplingAlgoUniform.tpl"

#endif
