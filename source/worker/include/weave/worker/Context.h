// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef INITIALIZATIONARTIFACT_H_2025_09_24_11_39_17
#define INITIALIZATIONARTIFACT_H_2025_09_24_11_39_17

namespace weave
{
	namespace Worker
	{
		namespace Module
		{
			template<Constants::Module module>
			struct Context;

			// Module
			template<Constants::Module module>
			struct Context; // Prevents use of generic/non-specialized template

			template<>
			struct Context<Constants::Module::Capturer>
			{
				IO::CapturerConfiguration capturerConfiguration;
			};

			template<>
			struct Context<Constants::Module::Displayer>
			{
				IO::DisplayerConfiguration displayerConfiguration;
			};

			template<>
			struct Context<Constants::Module::ImageProcessor>
			{
				worker::PreprocessorConfiguration processorConfiguration;
			};

			template<>
			struct Context<Constants::Module::ImageToTensor>
			{
				worker::InferenceInputProcessorConfiguration inferenceInputProcessorConfiguration;
			};

			template<>
			struct Context<Constants::Module::Inference>
			{
				worker::InferenceModelConfiguration inferenceModelConfiguration;
			};

			template<>
			struct Context<Constants::Module::ClientReceiver>
			{
				RingBufferConfiguration bufferConfiguration;
				worker::DecoderConfiguration decoderConfiguration;
				std::shared_ptr<Streaming::NetworkClient> networkClient;
			};

			template<>
			struct Context<Constants::Module::ClientSender>
			{
				RingBufferConfiguration bufferConfiguration;
				worker::EncoderConfiguration encoderConfiguration;
				std::shared_ptr<Streaming::NetworkClient> networkClient;
			};

			template<>
			struct Context<Constants::Module::ServerReceiver>
			{
				RingBufferConfiguration bufferConfiguration;
				worker::DecoderConfiguration decoderConfiguration;
				std::shared_ptr<Streaming::NetworkServer> networkServer;
			};

			template<>
			struct Context<Constants::Module::ServerSender>
			{
				RingBufferConfiguration bufferConfiguration;
				worker::EncoderConfiguration encoderConfiguration;
				std::shared_ptr<Streaming::NetworkServer> networkServer;
			};
		}
	}
}

#endif
