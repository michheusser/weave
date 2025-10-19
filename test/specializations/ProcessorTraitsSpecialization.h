// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef PROCESSORTRAITSSPECIALIZATION_H_2025_10_14_17_55_00
#define PROCESSORTRAITSSPECIALIZATION_H_2025_10_14_17_55_00

#include <weave/user/ProcessorTraits.h>
#include "../modules/Context.h"
#include "../modules/Capturer.h"
#include "../modules/Preprocessor.h"
#include "../modules/Displayer.h"


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
			using ModuleType = test::module::Capturer;
		};

		template<>
		struct ProcessorTraits<DefaultDisplayerProcessor>
		{
			using ProcessorTag = DefaultDisplayerProcessor;
			using ContextType = test::module::Context<test::module::constants::ModuleType::ImageDisplayer>;
			using ModuleType = test::module::Displayer;
		};

		template<>
		struct ProcessorTraits<DefaultNormalizerProcessor>
		{
			using ProcessorTag = DefaultNormalizerProcessor;
			using ContextType = test::module::Context<test::module::constants::ModuleType::ImageNormalizer>;
			using ModuleType = test::module::Preprocessor;
		};
	}
}

#endif
