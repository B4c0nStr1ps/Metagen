#pragma once
#include <vector>

namespace metagen{

struct FieldInfo
{
	std::string name;
	std::string type;
	std::string type_kind;
	std::string kind;

	long long size;
	long long alignment;
	long long offset;
};

struct TypeInfo
{
	std::string name;
	std::string type;
	std::string type_kind;
	std::string kind;

	bool is_struct;
	bool is_class;

	long long size;
	long long alignment;

	std::vector<FieldInfo> fields;
};

} //namespace metagen.