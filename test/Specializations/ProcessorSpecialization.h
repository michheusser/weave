// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef PROCESSORSPECIALIZATIONS_H_2025_10_14_17_40_36
#define PROCESSORSPECIALIZATIONS_H_2025_10_14_17_40_36

#include <weave/user/Processor.h>
#include <weave/error/Result.h>
#include "ProcessorTraitsSpecialization.h"
#include "../modules/Capturer.h"
#include "../modules/Displayer.h"
#include "../modules/Preprocessor.h"

namespace weave
{
	namespace user
	{
		// TODO Make generic, same as slot. Module is then selected in the ProcessorTraits. Here, the tuple unpacking happens (maybe)
		template<>
		class Processor<DefaultCapturerProcessor>
		{
		public:
			using ProcessorTag = DefaultCapturerProcessor;
			explicit Processor(const ProcessorTraits<ProcessorTag>::ContextType& context);
			void initialize();
			//error::Result process(buffer::Buffer<Module::Traits<moduleType>::buffers[0], Module::Traits<moduleType>::bufferPolicies[0]>::StorageType& outputBuffer, uint32_t* frameID = nullptr) noexcept;

		private:
			test::module::Capturer _capturer;
		};

		template<>
		class Processor<DefaultDisplayerProcessor>
		{
		public:
			using ProcessorTag = DefaultDisplayerProcessor;
			explicit Processor(const ProcessorTraits<ProcessorTag>::ContextType& context);
			void initialize();
			// error::Result process(const buffer::Buffer<Module::Traits<moduleType>::buffers[0], Module::Traits<moduleType>::bufferPolicies[0]>::StorageType& inputBuffer, uint32_t frameID) noexcept;

		private:
			test::module::Displayer _displayer;
		};

		template<>
		class Processor<DefaultNormalizerProcessor>
		{
		public:
			using ProcessorTag = DefaultNormalizerProcessor;
			explicit Processor(const ProcessorTraits<ProcessorTag>::ContextType& context);
			void initialize();
			//error::Result process(const buffer::Buffer<Module::Traits<moduleType>::buffers[0], Module::Traits<moduleType>::bufferPolicies[0]>::StorageType& inputBuffer, buffer::Buffer<Module::Traits<moduleType>::buffers[1], Module::Traits<moduleType>::bufferPolicies[1]>::StorageType& outputBuffer) noexcept;

		private:
			test::module::Preprocessor _imageProcessor;
		};
	} // worker
} // weave

#endif
