// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef CHANNEL_H_2025_09_17_11_14_09
#define CHANNEL_H_2025_09_17_11_14_09

#include <shared_mutex>
#include <condition_variable>

#include <weave/buffer/RingBuffer.h>
#include <weave/buffer/Reader.h>
#include <weave/buffer/Writer.h>
#include <weave/buffer/Constants.h>

#include <weave/error/Exception.h>
#include <weave/logging/Macros.h>

namespace weave
{
	namespace buffer
	{
		template<typename ChannelTag, typename SlotDataType, size_t numSlots, constants::PolicyType policyType>
		class Channel
		{
		public:
			using Tag = ChannelTag;
			using SlotData = SlotDataType;
			static constexpr size_t slots = numSlots;
			static constexpr constants::PolicyType policy = policyType;
			using RingBufferTag = ChannelTag;
			using ContextType = typename RingBuffer<RingBufferTag, SlotDataType, numSlots>::ContextType;
			explicit Channel(const ContextType& context)
			{
				_initialize(context);
			}

			// Channels contain synchronization artifacts and thus not copyable/movable. Even though the compiler deletes per default, we delete here explicitly to be more expressive and clear.
			Channel(const Channel&) = delete;
			Channel& operator=(const Channel&) = delete;
			Channel(Channel&&) = delete;
			Channel& operator=(Channel&&) = delete;

			Reader<ChannelTag, SlotDataType, slots, policy> reader() noexcept
			{
				return Reader<ChannelTag, SlotDataType, slots, policy>(_mutex, _conditionVariableRead, _conditionVariableWrite, _ringBuffer);
			}

			Writer<ChannelTag, SlotDataType, slots, policy> writer() noexcept
			{
				return Writer<ChannelTag, SlotDataType, slots, policy>(_mutex, _conditionVariableRead, _conditionVariableWrite, _ringBuffer);
			}

		private:
			void _initialize(const ContextType& context)
			{
				try
				{
					_ringBuffer.initialize(context);
				}
				catch (error::Exception& exception)
				{
					LOG_ERROR(exception.what());
					throw error::Exception("Failed");
				}
			}

		private:
			mutable std::shared_mutex _mutex;
			mutable std::condition_variable_any _conditionVariableRead;
			mutable std::condition_variable_any _conditionVariableWrite;
			RingBuffer<RingBufferTag, SlotDataType, numSlots> _ringBuffer;
		};
	}
}

#endif
