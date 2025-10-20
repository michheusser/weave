// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef EDGETRAITSSPECIALIZATION_H_2025_10_14_11_21_27
#define EDGETRAITSSPECIALIZATION_H_2025_10_14_11_21_27

#include <weave/user/EdgeTraits.h>
#include "ChannelTraitsSpecialization.h"
#include "../buffer/Context.h"

struct FirstRawImageEdge
{};

struct FirstNormalizedImageEdge
{};

struct FirstInferenceInputTensorEdge
{};

struct FirstInferenceOutputImageEdge
{};

struct SecondNormalizedImageEdge
{};

namespace weave
{
	namespace user
	{
		// TODO Consider making traits classes part of the actual class header (Channel.h)
		template<>
		struct EdgeTraits<FirstRawImageEdge>
		{
			using ChannelTag = DefaultImageChannel;
			using ContextType = ChannelTraits<ChannelTag>::ContextType;
		};

		template<>
		struct EdgeTraits<FirstNormalizedImageEdge>
		{
			using ChannelTag = DefaultImageChannel;
			using ContextType = ChannelTraits<ChannelTag>::ContextType;
		};

		template<>
		struct EdgeTraits<FirstInferenceInputTensorEdge>
		{
			using ChannelTag = DefaultInferenceInputTensorChannel;
			using ContextType = ChannelTraits<ChannelTag>::ContextType;
		};

		template<>
		struct EdgeTraits<FirstInferenceOutputImageEdge>
		{
			using ChannelTag = DefaultImageChannel;
			using ContextType = ChannelTraits<ChannelTag>::ContextType;
		};

		template<>
		struct EdgeTraits<SecondNormalizedImageEdge>
		{
			using ChannelTag = DefaultImageChannel;
			using ContextType = ChannelTraits<ChannelTag>::ContextType;
		};
	}
}

#endif
