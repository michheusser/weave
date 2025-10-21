// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef SLOTTRAITSSPECIALIZATION_H_2025_10_21_16_13_06
#define SLOTTRAITSSPECIALIZATION_H_2025_10_21_16_13_06

#include <weave/user/SlotTraits.h>
#include "../buffer/Context.h"
#include "../slots/Image.h"
#include "../slots/InferenceTensor.h"

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

namespace weave
{
	namespace user
	{
		template<>
		struct SlotTraits<ClientRawImageEdge>
		{
			using ContextType = test::buffer::Context<test::buffer::constants::BufferType::Image>;
			using SlotDataType = test::slot::Image;
		};

		template<>
		struct SlotTraits<ClientImageSendEdge>
		{
			using ContextType = test::buffer::Context<test::buffer::constants::BufferType::Image>;
			using SlotDataType = test::slot::Image;
		};

		template<>
		struct SlotTraits<ClientImageReceiveEdge>
		{
			using ContextType = test::buffer::Context<test::buffer::constants::BufferType::Image>;
			using SlotDataType = test::slot::Image;
		};

		template<>
		struct SlotTraits<ClientDisplayImageEdge>
		{
			using ContextType = test::buffer::Context<test::buffer::constants::BufferType::Image>;
			using SlotDataType = test::slot::Image;
		};

		template<>
		struct SlotTraits<ServerImageReceiveEdge>
		{
			using ContextType = test::buffer::Context<test::buffer::constants::BufferType::Image>;
			using SlotDataType = test::slot::Image;
		};

		template<>
		struct SlotTraits<ServerInferenceInputTensorEdge>
		{
			using ContextType = test::buffer::Context<test::buffer::constants::BufferType::InferenceInputTensor>;
			using SlotDataType = test::slot::InferenceTensor;
		};

		template<>
		struct SlotTraits<ServerImageSendEdge>
		{
			using ContextType = test::buffer::Context<test::buffer::constants::BufferType::Image>;
			using SlotDataType = test::slot::Image;
		};
	}
}

#endif
