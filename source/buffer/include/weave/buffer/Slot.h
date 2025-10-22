// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef SLOT_H_2025_09_30_16_59_17
#define SLOT_H_2025_09_30_16_59_17

#include <weave/user/SlotTraits.h>
#include <weave/error/Result.h>
#include <weave/logging/Macros.h>

namespace weave
{
	namespace buffer
	{
		template<typename SlotTag>
		class Slot
		{
		public:
			using SlotDataType = typename user::SlotTraits<SlotTag>::SlotDataType;
			using ContextType = typename SlotDataType::ContextType;
			using StorageType = typename SlotDataType::StorageType;
			explicit Slot()
			{}
			void initialize(const ContextType& context)
			{
				_slot.initialize(context);
			}
			StorageType& data() noexcept
			{
				return _slot.data();
			}

		private:
			SlotDataType _slot;
		};
	}
}

#endif
