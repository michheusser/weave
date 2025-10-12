// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef CONFIGURATIONDATABUILDER_H_2025_09_13_18_31_12
#define CONFIGURATIONDATABUILDER_H_2025_09_13_18_31_12
#include <sstream>

#include <weave/profiling/ConfigurationDataNode.h>
#include <weave/profiling/ConfigurationSerialization.h>

namespace weave
{
	namespace profiling
	{
		template <typename T>
		concept HasGetChildrenDataTreeS = requires(const T& t)
		{
			{ t.getChildrenDataTrees() } -> std::convertible_to<std::vector<std::shared_ptr<ConfigurationDataNode>>>;
		};

		template <typename T>
		concept Streamable = requires(std::ostream& os, const T& value)
		{
			os << value;
		};

		class ConfigurationDataBuilder
		{
		public:
			ConfigurationDataBuilder();

			template <typename ConfigDataType>
			static std::shared_ptr<ConfigurationDataNode> buildRootDataTree(std::string& name, ConfigDataType& configDataObject)
			{

				std::shared_ptr<ConfigurationDataNode> rootDataNode = std::make_shared<ConfigurationDataNode>();
				rootDataNode->name = name;
				if constexpr (HasGetChildrenDataTreeS<ConfigDataType>)
				{
					rootDataNode->children = configDataObject.getChildrenDataTrees();
				}
				else if constexpr (Streamable<ConfigDataType>) // In case e.g. a primitive is passed as a configuration data type
				{
					std::ostringstream oss;
					oss << configDataObject;
					rootDataNode->value = oss.str();
				}
				else
				{
					static_assert(false, "Configuration type must either have getChildrenDataTrees() function or be streamable.");
				}
				return rootDataNode;
			}

			/**
			 * @brief Pairs need to be given in as explicit pairs and not just {"configData",configData} because type deduction is apparently not clear or ambiguous
			 * @tparam Pairs
			 * @param pairs
			 * @return
			 */
			template <typename... Pairs>
			static std::vector<std::shared_ptr<ConfigurationDataNode>> buildDataTrees(Pairs&&... pairs)
			{
				ConfigurationDataBuilder builder;
				// C++17 fold expression, instead of template metaprogramming boilerplate (recursion for compile time for loop)
				(builder.addField(pairs), ...);
				return builder._dataTrees;
			}

			template <typename StringType, typename ConfigDataType>
			void addField(std::pair<StringType, ConfigDataType>& configDataPair)
			{
				const std::string& configDataName = configDataPair.first;
				const ConfigDataType& configDataObject = configDataPair.second;

				std::shared_ptr<ConfigurationDataNode> configDataNode = std::make_shared<ConfigurationDataNode>();
				configDataNode->name = configDataName;
				if constexpr (HasGetChildrenDataTreeS<ConfigDataType>)
				{
					 configDataNode->children = configDataObject.getChildrenDataTrees();
				}
				else if constexpr (Streamable<ConfigDataType>)
				{
					std::ostringstream oss;
					oss << configDataObject;
					configDataNode->value = oss.str();
				}
				else
				{
					static_assert(false, "Configuration Type must either have getChildrenDataTrees() function or be streamable");
				}
				_dataTrees.push_back(configDataNode);
			}

		private:
			std::vector<std::shared_ptr<ConfigurationDataNode>> _dataTrees;
		};
	}
}

#endif
