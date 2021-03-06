/**
 * @file
 *
 * @brief Test tools.
 *
 * @author perry.werneck@gmail.com
 *
 */

#include <iostream>
#include <udjat/tools/atom.h>
#include <udjat/tools/logger.h>

 using namespace std;
 using namespace Udjat;

//---[ Implement ]------------------------------------------------------------------------------------------

int main(int argc, char **argv) {

	/*
	cout 	<< "Processor by value: " << Dmi::Value(Dmi::PROCESSOR_INFO,0x10).as_string() << endl
			<< "Processor by name:  " << Dmi::Value("Processor",0x10).as_string() << endl;
	*/

	/*
	{
		Atom v1 = Atom::getFromStatic("teste");
		Atom v2("teste");

		cout << "V1=" << ((void *) v1.c_str()) << " V2=" << ((void *) v2.c_str()) << endl;
	}
	*/

	{
		Logger::redirect();

		Logger logger{"sample"};

		logger.info("The first value is '{}' and the second value is '{}'",
						"First value",
						2
					);


	}

	return 0;
}
