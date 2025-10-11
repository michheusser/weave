// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef TRACESPAN_H_2025_09_04_19_58_45
#define TRACESPAN_H_2025_09_04_19_58_45
#include <memory>
#include <chrono>
#include <weave/profiling/TraceSpanDataNode.h>

namespace weave
{
	namespace profiling
	{
		/**
		 * @class TraceSpan
		 * @brief This is an RAII measuring class to create a profiling traceSpan to be measured until it's destroyed. The important thing here is that it works in conjunction with a
		 * generalized tree containing all the data of all the nested calls. Each call to TraceSpan creates a new tree entry in the correct (current) location inside the tree.
		 * Since it is RAII, the object needs to be destructed once its scope ends, and its "left-over" is the TraceSpanData object inside the tree. The TraceSpan class is only there to
		 * manage the TraceSpanData creation and correct attachment to the tree and is not the data container itself.
		 */
		class TraceSpan
		{
		public:
			explicit TraceSpan(const std::string& name);
			~TraceSpan();
		private:
			// After the TraceSpan object is destructed, the data needs to be kept "alive" (thus shared)
			std::shared_ptr<TraceSpanDataNode> _dataNode;
			// TODO Consider if it's semantically meaningful for this to be a member variable. Also create documentation for this one.
			thread_local static std::shared_ptr<TraceSpanDataNode> _currentTraceSpanDataNode;
		};
	} // profiling
} // weave

#endif
