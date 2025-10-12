// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef READERACQUIRER_H_2025_09_29_17_19_47
#define READERACQUIRER_H_2025_09_29_17_19_47
#include <condition_variable>
#include <shared_mutex>
#include <weave/buffer/Constants.h>
#include <weave/buffer/QueueBuffer.h>
#include <weave/buffer/Policy.h>

namespace weave
{
	namespace buffer
	{
		// Policies for a reader are concerned with the question "What's the strategy if the buffer is empty?". In this case, we always wait so the implementation is the same for all policies.
		template <Constants::BufferType type, Constants::PolicyType policy>
		class ReaderAcquirer; // Cannot be instantiated

		template <Constants::BufferType type>
		class ReaderAcquirer<type, Constants::PolicyType::Lossless>
		{
		public:
			using Policy = Policy<Constants::PolicyType::Lossless>;

			static void acquire(std::shared_mutex& mutex, std::condition_variable_any& conditionVariableReader, QueueBuffer<type>& queueBuffer, Constants::ReaderState& readerState) noexcept
			{
				std::shared_lock lock(mutex);
				conditionVariableReader.wait(lock, [&queueBuffer]()
				{
					return !queueBuffer.empty();
				});
				readerState = Constants::ReaderState::Active;
			}
		};

		template <Constants::BufferType type>
		class ReaderAcquirer<type, Constants::PolicyType::Realtime>
		{
		public:
			using Policy = Policy<Constants::PolicyType::Realtime>;

			static void acquire(std::shared_mutex& mutex, std::condition_variable_any& conditionVariableReader, QueueBuffer<type>& queueBuffer, Constants::ReaderState& readerState) noexcept
			{
				ReaderAcquirer<type, Constants::PolicyType::Lossless>::acquire();
			}
		};

		template <Constants::BufferType type>
		class ReaderAcquirer<type, Constants::PolicyType::Attempts>
		{
		public:
			using Policy = Policy<Constants::PolicyType::Realtime>;

			static void acquire(std::shared_mutex& mutex, std::condition_variable_any& conditionVariableReader, QueueBuffer<type>& queueBuffer, Constants::ReaderState& readerState) noexcept
			{
				ReaderAcquirer<type, Constants::PolicyType::Lossless>::acquire();
			}
		};

		template <Constants::BufferType type>
		class ReaderAcquirer<type, Constants::PolicyType::LiveStream>
		{
		public:
			using Policy = Policy<Constants::PolicyType::LiveStream>;

			static void acquire(std::shared_mutex& mutex, std::condition_variable_any& conditionVariableReader, QueueBuffer<type>& queueBuffer, Constants::ReaderState& readerState) noexcept
			{
				ReaderAcquirer<type, Constants::PolicyType::Lossless>::acquire();
			}
		};

		template <Constants::BufferType type>
		class ReaderAcquirer<type, Constants::PolicyType::Throttled>
		{
		public:
			using Policy = Policy<Constants::PolicyType::Throttled>;

			static void acquire(std::shared_mutex& mutex, std::condition_variable_any& conditionVariableReader, QueueBuffer<type>& queueBuffer, Constants::ReaderState& readerState) noexcept
			{
				ReaderAcquirer<type, Constants::PolicyType::Lossless>::acquire();
			}
		};
	}
}

#endif
