#include "Logger.h"

void Logger::exit_error(const wchar_t* str, bool with_save) {
	tm date = get_current_time();
	std::wstring text = std::format(L"[{}:{}:{}] [ERROR] {}\n", date.tm_hour, date.tm_min, date.tm_sec, str);

	if(with_save) save_file(text.c_str());
	wprintf(text.c_str());

	system("pause");
	exit(1);
}

void Logger::info(const wchar_t* str) {
	tm date = get_current_time();
	std::wstring text = std::format(L"[{}:{}:{}] [INFO] {}\n", date.tm_hour, date.tm_min, date.tm_sec, str);

	save_file(text.c_str());
	wprintf(text.c_str());
}

void Logger::save_file(const wchar_t* str) {
	_fwprintf_p(f, str);
}

tm Logger::get_current_time()
{
	time_t now = time(0);
	tm current_time;

	gmtime_s(&current_time, &now);

	return current_time;
}
