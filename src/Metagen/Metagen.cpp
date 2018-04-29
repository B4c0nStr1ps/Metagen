// Metagen.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "clang_parsing.h"
#include "type_info.h"

int main(int argc, char** argv)
{
	std::cout << "LibClang experiments\n";
	std::vector<const char*> compiler_args = { "-std=c++11", "-D_GENERATE_REFLECTION_" };

	std::cout << "Arguments: " << argc << " -filename:" << argv[1] << std::endl;

	auto tu_types = metagen::ParseTranslationUnit(argv[1], compiler_args);

	std::stringstream ss;
	//Write include files
	ss << "#include \"Reflection.h\"" << std::endl;
	ss << "#include \"" << tu_types.tu_name << "\"" << std::endl;
	ss << std::endl;
	ss << metagen::GenerateTranslationUnitTypesMetadata(&tu_types) << std::endl;

	std::cout << ss.str() << std::endl;

	return 0;
}

