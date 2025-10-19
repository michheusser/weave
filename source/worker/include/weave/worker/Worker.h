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
#include <weave/user/WorkerTraits.h>
#include <weave/utilities/SignalManager.h>

namespace weave
{
	namespace worker
	{
		template<typename WorkerTag>
		class Worker
		{
		public:
			using SynchronizerTag = typename user::WorkerTraits<WorkerTag>::SynchronizerTag;
			explicit Worker(const typename user::WorkerTraits<WorkerTag>::ContextType& context) : _synchronizer(context)
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
				LOG_INFO("Worker shutdown.");
			}

			template<typename InChannelTupleType, typename OutChannelTupleType>
			void start(InChannelTupleType& inChannelTuple, OutChannelTupleType& outChannelTuple)
			{
				LOG_INFO("Starting worker...");
				_thread = std::thread(&Worker::_run<InChannelTupleType, OutChannelTupleType>, this, inChannelTuple, outChannelTuple);
			}

		private:
			template<typename InChannelTupleType, typename OutChannelTupleType>
			void _run(InChannelTupleType inChannelTuple, OutChannelTupleType outChannelTuple)
			{
				try
				{
					_synchronizer.initialize();
					error::Result result = _cycle(inChannelTuple, outChannelTuple);
					if (!result.ok())
					{
						throw error::Exception("Error in cycle");
					}
				}
				catch (error::Exception& exception)
				{
					LOG_ERROR(exception.what());
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
			Synchronizer<SynchronizerTag> _synchronizer;
		};
	}
}

#endif
