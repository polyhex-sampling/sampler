#include "samplingAlgoTool.hpp"

#include <algorithm>
#include <numeric>


template <class WRITER>
AdaptiveSampling<WRITER>::AdaptiveSampling(Tiling* tiling, data_T* data, const short int& ilut, WRITER& write, float (*func)(const Point&), const bool& interpolate, const bool& dither)
: m_tiling(tiling), m_data(data), m_lut_choice(ilut), m_write(write), m_dFunc(func), m_interpolate(interpolate), m_dither(dither)
{}

template <class WRITER>
Point AdaptiveSampling<WRITER>::tile2refPt(const TileState& tileState) const
{
	Point sample = tileState.tile().refPt();
	sample = m_tiling->backTransform( sample, tileState.lvl() );
	sample = sample.hex2cart();
	return sample;
}

template <class WRITER>
Point AdaptiveSampling<WRITER>::tile2bary(const TileState& tileState) const
{
	DBG("tile2bary()");
	Point sample = tileState.tile().refPt();
	sample = sample + m_data->barycenters[ tileState.tile().shapeId() ].cart2hex();
	sample = m_tiling->backTransform( sample, tileState.lvl() );
	sample = sample.hex2cart();
	return sample;
}

template <class WRITER>
Point AdaptiveSampling<WRITER>::tile2offset(const TileState& tileState, const float& rank) const
{
	DBG("tile2offset()");
	Point sample = tileState.tile().refPt();
	if(m_interpolate)
	{
		unsigned short int rankFloor = std::floor(rank);
		unsigned short int rankCeil = std::min((int)std::ceil(rank), m_tiling->subdivFactor()-2);
		float weight = rank - rankFloor;
		Point devFloor = m_data->offsets_list.at(m_lut_choice).at( tileState.offsetId() ).at( rankFloor );
		Point devCeil = m_data->offsets_list.at(m_lut_choice).at( tileState.offsetId() ).at( rankCeil );
		sample = sample + devFloor*(1.-weight) + devCeil*weight;
	}
	else
	{
		unsigned short int rankFloor = std::floor(rank);
		sample = sample + m_data->offsets_list.at(m_lut_choice).at( tileState.offsetId() ).at( rank ).cart2hex();
	}
	sample = m_tiling->backTransform( sample, tileState.lvl() );
	sample = sample.hex2cart();
	return sample;
}

template <class WRITER>
Point AdaptiveSampling<WRITER>::tile2sample(const TileState& tileState, const float& rank) const
{
	DBG("tile2sample()");
	if(m_lut_choice == -1)
		return tile2bary(tileState);
	else if( -1 < m_lut_choice && m_lut_choice < m_data->offsets_list.size() )
		return tile2offset(tileState, rank);
	else
		return tile2refPt(tileState);
}

template <class WRITER>
const bool AdaptiveSampling<WRITER>::crop(const Point& pt, const float& limit) const
{
	if(std::abs(pt.x()) > limit) return true;
	if(std::abs(pt.y()) > limit) return true;
	return false;
}

template <class WRITER>
const bool AdaptiveSampling<WRITER>::cropTile(const TileState& tileState, const float& spaceSize) const
{
	Point pt;
	float limit = 3;
	if( m_data->barycenters.empty() ) pt = tile2refPt(tileState);
	else
	{
		pt = tile2bary(tileState);
		limit /= 2;
	}
	limit = spaceSize + 0.6 * limit / std::pow( m_tiling->scaleRatio(), tileState.lvl() );
	DBG( "crop tile limit = " << limit << " (for lvl " << tileState.lvl() << ")" );
	return crop(pt, limit);
}


template <class WRITER>
std::vector<float> AdaptiveSampling<WRITER>::tileDensity(const TileState& tileState, const float& spaceSize) const
{
	unsigned int size0 = m_tiling->size();
	m_tiling->add(tileState);
	m_tiling->subdivide();
	std::vector<float> d(m_tiling->subdivFactor());
	Point pt;
	for(int i=0; i<m_tiling->subdivFactor(); ++i)
	{
		pt = tile2bary(m_tiling->last())/(2.*spaceSize);
		d[i] = m_dFunc(pt);
		DBG("   local density = " << d[i] << " at " << pt );
		m_tiling->next();
	}
	if(m_tiling->size() != size0) ERR("AdaptiveSampling::tileDensity : modified state of tiling structure");
	return d;
}
template <class WRITER>
std::vector<float> AdaptiveSampling<WRITER>::tileDensityKeep(const float& spaceSize) const
{
	m_tiling->subdivide();
	const unsigned short int sz = m_tiling->size();
	std::vector<float> d(m_tiling->subdivFactor());
	for(int i=0; i<m_tiling->subdivFactor(); ++i)
	{
		d[i] = m_dFunc(tile2bary(m_tiling->last(i))/(2.*spaceSize));
	}
	return d;
}
template <class WRITER>
const float AdaptiveSampling<WRITER>::max(const std::vector<float>& values) const
{
	float max = 0.;
	for(unsigned int i=0; i<values.size(); ++i) max = std::max(values[i], max);
	return max;
}
template <class WRITER>
const float AdaptiveSampling<WRITER>::mean(const std::vector<float>& values) const
{
	float sum = 0.;
	for(unsigned int i=0; i<values.size(); ++i) sum += values[i];
	return sum/(float)values.size();
}
template <class WRITER>
const float AdaptiveSampling<WRITER>::maxTileDensity(const TileState& tileState, const float& spaceSize) const
{
	std::vector<float> values = tileDensity(tileState, spaceSize);
	return max(values);
}
template <class WRITER>
const float AdaptiveSampling<WRITER>::meanTileDensity(const TileState& tileState, const float& spaceSize) const
{
	std::vector<float> values = tileDensity(tileState, spaceSize);
	return mean(values);
}


template <class WRITER>
const unsigned int AdaptiveSampling<WRITER>::generate(const float& white_density, const float& black_density, const unsigned short int& seed, const float& spaceSize)
{
	//g_debuglevel = 4;
	DBG( "AdaptiveSampling:generate()" );
	unsigned int nbSample = 0;
	short unsigned int finalLvl;
	float finalRank;
	const unsigned short int subdivFactor = m_tiling->subdivFactor();
	m_tiling->initialize(seed);
	m_write.clear(spaceSize);
	Point sample;
	while( !m_tiling->isEmpty() )
	{
		const TileState currentTile = m_tiling->last();
		DBG( "  tiling stack size = " << m_tiling->size() );
		DBG( "  current tile: lvl = " << currentTile.lvl() << ", rk = " << currentTile.rank() << ", rk2 = " << currentTile.rank2() << ", sId = " << currentTile.tile().shapeId() << ", oId = " << currentTile.offsetId() );

		if( cropTile(currentTile, spaceSize) )
		{
			m_tiling->next();
			DBG( "    crop tile" );
		}
		else
		{
			const std::vector<float> densityValues = tileDensityKeep(spaceSize);
			finalLvl = computeLvlMinusOneRank((black_density-white_density)*max(densityValues)+white_density, spaceSize, m_tiling->subdivFactor());
			DBG( "    subdivide: tiling stack size = " << m_tiling->size() );
			DBG( "    finalLvl = " << finalLvl );
			if(currentTile.lvl() >= finalLvl)
			{ /// LAST LEVEL
				m_tiling->next(subdivFactor-1);
				DBG( "      last level: remove tile from subdivision" );
				DBG( "                  tiling stack size = " << m_tiling->size() );
				finalLvl = computeLvl((black_density-white_density)*max(densityValues)+white_density, spaceSize, m_tiling->subdivFactor());
				/*/
				finalRank = computeRankCont((black_density-white_density)*mean(densityValues)+white_density, spaceSize, currentTile.lvl(), subdivFactor);
				DBG( "      finalRank = " << finalRank );
				if( currentTile.rank() <= finalRank )
				/*/
				finalRank = computeRankCont((black_density-white_density)*mean(densityValues)+white_density, spaceSize, currentTile.lvl(), subdivFactor);
				float finalRankMod = finalRank;
				if(!m_dither) finalRankMod = std::ceil(finalRank);
				DBG( "      finalRank = " << finalRankMod );
				if( currentTile.rank2() < finalRankMod )
				//*/
				{
					sample = tile2sample(currentTile, finalRank);
					DBG( "        select sample: rank2 = " << currentTile.rank2() );
					DBG( "                       pos = " << sample );
					if( !crop(sample, spaceSize) )
					{
						m_write.sample(sample, currentTile);
						++nbSample;
						DBG( "          keep sample" );
					}
					else
					{
						DBG( "          crop sample" );
					}
				}
				m_tiling->next();
			}
			else
			{
				DBG( "    keep subdivision" );
			}
		}
	}
	return nbSample;
}
