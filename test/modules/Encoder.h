// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser
#ifndef FRAMEENCODER_H
#define FRAMEENCODER_H

#include "Configuration.h"

#include "MessageHeader.h"
#include <vector>
#include <opencv2/videoio.hpp>
#include <weave/error/Exception.h>
#include <weave/error/Result.h>
#include <weave/logging/Macros.h>

namespace test
{
	namespace module
	{
		class Encoder
		{
		public:
			explicit Encoder(const EncoderConfiguration& configuration);
			// TODO Further constructors/Destructors
			weave::error::Result encodeFrameIntoBuffer(const cv::Mat& sourceFrame, std::vector<uint8_t>& destinationBuffer, uint32_t frameID) const noexcept;
		private:
			const std::string _encodingFormat;
			const std::vector<int> _encodingParameters;
		};
	}
} // test

#endif //FRAMEENCODER_H
