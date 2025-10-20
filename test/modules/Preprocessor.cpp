// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser
#include "Preprocessor.h"
#include <opencv2/imgproc.hpp>
#include <weave/error/Result.h>
#include "Configuration.h"

namespace test
{
	namespace module
	{
		Preprocessor::Preprocessor(const NormalizerConfiguration& configuration) :
			_destinationSize(configuration.destinationWidth, configuration.destinationHeight)
		{}

		Preprocessor::~Preprocessor()
		{}

		void Preprocessor::initialize()
		{}

		weave::error::Result Preprocessor::process(const cv::Mat& sourceFrame, cv::Mat& destinationFrame) const noexcept
		{
			cv::resize(sourceFrame, destinationFrame, _destinationSize); // If throws error, terminates due to noexcept (fatal error in hot path)
			return weave::error::Result::success();
		}
	}
}
