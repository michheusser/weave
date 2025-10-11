// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef SIGNALMANAGER_H_2025_09_01_15_00_32
#define SIGNALMANAGER_H_2025_09_01_15_00_32
#include <atomic>

namespace weave
{
	namespace utilities
	{
		class SignalManager
		{
		public:
			static void installHandlers();
			static void restoreHandlers();
			static bool shutdownRequested();
		private:
			static void _signalHandler(int signal);
			static std::atomic<bool> _shutdownRequested;
		};
	} // Application
} // LatencyLens

#endif
