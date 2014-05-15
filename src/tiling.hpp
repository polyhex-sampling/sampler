#ifndef TILING_H
#define TILING_H


#include <string>
#include <vector>

#include "matrix.hpp"
#include "tile.hpp"
#include "tileState.hpp"

typedef std::vector<Tile> rule_T;
typedef std::vector<rule_T> rules_T;

typedef std::vector<Point> lut_bary_T;
typedef std::vector<lut_bary_T> lut_offset_T;
typedef struct data{
	lut_bary_T barycenters;
	std::vector<lut_offset_T> offsets_list;
} data_T;


class Tiling
{
	private:
		std::vector<TileState> m_tiling; // tiling construction LIFO stack
		rules_T m_subdiv_rules; // tiles subdivision rule list
		unsigned short int  m_subdiv_factor; // subvision factor between to level
		Matrix2x2<int> m_mat; // subdivision transformation matrix
		std::vector<Matrix2x2<cf_T> > m_mat_inv; // inverse of m_mat at different level
		cf_T m_scale_ratio;
		//double m_rot_angle;

	public:
		Tiling();
		Tiling(const std::string& fn_rules);

	protected:
		void loadSubdivRules(const std::string& fn_rules);
		oid_T getOffsetId(const sid_T& shape_id, const unsigned short int rank);

	public:
		unsigned int ruleSize() const { return m_subdiv_rules.size(); };
		unsigned int size() const { return m_tiling.size(); };
		const unsigned short int& subdivFactor() const { return m_subdiv_factor; };
		const cf_T& scaleRatio() const { return m_scale_ratio; };
		bool isEmpty() const { return m_tiling.empty(); };
		const TileState& last() const { return m_tiling.back(); };
		const TileState& last(const unsigned int& i) const { return m_tiling[m_tiling.size()-i-1]; };

		void initialize(const unsigned int& seed);
		void add(const TileState& tileState);
		void subdivide(const unsigned short int& nb = 36);
		void next(const unsigned short int& nb = 1);
		template <class T>
		Vector<T> backTransform(const Vector<T>& pt, const short int lvl);
};


/*** Template Implementation ***/
template <class T>
Vector<T> Tiling::backTransform(const Vector<T>& pt, const short int lvl)
{
	if(lvl>0) return m_mat_inv[lvl-1]*pt;
	else return pt;
}

#endif
