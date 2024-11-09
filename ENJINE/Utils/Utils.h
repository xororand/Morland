#pragma once

#include <string>

#define to_ancii(x) Utils::encoding::to_utf8(x).c_str()

namespace Utils
{
	namespace encoding {
		std::string to_utf8(const std::wstring& str);
		std::wstring to_wide(const std::string& str);
	}
	namespace string {
		std::string replace(std::string s, char c1, char c2);
		std::wstring replace(std::wstring s, char c1, char c2);
	}
	//struct computer {
	//	static std::string getCpu();
	//	static std::wstring getGpu();
	//	static void getRAM(DWORD& max, DWORD& used);
	//	static void getDesktopResolution(int& screen_weight, int& screen_height);
	//};
};

