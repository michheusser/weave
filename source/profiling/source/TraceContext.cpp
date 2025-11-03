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
		bool TraceContext::_samplingEnabled = false;
		uint64_t TraceContext::_sampleRate = 1;
		uint64_t thread_local TraceContext::_sampledRootCounter = 0;

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

		void TraceContext::enableSampling(uint64_t sampleRate)
		{
			std::unique_lock lock(_mutex);
			_samplingEnabled = true;
			_sampleRate = sampleRate;
		}

		bool TraceContext::shouldTrace()
		{
			std::shared_lock lock(_mutex);
			if (!_samplingEnabled)
			{
				return true; // Always trace if not sampling
			}
			bool result = (_sampledRootCounter % _sampleRate) == 0;
			return result;
		}

		void TraceContext::increaseSampledRootCounter()
		{
			// No mutex, since it's thread_local
			++_sampledRootCounter;
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
	}
}
