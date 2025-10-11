// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser
#include <weave/profiling/ConfigurationCollector.h>
#include "ConfigurationPrinter.h"
#include <weave/logging/Macros.h>


namespace weave
{
	namespace profiling
	{
		ConfigurationCollector::ConfigurationCollector(const std::string& sessionName, const std::string& sessionDescription) : _sessionName(sessionName), _sessionDescription(sessionDescription)
		{}

		void ConfigurationCollector::display()
		{
			std::shared_lock<std::shared_mutex> lock(_mutex);
			std::string informationString = ConfigurationPrinter::print(_sessionName, _sessionDescription, _configDataTrees);
			LOG_INFO(informationString);
		}


		void ConfigurationCollector::dump(const std::string& directory)
		{
			std::shared_lock<std::shared_mutex> lock(_mutex);
			const std::string fileName = directory + "/" + _sessionName + "_CONFIG.json";
			ConfigurationPrinter::dumpToJson(fileName, _configDataTrees);
		}
	} // profiling
} // weave
