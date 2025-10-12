// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef TRAITS_H_2025_09_20_09_38_04
#define TRAITS_H_2025_09_20_09_38_04

#include <string_view>
#include "Constants.h"
#include <weave/buffer/Buffer.h>


namespace weave
{
	worker
	{
		namespace Worker
		{
			namespace Type
			{
				// Module
				template <Constants::Type module>
				struct Traits; // Prevents use of generic/non-specialized template

				template <>
				struct Traits<Constants::Type::Source>
				{
					static constexpr uint8_t numBuffers = 1;
				};

				template <>
				struct Traits<Constants::Type::Process>
				{
					static constexpr uint8_t numBuffers = 2;
				};

				template <>
				struct Traits<Constants::Type::Sink>
				{
					static constexpr uint8_t numBuffers = 1;
				};
			}

			namespace Module
			{
				// Module
				template <Constants::Module module>
				struct Traits; // Prevents use of generic/non-specialized template

				// TODO IMPORTANT: The architectural problem is that we don't have named buffers and wee need to, as well as named modules. Implementing these would solve
				//		the design flaw where neighbouring modules have to match buffers. Also I cannot use the same module in different parts with different parameters.
				//		Think of this. The idea in the end would be to be able to create a pipeline with a list of modules and buffers defined at compile time.
				template <>
				struct Traits<Constants::Module::Capturer>
				{
					static constexpr Constants::Type type = Constants::Type::Source;
					static constexpr uint8_t numBuffers = Type::Traits<type>::numBuffers;
					static constexpr std::array<buffer::Constants::BufferType, numBuffers> buffers = {buffer::Constants::BufferType::Image}; // TODO These three datastructures should be unified
					static constexpr std::array<buffer::Constants::PolicyType, numBuffers> bufferPolicies = {buffer::Constants::PolicyType::Lossless};
					static constexpr std::array<uint64_t, numBuffers> bufferSizes = {16};
					static constexpr std::string_view name = "Capturer";
				};

				template <>
				struct Traits<Constants::Module::Displayer>
				{
					static constexpr Constants::Type type = Constants::Type::Sink;
					static constexpr uint8_t numBuffers = Type::Traits<type>::numBuffers;
					static constexpr std::array<buffer::Constants::BufferType, numBuffers> buffers = {buffer::Constants::BufferType::Image};
					static constexpr std::array<buffer::Constants::PolicyType, numBuffers> bufferPolicies = {buffer::Constants::PolicyType::Lossless};
					static constexpr std::array<uint64_t, numBuffers> bufferSizes = {16};
					static constexpr std::string_view name = "Displayer";
				};

				template <>
				struct Traits<Constants::Module::ImageProcessor>
				{
					static constexpr Constants::Type type = Constants::Type::Process;
					static constexpr uint8_t numBuffers = Type::Traits<type>::numBuffers;
					static constexpr std::array<buffer::Constants::BufferType, numBuffers> buffers = {buffer::Constants::BufferType::Image, buffer::Constants::BufferType::Image};
					static constexpr std::array<buffer::Constants::PolicyType, numBuffers> bufferPolicies = {buffer::Constants::PolicyType::Lossless, buffer::Constants::PolicyType::Lossless};
					static constexpr std::array<uint64_t, numBuffers> bufferSizes = {16, 16};
					static constexpr std::string_view name = "Image Processor";
				};

				template <>
				struct Traits<Constants::Module::ImageToTensor>
				{
					static constexpr Constants::Type type = Constants::Type::Process;
					static constexpr uint8_t numBuffers = Type::Traits<type>::numBuffers;
					static constexpr std::array<buffer::Constants::BufferType, numBuffers> buffers = {buffer::Constants::BufferType::Image, buffer::Constants::BufferType::Inference};
					static constexpr std::array<buffer::Constants::PolicyType, numBuffers> bufferPolicies = {buffer::Constants::PolicyType::Lossless, buffer::Constants::PolicyType::Lossless};
					static constexpr std::array<uint64_t, numBuffers> bufferSizes = {16, 16};
					static constexpr std::string_view name = "Image To Tensor";
				};

				template <>
				struct Traits<Constants::Module::Inference>
				{
					static constexpr Constants::Type type = Constants::Type::Process;
					static constexpr uint8_t numBuffers = Type::Traits<type>::numBuffers;
					static constexpr std::array<buffer::Constants::BufferType, numBuffers> buffers = {buffer::Constants::BufferType::Inference, buffer::Constants::BufferType::Image};
					static constexpr std::array<buffer::Constants::PolicyType, numBuffers> bufferPolicies = {buffer::Constants::PolicyType::Lossless, buffer::Constants::PolicyType::Lossless};
					static constexpr std::array<uint64_t, numBuffers> bufferSizes = {16, 16};
					static constexpr std::string_view name = "Inference";
				};

				template <>
				struct Traits<Constants::Module::ClientReceiver>
				{
					static constexpr Constants::Type type = Constants::Type::Source;
					static constexpr uint8_t numBuffers = Type::Traits<type>::numBuffers;
					static constexpr std::array<buffer::Constants::BufferType, numBuffers> buffers = {buffer::Constants::BufferType::Image};
					static constexpr std::array<buffer::Constants::PolicyType, numBuffers> bufferPolicies = {buffer::Constants::PolicyType::Lossless};
					static constexpr std::array<uint64_t, numBuffers> bufferSizes = {16};
					static constexpr std::string_view name = "Client Receiver";
				};

				template <>
				struct Traits<Constants::Module::ClientSender>
				{
					static constexpr Constants::Type type = Constants::Type::Sink;
					static constexpr uint8_t numBuffers = Type::Traits<type>::numBuffers;
					static constexpr std::array<buffer::Constants::BufferType, numBuffers> buffers = {buffer::Constants::BufferType::Image};
					static constexpr std::array<buffer::Constants::PolicyType, numBuffers> bufferPolicies = {buffer::Constants::PolicyType::Lossless};
					static constexpr std::array<uint64_t, numBuffers> bufferSizes = {16};
					static constexpr std::string_view name = "Client Sender";
				};

				template <>
				struct Traits<Constants::Module::ServerReceiver>
				{
					static constexpr Constants::Type type = Constants::Type::Source;
					static constexpr uint8_t numBuffers = Type::Traits<type>::numBuffers;
					static constexpr std::array<buffer::Constants::BufferType, numBuffers> buffers = {buffer::Constants::BufferType::Image};
					static constexpr std::array<buffer::Constants::PolicyType, numBuffers> bufferPolicies = {buffer::Constants::PolicyType::Lossless};
					static constexpr std::array<uint64_t, numBuffers> bufferSizes = {16};
					static constexpr std::string_view name = "Server Receiver";
				};

				template <>
				struct Traits<Constants::Module::ServerSender>
				{
					static constexpr Constants::Type type = Constants::Type::Sink;
					static constexpr uint8_t numBuffers = Type::Traits<type>::numBuffers;
					static constexpr std::array<buffer::Constants::BufferType, numBuffers> buffers = {buffer::Constants::BufferType::Image};
					static constexpr std::array<buffer::Constants::PolicyType, numBuffers> bufferPolicies = {buffer::Constants::PolicyType::Lossless};
					static constexpr std::array<uint64_t, numBuffers> bufferSizes = {16};
					static constexpr std::string_view name = "Server Sender";
				};
			}
		}
	}
}

#endif
