// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef QUEUEBUFFER_H_2025_10_01_11_47_31
#define QUEUEBUFFER_H_2025_10_01_11_47_31

#include <weave/buffer/Constants.h>
#include <weave/buffer/Traits.h>
#include <weave/buffer/InternalData.h>

namespace weave
{
	namespace buffer
	{
		template <Constants::BufferType type>
		class QueueBuffer
		{
		public:
			// We do not throw errors, but rather have contracts (assert), since we have the invariant that reader and writer will
			// only exist when the QueueBuffer is not empty and not full respectively. Thus, we do not return error codes.
			QueueBuffer()
			{}

			void initialize(const typename Traits<type>::ConfigurationType& configuration)
			{
				for (InternalData<type>& curData : _dataArray)
				{
					curData.initialize(configuration);
				}
			}

			uint64_t used() const noexcept
			{
				return _usedCount;
			}

			uint64_t remaining() const noexcept
			{
				return NUM_SLOTS - used();
			}

			bool empty() const noexcept
			{
				return used() == 0;
			}

			bool full() const noexcept
			{
				return used() == NUM_SLOTS;
			}

			typename InternalData<type>::StorageType& newSlot() noexcept
			{
				assert(!full());
				return _dataArray[_head].data();
			}

			void pop() noexcept
			{
				assert(!empty());
				_tail = (_tail + 1) % NUM_SLOTS;
				--_usedCount;
			}

			void push(uint32_t frameID) noexcept
			{
				assert(!full());
				_frameIDs[_head] = frameID;
				_head = (_head + 1) % NUM_SLOTS;
				++_usedCount;
			}

			typename InternalData<type>::StorageType& front() noexcept
			{
				assert(!empty());
				return _dataArray[_tail].data();
			}

			const typename InternalData<type>::StorageType& front() const noexcept
			{
				assert(!empty());
				return _dataArray[_tail].data();
			}

			uint32_t frontFrame() const noexcept
			{
				assert(!empty());
				return _frameIDs[_tail];
			}
		private:
			// TODO In general a lot of improvement potential here by using a custom allocator or to make sure that all the underlying data
			//  is put together and not spread across the heap (e.g. if InternalData is a string or a vector)


			static constexpr uint64_t NUM_SLOTS = 16; // TODO Needs to come from the traits.
			std::array<InternalData<type>, NUM_SLOTS> _dataArray;
			std::array<uint32_t, NUM_SLOTS> _frameIDs;
			uint64_t _usedCount;
			uint64_t _head; // Writes/pushes happen here
			uint64_t _tail; // Reads/pops happen here
		};
	}
}

#endif
