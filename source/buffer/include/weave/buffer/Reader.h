#ifndef READER_H_2025_09_18_19_11_56
#define READER_H_2025_09_18_19_11_56

#include <condition_variable>
#include <shared_mutex>
#include <weave/error/Result.h>
#include <weave/buffer/Constants.h>
#include <weave/buffer/ReaderAcquirer.h>
#include <weave/user/Slot.h>

namespace weave
{
	namespace buffer
	{
		template <typename ChannelTag, constants::PolicyType policy, size_t numSlots>
		class Reader
		{
		public:
			using RingBufferTag = ChannelTag;
			using SlotTag = ChannelTag;
			using StorageType = typename user::Slot<SlotTag>::StorageType;

			explicit Reader(std::shared_mutex& mutex, std::condition_variable_any& conditionVariableRead, std::condition_variable_any& conditionVariableWrite,
			                RingBuffer<RingBufferTag, numSlots>& queueBuffer) noexcept : _mutex(mutex), _conditionVariableRead(conditionVariableRead), _conditionVariableWrite(conditionVariableWrite),
			                                                           _queueBuffer(queueBuffer), _state(constants::ReaderState::Released)
			{
				// In the future, we might want more sophisticated reading (e.g. reading without marking as read, etc.) so here we might change to std::shared_lock/std::condition_variable_any::notify_all()
				ReaderAcquirer<ChannelTag, policy, numSlots>::acquire(_mutex, _conditionVariableRead, _queueBuffer, _state);
			}

			// Copying is not allowed, because in a SCSP context, I cannot have two active readers or two active writers.
			Reader(const Reader& reader) = delete;

			Reader& operator=(const Reader& reader) = delete;

			// Readers are movable, because they handle with references or values (otherwise mutexes/condition variables are not movable/copyable)
			Reader(Reader&& reader) : _mutex(reader._mutex), _conditionVariableRead(reader._conditionVariableRead), _conditionVariableWrite(reader._conditionVariableWrite),
						  _queueBuffer(reader._queueBuffer), _state(reader._state)
			{
				reader._state = constants::ReaderState::Released;
			}

			Reader& operator=(Reader&& reader) = delete;

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

			const StorageType& data(error::Result* error = nullptr) noexcept
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
			RingBuffer<RingBufferTag, numSlots>& _queueBuffer;
			// Reader's State
			constants::ReaderState _state;
		};
	}
}

#endif
