// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser
#include <weave/profiling/MetricsContext.h>

#include <weave/profiling/TraceCollector.h>
#include <weave/error/Exception.h>
#include <weave/logging//Macros.h>

namespace weave
{
	namespace profiling
	{
		std::shared_mutex MetricsContext::_mutex;
		std::shared_ptr<MetricsCollector> MetricsContext::_metricsCollector = nullptr;

		void MetricsContext::init(const std::string& sessionName, const std::string& sessionDescription)
		{
			_metricsCollector = std::make_shared<MetricsCollector>(sessionName, sessionDescription);
		}

		bool MetricsContext::initialized()
		{
			const bool isInitialized = static_cast<bool>(_metricsCollector);
			return isInitialized;
		}

		void MetricsContext::addMetricsSpan(const std::shared_ptr<MetricsSpanData>& metricsSpanData)
		{
			try
			{
				if (!initialized())
				{
					throw error::Exception("Metrics Context not initialized.");
				}
				_metricsCollector->addMetricsSpan(metricsSpanData);
			}
			catch (error::Exception& exception)
			{
				LOG_ERROR(exception.what());
				throw error::Exception("Failed");
			}
		}

		void MetricsContext::dump(const std::string& directory)
		{
			try
			{
				if (!initialized())
				{
					throw error::Exception("Metrics Context not initialized.");
				}
				std::shared_lock<std::shared_mutex> lock(_mutex);
				_metricsCollector->dump(directory);
			}
			catch (error::Exception& exception)
			{
				LOG_ERROR(exception.what());
				throw error::Exception("Failed");
			}
		}

		void MetricsContext::display()
		{
			try
			{
				if (!initialized())
				{
					throw error::Exception("Metrics Context not initialized.");
				}
				std::shared_lock<std::shared_mutex> lock(_mutex);
				_metricsCollector->display();
			}
			catch (error::Exception& exception)
			{
				LOG_ERROR(exception.what());
				throw error::Exception("Failed");
			}
		}

		MetricsSpan metric(const std::string name, const std::string& type, const int count)
		{
			return MetricsSpan(name, type, count);
		}
	} // profiling
} // weave
