#include "stdafx.h"
#include "clang_parsing.h"
#include "clang_wrappers.h"

metagen::TranslationUnitTypes metagen::ParseTranslationUnit(std::string filename, std::vector<const char*> compiler_args)
{
	TranslationUnitTypes tu_types = { filename, {} };
	CXIndex index = clang_createIndex(0, 0);
	CXTranslationUnit unit = clang_parseTranslationUnit(index, filename.c_str(), compiler_args.data(), compiler_args.size(), nullptr, 0, CXTranslationUnit_None);
	if (unit == nullptr)
	{
		std::cerr << "Unable to parse translation unit " << filename << std::endl;
		return tu_types;
	}

	CXString translation_unit_name = clang_getTranslationUnitSpelling(unit);
	std::string tu_name = clang_getCString(translation_unit_name);
	std::cout << "Parsing the translation unit: " << tu_name << std::endl;

	tu_types.types = ParseCursorChildren(clang_getTranslationUnitCursor(unit), filename);

	clang_disposeString(translation_unit_name);
	clang_disposeTranslationUnit(unit);
	clang_disposeIndex(index);

	return tu_types;
}

std::string metagen::GenerateTranslationUnitTypesMetadata(const TranslationUnitTypes* tu_types)
{
	std::stringstream ss;

	for (auto tu_type : tu_types->types)
	{
		ss << "TYPEDESCRIPTOR_IMPL_GENERATION_BEGIN(" << tu_type.type << ")" << std::endl;
		for (auto field : tu_type.fields)
		{
			ss << "TYPEDESCRIPTOR_IMPL_FIELD_METADATA(" << field.name << ", " << field.type << ", " << field.offset << ")" << std::endl;
		}
		ss << "TYPEDESCRIPTOR_IMPL_GENERATION_END()" << std::endl;
		ss << "TYPERESOLVER_GENERATOR(" << tu_type.type << ")" << std::endl;
		ss << std::endl;
	}

	return ss.str();
}

void metagen::PrintTranslationUnitTypes(const TranslationUnitTypes* tu_types)
{
	std::cout << "Translation unit: " << tu_types->tu_name << std::endl;
	for (auto tu_type : tu_types->types)
	{
		std::cout << "[TYPE] name: " << tu_type.name << " kind: " << tu_type.kind << " type: " << tu_type.type << " type kind:" << tu_type.type_kind;
		std::cout << " size: " << tu_type.size << " alignment: " << tu_type.alignment << std::endl;
		for (auto field : tu_type.fields)
		{
			std::cout << "+---";
			std::cout << " name: " << field.name;
			std::cout << " kind: " << field.kind;
			std::cout << " type: " << field.type;
			std::cout << " type_kind: " << field.type_kind;
			std::cout << " size: " << field.size;
			std::cout << " alignment: " << field.alignment;
			std::cout << " offset: " << field.offset;
			std::cout << std::endl;
		}
	}
}

std::vector<metagen::TypeInfo> metagen::ParseCursorChildren(CXCursor parent, std::string filename)
{
	std::vector<metagen::TypeInfo> types = {};
	auto children = GetChildren(parent, filename);
	for (auto child : children)
	{
		if (child.kind == CXCursorKind::CXCursor_StructDecl)
		{
			types.push_back(ParseCursorStructDecl(child));
		}
		/*else if (child.kind == CXCursorKind::CXCursor_Namespace)
		{
			auto children_types = ParseCursorChildren(child, filename);
			types.insert(types.end(), children_types.begin(), children_types.end());
		}*/
		auto children_types = ParseCursorChildren(child, filename);
		types.insert(types.end(), children_types.begin(), children_types.end());
	}
	return types;
}

metagen::TypeInfo metagen::ParseCursorStructDecl(CXCursor cursor)
{
	TypeInfo type_info;

	type_info.is_struct = true;
	type_info.is_class = false;

	auto cursor_type =  clang_getCursorType(cursor);
	auto cursor_type_kind = cursor_type.kind;
	auto cursor_kind = cursor.kind;

	type_info.name = GetSpelling(cursor);
	type_info.kind = GetSpelling(cursor_kind);
	type_info.type = GetSpelling(cursor_type);
	type_info.type_kind = GetSpelling(cursor_type_kind);
	type_info.size = clang_Type_getSizeOf(cursor_type);
	type_info.alignment = clang_Type_getAlignOf(cursor_type);

	auto children = GetChildren(cursor);
	for (auto child : children)
	{
		if (child.kind == CXCursorKind::CXCursor_FieldDecl)
		{
			FieldInfo field_info;
			auto child_cursor_type = clang_getCursorType(child);
			field_info.name = GetSpelling(child);
			field_info.kind = GetSpelling(child.kind);
			field_info.type = GetSpelling(child_cursor_type);
			field_info.type_kind = GetSpelling(child_cursor_type.kind);
			field_info.size = clang_Type_getSizeOf(child_cursor_type);
			field_info.alignment = clang_Type_getAlignOf(child_cursor_type);
			field_info.offset = clang_Type_getOffsetOf(cursor_type, field_info.name.c_str()) / 8;
			type_info.fields.push_back(field_info);
		}
	}

	return type_info;
}

std::vector<metagen::TypeInfo> metagen::ParseCursorClassDecl(CXCursor parent, std::string filename)
{
	return std::vector<TypeInfo>();
}

std::vector<metagen::TypeInfo> metagen::ParseCursorNamespaceDecl(CXCursor parent, std::string filename)
{
	return std::vector<TypeInfo>();
}
