// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser
#include <weave/profiling/ConfigurationContext.h>
#include <weave/error/Exception.h>
#include <weave/logging/Macros.h>

namespace weave
{
	namespace profiling
	{
		std::shared_mutex ConfigurationContext::_mutex;
		std::shared_ptr<ConfigurationCollector> ConfigurationContext::_configurationCollector = nullptr;

		void ConfigurationContext::init(const std::string& sessionName, const std::string& sessionDescription)
		{
			std::unique_lock<std::shared_mutex> lock(_mutex);
			_configurationCollector = std::make_shared<ConfigurationCollector>(sessionName, sessionDescription);
		}

		bool ConfigurationContext::initialized()
		{
			const bool isInitialized = static_cast<bool>(_configurationCollector);
			return isInitialized;
		}

		void ConfigurationContext::dump(const std::string& directory)
		{
			try
			{
				if (!initialized())
				{
					throw error::Exception("Metrics Context not initialized.");
				}
				std::shared_lock<std::shared_mutex> lock(_mutex);
				_configurationCollector->dump(directory);
			}
			catch (error::Exception& exception)
			{
				LOG_ERROR(exception.what());
				throw error::Exception("Failed");
			}
		}

		void ConfigurationContext::display()
		{
			try
			{
				if (!initialized())
				{
					throw error::Exception("Metrics Context not initialized.");
				}
				std::shared_lock<std::shared_mutex> lock(_mutex);
				_configurationCollector->display();
			}
			catch (error::Exception& exception)
			{
				LOG_ERROR(exception.what());
				throw error::Exception("Failed");
			}
		}
	} // profiling
} // weave
