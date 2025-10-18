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
#include <weave/user/ChannelTraits.h>

namespace weave
{
	namespace buffer
	{
		template<typename ChannelTag, constants::PolicyType policy>
		class Channel
		{
		public:
			using Tag = ChannelTag;
			static constexpr constants::PolicyType policyType = policy;
			using RingBufferTag = typename user::ChannelTraits<ChannelTag>::RingBufferTag;
			explicit Channel(const typename user::ChannelTraits<ChannelTag>::ContextType& context)
			{
				_initialize(context);
			}

			// Custom move, only copies ringbuffer, otherwise no default move constructor! (mutexes/condition variables can't be move)
			Channel(Channel&& other) : _ringBuffer(std::move(other._ringBuffer))
			{}

			Reader<ChannelTag, policy> reader() noexcept
			{
				return Reader<ChannelTag, policy>(_mutex, _conditionVariableRead, _conditionVariableWrite, _ringBuffer);
			}

			Writer<ChannelTag, policy> writer() noexcept
			{
				return Writer<ChannelTag, policy>(_mutex, _conditionVariableRead, _conditionVariableWrite, _ringBuffer);
			}

		private:
			void _initialize(const typename user::ChannelTraits<ChannelTag>::ContextType& context)
			{
				_ringBuffer.initialize(context);
			}

		private:
			mutable std::shared_mutex _mutex;
			mutable std::condition_variable_any _conditionVariableRead;
			mutable std::condition_variable_any _conditionVariableWrite;
			RingBuffer<RingBufferTag> _ringBuffer;
		};
	}
}

#endif
