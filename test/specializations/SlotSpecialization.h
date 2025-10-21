// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef SLOTSPECIALIZATION_H_2025_10_13_18_14_08
#define SLOTSPECIALIZATION_H_2025_10_13_18_14_08

#include <onnxruntime_cxx_api.h>
#include <opencv2/core/mat.hpp>
#include <weave/user/Slot.h>
#include "../buffer/Context.h"

struct ClientRawImageEdge
{};

struct ClientImageSendEdge
{};

struct ClientImageReceiveEdge
{};

struct ClientDisplayImageEdge
{};

struct ServerImageReceiveEdge
{};

struct ServerInferenceInputTensorEdge
{};

struct ServerImageSendEdge
{};

// Defines a type of slot to be reused by multiple channels
namespace weave
{
	namespace user
	{
		template<>
		class Slot<ClientRawImageEdge>
		{
		public:
			// TODO This has to be made generic so it can be implemented by user. This contains already the Traits, so functional parts could be made generic and the user part a traits (TODO LATER)
			using Type = ClientRawImageEdge;
			using StorageType = cv::Mat;
			using ContextType = test::buffer::Context<test::buffer::constants::BufferType::Image>;
			Slot(); // TODO Make explicit
			void initialize(const ContextType& context);
			StorageType& data();

		private:
			StorageType _data;
		};

		template<>
		class Slot<ClientImageSendEdge>
		{
		public:
			// TODO This has to be made generic so it can be implemented by user. This contains already the Traits, so functional parts could be made generic and the user part a traits (TODO LATER)
			using Type = ClientImageSendEdge;
			using StorageType = cv::Mat;
			using ContextType = test::buffer::Context<test::buffer::constants::BufferType::Image>;
			Slot(); // TODO Make explicit
			void initialize(const ContextType& context);
			StorageType& data();

		private:
			StorageType _data;
		};

		template<>
		class Slot<ClientImageReceiveEdge>
		{
		public:
			// TODO This has to be made generic so it can be implemented by user. This contains already the Traits, so functional parts could be made generic and the user part a traits (TODO LATER)
			using Type = ClientImageReceiveEdge;
			using StorageType = cv::Mat;
			using ContextType = test::buffer::Context<test::buffer::constants::BufferType::Image>;
			Slot(); // TODO Make explicit
			void initialize(const ContextType& context);
			StorageType& data();

		private:
			StorageType _data;
		};

		template<>
		class Slot<ClientDisplayImageEdge>
		{
		public:
			// TODO This has to be made generic so it can be implemented by user. This contains already the Traits, so functional parts could be made generic and the user part a traits (TODO LATER)
			using Type = ClientDisplayImageEdge;
			using StorageType = cv::Mat;
			using ContextType = test::buffer::Context<test::buffer::constants::BufferType::Image>;
			Slot(); // TODO Make explicit
			void initialize(const ContextType& context);
			StorageType& data();

		private:
			StorageType _data;
		};

		template<>
		class Slot<ServerImageReceiveEdge>
		{
		public:
			// TODO This has to be made generic so it can be implemented by user. This contains already the Traits, so functional parts could be made generic and the user part a traits (TODO LATER)
			using Type = ServerImageReceiveEdge;
			using StorageType = cv::Mat;
			using ContextType = test::buffer::Context<test::buffer::constants::BufferType::Image>;
			Slot(); // TODO Make explicit
			void initialize(const ContextType& context);
			StorageType& data();

		private:
			StorageType _data;
		};

		template<>
		class Slot<ServerInferenceInputTensorEdge>
		{
		public:
			using Type = ServerInferenceInputTensorEdge;
			using StorageType = Ort::Value;
			using ContextType = test::buffer::Context<test::buffer::constants::BufferType::InferenceInputTensor>;
			Slot(); // TODO Make explicit
			void initialize(const ContextType& context);
			StorageType& data();

		private:
			StorageType _data;
			std::vector<float> _internalData;
		};

		template<>
		class Slot<ServerImageSendEdge>
		{
		public:
			// TODO This has to be made generic so it can be implemented by user. This contains already the Traits, so functional parts could be made generic and the user part a traits (TODO LATER)
			using Type = ServerImageSendEdge;
			using StorageType = cv::Mat;
			using ContextType = test::buffer::Context<test::buffer::constants::BufferType::Image>;
			Slot(); // TODO Make explicit
			void initialize(const ContextType& context);
			StorageType& data();

		private:
			StorageType _data;
		};
	}
}
#endif
