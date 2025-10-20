// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser
#include "InferenceModel.h"
#include "Constants.h"
#include <weave/error/Exception.h>
#include <weave/error/Result.h>
#include <weave/logging/Macros.h>

namespace test
{
	namespace module
	{
		InferenceModel::InferenceModel(const Context<constants::ModuleType::InferenceModel>& context) : _configuration(context.configuration), _env(nullptr), _session(nullptr)
		{}

		void InferenceModel::initialize()
		{
			// Initialize ONNX Runtime
			_env = std::make_unique<Ort::Env>(_configuration.loggingLevel, _configuration.logid.c_str());
			Ort::SessionOptions sessionOptions;
			sessionOptions.SetIntraOpNumThreads(1);

			// Load DPT model (download dpt_large_384.onnx or similar)
			// https://huggingface.co/Xenova/dpt-large/tree/main/onnx
			// Later download full precision model (model.onnx)
			std::string fullPath = _configuration.modelPath + "/" + _configuration.modelFile;
 			_session = std::make_unique<Ort::Session>(*_env, fullPath.c_str(), sessionOptions);
			// Get model input/output info
			Ort::AllocatorWithDefaultOptions allocator;
			_inputName = _session->GetInputNameAllocated(0, allocator);
			_outputName = _session->GetOutputNameAllocated(0, allocator);
		}

		weave::error::Result InferenceModel::process(const Ort::Value& inputTensor, cv::Mat& outputFrame) const noexcept
		{
			// Run inference
			const char* inputNames[] = {_inputName->get()};
			const char* outputNames[] = {_outputName->get()};
			std::vector<Ort::Value> outputTensors = _session->Run(Ort::RunOptions{nullptr}, inputNames, &inputTensor, 1, outputNames, 1);

			// Get output - we KNOW it's [1, 3, H, W] format
			float* outputData = outputTensors[0].GetTensorMutableData<float>();
			auto outputShape = outputTensors[0].GetTensorTypeAndShapeInfo().GetShape();

			// Log shape for verification
			LOG_DEBUG("Output tensor shape: [1, 3, {}, {}]", outputShape[2], outputShape[3]);

			int64_t outputHeight = outputShape[2];
			int64_t outputWidth = outputShape[3];

			// Convert from channels-first [1, 3, H, W] to OpenCV BGR format
			cv::Mat styledImage(outputHeight, outputWidth, CV_32FC3);

			for (int h = 0; h < outputHeight; h++)
			{
				for (int w = 0; w < outputWidth; w++)
				{
					// Convert RGB to BGR and handle channels-first format
					styledImage.at<cv::Vec3f>(h, w)[0] = outputData[2 * outputHeight * outputWidth + h * outputWidth + w]; // B (from R)
					styledImage.at<cv::Vec3f>(h, w)[1] = outputData[1 * outputHeight * outputWidth + h * outputWidth + w]; // G
					styledImage.at<cv::Vec3f>(h, w)[2] = outputData[0 * outputHeight * outputWidth + h * outputWidth + w]; // R (from B)
				}
			}

			// Resize if needed
			cv::Mat resizedImage;
			cv::resize(styledImage, resizedImage, cv::Size(_configuration.width, _configuration.height));

			// Clip values to [0, 255] and convert to 8-bit
			cv::Mat finalOutput;
			cv::normalize(resizedImage, finalOutput, 0, 255, cv::NORM_MINMAX, CV_8UC3);

			outputFrame = finalOutput;
			LOG_DEBUG("Style transfer completed successfully");
			return weave::error::Result::success();
		}
	} // module
} // test
