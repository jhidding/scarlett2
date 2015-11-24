#include "../argv/argv.hh"
#include "unittest.hh"
#include "../misc/longstring.hh"

using namespace Test;
using namespace System;
using Misc::LongString;

int main(int argc_, char **argv_)
{
	Argv argv = read_arguments(argc_, argv_,
		Option(0, "h", "help", "false",
			"print this help."),
		Option(0, "l", "list", "false",
			"list available tests."),
		Option(0, "n", "no-break", "false",
			"don't break after the first failed test."),
		Option(Option::VALUED | Option::CHECK, "s", "select", "-",
			"select a test to run, if '-' is given, all tests "
			"are run in alphanumerical order."));

	if (argv.get<bool>("help"))
	{
		std::cerr << "Test suite.\n"
			"Copyright Johan Hidding, June 2014 - licence: GPL3.\n\n";
		argv.print(std::cerr);
		exit(0);
	}

	if (argv.get<bool>("list"))
	{
		std::cerr << "Test suite.\n"
			"Copyright Johan Hidding, June 2014 - licence: GPL3.\n\n"
			"available tests:\n";

		for (auto &kv : Unit::instances())
		{
			std::cerr << kv.first << std::endl
				<< LongString(kv.second->description(), 72,
						[] () -> std::string { return " | "; })
				<< std::endl;
		}

		exit(0);
	}

	bool should_break = not argv.get<bool>("no-break");

	if (argv["select"] != "-")
	{
		run_test(Unit::instances()[argv["select"]]);
	}
	else
	{
		Unit::all(should_break);
	}

    return 0;
}
