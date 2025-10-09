// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser
#ifndef VIDEO_PREPROCESSOR_H
#define VIDEO_PREPROCESSOR_H

#include <opencv2/videoio.hpp>
#include "../Constants.h"
#include "../Configuration.h"
#include "../../Error/Exception.h"
#include "../../Logging/Macros.h"
#include "../../Buffering/Buffer.h"
#include "../../Error/Result.h"

namespace test
{
	namespace module
	{
		class Preprocessor
		{
		public:
			explicit Preprocessor(const PreprocessorConfiguration& configuration);
			~Preprocessor();
			Error::Result preprocessFrame(const cv::Mat& sourceFrame, cv::Mat& destinationFrame) const noexcept;
		private:
			const cv::Size _destinationSize;
		};
	} // Video
}
#endif //VIDEO_PREPROCESSOR_H
