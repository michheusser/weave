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

#include <weave/error/Exception.h>
#include <weave/logging/Macros.h>

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
			/*Channel(Channel&& other) : _ringBuffer(std::move(other._ringBuffer)) // TODO WRONG THIS IS CREATING ISSUES WITH MUTEXES!
			{}*/
			// Channels contain synchronization artifacts and thus not copyable/movable. Even though the compiler deletes per default, we delete here explicitly to be more expressive and clear.
			Channel(const Channel&) = delete;
			Channel& operator=(const Channel&) = delete;
			Channel(Channel&&) = delete;
			Channel& operator=(Channel&&) = delete;

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
			RingBuffer<RingBufferTag> _ringBuffer;
		};
	}
}

#endif
