// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef SESSIONEXECUTOR_H_2025_09_14_15_59_26
#define SESSIONEXECUTOR_H_2025_09_14_15_59_26

namespace weave
{
	namespace profiling
	{
		class SessionExecutor
		{
		public:
			static void executeDefaultServerSession(); // TODO Maybe add TRACE_META/METRICS_META To macros?
			static void executeDefaultClientSession();
		};
	}
}

#endif
