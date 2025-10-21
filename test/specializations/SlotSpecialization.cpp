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
		Slot<ClientRawImageEdge>::Slot()
		{}

		void Slot<ClientRawImageEdge>::initialize(const ContextType& context)
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

		Slot<ClientRawImageEdge>::StorageType& Slot<ClientRawImageEdge>::data()
		{
			return _data;
		}

		//
		Slot<ClientImageSendEdge>::Slot()
		{}

		void Slot<ClientImageSendEdge>::initialize(const ContextType& context)
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

		Slot<ClientImageSendEdge>::StorageType& Slot<ClientImageSendEdge>::data()
		{
			return _data;
		}

		//
		Slot<ClientImageReceiveEdge>::Slot()
		{}

		void Slot<ClientImageReceiveEdge>::initialize(const ContextType& context)
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

		Slot<ClientImageReceiveEdge>::StorageType& Slot<ClientImageReceiveEdge>::data()
		{
			return _data;
		}

		//
		Slot<ClientDisplayImageEdge>::Slot()
		{}

		void Slot<ClientDisplayImageEdge>::initialize(const ContextType& context)
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

		Slot<ClientDisplayImageEdge>::StorageType& Slot<ClientDisplayImageEdge>::data()
		{
			return _data;
		}

		//
		Slot<ServerImageReceiveEdge>::Slot()
		{}

		void Slot<ServerImageReceiveEdge>::initialize(const ContextType& context)
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

		Slot<ServerImageReceiveEdge>::StorageType& Slot<ServerImageReceiveEdge>::data()
		{
			return _data;
		}

		//
		Slot<ServerInferenceInputTensorEdge>::Slot() : _data(nullptr)
		{}

		void Slot<ServerInferenceInputTensorEdge>::initialize(const ContextType& context)
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

		Slot<ServerInferenceInputTensorEdge>::StorageType& Slot<ServerInferenceInputTensorEdge>::data()
		{
			return _data;
		}

		//
		Slot<ServerImageSendEdge>::Slot()
		{}

		void Slot<ServerImageSendEdge>::initialize(const ContextType& context)
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

		Slot<ServerImageSendEdge>::StorageType& Slot<ServerImageSendEdge>::data()
		{
			return _data;
		}
	}
}
