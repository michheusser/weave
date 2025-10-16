// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser
#include "Capturer.h"
#include <weave/error/Result.h>

#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include<opencv2/opencv.hpp>
#include <weave/error/Result.h>
#include <weave/logging/Macros.h>
//#include <weave/profiling/Macros.h> // TODO Put back

namespace test
{
	namespace module
	{
		uint64_t Capturer::nextFrameID = 1;

		Capturer::Capturer(const CapturerConfiguration& configuration) : _videoCapture(configuration.cameraID, configuration.captureAPI), _cameraID(configuration.cameraID)
		// Constructor doesn't throw per default
		{}

		Capturer::~Capturer()
		{}

		weave::error::Result Capturer::getFrame(cv::Mat& frameBuffer, uint32_t* frameID) noexcept
		{
			if (!_videoCapture.isOpened())
			{
				LOG_ERROR("Video Capture is not open");
				return {weave::error::Type::Capture, _cameraID};
			}
			_videoCapture.read(frameBuffer);
			LOG_DEBUG(
				"Frame Captured. Height: " + std::to_string(static_cast<int>(_videoCapture.get(cv::CAP_PROP_FRAME_HEIGHT))) + ", Width: " + std::to_string(static_cast<int>(_videoCapture.get(cv::
					CAP_PROP_FRAME_WIDTH))));
			if (frameID)
			{
				*frameID = nextFrameID;
			}
			//TRACE_SET_FRAME(nextFrameID);
			++nextFrameID;
			return weave::error::Result::success();
		}
	}
}
