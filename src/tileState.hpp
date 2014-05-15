#ifndef TILE_STATE_H
#define TILE_STATE_H


#include "tile.hpp"


class TileState
{
	private:
		Tile m_tile;
		short int m_rank; //rank in the parent's subdivision rule
		float m_rank2; //second order rank in the parent's subdivision rule
		oid_T m_offset_id; // id for offset lut. Construct from previous tiles information and subdivision rules
		unsigned short int m_subdiv_level; // subdivive level of the tile in the subdivision tree

	public:
		TileState(const sid_T& shape_id
				, const DPoint& ref_pt
				, const short int rank
				, const float rank2
				, const oid_T offset_id
				, const unsigned short int lvl);

		const Tile& tile() const { return m_tile; };
		const short int& rank() const { return m_rank; };
		const float& rank2() const { return m_rank2; };
		const oid_T& offsetId() const { return m_offset_id; };
		const unsigned short int& lvl() const { return m_subdiv_level; };
};


#endif
