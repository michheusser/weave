// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef MODULECONTEXT_H_2025_09_24_11_39_17
#define MODULECONTEXT_H_2025_09_24_11_39_17

#include "Constants.h"
#include "Configuration.h"

namespace test
{
	namespace module
	{
		template<constants::ModuleType moduleType>
		struct Context;

		template<>
		struct Context<constants::ModuleType::ImageCapturer>
		{
			CapturerConfiguration configuration;
		};

		template<>
		struct Context<constants::ModuleType::ImageDisplayer>
		{
			DisplayerConfiguration configuration;
		};

		template<>
		struct Context<constants::ModuleType::ImageNormalizer>
		{
			NormalizerConfiguration configuration;
		};

		template<>
		struct Context<constants::ModuleType::InferenceInputProcessor>
		{
			InferenceInputProcessorConfiguration configuration;
		};

		template<>
		struct Context<constants::ModuleType::InferenceModel>
		{
			InferenceModelConfiguration configuration;
		};
	}
}

#endif
