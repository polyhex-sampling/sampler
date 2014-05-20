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
	//*
	WriterFilePts write(fn_out);
	/*/
	WriterEmpty write;
	//*/
	char ans;
	float density = 4000;
	unsigned short int seed = 0;
	float spaceScale = 0.21;

	sampler.generateUniform(density, -1, write, seed, spaceScale);
}
