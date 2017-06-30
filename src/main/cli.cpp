#include <boost/program_options.hpp>
#include <iostream>

#include "sampler.hpp"
#include "tiling.hpp"

namespace boostPO = boost::program_options;

int main(int argc, char** argv)
{
	/* ARG PARSER *****************************************************/
	std::string fn_rules;
	std::string fn_bary;
	std::string fn_dev;
	std::string fn_out;
	std::string fn_image;
	unsigned int nbSample;
	unsigned short int seed;
	double lvl_min;
	double lvl_max;
	double rank_min;
	double rank_max;

	boostPO::variables_map vm;
	boostPO::options_description desc("Allowed options");
	desc.add_options()
		("help,h",
		 "produce help message")
		("rule,r",
		 boostPO::value<std::string>(&fn_rules)->required(),
		 "REQUIRED | Subdivision rule filename")
		("bary,b",
		 boostPO::value<std::string>(&fn_bary)->required(),
		 "REQUIRED | Barycenter offset filename (for each rule id)")
		("dev,d",
		 boostPO::value<std::string>(&fn_dev),
		 "Offset LUT filename (for each structural indices)")
		("out,o",
		 boostPO::value<std::string>(&fn_out),
		 "Output filename")
		("nbSample,n",
		 boostPO::value<unsigned int>(&nbSample)->default_value(1024),
		 "Number of sample de generate")
		("image,i",
		 boostPO::value<std::string>(&fn_image),
		 "Image for adaptive sampling")
		("lvlmin,l",
		 boostPO::value<double>(&lvl_min)->default_value(0),
		 "Level min")
		("lvlmax,L",
		 boostPO::value<double>(&lvl_max)->default_value(4),
		 "Level max")
		("rankmin,k",
		 boostPO::value<double>(&rank_min)->default_value(0),
		 "Rank min")
		("rankmax,K",
		 boostPO::value<double>(&rank_max)->default_value(29),
		 "Rank max")
		("seed,s",
		 boostPO::value<unsigned short int>(&seed)->default_value(0),
		 "Initial tile to use for sampling ([1-408], 0 = random)")
		;

	try
	{
		boostPO::store(
				boostPO::command_line_parser(argc, argv).
				options(desc).run(), vm);
		boostPO::notify(vm);
	}
	catch(boost::program_options::error& e)
	{
		std::cerr << e.what() << std::endl;
		std::cout << desc << "\n";
		exit(EXIT_FAILURE);
	}

	if(vm.count("help"))
	{
		std::cout << desc << "\n";
		exit(EXIT_SUCCESS);
	}

	/* PROG ***********************************************************/
	Sampler sampler(fn_rules, fn_bary, fn_dev);

	if( seed == 0 )
	{
		srand48(time(NULL));
		seed = std::ceil(drand48()*408);
	}
	if( vm.count("seed") ) seed = (seed-1)%408;

	if(fn_image.empty())
	{
		if( vm.count("out") )
		{
			WriterFileRaw write(fn_out);
			sampler.generateUniform(nbSample, 0, write, seed);
		}
		else
		{
			WriterEmpty write;
			sampler.generateUniform(nbSample, 0, write, seed);
		}
	}
	else
	{
		Image img(fn_image);
		double white_density = computeDensity(lvl_min, rank_min, 0.21, sampler.tiling().subdivFactor());
		double black_density = computeDensity(lvl_max, rank_max, 0.21, sampler.tiling().subdivFactor());
		
		if( vm.count("out") )
		{
			WriterFileRaw write(fn_out);
			sampler.generateAdaptive(&(img.dFunc), white_density, black_density, 0, write, seed);
		}
		else
		{
			WriterEmpty write;
			sampler.generateAdaptive(&(img.dFunc), white_density, black_density, 0, write, seed);
		}
	}
}
