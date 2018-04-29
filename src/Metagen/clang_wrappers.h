#pragma once
#include <vector>
#include <clang-c\Index.h>

namespace metagen
{
std::vector<CXCursor> GetChildren(CXCursor cursor);

std::vector<CXCursor> GetChildren(CXCursor cursor, std::string from_file);

std::string GetCursorLocationFilename(CXCursor cursor);

std::string GetClangFileName(CXFile file);

std::string GetStringFromCXString(CXString clang_string);

std::string GetSpelling(CXCursor cursor);

std::string GetSpelling(CXTypeKind type_kind);

std::string GetSpelling(CXType type);

std::string GetSpelling(CXCursorKind kind);

}