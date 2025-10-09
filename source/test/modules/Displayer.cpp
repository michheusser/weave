// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser
#include "Displayer.h"
#include "../Error/Exception.h"
#include "../Logging/Macros.h"
#include "../Profiling/Macros.h"
#include "../Application/SignalManager.h"

#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include<opencv2/opencv.hpp>
#include "../Error/Result.h"

namespace test
{
	namespace module
	{
		Displayer::Displayer(const DisplayerConfiguration& configuration) : _title(configuration.title)
		{}

		Displayer::~Displayer()
		{}

		Error::Result Displayer::showFrame(const cv::Mat& frameBuffer, const uint32_t frameID) noexcept
		{
			Error::Result result = DisplayBridge::showFrame(frameBuffer, _title, false);
			if (!result.ok())
			{
				LOG_ERROR("Error displaying frame: " + std::to_string(frameID));
				return {Error::Type::Display, frameID};
			}
			return Error::Result::success();
		}
	} // IO
}
