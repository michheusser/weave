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
			CapturerConfiguration capturerConfiguration;
		};

		template<>
		struct Context<constants::ModuleType::ImageDisplayer>
		{
			DisplayerConfiguration displayerConfiguration;
		};

		template<>
		struct Context<constants::ModuleType::ImageNormalizer>
		{
			PreprocessorConfiguration processorConfiguration;
		};
	}
}

#endif
