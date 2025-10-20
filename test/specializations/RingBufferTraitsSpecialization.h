// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef RINGBUFFERTRAITSSPECIALIZATION_H_2025_10_14_12_43_11
#define RINGBUFFERTRAITSSPECIALIZATION_H_2025_10_14_12_43_11

#include <weave/user/RingBufferTraits.h>
#include "SlotSpecialization.h"
#include "../buffer/Constants.h"
#include "../buffer/Context.h"

struct DefaultImageRingBuffer
{};

struct DefaultInferenceInputTensorRingBuffer
{};

namespace weave
{
	namespace user
	{
		template<>
		struct RingBufferTraits<DefaultImageRingBuffer>
		{
			using SlotTag = DefaultImageSlot;
			using ContextType = Slot<SlotTag>::ContextType;
		};

		template<>
		struct RingBufferTraits<DefaultInferenceInputTensorRingBuffer>
		{
			using SlotTag = DefaultInferenceInputTensorSlot;
			using ContextType = Slot<SlotTag>::ContextType;
		};
	}
}

#endif
