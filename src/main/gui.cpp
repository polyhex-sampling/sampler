#include <iostream>

#include <boost/program_options.hpp>
#include <QApplication>

#include "sampler.hpp"
#include "tiling.hpp"
#include "gui/window.hpp"

namespace boostPO = boost::program_options;

void validate(boost::any& v,
		const std::vector<std::string>& values,
		QString*, int)
{
	boostPO::validators::check_first_occurrence(v);
	const std::string& s = boostPO::validators::get_single_string(values);
	v = boost::any(QString::fromLocal8Bit(s.c_str()));
}

int main(int argc, char** argv)
{
	/* ARG PARSER *****************************************************/
	QString fn_rules;
	QString fn_bary;
	QString fn_offset;
	QString fn_out;

	boostPO::variables_map vm;
	boostPO::options_description desc("Allowed options");
	desc.add_options()
		("help,h",
		 "produce help message")
		("rule,r",
		 boostPO::value(&fn_rules)->required(),
		 "REQUIRED | subdivision rule filename")
		("bary,b",
		 boostPO::value(&fn_bary),
		 "barycenter offset filename (for each rule id)")
		("dev,d",
		 boostPO::value(&fn_offset),
		 "offset filename (for each structural indices)")
		("out,o",
		 boostPO::value(&fn_out),
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
	Sampler sampler(fn_rules.toStdString(), fn_bary.toStdString(), fn_offset.toStdString());

	QApplication app(argc, argv);
	MainWindow* window = new MainWindow(&sampler);
	window->show();
	app.exec();
}
