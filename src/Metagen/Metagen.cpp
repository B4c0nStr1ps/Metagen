// Metagen.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "clang_parsing.h"
#include "type_info.h"
#include "pugixml.hpp"
#include <filesystem>
namespace fs = std::experimental::filesystem;

std::wstring s2ws(const std::string str)
{
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.from_bytes(str);
}

std::string ws2s(const std::wstring wstr)
{
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.to_bytes(wstr);
}

int main(int argc, char** argv)
{
	std::cout << "LibClang experiments\n";
	std::vector<const char*> compiler_args = { "-std=c++11", "-D_GENERATE_REFLECTION_" };
	std::string project_folder = argv[1];
	std::string project_filename = argv[2];

	fs::path project_folder_path(project_folder);
	fs::path project_folder_filepath(project_filename);
	fs::path full_path = project_folder_path / project_folder_filepath;
	std::cout << full_path << std::endl;
	std::wstring project_file_fullpath = full_path.c_str();

	std::cout << "Arguments: " << argc << " -projectFolder: " << project_folder << "--projectFile" << project_filename << std::endl;
	std::wcout << "Parsing file: " << project_file_fullpath << std::endl;
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(project_file_fullpath.c_str());

	if (!result)
	{
		std::cout << "Error loading xml file " << argv[1] << " " << result.description() << std::endl;
		return 1;
	}

	std::vector<std::string> translation_unit_names;

	auto clinclude_nodes = doc.select_nodes(L"//ClInclude");
	for (auto clinclude_node : clinclude_nodes)
	{
		std::wstring include_file = clinclude_node.node().attribute(L"Include").value();
		fs::path include_rel_path(include_file);
		fs::path include_full_path = project_folder_path / include_rel_path;
		translation_unit_names.push_back(ws2s(include_full_path.c_str()));
		std::cout << "Found translation unit: " << ws2s(include_full_path.c_str()) << std::endl;
	}

	std::vector<metagen::TranslationUnitTypes> translation_units;

	for (auto tu_name : translation_unit_names)
	{
		translation_units.push_back(metagen::ParseTranslationUnit(tu_name, compiler_args));
	}
	
	std::stringstream ss;
	//Write include files
	ss << "#include <Reflection.h>" << std::endl;

	for (auto tu : translation_units)
	{
		ss << "#include \"" << tu.tu_name << "\"" << std::endl;
	}

	ss << std::endl;

	for (auto tu : translation_units)
	{
		ss << metagen::GenerateTranslationUnitTypesMetadata(&tu) << std::endl;
	}

	std::cout << ss.str() << std::endl;

	return 0;
}

