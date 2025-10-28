// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef METRICSSPAN_H_2025_09_10_10_55_03
#define METRICSSPAN_H_2025_09_10_10_55_03
#include <weave/profiling/MetricsSpanData.h>

namespace weave
{
	namespace profiling
	{
		class MetricsSpan
		{
		public:
			explicit MetricsSpan(uint64_t hash, const std::string_view& name, const std::string_view& type, const int count);
			~MetricsSpan();
		private:
			std::shared_ptr<MetricsSpanData> _data;
		};
	}
}

#endif
