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
		Displayer::Displayer(const Context<constants::ModuleType::ImageDisplayer>& context) : _title(context.configuration.title)
		{}

		Displayer::~Displayer()
		{}

		void Displayer::initialize()
		{}

		weave::error::Result Displayer::process(const cv::Mat& frameBuffer) noexcept
		{
			weave::error::Result result = utilities::DisplayBridge::showFrame(frameBuffer, _title, false);
			if (!result.ok())
			{
				uint32_t frameID = 0; // TODO Deal with frame
				LOG_ERROR("Error displaying frame: " + std::to_string(frameID));
				return {weave::error::Type::Display, frameID};
			}
			return weave::error::Result::success();
		}
	}
}
