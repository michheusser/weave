// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser
#include "Capturer.h"
#include "../Error/Exception.h"
#include "../Logging/Macros.h"
#include "../Profiling/Macros.h"
#include "../Application/SignalManager.h"

#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include<opencv2/opencv.hpp>

namespace test
{
	namespace module
	{
		uint64_t Capturer::nextFrameID = 1;

		Capturer::Capturer(const CapturerConfiguration& configuration) : _videoCapture(configuration.cameraID, configuration.captureAPI), _cameraID(configuration.cameraID) // Constructor doesn't throw per default
		{}

		Capturer::~Capturer()
		{}

		Error::Result Capturer::getFrame(cv::Mat& frameBuffer, uint32_t* frameID) noexcept
		{
			if (!_videoCapture.isOpened())
			{
				LOG_ERROR("Video Capture is not open");
				return {Error::Type::Capture, _cameraID};
			}
			_videoCapture.read(frameBuffer);
			LOG_DEBUG("Frame Captured. Height: " + std::to_string(static_cast<int>(_videoCapture.get(cv::CAP_PROP_FRAME_HEIGHT))) + ", Width: " + std::to_string(static_cast<int>(_videoCapture.get(cv::CAP_PROP_FRAME_WIDTH))));
			if (frameID)
			{
				*frameID = nextFrameID;
			}
			TRACE_SET_FRAME(nextFrameID);
			++nextFrameID;
			return Error::Result::success();
		}
	}
}
