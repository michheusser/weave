// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser
#include "SlotSpecialization.h"

namespace weave
{
	namespace user
	{
		Slot<ImageSlot>::Slot()
		{}

		void Slot<ImageSlot>::initialize(const typename Slot<ImageSlot>::ContextType& context)
		{

			_data.create(context.configuration.frameHeight, context.configuration.frameWidth, context.configuration.frameType);
		}

		Slot<ImageSlot>::StorageType& Slot<ImageSlot>::data()
		{
			return _data;
		}
	}
}
