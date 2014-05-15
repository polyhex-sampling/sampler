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
	std::string fn_offset;
	std::string fn_out;

	boostPO::variables_map vm;
	boostPO::options_description desc("Allowed options");
	desc.add_options()
		("help,h",
		 "produce help message")
		("rule,r",
		 boostPO::value<std::string>(&fn_rules)->required(),
		 "REQUIRED | subdivision rule filename")
		("bary,b",
		 boostPO::value<std::string>(&fn_bary),
		 "barycenter offset filename (for each rule id)")
		("offset,d",
		 boostPO::value<std::string>(&fn_offset),
		 "offset filename (for each structural indices)")
		("out,o",
		 boostPO::value<std::string>(&fn_out),
		 "output filename")
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
	try
	{
		Sampler sampler(fn_rules, fn_bary, fn_offset);
		//WriterFilePts write(fn_out);
		//WriterVector write;
		WriterEmpty write;
		char ans;
		float density = 4e6;
		unsigned short int seed = 0;
		float spaceScale = 0.21;

		unsigned int speed[3][4] = {{0,0,0,0},{0,0,0,0},{0,0,0,0}};
		unsigned int nbIter = 10;
		for(int iter=-2; iter<1; ++iter)
		{
			switch(iter)
			{
				case -2: std::cout << "======== REF PT ==========" << std::endl; break;
				case -1: std::cout << "======== BARY PT =========" << std::endl; break;
				case 0: std::cout << "======= OFFSET PT ========" << std::endl; break;
			}

			std::cout << "==== Test Uniform Min ====" << std::endl;
			for(int i=0; i<nbIter; ++i) speed[iter+2][0] += sampler.generateUniform(5, 0, iter, write, seed, spaceScale, true);
			speed[iter+2][0] /= nbIter;

			std::cout << "\n==== Test Uniform Max ====" << std::endl;
			for(int i=0; i<nbIter; ++i) speed[iter+2][1] += sampler.generateUniform(5, 35, iter, write, seed, spaceScale, true);
			speed[iter+2][1] /= nbIter;

			std::cout << "\n==== Test Adaptive Min ====" << std::endl;
			for(int i=0; i<nbIter; ++i) speed[iter+2][2] += sampler.generateAdaptive(&dFunc_linear, 0., 3440., iter, write, seed, spaceScale);
			speed[iter+2][2] /= nbIter;

			std::cout << "\n==== Test Adaptive Max ====" << std::endl;
			for(int i=0; i<nbIter; ++i) speed[iter+2][3] += sampler.generateAdaptive(&dFunc_linear, 0., 124000., iter, write, seed, spaceScale);
			speed[iter+2][3] /= nbIter;
			std::cout << "===========================\n" << std::endl;
		}
		std::cout << "\n================================== RESUME ==================================" << std::endl;
		std::cout << "(nb pt / s.)\t\tUniform\t\t\t\tAdaptive" << std::endl;
		std::cout << "ref pt\t\t" << (float)speed[0][0] << " - " << (float)speed[0][1] << "\t" << (float)speed[0][2] << " - " << (float)speed[0][3] << std::endl;
		std::cout << "bary pt\t\t" << (float)speed[1][0] << " - " << (float)speed[1][1] << "\t" << (float)speed[1][2] << " - " << (float)speed[1][3] << std::endl;
		std::cout << "offset pt\t" << (float)speed[2][0] << " - " << (float)speed[2][1] << "\t" << (float)speed[2][2] << " - " << (float)speed[2][3] << std::endl;
		std::cout << "============================================================================" << std::endl;
	}
	catch(const std::exception& e)
	{
		std::cerr << "Error : " << e.what() << std::endl;
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}
