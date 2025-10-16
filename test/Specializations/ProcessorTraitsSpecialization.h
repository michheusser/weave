// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef PROCESSORTRAITSSPECIALIZATION_H_2025_10_14_17_55_00
#define PROCESSORTRAITSSPECIALIZATION_H_2025_10_14_17_55_00

#include <weave/user/ProcessorTraits.h>
#include "../modules/Context.h"

struct DefaultCapturerProcessor
{};

struct DefaultDisplayerProcessor
{};

struct DefaultNormalizerProcessor
{};

namespace weave
{
	namespace user
	{
		template<>
		struct ProcessorTraits<DefaultCapturerProcessor>
		{
			using ProcessorTag = DefaultCapturerProcessor;
			using ContextType = test::module::Context<test::module::constants::ModuleType::ImageCapturer>;
		};

		template<>
		struct ProcessorTraits<DefaultDisplayerProcessor>
		{
			using ProcessorTag = DefaultDisplayerProcessor;
			using ContextType = test::module::Context<test::module::constants::ModuleType::ImageDisplayer>;
		};

		template<>
		struct ProcessorTraits<DefaultNormalizerProcessor>
		{
			using ProcessorTag = DefaultNormalizerProcessor;
			using ContextType = test::module::Context<test::module::constants::ModuleType::ImageNormalizer>;
		};
	}
}

#endif
