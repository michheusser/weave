// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser
#include <weave/profiling/TraceContext.h>

#include <weave/logging/Macros.h>
#include <weave/error/Exception.h>

namespace weave
{
	namespace profiling
	{
		std::shared_mutex TraceContext::_mutex;
		std::shared_ptr<TraceCollector> TraceContext::_traceCollector = nullptr;
		thread_local uint64_t TraceContext::_currentFrameID = 0;

		void TraceContext::init(const std::string sessionName, std::string sessionDescription)
		{
			std::unique_lock<std::shared_mutex> lock(_mutex);
			_traceCollector = std::make_shared<TraceCollector>(sessionName, sessionDescription);
		}

		bool TraceContext::initialized()
		{
			const bool isInitialized = static_cast<bool>(_traceCollector);
			return isInitialized;
		}

		void TraceContext::addTraceSpanTree(const std::shared_ptr<TraceSpanDataNode>& traceSpanTree)
		{
			try
			{
				if (!initialized())
				{
					throw error::Exception("Trace Context not initialized.");
				}
				_traceCollector->addTraceSpanTree(traceSpanTree);
			}
			catch (error::Exception& exception)
			{
				LOG_ERROR(exception.what());
				throw error::Exception("Failed");
			}
		}

		uint64_t TraceContext::getCurrentFrame()
		{
			return _currentFrameID; // Thread local, no need for mutex
		}

		void TraceContext::setCurrentFrame(uint32_t frameID)
		{
			_currentFrameID = frameID; // Thread local, no need for mutex
		}

		void TraceContext::dump(const std::string& directory)
		{
			try
			{
				if (!initialized())
				{
					throw error::Exception("Trace Context not initialized.");
				}
				std::shared_lock<std::shared_mutex> lock(_mutex);
				_traceCollector->dump(directory);
			}
			catch (error::Exception& exception)
			{
				LOG_ERROR(exception.what());
				throw error::Exception("Failed");
			}
		}

		void TraceContext::display()
		{
			try
			{
				if (!initialized())
				{
					throw error::Exception("Trace Context not initialized.");
				}
				std::shared_lock<std::shared_mutex> lock(_mutex);
				_traceCollector->display();
			}
			catch (error::Exception& exception)
			{
				LOG_ERROR(exception.what());
				throw error::Exception("Failed");
			}
		}


		TraceSpan trace(const std::string& name)
		{
			return TraceSpan(name);
		}
	} // profiling
} // weave
