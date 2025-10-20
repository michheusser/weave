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
		Slot<DefaultImageSlot>::Slot()
		{}

		void Slot<DefaultImageSlot>::initialize(const ContextType& context)
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

		Slot<DefaultImageSlot>::StorageType& Slot<DefaultImageSlot>::data()
		{
			return _data;
		}

		Slot<DefaultInferenceInputTensorSlot>::Slot() : _data(nullptr)
		{}

		void Slot<DefaultInferenceInputTensorSlot>::initialize(const ContextType& context)
		{
			try
			{
				size_t inputTensorSize = 1 * 3 * context.configuration.height * context.configuration.width;
				std::vector<int64_t> inputShape = {1, 3, context.configuration.height, context.configuration.width};
				_internalData = std::vector<float>(inputTensorSize, 0.0f);
				Ort::MemoryInfo memoryInfo = Ort::MemoryInfo::CreateCpu(context.configuration.tensorAllocatorType, context.configuration.tensorMemoryType);
				_data = Ort::Value::CreateTensor<float>(memoryInfo, _internalData.data(), inputTensorSize, inputShape.data(), inputShape.size());
			}
			catch (cv::Exception& exception)
			{
				LOG_ERROR(exception.what());
				throw error::Exception("Failed.");
			}
		}

		Slot<DefaultInferenceInputTensorSlot>::StorageType& Slot<DefaultInferenceInputTensorSlot>::data()
		{
			return _data;
		}
	}
}
