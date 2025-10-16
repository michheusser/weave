// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser
#include "ProcessorSpecialization.h"

namespace weave
{
	namespace user
	{
		Processor<DefaultCapturerProcessor>::Processor(const ProcessorTraits<ProcessorTag>::ContextType& context) : _capturer(context.capturerConfiguration)
		{}

		void Processor<DefaultCapturerProcessor>::initialize()
		{}

		// TODO Put back
		/*Error::Result Processor<DefaultCapturerProcessor>::process(buffer::Buffer<Module::Traits<moduleType>::buffers[0], Module::Traits<moduleType>::bufferPolicies[0]>::StorageType& outputBuffer, uint32_t* frameID) noexcept
		{
			Error::Result result = _capturer.getFrame(outputBuffer, frameID);
			if (!result.ok())
			{
				LOG_ERROR("Capturer: Error in processing.");
				return {Error::Type::Processing, 0};
			}
			return Error::Result::success();
		}*/

		Processor<DefaultDisplayerProcessor>::Processor(const ProcessorTraits<ProcessorTag>::ContextType& context) : _displayer(context.displayerConfiguration)
		{}

		void Processor<DefaultDisplayerProcessor>::initialize()
		{}

		// TODO Put back
		/*Error::Result Processor<DefaultDisplayerProcessor>::process(const buffer::Buffer<Module::Traits<moduleType>::buffers[0], Module::Traits<moduleType>::bufferPolicies[0]>::StorageType& inputBuffer, uint32_t frameID) noexcept
		{
			Error::Result result = _displayer.showFrame(inputBuffer, frameID);
			if (!result.ok())
			{
				LOG_ERROR("Displayer: Error in processing.");
				return {Error::Type::Processing, 0};
			}
			return Error::Result::success();
		} */

		Processor<DefaultNormalizerProcessor>::Processor(const ProcessorTraits<ProcessorTag>::ContextType& context) : _imageProcessor(context.processorConfiguration)
		{}

		void Processor<DefaultNormalizerProcessor>::initialize()
		{}

		// TODO Put back
		/*Error::Result Processor<DefaultNormalizerProcessor>::process(const buffer::Buffer<Module::Traits<moduleType>::buffers[0], Module::Traits<moduleType>::bufferPolicies[0]>::StorageType& inputBuffer,buffer::Buffer<Module::Traits<moduleType>::buffers[1], Module::Traits<moduleType>::bufferPolicies[1]>::StorageType& outputBuffer) noexcept
		{
			Error::Result result = _imageProcessor.preprocessFrame(inputBuffer, outputBuffer);
			if (!result.ok())
			{
				LOG_ERROR("Image Processor: Error in processing.");
				return {Error::Type::Processing, 0};
			}
			return Error::Result::success();
		}*/
	}
}
