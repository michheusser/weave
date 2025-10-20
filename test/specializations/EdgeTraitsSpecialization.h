// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef EDGETRAITSSPECIALIZATION_H_2025_10_14_11_21_27
#define EDGETRAITSSPECIALIZATION_H_2025_10_14_11_21_27

#include <weave/user/EdgeTraits.h>
#include "ChannelTraitsSpecialization.h"
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

namespace weave
{
	namespace user
	{
		// TODO Consider making traits classes part of the actual class header (Channel.h)
		template<>
		struct EdgeTraits<ClientRawImageEdge>
		{
			using ChannelTag = DefaultImageChannel;
			using ContextType = ChannelTraits<ChannelTag>::ContextType;
		};

		template<>
		struct EdgeTraits<ClientImageSendEdge>
		{
			using ChannelTag = DefaultImageChannel;
			using ContextType = ChannelTraits<ChannelTag>::ContextType;
		};

		template<>
		struct EdgeTraits<ClientImageReceiveEdge>
		{
			using ChannelTag = DefaultImageChannel;
			using ContextType = ChannelTraits<ChannelTag>::ContextType;
		};

		template<>
		struct EdgeTraits<ClientDisplayImageEdge>
		{
			using ChannelTag = DefaultImageChannel;
			using ContextType = ChannelTraits<ChannelTag>::ContextType;
		};

		template<>
		struct EdgeTraits<ServerImageReceiveEdge>
		{
			using ChannelTag = DefaultImageChannel;
			using ContextType = ChannelTraits<ChannelTag>::ContextType;
		};

		template<>
		struct EdgeTraits<ServerInferenceInputTensorEdge>
		{
			using ChannelTag = DefaultInferenceInputTensorChannel;
			using ContextType = ChannelTraits<ChannelTag>::ContextType;
		};

		template<>
		struct EdgeTraits<ServerImageSendEdge>
		{
			using ChannelTag = DefaultImageChannel;
			using ContextType = ChannelTraits<ChannelTag>::ContextType;
		};
	}
}

#endif
