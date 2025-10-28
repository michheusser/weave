// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef METRICSCONTEXT_H_2025_09_10_10_54_02
#define METRICSCONTEXT_H_2025_09_10_10_54_02
#include <shared_mutex>
#include <string>

#include <weave/profiling/MetricsCollector.h>
#include <weave/profiling/MetricsSpan.h>

namespace weave
{
	namespace profiling
	{
		class MetricsContext
		{
		public:
			static void init(const std::string& sessionName, const std::string& sessionDescription);
			static bool initialized();
			static void addMetricsSpan(const std::shared_ptr<MetricsSpanData>& metricsSpanData);
			static void dump(const std::string& directory = "");
			static void display();

		private:
			static std::shared_mutex _mutex;
			static std::shared_ptr<MetricsCollector> _metricsCollector;
		};

		MetricsSpan metric(uint64_t hash, const std::string_view& name, const std::string_view& type, const int count);
	}
}

#endif
