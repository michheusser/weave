// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser
#include "ConfigurationPrinter.h"
#include <sstream>
#include <fstream>
#include <weave/error/Exception.h>
#include <weave/logging/Macros.h>

namespace weave
{
	namespace profiling
	{
		std::string ConfigurationPrinter::print(const std::string& sessionName, const std::string& sessionDescription, std::vector<std::shared_ptr<ConfigurationDataNode> >& configurationDataTrees)
		{
			std::ostringstream output;

			// Session header
			output << "=== " << sessionName << " ===\n";
			output << sessionDescription << "\n\n";

			// Print each configuration tree
			for (size_t i = 0; i < configurationDataTrees.size(); ++i)
			{
				const auto& tree = configurationDataTrees[i];
				output << "Configuration: " << tree->name << "\n";
				output << printNode(tree, "", true);
				if (i < configurationDataTrees.size() - 1)
				{
					output << "\n";
				}
			}

			return output.str();
		}

		std::string ConfigurationPrinter::printNode(const std::shared_ptr<ConfigurationDataNode>& node, const std::string& prefix, bool isLast)
		{
			std::ostringstream output;

			// Current node
			std::string connector = isLast ? "└── " : "├── ";
			output << prefix << connector << node->name;

			// If it's a leaf node, show the value
			if (node->children.empty() && !node->value.empty())
			{
				output << ": " << node->value;
			}
			output << "\n";

			// Print children
			std::string childPrefix = prefix + (isLast ? "    " : "│   ");
			for (size_t i = 0; i < node->children.size(); ++i)
			{
				bool isLastChild = (i == node->children.size() - 1);
				output << printNode(node->children[i], childPrefix, isLastChild);
			}

			return output.str();
		}

		void ConfigurationPrinter::dumpToJson(const std::string& filename, std::vector<std::shared_ptr<ConfigurationDataNode> >& configurationDataTrees)
		{
			try
			{
				std::filesystem::path filepath(filename);
				std::filesystem::path parent_dir = filepath.parent_path();

				// Create all intermediate directories
				if (!parent_dir.empty())
				{
					std::error_code ec;
					std::filesystem::create_directories(parent_dir, ec);
					if (ec)
					{
						throw error::Exception("Failed to create directories: " + ec.message());
					}
				}

				std::ofstream jsonFile(filename);
				if (!jsonFile.is_open())
				{
					throw error::Exception("Failed to open file: " + filename);
				}

				jsonFile << "{\n";
				jsonFile << "  \"configurations\": {\n";

				for (size_t i = 0; i < configurationDataTrees.size(); ++i)
				{
					const auto& tree = configurationDataTrees[i];
					jsonFile << "    \"" << tree->name << "\": ";
					jsonFile << nodeToJson(tree, 2);

					if (i < configurationDataTrees.size() - 1)
					{
						jsonFile << ",";
					}
					jsonFile << "\n";
				}

				jsonFile << "  }\n";
				jsonFile << "}\n";

				jsonFile.close();
			}
			catch (error::Exception& exception)
			{
				LOG_WARN(exception.what());
				// No re-throwing
			}
		}

		std::string ConfigurationPrinter::nodeToJson(const std::shared_ptr<ConfigurationDataNode>& node, int indentLevel)
		{
			std::ostringstream json;

			if (node->children.empty())
			{
				// Leaf node - output the value
				json << "\"" << node->value << "\"";
			}
			else
			{
				// Internal node - output children as JSON object
				json << "{\n";

				for (size_t i = 0; i < node->children.size(); ++i)
				{
					const auto& child = node->children[i];
					json << indent(indentLevel + 1) << "\"" << child->name << "\": ";
					json << nodeToJson(child, indentLevel + 1);

					if (i < node->children.size() - 1)
					{
						json << ",";
					}
					json << "\n";
				}

				json << indent(indentLevel) << "}";
			}

			return json.str();
		}

		std::string ConfigurationPrinter::indent(int level)
		{
			return std::string(level * 2, ' ');
		}
	}
}
