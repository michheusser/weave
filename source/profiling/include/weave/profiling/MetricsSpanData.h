// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef METRICSSPANDATA_H_2025_09_10_11_25_14
#define METRICSSPANDATA_H_2025_09_10_11_25_14
#include <string>
#include <thread>

namespace weave
{
	namespace profiling
	{
		struct MetricsSpanData
		{
			uint64_t hash; // TODO To be done from name and type? What's the unique-key?
			std::string_view name;
			std::string_view type;
			pid_t processID;
			std::thread::id threadID; // Obfuscated type since it differs depending on OS (has overloaded ostream operator)
			uint64_t startNs;
			uint64_t endNs;
			uint64_t count; // e.g. frames in a span (typically 1), bytes in a span, ...
		};
	}
}

#endif
