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

#include <weave/user/ChannelTraits.h>

namespace weave
{
	namespace buffer
	{
		// Policies for a reader are concerned with the question "What's the strategy if the buffer is empty?". In this case, we always wait so the implementation is the same for all policies.
		template <typename ChannelTag, constants::PolicyType policy>
		class ReaderAcquirer; // Cannot be instantiated

		template <typename ChannelTag>
		class ReaderAcquirer<ChannelTag, constants::PolicyType::Lossless>
		{
		public:
			using Policy = Policy<constants::PolicyType::Lossless>;
			using RingBufferTag = user::ChannelTraits<ChannelTag>::RingBufferTag;

			static void acquire(std::shared_mutex& mutex, std::condition_variable_any& conditionVariableReader, RingBuffer<RingBufferTag>& queueBuffer, constants::ReaderState& readerState) noexcept
			{
				std::shared_lock lock(mutex);
				conditionVariableReader.wait(lock, [&queueBuffer]()
				{
					return !queueBuffer.empty();
				});
				readerState = constants::ReaderState::Active;
			}
		};

		template <typename ChannelTag>
		class ReaderAcquirer<ChannelTag, constants::PolicyType::Realtime>
		{
		public:
			using Policy = Policy<constants::PolicyType::Realtime>;
			using RingBufferTag = user::ChannelTraits<ChannelTag>::RingBufferTag;

			static void acquire(std::shared_mutex& mutex, std::condition_variable_any& conditionVariableReader, RingBuffer<RingBufferTag>& queueBuffer, constants::ReaderState& readerState) noexcept
			{
				ReaderAcquirer<ChannelTag, constants::PolicyType::Lossless>::acquire();
			}
		};

		template <typename ChannelTag>
		class ReaderAcquirer<ChannelTag, constants::PolicyType::Attempts>
		{
		public:
			using Policy = Policy<constants::PolicyType::Realtime>;
			using RingBufferTag = user::ChannelTraits<ChannelTag>::RingBufferTag;

			static void acquire(std::shared_mutex& mutex, std::condition_variable_any& conditionVariableReader, RingBuffer<RingBufferTag>& queueBuffer, constants::ReaderState& readerState) noexcept
			{
				ReaderAcquirer<ChannelTag, constants::PolicyType::Lossless>::acquire();
			}
		};

		template <typename ChannelTag>
		class ReaderAcquirer<ChannelTag, constants::PolicyType::LiveStream>
		{
		public:
			using Policy = Policy<constants::PolicyType::LiveStream>;
			using RingBufferTag = user::ChannelTraits<ChannelTag>::RingBufferTag;

			static void acquire(std::shared_mutex& mutex, std::condition_variable_any& conditionVariableReader, RingBuffer<RingBufferTag>& queueBuffer, constants::ReaderState& readerState) noexcept
			{
				ReaderAcquirer<ChannelTag, constants::PolicyType::Lossless>::acquire();
			}
		};

		template <typename ChannelTag>
		class ReaderAcquirer<ChannelTag, constants::PolicyType::Throttled>
		{
		public:
			using Policy = Policy<constants::PolicyType::Throttled>;
			using RingBufferTag = user::ChannelTraits<ChannelTag>::RingBufferTag;

			static void acquire(std::shared_mutex& mutex, std::condition_variable_any& conditionVariableReader, RingBuffer<RingBufferTag>& queueBuffer, constants::ReaderState& readerState) noexcept
			{
				ReaderAcquirer<ChannelTag, constants::PolicyType::Lossless>::acquire();
			}
		};
	}
}

#endif
