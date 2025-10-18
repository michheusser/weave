// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser
#ifndef VIDEO_PREPROCESSOR_H
#define VIDEO_PREPROCESSOR_H

#include <opencv2/videoio.hpp>
#include <weave/error/Result.h>
#include "Configuration.h"

namespace test
{
	namespace module
	{
		class Preprocessor
		{
		public:
			explicit Preprocessor(const PreprocessorConfiguration& configuration);
			~Preprocessor();
			void initialize();
			weave::error::Result process(const cv::Mat& sourceFrame, cv::Mat& destinationFrame) const noexcept;
		private:
			const cv::Size _destinationSize;
		};
	}
}
#endif //VIDEO_PREPROCESSOR_H
