// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser
#ifndef VIDEO_CAPTURER_H
#define VIDEO_CAPTURER_H

#include <opencv2/videoio.hpp>
#include <weave/error/Result.h>
#include "Configuration.h"

#include "Context.h"

namespace test
{
	namespace module
	{
		class Capturer
		{
		public:
			using ContextType = Context<constants::ModuleType::ImageCapturer>;
			explicit Capturer(const ContextType& context);
			// TODO Further constructors missing
			~Capturer();
			void initialize();
			weave::error::Result process(cv::Mat& frameBuffer) noexcept;

		private:
			cv::VideoCapture _videoCapture;
			uint32_t _cameraID;
		};
	} // namespace module
} // namespace test

#endif
