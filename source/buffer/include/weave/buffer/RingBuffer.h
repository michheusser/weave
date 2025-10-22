// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef RINGBUFFER_H_2025_10_01_11_47_31
#define RINGBUFFER_H_2025_10_01_11_47_31

#include <cstdint>
#include <cassert>
#include <array>

#include <weave/buffer/Slot.h>
#include <weave/error/Exception.h>
#include <weave/logging/Macros.h>

namespace weave
{
	namespace buffer
	{
		template<typename RingBufferTag, size_t numSlots>
		class RingBuffer
		{
		public:
			using SlotTag = RingBufferTag;
			using ContextType = typename Slot<SlotTag>::ContextType;

			// We do not throw errors, but rather have contracts (assert), since we have the invariant that reader and writer will
			// only exist when the RingBuffer is not empty and not full respectively. Thus, we do not return error codes.
			RingBuffer() : _usedCount(0), _head(0), _tail(0)
			{}

			void initialize(const ContextType& context)
			{
				try
				{
					for (Slot<SlotTag>& curSlot: _slotArray)
					{
						curSlot.initialize(context);
					}
				}
				catch (error::Exception& exception)
				{
					LOG_ERROR(exception.what());
					throw error::Exception("Failed");
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

			typename Slot<SlotTag>::StorageType& newSlot() noexcept
			{
				assert(!full());
				return _slotArray[_head].data();
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

			typename Slot<SlotTag>::StorageType& front() noexcept
			{
				assert(!empty());
				return _slotArray[_tail].data();
			}

			const typename Slot<SlotTag>::StorageType& front() const noexcept
			{
				assert(!empty());
				return _slotArray[_tail].data();
			}

			uint32_t frontFrame() const noexcept
			{
				assert(!empty());
				return _frameIDs[_tail];
			}

		private:
			// TODO In general a lot of improvement potential here by using a custom allocator or to make sure that all the underlying data
			//  is put together and not spread across the heap (e.g. if InternalData is a string or a vector)

			static constexpr uint64_t NUM_SLOTS = numSlots;
			std::array<Slot<SlotTag>, NUM_SLOTS> _slotArray;
			std::array<uint32_t, NUM_SLOTS> _frameIDs;
			uint64_t _usedCount;
			uint64_t _head; // Writes/pushes happen here
			uint64_t _tail; // Reads/pops happen here
		};
	}
}

#endif
