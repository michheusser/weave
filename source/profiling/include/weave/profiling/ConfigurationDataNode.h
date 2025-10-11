// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef CONFIGURATIONDATANODE_H_2025_09_13_17_03_16
#define CONFIGURATIONDATANODE_H_2025_09_13_17_03_16
#include <string>
#include <vector>

namespace weave
{
	namespace profiling
	{
		struct ConfigurationDataNode
		{
			std::string name;
			std::string value; // Stays empty unless it's a leaf node (no children)
			std::vector<std::shared_ptr<ConfigurationDataNode>> children;
		};
	}
}

#endif
