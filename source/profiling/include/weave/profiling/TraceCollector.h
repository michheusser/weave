// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef TRACECOLLECTOR_H_2025_09_04_17_46_09
#define TRACECOLLECTOR_H_2025_09_04_17_46_09
#include <shared_mutex>
#include <string>

#include <weave/profiling/TraceSpanDataNode.h>

namespace weave
{
	namespace profiling
	{
		/**
		 * @class TraceCollector
		 * @brief It serves as an aggregator of different TraceSpan Trees from all different threads creating their own trace data tree (TraceSpanNodeData). The TraceContext creates and handles a
		 * global instance of this collector across all threads that can be called from the TraceSpan class when creating a TraceSpanNodeData tree to be added to the collector. Since it's being accessed
		 * from multiple threads potentially, needs to be thread-safe.
		 */
		class TraceCollector
		{
		public:
			explicit TraceCollector(const std::string& sessionName, const std::string& sessionDescription);
			void addTraceSpanTree(const std::shared_ptr<TraceSpanDataNode>& traceSpanTree);
			void display();
			/**
			 * 	@brief Saves to Chrome Trace Event Format
			 * 	Can be loaded into chrome://tracing
			 */
			void dump(const std::string& directory);

		private:
			mutable std::shared_mutex _mutex;
			std::string _sessionName;
			std::string _sessionDescription;
			std::vector<std::shared_ptr<TraceSpanDataNode>> _traceSpanTrees;
		};
	} // profiling
} // weave

#endif
