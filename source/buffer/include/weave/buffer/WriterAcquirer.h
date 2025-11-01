// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef WRITERACQUIRER_H_2025_09_29_17_20_01
#define WRITERACQUIRER_H_2025_09_29_17_20_01

#include <condition_variable>
#include <shared_mutex>
#include <thread>

#include <weave/buffer/Policy.h>
#include <weave/buffer/Constants.h>
#include <weave/buffer/RingBuffer.h>

namespace weave
{
	namespace buffer
	{
		// Policies for a writer are concerned with the question "What's the strategy if the buffer is full?"
		template <typename ChannelTag, typename SlotDataType, size_t numSlots, buffer::constants::PolicyType policyType>
		class WriterAcquirer; // Cannot be instantiated

		// TODO Approach: Implement single specialized policies, and then try to make generic using the parameters of the policy to avoid specialization (if possible)
		template <typename ChannelTag, typename SlotDataType, size_t numSlots>
		class WriterAcquirer<ChannelTag, SlotDataType, numSlots, constants::PolicyType::Lossless>
		{
			using Policy = Policy<constants::PolicyType::Lossless>;
			using RingBufferTag = ChannelTag;

		public:
			static void acquire(std::shared_mutex& mutex, std::condition_variable_any& conditionVariableWrite, RingBuffer<RingBufferTag, SlotDataType, numSlots>& ringBuffer, constants::WriterState& writerState) noexcept
			{
				std::unique_lock lock(mutex);
				conditionVariableWrite.wait(lock, [&ringBuffer]()
				{
					return !ringBuffer.full();
				});
				lock.unlock();
				writerState = constants::WriterState::Active;
			}
		};

		template <typename ChannelTag, typename SlotDataType, size_t numSlots>
		class WriterAcquirer<ChannelTag, SlotDataType, numSlots, constants::PolicyType::Realtime>
		{
		public:
			using Policy = Policy<constants::PolicyType::Realtime>;
			using RingBufferTag = ChannelTag;

			static void acquire(std::shared_mutex& mutex, std::condition_variable_any& conditionVariableWrite, RingBuffer<RingBufferTag, SlotDataType, numSlots>& ringBuffer, constants::WriterState& writerState) noexcept
			{
				std::unique_lock lock(mutex);
				if (ringBuffer.full())
				{
					ringBuffer.pop();
				}
				lock.unlock();
				writerState = constants::WriterState::Active;
			}
		};

		template <typename ChannelTag, typename SlotDataType, size_t numSlots>
		class WriterAcquirer<ChannelTag, SlotDataType, numSlots, constants::PolicyType::Attempts>
		{
		public:
			using Policy = Policy<constants::PolicyType::Attempts>;
			using RingBufferTag = ChannelTag;

			static void acquire(std::shared_mutex& mutex, std::condition_variable_any& conditionVariableWrite, RingBuffer<RingBufferTag, SlotDataType, numSlots>& ringBuffer, constants::WriterState& writerState) noexcept
			{
				for (int i = 1; i <= Policy::numAttempts; ++i)
				{
					std::unique_lock lock(mutex);
					if (!ringBuffer.full())
					{
						break;
					}
					if (i == Policy::numAttempts)
					{
						ringBuffer.pop(); // we discard oldest frame without reading
					}
					else
					{
						lock.unlock();
						std::this_thread::sleep_for(std::chrono::milliseconds(Policy::timeoutMS));
					}
				}
				writerState = constants::WriterState::Active;
			}
		};


		template <typename ChannelTag, typename SlotDataType, size_t numSlots>
		class WriterAcquirer<ChannelTag, SlotDataType, numSlots, constants::PolicyType::LiveStream>
		{
		public:
			using Policy = Policy<constants::PolicyType::LiveStream>;
			using RingBufferTag = ChannelTag;

			static void acquire(std::shared_mutex& mutex, std::condition_variable_any& conditionVariableWrite, RingBuffer<RingBufferTag, SlotDataType, numSlots>& ringBuffer, constants::WriterState& writerState) noexcept
			{
				std::unique_lock lock(mutex);
				conditionVariableWrite.wait_for(std::chrono::milliseconds(Policy::timeoutMS),[](){return true;});
				if (ringBuffer.full())
				{
					ringBuffer.pop();
				}
				lock.unlock();
				writerState = constants::WriterState::Active;
			}
		};


		template <typename ChannelTag, typename SlotDataType, size_t numSlots>
		class WriterAcquirer<ChannelTag, SlotDataType, numSlots, constants::PolicyType::Throttled>
		{
		public:
			using Policy = Policy<constants::PolicyType::Throttled>;
			using RingBufferTag = ChannelTag;

			static void acquire(std::shared_mutex& mutex, std::condition_variable_any& conditionVariableWrite, RingBuffer<RingBufferTag, SlotDataType, numSlots>& ringBuffer, constants::WriterState& writerState) noexcept
			{
				std::unique_lock lock(mutex);
				if (!ringBuffer.full())
				{
					lock.unlock();
					writerState = constants::WriterState::Active;
				}
				else
				{
					lock.unlock();
					writerState = constants::WriterState::Discarded;
				}
			}
		};
	}
}

#endif
