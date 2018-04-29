#pragma once
#include "type_info.h"
#include <vector>

namespace metagen {

struct TranslationUnitTypes
{
	std::string tu_name;
	std::vector<TypeInfo> types;
};

TranslationUnitTypes ParseTranslationUnit(std::string filename, std::vector<const char*> compiler_args);

std::string GenerateTranslationUnitTypesMetadata(const TranslationUnitTypes* tu_types);

void PrintTranslationUnitTypes(const TranslationUnitTypes* tu_types);

std::vector<TypeInfo> ParseCursorChildren(CXCursor parent, std::string filename);

metagen::TypeInfo ParseCursorStructDecl(CXCursor parent);

std::vector<TypeInfo> ParseCursorClassDecl(CXCursor parent, std::string filename);

std::vector<TypeInfo> ParseCursorNamespaceDecl(CXCursor parent, std::string filename);

} //namespace metagen
