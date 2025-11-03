// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser
#include "TracePrinter.h"

#include <fstream>

#include <iomanip>
#include <weave/logging/Macros.h>
#include <weave/error/Exception.h>

namespace weave
{
	namespace profiling
	{
		std::string TracePrinter::print(std::vector<std::shared_ptr<TraceSpanDataNode>>& traceSpanTrees)
		{
			std::ostringstream output;

			for (size_t i = 0; i < traceSpanTrees.size(); ++i)
			{
				const auto& rootNode = traceSpanTrees[i];
				if (!rootNode) continue;

				// Print header for each tree
				output << "Trace Tree (Thread: " << std::hex << rootNode->threadID
					<< ", Process: " << std::dec << rootNode->processID << ")\n";

				printNode(output, rootNode, "", true, true);

				// Add separator between trees (except for the last one)
				if (i < traceSpanTrees.size() - 1)
				{
					output << "\n";
				}
			}

			return output.str();
		}

		void TracePrinter::printNode(std::ostringstream& output,
		                             const std::shared_ptr<TraceSpanDataNode>& node,
		                             const std::string& prefix,
		                             bool isLast,
		                             bool isRoot)
		{
			if (!node) return;

			uint64_t durationNs = node->endInNanoseconds - node->startInNanoseconds;
			double durationMs = durationNs / 1000000.0; // Convert to milliseconds

			// Calculate percentage relative to parent
			double percentage = 100.0;
			if (!isRoot)
			{
				auto parentPtr = node->parent.lock();
				if (parentPtr)
				{
					uint64_t parentDurationNs = parentPtr->endInNanoseconds - parentPtr->startInNanoseconds;
					if (parentDurationNs > 0)
					{
						percentage = (static_cast<double>(durationNs) / parentDurationNs) * 100.0;
					}
				}
			}

			// Print the current node
			if (isRoot)
			{
				output << node->name << " (" << formatDuration(durationMs) << ", "
					<< std::fixed << std::setprecision(1) << percentage << "%)\n";
			}
			else
			{
				output << prefix << (isLast ? "└── " : "├── ")
					<< node->name << " (" << formatDuration(durationMs) << ", "
					<< std::fixed << std::setprecision(1) << percentage << "%)\n";
			}

			// Print children
			for (size_t i = 0; i < node->children.size(); ++i)
			{
				bool childIsLast = (i == node->children.size() - 1);
				std::string childPrefix = prefix;

				if (!isRoot)
				{
					childPrefix += (isLast ? "    " : "│   ");
				}

				printNode(output, node->children[i], childPrefix, childIsLast, false);
			}
		}

		std::string TracePrinter::formatDuration(double durationMs)
		{
			std::ostringstream formatted;
			formatted << std::fixed << std::setprecision(2) << durationMs << "ms";
			return formatted.str();
		}

		void TracePrinter::dumpToJson(const std::string& filename, std::vector<std::shared_ptr<TraceSpanDataNode>>& traceSpanTrees)
		{
			try
			{
				std::filesystem::path filepath(filename);
				std::filesystem::path parent_dir = filepath.parent_path();

				// Create all intermediate directories
				if (!parent_dir.empty()) {
					std::error_code ec;
					std::filesystem::create_directories(parent_dir, ec);
					if (ec) {
						throw error::Exception("Failed to create directories: " + ec.message());
					}
				}

				std::vector<std::shared_ptr<TraceSpanDataNode>> serializedTraceSpanTrees = serializeTraceSpanTrees(traceSpanTrees);
				// Build JSON in memory first
				std::ofstream file(filename);
				if (!file.is_open())
				{
					throw error::Exception("Failed to open file: " + filename);
				}

				file << "[\n";

				for (size_t i = 0; i < serializedTraceSpanTrees.size(); ++i)
				{
					const auto& node = serializedTraceSpanTrees[i];
					uint64_t duration = node->endInNanoseconds - node->startInNanoseconds;

					// Convert thread::id to string
					std::ostringstream tid_stream;
					tid_stream << node->threadID;

					file << "  {\n"
						<< "    \"name\": \"" << node->name << "\",\n"
						<< "    \"cat\": \"function\",\n"
						<< "    \"ph\": \"X\",\n"
						<< "    \"ts\": " << (node->startInNanoseconds / 1000) << ",\n"
						<< "    \"dur\": " << (duration / 1000) << ",\n"
						<< "    \"tid\": \"" << tid_stream.str() << "\",\n"
						<< "    \"pid\": " << node->processID << ",\n"

						<< "    \"args\": {\n"
						// Further metadata:
						// << "      \"frameID\": " << node->frameID << "\n"
						<< "    }\n"
						<< "  }";

					if (i < serializedTraceSpanTrees.size() - 1)
					{
						file << ",";
					}
					file << "\n";
				}

				file << "]\n";
			}
			catch (error::Exception& exception)
			{
				LOG_WARN(exception.what());
				// No re-throwing
			}
		}

		std::vector<std::shared_ptr<TraceSpanDataNode>> TracePrinter::serializeTraceSpanTrees(std::vector<std::shared_ptr<TraceSpanDataNode>>& traceSpanTrees)
		{
			try
			{
				if (traceSpanTrees.empty())
				{
					LOG_WARN("Empty traceSpan tree vector");
					return {};
				}
				std::vector<std::shared_ptr<TraceSpanDataNode>> results;
				// DFT using iterators to keep space complexity theta(height) instead of O(n) for each tree
				for (const std::shared_ptr<TraceSpanDataNode>& curRootNode : traceSpanTrees)
				{
					std::vector<std::shared_ptr<TraceSpanDataNode>> rootNodeVector = {curRootNode};
					std::stack<std::pair<std::vector<std::shared_ptr<TraceSpanDataNode>>::iterator, std::vector<std::shared_ptr<TraceSpanDataNode>>::iterator>> nodes;
					nodes.push({rootNodeVector.begin(), rootNodeVector.end()});
					while (!nodes.empty())
					{
						auto& [curNodeIter, curNodeIterEnd] = nodes.top();
						if (curNodeIter != curNodeIterEnd)
						{
							std::shared_ptr<TraceSpanDataNode> curNode = *curNodeIter;
							results.push_back(curNode);
							++curNodeIter;
							nodes.push({curNode->children.begin(), curNode->children.end()});
						}
						else
						{
							nodes.pop();
						}
					}
				}
				return results;
			}
			catch (error::Exception& exception)
			{
				LOG_ERROR(exception.what());
				throw error::Exception("Failed.");
			}
		}
	}
}
