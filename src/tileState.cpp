#include "tileState.hpp"


TileState::TileState(const sid_T& shape_id
		, const DPoint& ref_pt
		, const short int rank
		, const float rank2
		, const oid_T offset_id
		, const unsigned short int lvl)
	: m_tile( Tile(shape_id, ref_pt) )
	, m_rank(rank), m_rank2(rank2)
	, m_offset_id(offset_id)
	, m_subdiv_level(lvl)
{}
