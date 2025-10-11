// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef TRACEPRINTER_H_2025_09_08_13_42_05
#define TRACEPRINTER_H_2025_09_08_13_42_05

#include <string>
#include <weave/profiling/TraceSpanDataNode.h>

namespace weave
{
	namespace profiling
	{
		class TracePrinter
		{
		public:
			static std::string print(std::vector<std::shared_ptr<TraceSpanDataNode>>& traceSpanTrees);
			static void dumpToJson(const std::string& filename, std::vector<std::shared_ptr<TraceSpanDataNode>>& traceSpanTrees);

		private:
			static void printNode(std::ostringstream& output, const std::shared_ptr<TraceSpanDataNode>& node, const std::string& prefix, bool isLast, bool isRoot);
			static std::string formatDuration(double durationMs);
			static std::vector<std::shared_ptr<TraceSpanDataNode>> serializeTraceSpanTrees(std::vector<std::shared_ptr<TraceSpanDataNode>>& traceSpanTrees);
		};
	} // profiling
} // weave

#endif
