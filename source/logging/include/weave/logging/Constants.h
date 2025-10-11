// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef CONSTANTS_H_2025_09_02_12_29_17
#define CONSTANTS_H_2025_09_02_12_29_17
#include <string_view>

namespace weave
{
	namespace logging
	{
		constexpr std::string_view DEFAULT_LOGGER_NAME = "weave";
		constexpr bool DEFAULT_FILE_LOGGING_ENABLED = true;
		constexpr int DEFAULT_FILE_SIZE_IN_BYTES = 10 * 1024*1024;
		constexpr int DEFAULT_MAX_FILES = 5;
		constexpr std::string_view DEFAULT_CONSOLE_PATTERN = "%^[%Y-%m-%d %H:%M:%S.%e] [%L] [%s:%# %!] %v%$";
		constexpr std::string_view DEFAULT_FILE_PATTERN = "[%Y-%m-%d %H:%M:%S.%e] [%t] [%L] [%s:%# %!] %v";
	}
}

#endif
