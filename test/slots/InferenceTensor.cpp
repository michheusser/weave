// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser
#include "InferenceTensor.h"
#include <weave/logging/Macros.h>
#include <weave/error/Exception.h>

namespace test
{
	namespace slot
	{
		InferenceTensor::InferenceTensor() : _data(nullptr)
		{}

		void InferenceTensor::initialize(const ContextType& context)
		{
			try
			{
				size_t inputTensorSize = 1 * 3 * context.configuration.height * context.configuration.width;
				std::vector<int64_t> inputShape = {1, 3, context.configuration.height, context.configuration.width};
				_internalData = std::vector<float>(inputTensorSize, 0.0f);
				Ort::MemoryInfo memoryInfo = Ort::MemoryInfo::CreateCpu(context.configuration.tensorAllocatorType, context.configuration.tensorMemoryType);
				_data = Ort::Value::CreateTensor<float>(memoryInfo, _internalData.data(), inputTensorSize, inputShape.data(), inputShape.size());
			}
			catch (Ort::Exception& exception)
			{
				LOG_ERROR(exception.what());
				throw weave::error::Exception("Failed.");
			}
		}

		InferenceTensor::StorageType& InferenceTensor::data()
		{
			return _data;
		}
	} // slot
} // test
