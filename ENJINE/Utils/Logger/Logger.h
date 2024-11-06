#pragma once
#include <format>
#include <mutex>
#include <stdlib.h>
#include <corecrt_wstdio.h>

class Logger {
private:
	FILE* f;
public:
	Logger() {
		tm date = get_current_time();
		errno_t err = fopen_s(&f, std::format("logs_{}_{}_{}.txt", date.tm_mday, date.tm_mon, date.tm_year).c_str(), "a");
		if (err != NULL) {
			exit_error(L"FILE OPEN ERROR", false);
			return;
		}
	}
	~Logger() {
		fclose(f);
	}

	void exit_error(const wchar_t* str, bool with_save = true);
	void info(const wchar_t* str);

private:
	void save_file(const wchar_t* str);
	tm get_current_time();
};

