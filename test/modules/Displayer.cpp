// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser
#include "Displayer.h"

#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include<opencv2/opencv.hpp>
#include <weave/error/Result.h>
#include <weave/logging/Macros.h>
#include "../utilities/DisplayBridge.h"

namespace test
{
	namespace module
	{
		Displayer::Displayer(const DisplayerConfiguration& configuration) : _title(configuration.title)
		{}

		Displayer::~Displayer()
		{}

		weave::error::Result Displayer::showFrame(const cv::Mat& frameBuffer, const uint32_t frameID) noexcept
		{
			weave::error::Result result = DisplayBridge::showFrame(frameBuffer, _title, false);
			if (!result.ok())
			{
				LOG_ERROR("Error displaying frame: " + std::to_string(frameID));
				return {weave::error::Type::Display, frameID};
			}
			return weave::error::Result::success();
		}
	}
}
