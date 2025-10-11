// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef BUFFERCONTEXT_H_2025_09_24_11_39_17
#define BUFFERCONTEXT_H_2025_09_24_11_39_17

#include "Constants.h"
#include "Configuration.h"

namespace test
{
	namespace buffer
	{
		template<constants::BufferType bufferType>
		struct Context; // Prevents use of generic/non-specialized template

		template<>
		struct Context<constants::BufferType::Image>
		{
			ImageBufferConfiguration configuration;
		};
	}
}

#endif
