#pragma once
#include <format>
#include <stdlib.h>
#include <corecrt_wstdio.h>

class Logger {
public:
	static void exit_error(const wchar_t* str);
	static void info(const wchar_t* str);
};

