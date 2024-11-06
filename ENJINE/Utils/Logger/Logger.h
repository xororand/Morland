#pragma once
#include <format>
#include <stdlib.h>
#include <corecrt_wstdio.h>

class Logger {
public:
	static void exit_error(const wchar_t* str, bool with_save = true);
	static void info(const wchar_t* str);
private:
	static void save_file(const wchar_t* str);
	static tm get_current_time();
};

