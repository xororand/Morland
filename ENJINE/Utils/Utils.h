#pragma once
#include <string>

namespace Utils
{
	namespace encoding {
		std::string to_utf8(const std::wstring& str);
		std::wstring to_wide(const std::string& str);
	}
};

