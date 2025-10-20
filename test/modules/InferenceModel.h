// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef INFERENCEMODEL_H_2025_08_20_16_19_46
#define INFERENCEMODEL_H_2025_08_20_16_19_46

#include "Configuration.h"

#include <opencv2/opencv.hpp>
#include <onnxruntime_cxx_api.h>
#include <weave/error/Exception.h>
#include <weave/error/Result.h>
#include <weave/logging/Macros.h>

#include "Context.h"

namespace test
{
	namespace module
	{
		class InferenceModel
		{
		public:
			explicit InferenceModel(const Context<constants::ModuleType::InferenceModel>& context);
			void initialize();
			weave::error::Result process(const Ort::Value& inputTensor, cv::Mat& outputFrame) const noexcept;
		private:
			InferenceModelConfiguration _configuration;
			std::unique_ptr<Ort::Env> _env;
			std::optional<Ort::AllocatedStringPtr> _inputName;
			std::optional<Ort::AllocatedStringPtr> _outputName;
			std::unique_ptr<Ort::Session> _session;
		};
	} // module
} // test

#endif
