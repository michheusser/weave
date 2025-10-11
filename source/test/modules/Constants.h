// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef MODULECONSTANTS_H_2025_08_27_15_56_46
#define MODULECONSTANTS_H_2025_08_27_15_56_46

#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio.hpp>

namespace test
{
	namespace module
	{
		namespace constants
		{
			enum class ModuleType
			{
				ImageCapturer,
				ImageNormalizer,
				ImageDisplayer
			};

			constexpr int CAMERA_ID = 0;
			constexpr int OPENCV_VIDEO_CAPTURE_API = cv::CAP_ANY; // Autodetect

			constexpr std::string_view DEFAULT_DISPLAY_TITLE = "Frame";

			constexpr int FRAME_HEIGHT = 256;
			constexpr int FRAME_WIDTH = 256;
			constexpr int FRAME_TYPE = CV_8UC3; // RGB, 8-bit
		}
	}
}


#endif