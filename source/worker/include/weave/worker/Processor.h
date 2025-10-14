// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef PROCESSOR_H_2025_09_20_15_34_46
#define PROCESSOR_H_2025_09_20_15_34_46

namespace weave
{
	namespace Worker
	{
		template<Constants::Module module>
		class Processor; // Never instanciated as generic

		template<>
		class Processor<Constants::Module::Capturer>
		{
		public:
			static constexpr Constants::Module moduleType = Constants::Module::Capturer;
			explicit Processor(const Module::Context<moduleType>& context);
			void initialize();
			Error::Result process(buffer::Buffer<Module::Traits<moduleType>::buffers[0], Module::Traits<moduleType>::bufferPolicies[0]>::StorageType& outputBuffer,
			                      uint32_t* frameID = nullptr) noexcept;

		private:
			IO::Capturer _capturer;
		};

		template<>
		class Processor<Constants::Module::Displayer>
		{
		public:
			static constexpr Constants::Module moduleType = Constants::Module::Displayer;
			explicit Processor(const Module::Context<moduleType>& context);
			void initialize();
			Error::Result process(const buffer::Buffer<Module::Traits<moduleType>::buffers[0], Module::Traits<moduleType>::bufferPolicies[0]>::StorageType& inputBuffer, uint32_t frameID) noexcept;

		private:
			IO::Displayer _displayer;
		};

		template<>
		class Processor<Constants::Module::ImageProcessor>
		{
		public:
			static constexpr Constants::Module moduleType = Constants::Module::ImageProcessor;
			explicit Processor(const Module::Context<moduleType>& context);
			void initialize();
			Error::Result process(const buffer::Buffer<Module::Traits<moduleType>::buffers[0], Module::Traits<moduleType>::bufferPolicies[0]>::StorageType& inputBuffer,
			                      buffer::Buffer<Module::Traits<moduleType>::buffers[1], Module::Traits<moduleType>::bufferPolicies[1]>::StorageType& outputBuffer) noexcept;

		private:
			Preprocessor _imageProcessor;
		};

		template<>
		class Processor<Constants::Module::ImageToTensor>
		{
		public:
			static constexpr Constants::Module moduleType = Constants::Module::ImageToTensor;
			explicit Processor(const Module::Context<moduleType>& context);
			void initialize();
			Error::Result process(const buffer::Buffer<Module::Traits<moduleType>::buffers[0], Module::Traits<moduleType>::bufferPolicies[0]>::StorageType& inputBuffer,
			                      buffer::Buffer<Module::Traits<moduleType>::buffers[1], Module::Traits<moduleType>::bufferPolicies[1]>::StorageType& outputBuffer) noexcept;

		private:
			InferenceInputProcessor _inferenceInputProcessor;
		};

		template<>
		class Processor<Constants::Module::Inference>
		{
		public:
			static constexpr Constants::Module moduleType = Constants::Module::Inference;
			explicit Processor(const Module::Context<moduleType>& context);
			void initialize();
			Error::Result process(const buffer::Buffer<Module::Traits<moduleType>::buffers[0], Module::Traits<moduleType>::bufferPolicies[0]>::StorageType& inputBuffer,
			                      buffer::Buffer<Module::Traits<moduleType>::buffers[1], Module::Traits<moduleType>::bufferPolicies[1]>::StorageType& outputBuffer) noexcept;

		private:
			InferenceModel _inferenceModel;
		};

		template<>
		class Processor<Constants::Module::ClientReceiver>
		{
		public:
			static constexpr Constants::Module moduleType = Constants::Module::ClientReceiver;
			explicit Processor(const Module::Context<moduleType>& context);
			void initialize();
			Error::Result process(buffer::Buffer<Module::Traits<moduleType>::buffers[0], Module::Traits<moduleType>::bufferPolicies[0]>::StorageType& outputBuffer,
			                      uint32_t* frameID = nullptr) noexcept;

		private:
			const std::shared_ptr<Streaming::NetworkClient> _networkClient;
			const Decoder _decoder;
			const RingBufferConfiguration _bufferConfiguration;
			std::vector<uint8_t> _buffer;
		};

		template<>
		class Processor<Constants::Module::ClientSender>
		{
		public:
			static constexpr Constants::Module moduleType = Constants::Module::ClientSender;
			explicit Processor(const Module::Context<moduleType>& context);
			void initialize();
			Error::Result process(const buffer::Buffer<Module::Traits<moduleType>::buffers[0], Module::Traits<moduleType>::bufferPolicies[0]>::StorageType& inputBuffer, uint32_t frameID) noexcept;

		private:
			const std::shared_ptr<Streaming::NetworkClient> _networkClient;
			const Encoder _encoder;
			const RingBufferConfiguration _bufferConfiguration;
			std::vector<uint8_t> _buffer;
		};

		template<>
		class Processor<Constants::Module::ServerReceiver>
		{
		public:
			static constexpr Constants::Module moduleType = Constants::Module::ServerReceiver;
			explicit Processor(const Module::Context<moduleType>& context);
			void initialize();
			Error::Result process(buffer::Buffer<Module::Traits<moduleType>::buffers[0], Module::Traits<moduleType>::bufferPolicies[0]>::StorageType& outputBuffer,
			                      uint32_t* frameID = nullptr) noexcept;

		private:
			const std::shared_ptr<Streaming::NetworkServer> _networkServer;
			const Decoder _decoder;
			const RingBufferConfiguration _bufferConfiguration;
			std::vector<uint8_t> _buffer;
		};

		template<>
		class Processor<Constants::Module::ServerSender>
		{
		public:
			static constexpr Constants::Module moduleType = Constants::Module::ServerSender;
			explicit Processor(const Module::Context<moduleType>& context);
			void initialize();
			Error::Result process(const buffer::Buffer<Module::Traits<moduleType>::buffers[0], Module::Traits<moduleType>::bufferPolicies[0]>::StorageType& inputBuffer, uint32_t frameID) noexcept;

		private:
			const std::shared_ptr<Streaming::NetworkServer> _networkServer;
			const Encoder _encoder;
			const RingBufferConfiguration _bufferConfiguration;
			std::vector<uint8_t> _buffer;
		};
	}
}

#endif
