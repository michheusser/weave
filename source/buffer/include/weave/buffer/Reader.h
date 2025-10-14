#ifndef READER_H_2025_09_18_19_11_56
#define READER_H_2025_09_18_19_11_56

#include <condition_variable>
#include <shared_mutex>
#include <weave/error/Result.h>
#include <weave/buffer/Constants.h>
#include <weave/buffer/ReaderAcquirer.h>
#include <weave/user/Slot.h>
#include <weave/user/RingBufferTraits.h>
#include <weave/user/ChannelTraits.h>

namespace weave
{
	namespace buffer
	{
		template <typename ChannelTag, constants::PolicyType policy>
		class Reader
		{
		public:
			using RingBufferTag = user::ChannelTraits<ChannelTag>::RingBufferTag;
			using SlotTag = user::RingBufferTraits<RingBufferTag>::SlotTag;

			explicit Reader(std::shared_mutex& mutex, std::condition_variable_any& conditionVariableRead, std::condition_variable_any& conditionVariableWrite,
			                RingBuffer<RingBufferTag>& queueBuffer) noexcept : _mutex(mutex), _conditionVariableRead(conditionVariableRead), _conditionVariableWrite(conditionVariableWrite),
			                                                           _queueBuffer(queueBuffer), _state(constants::ReaderState::Released)
			{
				// In the future, we might want more sophisticated reading (e.g. reading without marking as read, etc.) so here we might change to std::shared_lock/std::condition_variable_any::notify_all()
				ReaderAcquirer<ChannelTag, policy>::acquire(_mutex, _conditionVariableRead, _queueBuffer, _state);
			}

			~Reader()
			{
				if (_state == constants::ReaderState::Active)
				{
					discard();
				}
			}

			bool active() const noexcept
			{
				return _state == constants::ReaderState::Active;
			}

			const typename user::Slot<SlotTag>::StorageType& data(error::Result* error = nullptr) noexcept
			{
				if (error)
				{
					*error = active()? error::Result::success() : error::Result{error::Type::Processing, 0};
				}
				return _queueBuffer.front();
			}

			uint64_t getFrame() const noexcept
			{
				return _queueBuffer.frontFrame();
			}

			void release() noexcept
			{
				_queueBuffer.pop();
				_state = constants::ReaderState::Released;
				_conditionVariableWrite.notify_one(); // Only one writer can go and write
			}

			void discard() const noexcept
			{
				_conditionVariableWrite.notify_one();
			}

		private:
			std::shared_mutex& _mutex;
			std::condition_variable_any& _conditionVariableRead;
			std::condition_variable_any& _conditionVariableWrite;
			RingBuffer<RingBufferTag>& _queueBuffer;
			// Reader's State
			constants::ReaderState _state;
		};
	}
}

#endif
