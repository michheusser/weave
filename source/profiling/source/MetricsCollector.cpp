// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser
#include <weave/profiling/MetricsCollector.h>
#include <weave/logging//Macros.h>
#include "MetricsPrinter.h"

namespace weave
{
	namespace profiling
	{
		MetricsCollector::MetricsCollector(const std::string& sessionName, const std::string& sessionDescription) : _sessionName(sessionName), _sessionDescription(sessionName)
		{}

		void MetricsCollector::addMetricsSpan(const std::shared_ptr<MetricsSpanData>& metricsSpanData)
		{
			std::unique_lock lock(_mutex);
			_metricsSpans.push_back(metricsSpanData);
		}

		void MetricsCollector::display()
		{
			std::shared_lock<std::shared_mutex> lock(_mutex);
			std::string informationString = MetricsPrinter::print(_sessionName, _sessionDescription, _metricsSpans);
			LOG_INFO(informationString);
		}

		void MetricsCollector::dump(const std::string& directory)
		{
			std::shared_lock<std::shared_mutex> lock(_mutex);
			const std::string fileName = directory + "/" + _sessionName + "_METRICS.csv";
			MetricsPrinter::dumpToCsv(fileName, _metricsSpans);
		}
	}
}
