#ifndef SAMPLING_ALGO_ADAPTIVE_H
#define SAMPLING_ALGO_ADAPTIVE_H


template <class WRITER>
class AdaptiveSampling
{
	private:
		Tiling* m_tiling;
		data_T* m_data;
		char m_lut_choice;
		WRITER& m_write;
		float (*m_dFunc)(const Point&);
		bool m_interpolate;
		bool m_dither;

	public:
		AdaptiveSampling(Tiling* tiling, data_T* data, const short int& ilut, WRITER& write, float (*func)(const Point&), const bool& interpolate=true, const bool& dither=true);
	
	private:
		Point tile2refPt(const TileState& tileState) const;
		Point tile2bary(const TileState& tileState) const;
		Point tile2offset(const TileState& tileState, const float& rank) const;
		Point tile2sample(const TileState& tileState, const float& rank) const;

		const bool crop(const Point& pt, const float& limit) const;
		const bool cropTile(const TileState& tileState, const float& spaceSize) const;

		std::vector<float> tileDensity(const TileState& tileState, const float& spaceSize) const;
		std::vector<float> tileDensityKeep(const float& spaceSize) const;
		const float max(const std::vector<float>& values) const;
		const float mean(const std::vector<float>& values) const;
		const float maxTileDensity(const TileState& tileState, const float& spaceSize) const;
		const float meanTileDensity(const TileState& tileState, const float& spaceSize) const;

	public:
		const unsigned int generate(const float& white_density, const float& black_density, const unsigned short int& seed=0, const float& spaceSize=1);
};


#include "samplingAlgoAdaptive.tpl"

#endif
