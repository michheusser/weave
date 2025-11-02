// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef CONSTANTS_H_2025_09_02_16_55_57
#define CONSTANTS_H_2025_09_02_16_55_57
#include <string_view>

namespace weave
{
	namespace profiling
	{
		namespace constants
		{
			constexpr size_t MAX_RESERVOIR_SIZE = 1000;

			constexpr uint64_t DEFAULT_SAMPLE_RATE = 1000;

			constexpr std::string_view METRIC_FRAME_TYPE_STRING = "frame";
			constexpr std::string_view METRIC_BYTE_TYPE_STRING = "byte";

			constexpr std::string_view DEFAULT_CLIENT_SESSION_NAME = "ClientProfilingSession_";
			constexpr std::string_view DEFAULT_CLIENT_SESSION_DESCRIPTION = "Client profiling session for future reference";

			constexpr std::string_view DEFAULT_SERVER_SESSION_NAME = "ServerProfilingSession_";
			constexpr std::string_view DEFAULT_SERVER_SESSION_DESCRIPTION = "Server profiling session for future reference";

			constexpr uint64_t DEFAULT_NUM_CYCLES = 10;
			constexpr std::string_view MONITORING_REPORTS_LOCATION = WEAVE_MONITORING_REPORTS_LOCATION;

		}
	}
}

#endif
