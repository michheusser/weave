// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser
#include "SlotSpecialization.h"
#include <weave/logging/Macros.h>
#include <weave/error/Exception.h>

namespace weave
{
	namespace user
	{
		Slot<ImageSlot>::Slot()
		{}

		void Slot<ImageSlot>::initialize(const typename Slot<ImageSlot>::ContextType& context)
		{
			try
			{
				_data.create(context.configuration.frameHeight, context.configuration.frameWidth, context.configuration.frameType);
			}
			catch (cv::Exception& exception)
			{
				LOG_ERROR(exception.what());
				throw error::Exception("Failed.");
			}
		}

		Slot<ImageSlot>::StorageType& Slot<ImageSlot>::data()
		{
			return _data;
		}
	}
}
