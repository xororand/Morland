#include "Logger.h"

void Logger::exit_error(const wchar_t* str) {
	wprintf(std::format(L"[ERROR] {}", str).c_str());
	system("pause");
	exit(1);
}

void Logger::info(const wchar_t* str) {
	wprintf(std::format(L"[INFO] {}", str).c_str());
}
