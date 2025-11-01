// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef INFERENCEINPUTPROCESSOR_H_2025_08_20_16_19_29
#define INFERENCEINPUTPROCESSOR_H_2025_08_20_16_19_29

#include "Configuration.h"

#include <opencv2/videoio.hpp>
#include <onnxruntime_cxx_api.h>
#include <weave/error/Exception.h>
#include <weave/error/Result.h>
#include <weave/logging/Macros.h>

#include "Context.h"

namespace test
{
	namespace module
	{
		class InferenceInputProcessor
		{
		public:
			using ContextType = Context<constants::ModuleType::InferenceInputProcessor>;
			explicit InferenceInputProcessor(const ContextType& context);
			void initialize();
			weave::error::Result process(const cv::Mat& convertedFrame, Ort::Value& inputTensor) const noexcept;
		private:
			InferenceInputProcessorConfiguration _configuration;
		};
	} // module
} // test

#endif
