// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef CONFIGURATIONCOLLECTOR_H_2025_09_13_17_05_33
#define CONFIGURATIONCOLLECTOR_H_2025_09_13_17_05_33
#include <shared_mutex>

#include <weave/profiling/ConfigurationDataBuilder.h>
#include <weave/profiling/ConfigurationDataNode.h>

namespace weave
{
	namespace profiling
	{
		class ConfigurationCollector
		{
		public:
			explicit ConfigurationCollector(const std::string& sessionName, const std::string& sessionDescription);
			void display();
			void dump(const std::string& directory);

			template <typename ConfigType>
			void addConfiguration(std::string& name, ConfigType& config)
			{
				std::shared_ptr<ConfigurationDataNode> dataTreeRootNode = ConfigurationDataBuilder::buildRootDataTree(name, config);
				_configDataTrees.push_back(dataTreeRootNode);
			}

		private:
			mutable std::shared_mutex _mutex;
			std::string _sessionName;
			std::string _sessionDescription;
			std::vector<std::shared_ptr<ConfigurationDataNode>> _configDataTrees;
		};
	}
}

#endif
