#include "stdafx.h"
#include "clang_wrappers.h"

std::vector<CXCursor> metagen::GetChildren(CXCursor cursor)
{
	std::vector<CXCursor> children;

	clang_visitChildren(cursor,
		[](CXCursor child, CXCursor parent, CXClientData client_data)
	{
		std::vector<CXCursor>* result = reinterpret_cast<std::vector<CXCursor>*>(client_data);
		result->push_back(child);
		return CXChildVisit_Continue;
	},
		&children);

	return children;
}

std::vector<CXCursor> metagen::GetChildren(CXCursor cursor, std::string from_file)
{
	std::vector<CXCursor> children;

	struct InputData
	{
		std::vector<CXCursor>* cursors_;
		std::string filename_filter_;
	};

	InputData input_data = { &children, from_file };

	clang_visitChildren(cursor,
		[](CXCursor child, CXCursor parent, CXClientData client_data)
	{
		InputData* input_data = reinterpret_cast<InputData*>(client_data);

		std::string cursor_filename = GetCursorLocationFilename(child);
		if (cursor_filename == input_data->filename_filter_)
		{
			input_data->cursors_->push_back(child);
		}
		return CXChildVisit_Continue;
	},
		&input_data);

	return children;
}

std::string metagen::GetCursorLocationFilename(CXCursor cursor)
{
	CXSourceLocation location = clang_getCursorLocation(cursor);
	CXFile file_location;
	unsigned int line;
	unsigned int column;
	unsigned int offset;
	clang_getSpellingLocation(location, &file_location, &line, &column, &offset);
	std::string filename = GetClangFileName(file_location);
	return filename;
}

std::string metagen::GetClangFileName(CXFile file)
{
	CXString clang_file_name = clang_getFileName(file);
	std::string file_name = GetStringFromCXString(clang_file_name);
	clang_disposeString(clang_file_name);
	return file_name;
}

std::string metagen::GetStringFromCXString(CXString clang_string)
{
	std::string result;
	result.assign(clang_getCString(clang_string));
	return result;
}

std::string metagen::GetSpelling(CXCursor cursor)
{
	auto clang_str = clang_getCursorSpelling(cursor);
	std::string spelling = GetStringFromCXString(clang_str);
	clang_disposeString(clang_str);
	return spelling;
}

std::string metagen::GetSpelling(CXTypeKind type_kind)
{
	auto clang_str = clang_getTypeKindSpelling(type_kind);
	std::string spelling = GetStringFromCXString(clang_str);
	clang_disposeString(clang_str);
	return spelling;
}

std::string metagen::GetSpelling(CXType type)
{
	auto clang_str = clang_getTypeSpelling(type);
	std::string spelling = GetStringFromCXString(clang_str);
	clang_disposeString(clang_str);
	return spelling;
}

std::string metagen::GetSpelling(CXCursorKind kind)
{
	auto clang_str = clang_getCursorKindSpelling(kind);
	std::string spelling = GetStringFromCXString(clang_str);
	clang_disposeString(clang_str);
	return spelling;
}
