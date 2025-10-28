// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef METRICSCOLLECTOR_H_2025_09_10_10_54_29
#define METRICSCOLLECTOR_H_2025_09_10_10_54_29
#include <shared_mutex>
#include <string>

#include <weave/profiling/MetricsSpanData.h>
#include "FunctionStatistics.h"

namespace weave
{
	namespace profiling
	{
		class MetricsCollector
		{
		public:
			// TODO Everything should be done compile time
			// TODO Creation of the metrics map, if possible at init.
			explicit MetricsCollector(const std::string& sessionName, const std::string& sessionDescription);
			void addMetricsSpan(const std::shared_ptr<MetricsSpanData>& metricsSpanData);
			void display();
			void dump(const std::string& directory);

		private:
			mutable std::shared_mutex _mutex;
			std::string _sessionName;
			std::string _sessionDescription;
			std::unordered_map<uint64_t, FunctionStatistics> _statistics;
		};
	}
}

#endif
