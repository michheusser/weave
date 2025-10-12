
#ifndef WRITER_H_2025_09_18_19_23_47
#define WRITER_H_2025_09_18_19_23_47

#include <weave/buffer/Traits.h>
#include <weave/buffer/Constants.h>
#include <weave/buffer/QueueBuffer.h>
#include <weave/buffer/WriterAcquirer.h>
#include <weave/error/Constants.h>
#include <weave/error/Result.h>

namespace weave
{
	namespace buffer
	{
		// TODO Expand in case it's not a SCSP but MCMP for completeness sake! Acquirers also change!
		template <Constants::BufferType type, Constants::PolicyType policy>
		class Writer
		{
		public:
			explicit Writer(std::shared_mutex& mutex, std::condition_variable_any& conditionVariableRead, std::condition_variable_any& conditionVariableWrite,
			                QueueBuffer<type>& queueBuffer) noexcept : _mutex(mutex), _conditionVariableRead(conditionVariableRead), _conditionVariableWrite(conditionVariableWrite), _queueBuffer(queueBuffer), _state(Constants::WriterState::Discarded)
			{
				WriterAcquirer<type, policy>::acquire(_mutex, _conditionVariableWrite, _queueBuffer, _state);
			}

			~Writer()
			{
				if (_state == Constants::WriterState::Active)
				{
					discard();
				}
			}

			bool active() const noexcept
			{
				return _state == Constants::WriterState::Active;
			}

			typename Traits<type>::StorageType& data(Error::Result* error = nullptr) noexcept
			{
				if (error)
				{
					*error = active()? Error::Result::success() : Error::Result{Error::Type::Processing, 0};
				}
				return _queueBuffer.newSlot();
			}

			void publish(uint32_t frameID) noexcept
			{
				_queueBuffer.push(frameID);
				_state = Constants::WriterState::Published;
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
			QueueBuffer<type>& _queueBuffer;
			// Writer's State
			Constants::WriterState _state;
		};
	}
}

#endif
