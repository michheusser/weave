// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef SLOT_H_2025_09_30_16_59_17
#define SLOT_H_2025_09_30_16_59_17


namespace weave
{
	namespace buffer
	{
		template<typename SlotTag, typename SlotDataType> // TODO Probably don't need a SlotTag anymore here
		class Slot
		{
		public:
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
