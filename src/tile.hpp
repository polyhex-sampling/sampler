#ifndef TILE_H
#define TILE_H


#include "vector.hpp"


class Tile
{
	private:
		DPoint m_ref_pt; // discrete ref point (hex coord)
		sid_T m_shape_id; // id for subdivision rules

	public:
		Tile(const sid_T& shape_id
				, const DPoint& ref_pt);

		const DPoint& refPt() const { return m_ref_pt; };
		const sid_T& shapeId() const { return m_shape_id; };
};


#endif
