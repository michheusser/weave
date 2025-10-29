// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef METRICSPRINTER_H_2025_09_10_10_55_24
#define METRICSPRINTER_H_2025_09_10_10_55_24
#include <string>

#include <weave/profiling/MetricsSpanData.h>

#include "weave/profiling/FunctionStatistics.h"

namespace weave
{
	namespace profiling
	{
		class MetricsPrinter
		{
		public:
			static std::string print(const std::string& sessionName, const std::string& sessionDescription, std::unordered_map<uint64_t, FunctionStatistics>& statistics);
			static void dumpToCsv(const std::string& filename, std::unordered_map<uint64_t, FunctionStatistics>& statistics);

		private:
			static uint64_t _computePercentile(std::vector<uint64_t> samples, double percentile);
			static double _nsToMs(uint64_t ns);
			static double _nsToS(uint64_t ns);
			static double _computeThroughput(uint64_t count, uint64_t durationNs);
		};
	} // namespace profiling
} // namespace weave

#endif
