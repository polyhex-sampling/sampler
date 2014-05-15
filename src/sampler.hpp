#ifndef SAMPLER_H
#define SAMPLER_H


#include <ctime>

#include "densityFunction.hpp"
#include "tiling.hpp"
#include "samplingAlgoAdaptive.hpp"
#include "samplingAlgoUniform.hpp"
#include "writer.hpp"


class Sampler
{
	private:
		data_T m_data;
		Tiling m_tiling;

	public:
		Sampler(const std::string& fn_rules, const std::string& fn_bary, const std::string& fn_offset);

	private:
		void loadBarycenters(const std::string& fn_bary);
		void loadOffsets(const std::string& fn_offset);

	public:
		const Tiling& tiling() const { return m_tiling; };
		template <class WRITER>
		unsigned int generateUniform(const float& density,  const short int& ilut, WRITER& writer, const unsigned short int& seed=0, const float& spaceSize=0.235, const bool& crop=true);
		template <class WRITER>
		unsigned int generateUniform(const unsigned short int& finalLvl, const unsigned short int& finalRank, const short int& ilut, WRITER& writer, const unsigned short int& seed=0, const float& spaceSize=0.235, const bool& crop=true);
		template <class WRITER>
		unsigned int generateAdaptive(float (*func)(const Point&), const float& white_density, const float& black_density,  const short int& ilut, WRITER& writer, const unsigned short int& seed=0, const float& spaceSize=0.235, const bool& interpolate=true, const bool& dither=true);

};

/*** TEMPLATE IMPLEMENTATION ***/
template <class WRITER>
unsigned int Sampler::generateUniform(const float& density, const short int& ilut, WRITER& write, const unsigned short int& seed, const float& spaceSize, const bool& crop)
{
	UniformSampling<WRITER> algorithm(&m_tiling, &m_data, ilut, write);
	unsigned int nbSample;
	std::clock_t t = std::clock();
	if(crop)
		nbSample = algorithm.generateSquareDomain(density, seed, spaceSize);
	else
		nbSample = algorithm.generateTileDomain(density, seed, spaceSize);
	t = std::clock()-t;
	float dur = (float) t / CLOCKS_PER_SEC;
	std::cout << "M Generated " << nbSample << " samples in " << dur << " seconds (" << nbSample/dur << " s./sec)" << std::endl;
	return floor(nbSample/dur);
}
template <class WRITER>
unsigned int Sampler::generateUniform(const unsigned short int& finalLvl, const unsigned short int& finalRank, const short int& ilut, WRITER& write, const unsigned short int& seed, const float& spaceSize, const bool& crop)
{
	UniformSampling<WRITER> algorithm(&m_tiling, &m_data, ilut, write);
	unsigned int nbSample;
	std::clock_t t = std::clock();
	if(crop)
		nbSample = algorithm.generateSquareDomain(finalLvl, finalRank, seed, spaceSize);
	else
		nbSample = algorithm.generateTileDomain(finalLvl, finalRank, seed, spaceSize);
	t = std::clock()-t;
	float dur = (float) t / CLOCKS_PER_SEC;
	std::cout << "M Generated " << nbSample << " samples in " << dur << " seconds (" << nbSample/dur << " s./sec)" << std::endl;
	return floor(nbSample/dur);
}

template <class WRITER>
unsigned int Sampler::generateAdaptive(float (*func)(const Point&), const float& white_density, const float& black_density, const short int& ilut, WRITER& write, const unsigned short int& seed, const float& spaceSize, const bool& interpolate, const bool& dither)
{
	AdaptiveSampling<WRITER> algorithm(&m_tiling, &m_data, ilut, write, func, interpolate, dither);
	std::clock_t t = std::clock();
	const unsigned int nbSample = algorithm.generate(white_density, black_density, seed, spaceSize);
	t = std::clock()-t;
	float dur = (float) t / CLOCKS_PER_SEC;
	std::cout << "M Generated " << nbSample << " samples in " << dur << " seconds (" << nbSample/dur << " s./sec)" << std::endl;
	return floor(nbSample/dur);
}

#endif
