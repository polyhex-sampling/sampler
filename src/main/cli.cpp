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
		 boostPO::value<std::string>(&fn_bary),
		 "Barycenter offset filename (for each rule id)")
		("dev,d",
		 boostPO::value<std::string>(&fn_dev),
		 "Offset filename (for each structural indices)")
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
	/*
	WriterFilePts write(fn_out);
	/*/
	WriterEmpty write;
	//*/
	char ans;
	float density = 2;
	unsigned short int seed = 0;
	float spaceScale = 0.21;
	while(true)
	{
		std::cout << "=================================" << std::endl;
		std::cout << "? Generate a distribution (Y/n) ? ";
		if( std::cin.peek() == '\n' ) ans='y';
		else if( !(std::cin >> ans) ) break;
		std::cin.ignore();
		if( std::cin.fail() || ans=='n' || ans=='N') break;

		std::cout << "? set initial seed [0-" << sampler.tiling().ruleSize()-1 << "] (" << ++seed << "): ";
		if( std::cin.peek() == '\n' );
		else if( !(std::cin >> seed) ) break;
		std::cin.ignore();

		std::cout << "? set final density [0-inf] (" << density << "): ";
		if( std::cin.peek() == '\n' );
		else if( !(std::cin >> density) ) break;
		std::cin.ignore();

		std::cout << "? set boundary (" << spaceScale << "): ";
		if( std::cin.peek() == '\n' );
		else if( !(std::cin >> spaceScale) ) break;
		std::cin.ignore();

		sampler.generateUniform(density, -1, write, seed, spaceScale);
	}
}
