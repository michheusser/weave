// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser
#include <weave/utilities/SignalManager.h>
#include <csignal>

namespace weave
{
	namespace utilities
	{
		std::atomic<bool> SignalManager::_shutdownRequested{false};

		void SignalManager::installHandlers()
		{
			std::signal(SIGINT, _signalHandler);
			std::signal(SIGTERM, _signalHandler);
		}

		void SignalManager::restoreHandlers()
		{
			std::signal(SIGINT, SIG_DFL); // Restore default behavior
			std::signal(SIGTERM, SIG_DFL);
		}

		bool SignalManager::shutdownRequested()
		{
			return _shutdownRequested.load(std::memory_order_relaxed);
		}

		void SignalManager::_signalHandler(int signal)
		{
			if (signal == SIGINT || signal == SIGTERM)
			{
				_shutdownRequested.store(true, std::memory_order_relaxed);
			}
		}
	} // Application
} // LatencyLens
