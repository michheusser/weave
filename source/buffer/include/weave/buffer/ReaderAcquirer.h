// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef READERACQUIRER_H_2025_09_29_17_19_47
#define READERACQUIRER_H_2025_09_29_17_19_47

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
		// Policies for a reader are concerned with the question "What's the strategy if the buffer is empty?". In this case, we always wait so the implementation is the same for all policies.
		template <typename ChannelTag, typename SlotDataType, size_t numSlots, buffer::constants::PolicyType policyType>
		class ReaderAcquirer; // Cannot be instantiated

		template <typename ChannelTag, typename SlotDataType, size_t numSlots>
		class ReaderAcquirer<ChannelTag, SlotDataType, numSlots, constants::PolicyType::Lossless>
		{
		public:
			using Policy = Policy<constants::PolicyType::Lossless>;
			using RingBufferTag = ChannelTag;

			static void acquire(std::shared_mutex& mutex, std::condition_variable_any& conditionVariableReader, RingBuffer<RingBufferTag, SlotDataType, numSlots>& queueBuffer, constants::ReaderState& readerState) noexcept
			{
				std::shared_lock lock(mutex);
				conditionVariableReader.wait(lock, [&queueBuffer]()
				{
					return !queueBuffer.empty();
				});
				readerState = constants::ReaderState::Active;
			}
		};

		template <typename ChannelTag, typename SlotDataType, size_t numSlots>
		class ReaderAcquirer<ChannelTag, SlotDataType, numSlots, constants::PolicyType::Realtime>
		{
		public:
			using Policy = Policy<constants::PolicyType::Realtime>;
			using RingBufferTag = ChannelTag;

			static void acquire(std::shared_mutex& mutex, std::condition_variable_any& conditionVariableReader, RingBuffer<RingBufferTag, SlotDataType, numSlots>& queueBuffer, constants::ReaderState& readerState) noexcept
			{
				ReaderAcquirer<ChannelTag, SlotDataType, numSlots, constants::PolicyType::Lossless>::acquire();
			}
		};

		template <typename ChannelTag, typename SlotDataType, size_t numSlots>
		class ReaderAcquirer<ChannelTag, SlotDataType, numSlots, constants::PolicyType::Attempts>
		{
		public:
			using Policy = Policy<constants::PolicyType::Realtime>;
			using RingBufferTag = ChannelTag;

			static void acquire(std::shared_mutex& mutex, std::condition_variable_any& conditionVariableReader, RingBuffer<RingBufferTag, SlotDataType, numSlots>& queueBuffer, constants::ReaderState& readerState) noexcept
			{
				ReaderAcquirer<ChannelTag, SlotDataType, numSlots, constants::PolicyType::Lossless>::acquire();
			}
		};

		template <typename ChannelTag, typename SlotDataType, size_t numSlots>
		class ReaderAcquirer<ChannelTag, SlotDataType, numSlots, constants::PolicyType::LiveStream>
		{
		public:
			using Policy = Policy<constants::PolicyType::LiveStream>;
			using RingBufferTag = ChannelTag;

			static void acquire(std::shared_mutex& mutex, std::condition_variable_any& conditionVariableReader, RingBuffer<RingBufferTag, SlotDataType, numSlots>& queueBuffer, constants::ReaderState& readerState) noexcept
			{
				ReaderAcquirer<ChannelTag, SlotDataType, numSlots, constants::PolicyType::Lossless>::acquire();
			}
		};

		template <typename ChannelTag, typename SlotDataType, size_t numSlots>
		class ReaderAcquirer<ChannelTag, SlotDataType, numSlots, constants::PolicyType::Throttled>
		{
		public:
			using Policy = Policy<constants::PolicyType::Throttled>;
			using RingBufferTag = ChannelTag;

			static void acquire(std::shared_mutex& mutex, std::condition_variable_any& conditionVariableReader, RingBuffer<RingBufferTag, SlotDataType, numSlots>& queueBuffer, constants::ReaderState& readerState) noexcept
			{
				ReaderAcquirer<ChannelTag, SlotDataType, numSlots, constants::PolicyType::Lossless>::acquire();
			}
		};
	}
}

#endif
