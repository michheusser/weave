// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser
#include <weave/profiling/TraceSpan.h>
#include <weave/logging/Macros.h>
#include <weave/error/Exception.h>
#include <chrono>
#include <unistd.h>

#include <weave/profiling/TraceContext.h>

namespace weave
{
	namespace profiling
	{
		thread_local std::shared_ptr<TraceSpanDataNode> TraceSpan::_currentTraceSpanDataNode = nullptr;

		TraceSpan::TraceSpan(const std::string& name) : _dataNode(std::make_shared<TraceSpanDataNode>())
		{
			try
			{
				const std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

				_dataNode->name = name;
				_dataNode->threadID = std::this_thread::get_id();
				_dataNode->processID = getpid();
				_dataNode->startInNanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(start.time_since_epoch()).count();
				_dataNode->parent = _currentTraceSpanDataNode;

				if (_currentTraceSpanDataNode)
				{
					_currentTraceSpanDataNode->children.push_back(_dataNode);
				}
				_currentTraceSpanDataNode = _dataNode;
			}
			catch (error::Exception& exception)
			{
				LOG_ERROR(exception.what());
				throw error::Exception("Failed");
			}
		}

		TraceSpan::~TraceSpan()
		{
			try
			{
				const std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
				_dataNode->frameID = TraceContext::getCurrentFrame(); // Saved here, since frameID might be set after the TRACE_FUNCTION macro which calls the constructor
				_dataNode->endInNanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(end.time_since_epoch()).count();
				// We do not check for expired, since that would be implying that I don't trust my own design. Even for future modifications,
				// modern C++ philosophy is: "Don't code defensively against your own design violations". It can also imply that a condition that
				// actually never happens could in fact happen. A check would also not actually fix the design violation, it just masks symptoms.
				// IF RAII order is broken, the timing measurements are already wrong, regardless of whether there's a crash or not. This would be otherwise
				// candidate for an assert.
				_currentTraceSpanDataNode = _dataNode->parent.lock();
				if (!_currentTraceSpanDataNode) // Root node
				{
					TraceContext::addTraceSpanTree(_dataNode);
				}
			}
			catch (error::Exception& exception)
			{
				LOG_ERROR(exception.what());
				// We don't re-throw: If this destructor is called already during stack-unwinding because of another error-throwing
				// a further throw will call std::terminate() stopping the stack unwind.
			}
		}
	} // profiling
} // weave
