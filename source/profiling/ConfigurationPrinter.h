// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef CONFIGURATIONPRINTER_H_2025_09_13_16_36_21
#define CONFIGURATIONPRINTER_H_2025_09_13_16_36_21
#include <string>

#include <weave/profiling/ConfigurationDataNode.h>

namespace weave
{
	namespace profiling
	{
		class ConfigurationPrinter
		{
		public:
			static std::string print(const std::string& sessionName, const std::string& sessionDescription, std::vector<std::shared_ptr<ConfigurationDataNode>>& configurationDataTrees);
			static void dumpToJson(const std::string& filename, std::vector<std::shared_ptr<ConfigurationDataNode>>& configurationDataTrees);

		private:
			static std::string printNode(const std::shared_ptr<ConfigurationDataNode>& node, const std::string& prefix = "", bool isLast = true);
			static std::string nodeToJson(const std::shared_ptr<ConfigurationDataNode>& node, int indentLevel = 0);
			static std::string indent(int level);
		};
	}
}

#endif
