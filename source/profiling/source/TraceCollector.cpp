// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser
#include <weave/profiling/TraceCollector.h>
#include "TracePrinter.h"
#include <weave/logging//Macros.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <numeric>

namespace weave
{
	namespace profiling
	{
		TraceCollector::TraceCollector(const std::string& sessionName, const std::string& sessionDescription) : _sessionName(sessionName), _sessionDescription(sessionDescription)
		{}

		void TraceCollector::addTraceSpanTree(const std::shared_ptr<TraceSpanDataNode>& traceSpanTree)
		{
			std::unique_lock lock(_mutex);
			_traceSpanTrees.push_back(traceSpanTree);
		}

		void TraceCollector::dump(const std::string& directory)
		{
			std::shared_lock<std::shared_mutex> lock(_mutex);
			const std::string fileName = directory + "/" + _sessionName + "_TRACE.json";
			TracePrinter::dumpToJson(fileName, _traceSpanTrees);
		}

		void TraceCollector::display()
		{
			std::shared_lock<std::shared_mutex> lock(_mutex);
			std::string informationString = TracePrinter::print(_traceSpanTrees);
			LOG_INFO(informationString);
		}
	}
}
