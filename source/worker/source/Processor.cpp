// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser
#include <weave/worker/Processor.h>
#include <weave/buffer/Context.h>

namespace weave
{
	worker
	{
		namespace Worker
		{
			Processor<Constants::Module::Capturer>::Processor(const Module::Context<Constants::Module::Capturer>& context) : _capturer(context.capturerConfiguration)
			{}

			void Processor<Constants::Module::Capturer>::initialize()
			{}

			Error::Result Processor<Constants::Module::Capturer>::process(buffer::Buffer<Module::Traits<moduleType>::buffers[0], Module::Traits<moduleType>::bufferPolicies[0]>::StorageType& outputBuffer, uint32_t* frameID) noexcept
			{
				Error::Result result = _capturer.getFrame(outputBuffer, frameID);
				if (!result.ok())
				{
					LOG_ERROR("Capturer: Error in processing.");
					return {Error::Type::Processing, 0};
				}
				return Error::Result::success();
			}

			Processor<Constants::Module::Displayer>::Processor(const Module::Context<Constants::Module::Displayer>& context) : _displayer(context.displayerConfiguration)
			{}

			void Processor<Constants::Module::Displayer>::initialize()
			{}

			Error::Result Processor<Constants::Module::Displayer>::process(const buffer::Buffer<Module::Traits<moduleType>::buffers[0], Module::Traits<moduleType>::bufferPolicies[0]>::StorageType& inputBuffer, uint32_t frameID) noexcept
			{
				Error::Result result = _displayer.showFrame(inputBuffer, frameID);
				if (!result.ok())
				{
					LOG_ERROR("Displayer: Error in processing.");
					return {Error::Type::Processing, 0};
				}
				return Error::Result::success();
			}

			Processor<Constants::Module::ImageProcessor>::Processor(const Module::Context<Constants::Module::ImageProcessor>& context) : _imageProcessor(context.processorConfiguration)
			{}

			void Processor<Constants::Module::ImageProcessor>::initialize()
			{}

			Error::Result Processor<Constants::Module::ImageProcessor>::process(const buffer::Buffer<Module::Traits<moduleType>::buffers[0], Module::Traits<moduleType>::bufferPolicies[0]>::StorageType& inputBuffer, buffer::Buffer<Module::Traits<moduleType>::buffers[1], Module::Traits<moduleType>::bufferPolicies[1]>::StorageType& outputBuffer) noexcept
			{
				Error::Result result = _imageProcessor.preprocessFrame(inputBuffer, outputBuffer);
				if (!result.ok())
				{
					LOG_ERROR("Image Processor: Error in processing.");
					return {Error::Type::Processing, 0};
				}
				return Error::Result::success();
			}

			Processor<Constants::Module::ImageToTensor>::Processor(const Module::Context<Constants::Module::ImageToTensor>& context) : _inferenceInputProcessor(
				context.inferenceInputProcessorConfiguration)
			{}

			void Processor<Constants::Module::ImageToTensor>::initialize()
			{}

			Error::Result Processor<Constants::Module::ImageToTensor>::process(const buffer::Buffer<Module::Traits<moduleType>::buffers[0], Module::Traits<moduleType>::bufferPolicies[0]>::StorageType& inputBuffer, buffer::Buffer<Module::Traits<moduleType>::buffers[1], Module::Traits<moduleType>::bufferPolicies[1]>::StorageType& outputBuffer) noexcept
			{
				Error::Result result = _inferenceInputProcessor.preprocessFrame(inputBuffer, outputBuffer);
				if (!result.ok())
				{
					LOG_ERROR("Image To Tensor: Error in processing.");
					return {Error::Type::Processing, 0};
				}
				return Error::Result::success();
			}

			Processor<Constants::Module::Inference>::Processor(const Module::Context<Constants::Module::Inference>& context) : _inferenceModel(context.inferenceModelConfiguration)
			{}

			void Processor<Constants::Module::Inference>::initialize()
			{
				_inferenceModel.initialize();
			}

			Error::Result Processor<Constants::Module::Inference>::process(const buffer::Buffer<Module::Traits<moduleType>::buffers[0], Module::Traits<moduleType>::bufferPolicies[0]>::StorageType& inputBuffer, buffer::Buffer<Module::Traits<moduleType>::buffers[1], Module::Traits<moduleType>::bufferPolicies[1]>::StorageType& outputBuffer) noexcept
			{
				Error::Result result = _inferenceModel.executeInference(inputBuffer, outputBuffer);
				if (!result.ok())
				{
					LOG_ERROR("Inference: Error in processing.");
					return {Error::Type::Processing, 0};
				}
				return Error::Result::success();
			}

			Processor<Constants::Module::ClientReceiver>::Processor(const Module::Context<Constants::Module::ClientReceiver>& context) : _networkClient(context.networkClient),
				_decoder(context.decoderConfiguration), _bufferConfiguration(context.bufferConfiguration)
			{}

			void Processor<Constants::Module::ClientReceiver>::initialize()
			{
				_buffer.resize(_bufferConfiguration.bufferSize);
			}

			Error::Result Processor<Constants::Module::ClientReceiver>::process(buffer::Buffer<Module::Traits<moduleType>::buffers[0], Module::Traits<moduleType>::bufferPolicies[0]>::StorageType& outputBuffer, uint32_t* frameID) noexcept
			{
				Error::Result resultReceive = _networkClient->receive(_buffer);
				if (!resultReceive.ok())
				{
					LOG_ERROR("Client Receiver: Error in processing.");
					return {Error::Type::Processing, 0};
				}

				Error::Result resultDecode = _decoder.decodeBufferIntoFrame(_buffer, outputBuffer, frameID);
				if (!resultDecode.ok())
				{
					LOG_ERROR("Client Receiver: Error in processing.");
					return {Error::Type::Processing, 0};
				}
				return Error::Result::success();
			}

			Processor<Constants::Module::ClientSender>::Processor(const Module::Context<Constants::Module::ClientSender>& context) : _networkClient(context.networkClient),
				_encoder(context.encoderConfiguration), _bufferConfiguration(context.bufferConfiguration)
			{}

			void Processor<Constants::Module::ClientSender>::initialize()
			{
				_buffer.resize(_bufferConfiguration.bufferSize);
			}

			Error::Result Processor<Constants::Module::ClientSender>::process(const buffer::Buffer<Module::Traits<moduleType>::buffers[0], Module::Traits<moduleType>::bufferPolicies[0]>::StorageType& inputBuffer, uint32_t frameID) noexcept
			{
				Error::Result resultEncode = _encoder.encodeFrameIntoBuffer(inputBuffer, _buffer, frameID);
				if (!resultEncode.ok())
				{
					LOG_ERROR("Client Sender: Error in processing.");
					return {Error::Type::Processing, 0};
				}

				Error::Result resultSend = _networkClient->send(_buffer);

				if (!resultSend.ok())
				{
					LOG_ERROR("Client Sender: Error in processing.");
					return {Error::Type::NetworkSend, 0};
				}
				return Error::Result::success();
			}

			Processor<Constants::Module::ServerReceiver>::Processor(const Module::Context<Constants::Module::ServerReceiver>& context) : _networkServer(context.networkServer),
				_decoder(context.decoderConfiguration), _bufferConfiguration(context.bufferConfiguration)
			{}

			void Processor<Constants::Module::ServerReceiver>::initialize()
			{
				_buffer.resize(_bufferConfiguration.bufferSize);
			}

			Error::Result Processor<Constants::Module::ServerReceiver>::process(buffer::Buffer<Module::Traits<moduleType>::buffers[0], Module::Traits<moduleType>::bufferPolicies[0]>::StorageType& outputBuffer, uint32_t* frameID) noexcept
			{
				Error::Result resultReceive = _networkServer->receive(_buffer);
				if (!resultReceive.ok())
				{
					LOG_ERROR("Server Receiver: Error in processing.");
					return {Error::Type::Processing, 0};
				}

				Error::Result resultDecode = _decoder.decodeBufferIntoFrame(_buffer, outputBuffer, frameID);
				if (!resultDecode.ok())
				{
					LOG_ERROR("Server Receiver: Error in processing.");
					return {Error::Type::Processing, 0};
				}
				return Error::Result::success();
			}

			Processor<Constants::Module::ServerSender>::Processor(const Module::Context<Constants::Module::ServerSender>& context) : _networkServer(context.networkServer),
				_encoder(context.encoderConfiguration), _bufferConfiguration(context.bufferConfiguration)
			{}

			void Processor<Constants::Module::ServerSender>::initialize()
			{
				_buffer.resize(_bufferConfiguration.bufferSize);
			}

			Error::Result Processor<Constants::Module::ServerSender>::process(const buffer::Buffer<Module::Traits<moduleType>::buffers[0], Module::Traits<moduleType>::bufferPolicies[0]>::StorageType& inputBuffer, uint32_t frameID) noexcept
			{
				Error::Result resultEncode = _encoder.encodeFrameIntoBuffer(inputBuffer, _buffer, frameID);
				if (!resultEncode.ok())
				{
					LOG_ERROR("Server Sender: Error in processing.");
					return {Error::Type::Processing, 0};
				}

				Error::Result resultSend = _networkServer->send(_buffer);

				if (!resultSend.ok())
				{
					LOG_ERROR("Server Sender: Error in processing.");
					return {Error::Type::NetworkSend, 0};
				}
				return Error::Result::success();
			}
		}
	}
}
