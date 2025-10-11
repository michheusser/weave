// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef BUFFERCONSTANTS_H_2025_08_27_15_56_46
#define BUFFERCONSTANTS_H_2025_08_27_15_56_46

#include <opencv2/imgcodecs.hpp>

namespace test
{
	namespace buffer
	{
		namespace constants
		{
			enum class BufferType
			{
				Image
			};

			constexpr int RAW_FRAME_HEIGHT = 1080;
			constexpr int RAW_FRAME_WIDTH = 1920;
			constexpr int RAW_FRAME_TYPE = CV_8UC3; // RGB, 8-bit

			constexpr int FRAME_HEIGHT = 256;
			constexpr int FRAME_WIDTH = 256;
			constexpr int FRAME_TYPE = CV_8UC3; // RGB, 8-bit

		}
	}
}


#endif