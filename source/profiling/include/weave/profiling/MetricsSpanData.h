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
			std::string name;
			std::thread::id threadID; // Obfuscated type since it differs depending on OS (has overloaded ostream operator)
			pid_t processID;
			uint64_t startInNanoseconds;
			uint64_t endInNanoseconds;
			std::string type;
			uint64_t count;
		};
	}
}

#endif
