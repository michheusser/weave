// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser
#include <weave/buffer/InternalData.h>

namespace weave
{
	namespace buffer
	{
		InternalData<Constants::BufferType::Image>::InternalData()
		{}

		void InternalData<Constants::BufferType::Image>::initialize(const typename Traits<Constants::BufferType::Image>::ConfigurationType& configuration)
		{
			_data.create(configuration.frameHeight, configuration.frameWidth, configuration.frameType);
		}

		InternalData<Constants::BufferType::Image>::StorageType& InternalData<Constants::BufferType::Image>::data()
		{
			return _data;
		}

		InternalData<Constants::BufferType::Inference>::InternalData() : _data(nullptr)
		{}

		void InternalData<Constants::BufferType::Inference>::initialize(const typename Traits<Constants::BufferType::Inference>::ConfigurationType& configuration)
		{
			size_t inputTensorSize = 1 * 3 * configuration.height * configuration.width;
			std::vector<int64_t> inputShape = {1, 3, configuration.height, configuration.width};
			_internalData = std::vector<float>(inputTensorSize, 0.0f);
			Ort::MemoryInfo memoryInfo = Ort::MemoryInfo::CreateCpu(configuration.tensorAllocatorType, configuration.tensorMemoryType);
			_data = Ort::Value::CreateTensor<float>(memoryInfo, _internalData.data(), inputTensorSize, inputShape.data(), inputShape.size());
		}

		InternalData<Constants::BufferType::Inference>::StorageType& InternalData<Constants::BufferType::Inference>::data()
		{
			return _data;
		}
	}
}
