// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef WRITERACQUIRER_H_2025_09_29_17_20_01
#define WRITERACQUIRER_H_2025_09_29_17_20_01

#include <condition_variable>
#include <shared_mutex>

#include <weave/buffer/Constants.h>
#include <weave/buffer/QueueBuffer.h>
#include <weave/buffer/Policy.h>

namespace weave
{
	namespace buffer
	{
		// Policies for a writer are concerned with the question "What's the strategy if the buffer is full?"
		template <Constants::BufferType type, Constants::PolicyType policy>
		class WriterAcquirer; // Cannot be instantiated

		// TODO Approach: Implement single specialized policies, and then try to make generic using the parameters of the policy to avoid specialization (if possible)
		template <Constants::BufferType type>
		class WriterAcquirer<type, Constants::PolicyType::Lossless>
		{
			using Policy = Policy<Constants::PolicyType::Lossless>;

		public:
			static void acquire(std::shared_mutex& mutex, std::condition_variable_any& conditionVariableWrite, QueueBuffer<type>& queueBuffer, Constants::WriterState& writerState) noexcept
			{
				std::unique_lock lock(mutex);
				conditionVariableWrite.wait(lock, [&queueBuffer]()
				{
					return !queueBuffer.full();
				});
				lock.unlock();
				writerState = Constants::WriterState::Active;
			}
		};

		template <Constants::BufferType type>
		class WriterAcquirer<type, Constants::PolicyType::Realtime>
		{
		public:
			using Policy = Policy<Constants::PolicyType::Realtime>;

			static void acquire(std::shared_mutex& mutex, std::condition_variable_any& conditionVariableWrite, QueueBuffer<type>& queueBuffer, Constants::WriterState& writerState) noexcept
			{
				std::unique_lock lock(mutex);
				if (queueBuffer.full())
				{
					queueBuffer.pop();
				}
				lock.unlock();
				writerState = Constants::WriterState::Active;
			}
		};

		template <Constants::BufferType type>
		class WriterAcquirer<type, Constants::PolicyType::Attempts>
		{
		public:
			using Policy = Policy<Constants::PolicyType::Attempts>;

			static void acquire(std::shared_mutex& mutex, std::condition_variable_any& conditionVariableWrite, QueueBuffer<type>& queueBuffer, Constants::WriterState& writerState) noexcept
			{
				for (int i = 1; i <= Policy::numAttempts; ++i)
				{
					std::unique_lock lock(mutex);
					if (!queueBuffer.full())
					{
						break;
					}
					if (i == Policy::numAttempts)
					{
						queueBuffer.pop(); // we discard oldest frame without reading
					}
					else
					{
						lock.unlock();
						std::this_thread::sleep_for(std::chrono::milliseconds(Policy::timeoutMS));
					}
				}
				writerState = Constants::WriterState::Active;
			}
		};


		template <Constants::BufferType type>
		class WriterAcquirer<type, Constants::PolicyType::LiveStream>
		{
		public:
			using Policy = Policy<Constants::PolicyType::LiveStream>;

			static void acquire(std::shared_mutex& mutex, std::condition_variable_any& conditionVariableWrite, QueueBuffer<type>& queueBuffer, Constants::WriterState& writerState) noexcept
			{
				std::unique_lock lock(mutex);
				conditionVariableWrite.wait_for(std::chrono::milliseconds(Policy::timeoutMS),[](){return true;});
				if (queueBuffer.full())
				{
					queueBuffer.pop();
				}
				lock.unlock();
				writerState = Constants::WriterState::Active;
			}
		};


		template <Constants::BufferType type>
		class WriterAcquirer<type, Constants::PolicyType::Throttled>
		{
		public:
			using Policy = Policy<Constants::PolicyType::Throttled>;

			static void acquire(std::shared_mutex& mutex, std::condition_variable_any& conditionVariableWrite, QueueBuffer<type>& queueBuffer, Constants::WriterState& writerState) noexcept
			{
				std::unique_lock lock(mutex);
				if (!queueBuffer.full())
				{
					lock.unlock();
					writerState = Constants::WriterState::Active;
				}
				else
				{
					lock.unlock();
					writerState = Constants::WriterState::Discarded;
				}
			}
		};
	}
}

#endif
