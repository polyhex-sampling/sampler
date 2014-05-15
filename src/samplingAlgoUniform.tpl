#include <cmath>
#include <algorithm>

#include "samplingAlgoTool.hpp"


template <class WRITER>
UniformSampling<WRITER>::UniformSampling(Tiling* tiling, data_T* data, const short int& ilut, WRITER& write)
: m_tiling(tiling), m_data(data), m_lut_choice(ilut), m_write(write)
{}

template <class WRITER>
Point UniformSampling<WRITER>::tile2refPt(const TileState& tileState) const
{
	DBG("UniformSampling::tile2refPt()");
	Point sample = tileState.tile().refPt();
	sample = m_tiling->backTransform( sample, tileState.lvl() );
	sample = sample.hex2cart();
	DBG("~~");
	return sample;
}

template <class WRITER>
Point UniformSampling<WRITER>::tile2bary(const TileState& tileState) const
{
	DBG("UniformSampling::tile2bary()");
	Point sample = tileState.tile().refPt();
	sample = sample + m_data->barycenters[ tileState.tile().shapeId() ].cart2hex();
	sample = m_tiling->backTransform( sample, tileState.lvl() );
	sample = sample.hex2cart();
	DBG("~~");
	return sample;
}

template <class WRITER>
Point UniformSampling<WRITER>::tile2offset(const TileState& tileState, const unsigned short int& rank) const
{
	DBG("UniformSampling::tile2offset()");
	Point sample = tileState.tile().refPt();
	Point dev = m_data->offsets_list.at(m_lut_choice).at( tileState.offsetId() ).at( rank );
	DBG( "  offsetId = " << tileState.offsetId() << ", rank = " << rank << ", dev = " << dev);
	sample = sample + dev.cart2hex();
	sample = m_tiling->backTransform( sample, tileState.lvl() );
	sample = sample.hex2cart();
	DBG("~~");
	return sample;
}

template <class WRITER>
Point UniformSampling<WRITER>::tile2sample(const TileState& tileState, const unsigned short int& rank) const
{
	DBG("UniformSampling::tile2sample()");
	if(m_lut_choice == -1)
		return tile2bary(tileState);
	else if( -1 < m_lut_choice && m_lut_choice < m_data->offsets_list.size() )
	{
		DBG("~~");
		return tile2offset(tileState, rank);
	}
	else
	{
		DBG("~~");
		return tile2refPt(tileState);
	}
}

template <class WRITER>
const bool UniformSampling<WRITER>::crop(const Point& pt, const float& limit) const
{
	if(std::abs(pt.x()) > limit) return true;
	if(std::abs(pt.y()) > limit) return true;
	DBG("~~");
	return false;
}

template <class WRITER>
const bool UniformSampling<WRITER>::cropTile(const TileState& tileState, const float& spaceSize) const
{
	DBG( "UniformSampling::cropTile()" );
	Point pt;
	float limit = 3;
	if( m_data->barycenters.empty() ) pt = tile2refPt(tileState);
	else
	{
		pt = tile2bary(tileState);
		limit /= 2;
	}
	limit = spaceSize + 0.6 * limit / std::pow( m_tiling->scaleRatio(), tileState.lvl() );
	DBG( "  limit = " << limit << "(spaceSize = " << spaceSize << "lvl = " << tileState.lvl() << ")" );
	DBG("~~");
	return crop(pt, limit);
}

template <class WRITER>
const unsigned int UniformSampling<WRITER>::generate(const float& density, const unsigned short int& seed, const float& spaceSize)
{
	//return generateTileDomain(density, seed, spaceSize);
	return generateSquareDomain(density, seed, spaceSize);
}

template <class WRITER>
const unsigned int UniformSampling<WRITER>::generate(const unsigned short int& finalLvl, const unsigned short int& finalRank, const unsigned short int& seed, const float& spaceSize)
{
	//return generateTileDomain(finalLvl, finalRank, seed, spaceSize);
	return generateSquareDomain(finalLvl, finalRank, seed, spaceSize);
}

template <class WRITER>
const unsigned int UniformSampling<WRITER>::generateSquareDomain(const float& density, const unsigned short int& seed, const float& spaceSize)
{
	unsigned short int finalLvl = computeLvlMinusHalfRank(density, spaceSize, m_tiling->subdivFactor());
	unsigned short int finalRank = computeRank(density, spaceSize, finalLvl, m_tiling->subdivFactor());
	return generateSquareDomain(finalLvl, finalRank, seed, spaceSize);
}

template <class WRITER>
const unsigned int UniformSampling<WRITER>::generateSquareDomain(const unsigned short int& finalLvl, const unsigned short int& finalRank, const unsigned short int& seed, const float& spaceSize)
{
	//g_debuglevel = 4;
	DBG( "UniformSampling:generateSquareDomain()" );
	DBG( "  final lvl = " << finalLvl << ", final rk = " << finalRank );
	unsigned int nbSample = 0;
	m_tiling->initialize(seed);
	m_write.clear(spaceSize);
	Point sample;
	while( !m_tiling->isEmpty() )
	{
		TileState currentTile = m_tiling->last();
		DBG( "  tiling stack size = " << m_tiling->size() );
		DBG( "  current tile: lvl = " << currentTile.lvl() << ", rk = " << currentTile.rank() << ", rk2 = " << currentTile.rank2() << ", sId = " << currentTile.tile().shapeId() << ", oId = " << currentTile.offsetId() );
		if(currentTile.lvl() < finalLvl-1)
		{ /// SUBDIVISION
			if( cropTile(currentTile, spaceSize) )
			{
				m_tiling->next();
				DBG( "    subdivide -> crop" );
			}
			else
			{
				m_tiling->subdivide();
				DBG( "    subdivide -> ok" );
			}
		}
		else if(currentTile.lvl() == finalLvl-1)
		{ /// LAST SUBDIVISION
			if( cropTile(currentTile, spaceSize) )
			{
				m_tiling->next();
				DBG( "    subdivide last -> crop" );
			}
			else
			{
				m_tiling->subdivide(finalRank);
				DBG( "    subdivide last -> ok" );
			}
		}
		else
		{ /// SELECTION
			sample = tile2sample(currentTile, finalRank);
			DBG( "    sample pos=" << sample );
			if( !crop(sample, spaceSize) )
			{
				DBG( "      write" );
				m_write.sample(sample, currentTile);
				++nbSample;
			}
			else
			{
				DBG( "      crop" );
			}
			m_tiling->next();
		}
	}
	DBG("~~");
	return nbSample;
}

template <class WRITER>
const unsigned int UniformSampling<WRITER>::generateTileDomain(const float& density, const unsigned short int& seed, const float& spaceSize)
{
	unsigned short int finalLvl = std::floor(density);
	unsigned short int finalRank = std::min(std::floor(100.*(density-finalLvl)+0.5), 35.);
	DBG("finalLvl = " << finalLvl);
	DBG("finalRank = " << finalRank);
	generateTileDomain(finalLvl, finalRank, seed, spaceSize);
}

template <class WRITER>
const unsigned int UniformSampling<WRITER>::generateTileDomain(const unsigned short int& finalLvl, const unsigned short int& finalRank, const unsigned short int& seed, const float& spaceSize)
{
	//g_debuglevel = 4;
	unsigned int nbSample = 0;
	m_tiling->initialize(seed);
	m_write.clear(spaceSize);
	Point sample;
	while( !m_tiling->isEmpty() )
	{
		TileState currentTile = m_tiling->last();
		if(currentTile.lvl() < finalLvl-1)
		{ /// SUBDIVISION
			m_tiling->subdivide();
		}
		else if(currentTile.lvl() == finalLvl-1)
		{
			m_tiling->subdivide(finalRank);
		}
		else
		{ /// SELECTION
			sample = tile2sample(currentTile, finalRank);
			DBG( "writing:   lvl=" << m_tiling->last().lvl() << " rank=" << m_tiling->last().rank() << " | stack size = " << m_tiling->size() );
			m_write.sample(sample, currentTile);
			++nbSample;
			DBG( "next tile");
			m_tiling->next();
		}
	}
	return nbSample;
}
