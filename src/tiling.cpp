#include "tiling.hpp"

#include <ctime>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>


Tiling::Tiling()
{}

Tiling::Tiling(const std::string& fn_rules)
{
	loadSubdivRules(fn_rules);
}

void Tiling::loadSubdivRules(const std::string& fn_rules)
{
	std::cout << ">> loading tiles subdivision rules" << std::endl;
	std::clock_t t = std::clock();
	std::ifstream file(fn_rules.c_str());
	if(!file.is_open())
	{
		ERR( "file not found: " << fn_rules );
		exit(EXIT_FAILURE);
	}

	// Get subdivision transformation matrix
	Matrix2x2<int> bijMat;
	int _trash;
	file >> _trash >> _trash >> m_mat >> bijMat;
	m_mat.transpose(); //file's matrix is wrong !!
	DBG( "m_mat: " << m_mat );
	m_mat_inv.push_back(m_mat);
	m_mat_inv.at(0).inverse();
	DBG( "m_mat_inv: " << m_mat_inv.at(0) );
	for(int i=0; i<MAX_LVL-1; ++i) m_mat_inv.push_back( m_mat_inv.at(0)*m_mat_inv.at(i) );
	file.get();
	//m_rot_angle = m_mat.getAngle();
	m_scale_ratio = m_mat.getScale();
	DBG( "m_scale_ratio: " << m_scale_ratio );

	// Get subdivision rules
	rule_T rule;
	unsigned int shape_id;
	DPoint ref_pt;

	std::stringstream value;
	int valStep = 0;
	while(!file.eof())
	{
		char c = file.get();
		if(c == '\t' || c == ' ' || c == '\n')
		{
			switch(valStep)
			{
				case 0:
					value >> shape_id;
					shape_id--; // indices now start from 0
					valStep++;
					break;
				case 1:
					value >> ref_pt.x();
					valStep++;
					break;
				case 2:
					value >> ref_pt.y();
					rule.push_back(Tile(shape_id, ref_pt));
					valStep = 0;
					break;
			}
			value.clear();

			if(c == '\n')
			{
				m_subdiv_rules.push_back(rule);
				rule.clear();
			}
		}
		else value << c;
	}
	if(valStep == 2)
	{
		value >> ref_pt.y();
		rule.push_back(Tile(shape_id, ref_pt));
		m_subdiv_rules.push_back(rule);
	}

	file.close();
	t = std::clock()-t;
	float dur = (float) t / CLOCKS_PER_SEC;

	// Get subdivision factor
	m_subdiv_factor = m_subdiv_rules.at(0).size();

	std::cout << "    → subdivision factor = " << m_subdiv_factor << std::endl;
	std::cout << "    → number of tile = " << m_subdiv_rules.size() << std::endl;
	std::cout << "    → loading duration = " << dur << " s." << std::endl;
	std::cout << std::endl;
}

oid_T Tiling::getOffsetId(const sid_T& shape_id, const unsigned short int rank)
{
	return rank * ruleSize() + shape_id;
}

void Tiling::initialize(const unsigned int& seed)
{
	m_tiling.clear();
	m_tiling.reserve(MAX_LVL*m_subdiv_factor); // allow generation of 37^7 samples (~95 billions)
	m_tiling.push_back( TileState(seed, DPoint(0, 0), 0, 0, 0, 0));
}

void Tiling::add(const TileState& tileState)
{
	m_tiling.push_back(tileState);
}

void Tiling::subdivide(const unsigned short int& nb)
{
	TileState parent = m_tiling.back();
	m_tiling.pop_back();
	rule_T rule = m_subdiv_rules[parent.tile().shapeId()];
	DPoint newpos = m_mat*parent.tile().refPt();
	short int newlvl = parent.lvl()+1;
	DBG( "subdivision of parent: lvl=" << parent.lvl() << " rank=" << parent.rank() << " | stack size = " << size() );
	for(int i=nb; i>=0; i--)
	{
		m_tiling.push_back(TileState(
					rule[i].shapeId()
					, newpos + rule[i].refPt()
					, i
					, i + parent.rank()/(float)m_subdiv_factor
					, getOffsetId(parent.tile().shapeId(), i)
					, newlvl) );
		DBG( "  sub: lvl=" << last().lvl() << " rank=" << last().rank() << " | stack size = " << size() );
	}
}

void Tiling::next(const unsigned short int& nb)
{
	for(int i=0; i<nb; ++i)
	{
		DBG( "next : lvl=" << last().lvl() << " rank=" << last().rank() << " | stack size = " << size()-1 );
		m_tiling.pop_back();
	}
}
