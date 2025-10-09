// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef DISPLAYER_H_2025_09_16_15_36_58
#define DISPLAYER_H_2025_09_16_15_36_58
#include <opencv2/videoio.hpp>
#include "Configuration.h"
#include "../Buffering/Buffer.h"
#include "DisplayBridge.h"
#include "../Error/Result.h"

namespace test
{
	namespace module
	{
		class Displayer
		{
		public:
			explicit Displayer(const DisplayerConfiguration& configuration);
			~Displayer();
			Error::Result showFrame(const cv::Mat& frameBuffer, const uint32_t frameID) noexcept;
		private:
			std::string _title;
		};
	} // IO
}

#endif
