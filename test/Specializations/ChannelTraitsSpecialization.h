// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef CHANNELTRAITSSPECIALIZATION_H_2025_10_14_11_23_31
#define CHANNELTRAITSSPECIALIZATION_H_2025_10_14_11_23_31

#include <weave/user/ChannelTraits.h>
#include "RingBufferTraitsSpecialization.h"
#include "../buffer/Constants.h"
#include "../buffer/Context.h"

struct DefaultImageChannel
{};

namespace weave
{
	namespace user
	{
		// TODO Consider making traits classes part of the actual class header (Channel.h)
		template<>
		struct ChannelTraits<DefaultImageChannel>
		{
			using RingBufferTag = DefaultImageRingBuffer;
			using ContextType = test::buffer::Context<test::buffer::constants::BufferType::Image>; // TODO Harmonize Configuration/Context inheritance
		};
	}
}
#endif
