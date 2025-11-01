// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef WORKER_H_2025_09_20_13_44_55
#define WORKER_H_2025_09_20_13_44_55

#include <thread>
#include <weave/worker/Synchronizer.h>
#include <weave/error/Result.h>
#include <weave/error/Exception.h>
#include <weave/logging/Macros.h>
#include <weave/utilities/SignalManager.h>
#include <weave/utilities/Reflection.h>

namespace weave
{
	namespace worker
	{
		template<typename WorkerTag, typename ModuleType>
		class Worker
		{
		public:
			using SynchronizerTag = WorkerTag;
			using ContextType = typename Synchronizer<SynchronizerTag, ModuleType>::ContextType;
			static constexpr std::string_view name = utilities::typeName<WorkerTag>();
			explicit Worker(const ContextType& context) : _synchronizer(context)
			{}

			// Custom move, only copies synchronizer, otherwise no default move constructor! (threads can't be moved)
			Worker(Worker&& other) : _synchronizer(std::move(other._synchronizer))
			{}

			~Worker()
			{
				if (_thread.joinable())
				{
					_thread.join();
				}
				LOG_INFO(std::string(name) + ": worker shutdown.");
}

			template<typename InChannelTupleType, typename OutChannelTupleType>
			void start(InChannelTupleType& inChannelTuple, OutChannelTupleType& outChannelTuple)
			{
				try
				{
				LOG_INFO(std::string(name) + ": starting worker...");
					// We don't pass by reference to avoid lifetime issues with the tuples (even though they contain references!)
					_thread = std::thread(&Worker::_run<InChannelTupleType, OutChannelTupleType>, this, inChannelTuple, outChannelTuple);
				}
				catch (std::system_error& exception)
				{
					throw error::Exception("Failed");
					LOG_ERROR(exception.what());
				}
			}

		private:
			template<typename InChannelTupleType, typename OutChannelTupleType>
			void _run(InChannelTupleType inChannelTuple, OutChannelTupleType outChannelTuple)
			{
				_synchronizer.initialize();
				error::Result result = _cycle(inChannelTuple, outChannelTuple);
				if (!result.ok())
				{
					// No throwing since it's thread's entry point (would call std::terminate)
					LOG_ERROR("Error in cycle. Terminating thread.");
				}
				else
				{
					LOG_DEBUG("Thread correctly terminated.");
				}
			}

			template<typename InChannelTupleType, typename OutChannelTupleType>
			error::Result _cycle(InChannelTupleType& inChannelTuple, OutChannelTupleType& outChannelTuple) noexcept
			{
				while (!utilities::SignalManager::shutdownRequested())
				{
					error::Result result = _synchronizer.cycle(inChannelTuple, outChannelTuple);
					if (!result.ok())
					{
						LOG_ERROR("Cycle: Error in cycle.");
						return {error::Type::Cycle, 0}; // Right now a failed iteration shuts down the worker. Strategy might change in the future
					}
				}
				return error::Result::success();
			}

		private:
			std::thread _thread;
			Synchronizer<SynchronizerTag, ModuleType> _synchronizer;
		};
	}
}

#endif
