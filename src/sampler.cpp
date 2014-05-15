#include "sampler.hpp"

#include <cstdlib>
#include <fstream>
#include <sstream>
#include <stdexcept>


Sampler::Sampler(const std::string& fn_rules, const std::string& fn_bary, const std::string& fn_offset)
{
	m_tiling = Tiling(fn_rules);
	loadBarycenters(fn_bary);
	loadOffsets(fn_offset);
}

void Sampler::loadBarycenters(const std::string& fn_bary)
{
	std::cout << ">> loading tiles barycenters" << std::endl;
	std::clock_t t = std::clock();
	std::ifstream file(fn_bary.c_str());
	if(!file.is_open())
	{
		ERR( "file not found: " << fn_bary );
	}

	Point v;
	while(file >> v.x() >> v.y())
	{
		m_data.barycenters.push_back(v);
	}

	if(m_data.barycenters.size() != m_tiling.ruleSize())
	{
		ERR( "number of barycenters and number of rules are not the same: " << m_data.barycenters.size() << " != " << m_tiling.size() );
		std::exit(EXIT_FAILURE);
	}
	file.close();
	t = std::clock()-t;
	float dur = (float) t / CLOCKS_PER_SEC;

	std::cout << "    → number of barycenter = " << m_data.barycenters.size() << std::endl;
	std::cout << "    → loading duration = " << dur << " s." << std::endl;
	std::cout << std::endl;
}

void Sampler::loadOffsets(const std::string& fn_offset)
{
	if(fn_offset != "")
	{
		std::cout << ">> loading offset lut from \"" << fn_offset << "\"" << std::endl;
		std::clock_t t = std::clock();
		std::ifstream file_offset(fn_offset.c_str(), std::ios::binary);
		if(!file_offset.is_open())
		{
			ERR( "file not found: " << fn_offset );
			exit(EXIT_FAILURE);
		}

		int lutSize;
		file_offset >> lutSize;
		if( lutSize !=m_tiling.subdivFactor() * m_tiling.ruleSize() )
		{
			ERR( "wrong number of offset (row: " << lutSize << " in file / " << m_tiling.subdivFactor() * m_tiling.ruleSize() << " needed)" );
			exit(EXIT_FAILURE);
		}
		int colSize;
		file_offset >> colSize;
		file_offset.ignore(100, '\n');
		if( colSize != m_tiling.subdivFactor()-1 )
		{
			ERR( "wrong number of offset (col: " << colSize << " in file / " << m_tiling.subdivFactor()-1 << " needed)" );
			exit(EXIT_FAILURE);
		}
		m_data.offsets_list.resize(m_data.offsets_list.size()+1);
		m_data.offsets_list.back().resize(lutSize);

		for(unsigned int id=0; id<lutSize; ++id)
		{
			m_data.offsets_list.back()[id].resize(m_tiling.subdivFactor()-1);
			file_offset.read((char *) &m_data.offsets_list.back()[id][0], 2*colSize*sizeof(float));
			//if(id%100000 == 0)
			//{
			//	std::cout << "\r" << std::floor(100.*id/lutSize) << "%      ";
			//	std::cout.flush();
			//}
		}
		file_offset.close();
		t = std::clock()-t;
		float dur = (float) t / CLOCKS_PER_SEC;

		std::cout << "\r";
		std::cout << "    → number of offset lut = " << m_data.offsets_list.size() << std::endl;
		std::cout << "    → number of offset = " << lutSize << " (x" << m_tiling.subdivFactor()-1 << ")" << std::endl;
		std::cout << "    → loading duration = " << dur << " s." << std::endl;
		std::cout << std::endl;
	}
	else
	{
		std::cout << ">> no offset lut loaded" << std::endl;
	}
}
