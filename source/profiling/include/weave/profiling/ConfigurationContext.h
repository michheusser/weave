// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef CONFIGURATIONCONTEXT_H_2025_09_13_17_12_39
#define CONFIGURATIONCONTEXT_H_2025_09_13_17_12_39
#include <string>

#include <weave/profiling/ConfigurationCollector.h>

namespace weave
{
	namespace profiling
	{
		class ConfigurationContext
		{
		public:
			static void init(const std::string& sessionName, const std::string& sessionDescription);
			static bool initialized();
			static void dump(const std::string& directory = "");
			static void display();

			template <typename ConfigType>
			static void addConfiguration(std::string name, ConfigType& config)
			{
				_configurationCollector->addConfiguration(name,config);
			}
		private:
			static std::shared_mutex _mutex;
			static std::shared_ptr<ConfigurationCollector> _configurationCollector;
		};
	} // profiling
} // weave

#endif
