// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser
#ifndef VIDEO_DECODER_H
#define VIDEO_DECODER_H

#include <weave/error/Exception.h>
#include <weave/error/Result.h>
#include <weave/logging/Macros.h>
#include <opencv2/videoio.hpp>
#include "Configuration.h"

namespace test
{
	namespace module
	{
		class Decoder
		{
		public:
			explicit Decoder(const DecoderConfiguration& configuration);
			~Decoder();
			// TODO Further constructors/Destructors
			weave::error::Result decodeBufferIntoFrame(const std::vector<uint8_t>& sourceBuffer, cv::Mat& destinationFrame, uint32_t* frameID = nullptr) const noexcept;

		private:
			DecoderConfiguration _configuration;
			// TODO Decide on how to admin the flags
		};
	} // Video
} // test

#endif //VIDEO_DECODER_H
