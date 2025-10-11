// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef METRICSPRINTER_H_2025_09_10_10_55_24
#define METRICSPRINTER_H_2025_09_10_10_55_24
#include <string>

#include <weave/profiling/MetricsSpanData.h>

namespace weave
{
	namespace profiling
	{
		class MetricsPrinter
		{
		public:
			static std::string print(const std::string& sessionName, const std::string& sessionDescription, std::vector<std::shared_ptr<MetricsSpanData>>& metricsSpanTrees);
			static void dumpToCsv(const std::string& filename, std::vector<std::shared_ptr<MetricsSpanData>>& metricsSpanTrees);
		};
	} // profiling
} // weave

#endif
