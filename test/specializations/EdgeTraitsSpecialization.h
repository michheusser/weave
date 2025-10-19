// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef EDGETRAITSSPECIALIZATION_H_2025_10_14_11_21_27
#define EDGETRAITSSPECIALIZATION_H_2025_10_14_11_21_27

#include <weave/user/EdgeTraits.h>
#include "ChannelTraitsSpecialization.h"
#include "../buffer/Context.h"

struct FirstImageEdge
{};

struct SecondImageEdge
{};

namespace weave
{
	namespace user
	{
		// TODO Consider making traits classes part of the actual class header (Channel.h)
		template<>
		struct EdgeTraits<FirstImageEdge>
		{
			using ChannelTag = DefaultImageChannel;
			using ContextType = ChannelTraits<ChannelTag>::ContextType;
		};

		template<>
		struct EdgeTraits<SecondImageEdge>
		{
			using ChannelTag = DefaultImageChannel;
			using ContextType = ChannelTraits<ChannelTag>::ContextType;
		};

	}
}

#endif