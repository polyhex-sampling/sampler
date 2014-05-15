#include "tile.hpp"


Tile::Tile(const sid_T& shape_id
		, const DPoint& ref_pt)
	: m_shape_id(shape_id)
	, m_ref_pt(ref_pt)
{}
