// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser
#include "InferenceInputProcessor.h"
#include <weave/error/Exception.h>
#include <weave/error/Result.h>
#include <weave/logging/Macros.h>

#include "Constants.h"
#include <opencv2/opencv.hpp>

namespace test
{
	namespace module
	{
		InferenceInputProcessor::InferenceInputProcessor(const InferenceInputProcessorConfiguration& configuration) : _configuration(configuration)
		{}

		void InferenceInputProcessor::initialize()
		{}

		weave::error::Result InferenceInputProcessor::process(const cv::Mat& sourceFrame, Ort::Value& inputTensor) const noexcept
		{
			cv::Mat convertedFrame;
			sourceFrame.convertTo(convertedFrame, _configuration.rtype, _configuration.alpha); // TODO Big optimization potential. This is not zero-copy!

			int inputWidth = _configuration.inputWidth;
			int inputHeight = _configuration.inputHeight;

			// Normalize with ImageNet mean and std
			cv::Scalar mean(std::get<0>(_configuration.mean), std::get<1>(_configuration.mean), std::get<2>(_configuration.mean));
			cv::Scalar std(std::get<0>(_configuration.std), std::get<1>(_configuration.std), std::get<2>(_configuration.std));
			cv::subtract(convertedFrame, mean, convertedFrame);
			cv::divide(convertedFrame, std, convertedFrame);

			// Convert from HWC to CHW format
			std::vector<int64_t> inputShape = {1, 3, inputHeight, inputWidth};
			size_t inputTensorSize = 1 * 3 * inputHeight * inputWidth;
			std::vector<float> inputTensorValues(inputTensorSize);

			int channelSize = inputHeight * inputWidth;
			for (int c = 0; c < 3; c++)
			{
				for (int h = 0; h < inputHeight; h++)
				{
					for (int w = 0; w < inputWidth; w++)
					{
						inputTensorValues[c * channelSize + h * inputWidth + w] = convertedFrame.at<cv::Vec3f>(h, w)[2 - c]; // BGR to RGB
					}
				}
			}

			// Copy data into preallocated tensor buffer
			float* tensorData = inputTensor.GetTensorMutableData<float>();
			std::memcpy(tensorData, inputTensorValues.data(), inputTensorSize * sizeof(float));
			return weave::error::Result::success();
		}
	} // module
} // test
