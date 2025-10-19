#ifndef WRITER_H_2025_09_18_19_23_47
#define WRITER_H_2025_09_18_19_23_47

#include <condition_variable>
#include <shared_mutex>
#include <weave/error/Result.h>
#include <weave/buffer/Constants.h>
#include <weave/buffer/WriterAcquirer.h>
#include <weave/user/Slot.h>
#include <weave/user/RingBufferTraits.h>
#include <weave/user/ChannelTraits.h>

namespace weave
{
	namespace buffer
	{
		// TODO Expand in case it's not a SCSP but MCMP for completeness sake! Acquirers also change!
		template<typename ChannelTag, constants::PolicyType policy, size_t numSlots>
		class Writer
		{
		public:
			using RingBufferTag = typename user::ChannelTraits<ChannelTag>::RingBufferTag;
			using SlotTag = typename user::RingBufferTraits<RingBufferTag>::SlotTag;
			using StorageType = typename user::Slot<SlotTag>::StorageType;

			explicit Writer(std::shared_mutex& mutex, std::condition_variable_any& conditionVariableRead, std::condition_variable_any& conditionVariableWrite,
			                RingBuffer<RingBufferTag, numSlots>& queueBuffer) noexcept : _mutex(mutex), _conditionVariableRead(conditionVariableRead), _conditionVariableWrite(conditionVariableWrite),
			                                                                   _queueBuffer(queueBuffer), _state(constants::WriterState::Discarded)
			{
				WriterAcquirer<ChannelTag, policy, numSlots>::acquire(_mutex, _conditionVariableWrite, _queueBuffer, _state);
			}

			// Copying is not allowed, because in a SCSP context, I cannot have two active readers or two active writers.
			Writer(const Writer& writer) = delete;

			Writer& operator=(const Writer& writer) = delete;

			// Readers are movable, because they handle with references or values (otherwise mutexes/condition variables are not movable/copyable)
			Writer(Writer&& writer) : _mutex(writer._mutex), _conditionVariableRead(writer._conditionVariableRead), _conditionVariableWrite(writer._conditionVariableWrite),
			                          _queueBuffer(writer._queueBuffer), _state(writer._state)
			{
				writer._state = constants::WriterState::Discarded;
			}

			Writer& operator=(Writer&& writer) = delete;

			~Writer()
			{
				if (_state == constants::WriterState::Active)
				{
					discard();
				}
			}

			bool active() const noexcept
			{
				return _state == constants::WriterState::Active;
			}

			StorageType& data(error::Result* error = nullptr) noexcept
			{
				if (error)
				{
					*error = active() ? error::Result::success() : error::Result{error::Type::Processing, 0};
				}
				return _queueBuffer.newSlot();
			}

			void publish(uint32_t frameID) noexcept
			{
				_queueBuffer.push(frameID);
				_state = constants::WriterState::Published;
				_conditionVariableRead.notify_one(); // Readers right now change the state (change buffer to empty, so only one reader at a time)
			}

			void discard() noexcept // For error control and explicit abandonment of a certain write
			{
				_conditionVariableRead.notify_one(); // idem.
			}

		private:
			std::shared_mutex& _mutex;
			std::condition_variable_any& _conditionVariableRead;
			std::condition_variable_any& _conditionVariableWrite;
			RingBuffer<RingBufferTag, numSlots>& _queueBuffer;
			// Writer's State
			constants::WriterState _state;
		};
	}
}

#endif
