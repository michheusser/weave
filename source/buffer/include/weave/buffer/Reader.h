#ifndef READER_H_2025_09_18_19_11_56
#define READER_H_2025_09_18_19_11_56

#include <weave/buffer/Traits.h>
#include <weave/buffer/Constants.h>
#include <weave/buffer/Policy.h>
#include <weave/buffer/QueueBuffer.h>
#include <weave/buffer/ReaderAcquirer.h>
#include <weave/error/Result.h>

namespace weave
{
	namespace buffer
	{
		template <Constants::BufferType type, Constants::PolicyType policy>
		class Reader
		{
		public:
			explicit Reader(std::shared_mutex& mutex, std::condition_variable_any& conditionVariableRead, std::condition_variable_any& conditionVariableWrite,
			                QueueBuffer<type>& queueBuffer) noexcept : _mutex(mutex), _conditionVariableRead(conditionVariableRead), _conditionVariableWrite(conditionVariableWrite),
			                                                           _queueBuffer(queueBuffer), _state(Constants::ReaderState::Released)
			{
				// In the future, we might want more sophisticated reading (e.g. reading without marking as read, etc.) so here we might change to std::shared_lock/std::condition_variable_any::notify_all()
				ReaderAcquirer<type, policy>::acquire(_mutex, _conditionVariableRead, _queueBuffer, _state);
			}

			~Reader()
			{
				if (_state == Constants::ReaderState::Active)
				{
					discard();
				}
			}

			bool active() const noexcept
			{
				return _state == Constants::ReaderState::Active;
			}

			const typename Traits<type>::StorageType& data(Error::Result* error = nullptr) noexcept
			{
				if (error)
				{
					*error = active()? Error::Result::success() : Error::Result{Error::Type::Processing, 0};
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
				_state = Constants::ReaderState::Released;
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
			QueueBuffer<type>& _queueBuffer;
			// Reader's State
			Constants::ReaderState _state;
		};
	}
}

#endif
