// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser
#ifndef VIDEO_CAPTURER_H
#define VIDEO_CAPTURER_H

#include "Configuration.h"
#include <opencv2/videoio.hpp>
#include <weave/error/Result.h>

namespace test
{
	namespace module
	{
		class Capturer
		{
		public:
			explicit Capturer(const CapturerConfiguration& configuration);
			// TODO Further constructors missing
			~Capturer();
			void initialize();
			weave::error::Result process(cv::Mat& frameBuffer) noexcept;

		private:
			cv::VideoCapture _videoCapture;
			uint32_t _cameraID;
			static uint64_t nextFrameID;
		};
	}
}

#endif
