// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser
#include <weave/profiling/MetricsSpan.h>

#include <unistd.h>

#include <weave/profiling/MetricsContext.h>
#include <weave/error/Exception.h>
#include <weave/profiling/Macros.h>
#include <weave/logging/Macros.h>

namespace weave
{
	namespace profiling
	{
		MetricsSpan::MetricsSpan(uint64_t hash, const std::string_view& name, const std::string_view& type, const int count) : _data(std::make_shared<MetricsSpanData>())
		{
			try
			{
				const std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

				_data->hash = hash;
				_data->name = name;
				_data->threadID = std::this_thread::get_id();
				_data->processID = getpid();
				_data->type = type;
				_data->count = count;
				_data->startNs = std::chrono::duration_cast<std::chrono::nanoseconds>(start.time_since_epoch()).count();
			}
			catch (error::Exception& exception)
			{
				LOG_ERROR(exception.what());
				throw error::Exception("Failed");
			}
		}

		MetricsSpan::~MetricsSpan()
		{
			try
			{
				const std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
				_data->endNs = std::chrono::duration_cast<std::chrono::nanoseconds>(end.time_since_epoch()).count();
				MetricsContext::addMetricsSpan(_data);
			}
			catch (error::Exception& exception)
			{
				LOG_ERROR(exception.what());
				// We don't re-throw: If this destructor is called already during stack-unwinding because of another error-throwing
				// a further throw will call std::terminate() stopping the stack unwind.
			}
		}
	}
}
