// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef TRACECONTEXT_H_2025_09_04_17_53_12
#define TRACECONTEXT_H_2025_09_04_17_53_12
#include <string>
#include <weave/profiling/TraceCollector.h>
#include <weave/profiling/TraceSpan.h>

#include "constants.h"

namespace weave
{
	namespace profiling
	{
		/**
		 * @class TraceContext
		 * @brief
		 * We use a singleton pattern here only because we have an observable construct that does not affect functionality in the classes being injected to.
		 * In other words, as with logging, tracing can be completely removed from the code and will not change or affect functionality, and thus it is exempted
		 * from the best practice of not using global variables. The alternative would be to pass the tracer object into every function, which is not feasible and
		 * would unnecessarily pollute the code.
		 */
		class TraceContext
		{
		public:
			static void init(const std::string sessionName, const std::string sessionDescription);
			static bool initialized();
			static void enableSampling(uint64_t sampleRate = constants::DEFAULT_SAMPLE_RATE);
			static bool shouldTrace();
			static void increaseSampledRootCounter();
			static void addTraceSpanTree(const std::shared_ptr<TraceSpanDataNode>& traceSpanTree);
			static void dump(const std::string& directory = "");
			static void display();

		private:
			static std::shared_mutex _mutex;
			static std::shared_ptr<TraceCollector> _traceCollector;
			static bool _samplingEnabled;
			static uint64_t _sampleRate;
			static thread_local uint64_t _sampledRootCounter; // Samples happen homogeneously across workers, thus thread_local
		};

		/**
		 * Free function allows for:
		 * 1. Better inlining (no virtual dispatch)
		 * 2. Cleaner macros
		 * 3. C compatibility if needed
		 * 4. Simpler usage (no class qualification needed)
		 * @return
		 */
		TraceSpan trace(const std::string& name);
	}
}

#endif
