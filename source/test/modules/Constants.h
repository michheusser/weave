// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef CONSTANTS_H_2025_08_27_15_56_46
#define CONSTANTS_H_2025_08_27_15_56_46

#include <opencv2/imgcodecs.hpp>

namespace test
{
	namespace module
	{
		namespace Constants
		{
			constexpr int CAMERA_ID = 0;
			constexpr int OPENCV_VIDEO_CAPTURE_API = cv::CAP_ANY; // Autodetect

			constexpr std::string_view DEFAULT_DISPLAY_TITLE = "Frame";
		}
	}
}


#endif