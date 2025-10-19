// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser
#include "Encoder.h"
#include "Constants.h"
#include <weave/error/Exception.h>
#include <weave/error/Result.h>
#include <weave/logging/Macros.h>
#include "MessageHeader.h"

#include <opencv2/imgcodecs.hpp>

namespace test
{
	namespace module
	{
		Encoder::Encoder(const EncoderConfiguration& configuration) : _encodingFormat(configuration.encodingFormat), _encodingParameters(configuration.encodingParameters)
		{}

		weave::error::Result Encoder::encodeFrameIntoBuffer(const cv::Mat& sourceFrame, std::vector<uint8_t>& destinationBuffer, const uint32_t frameID) const noexcept
		{
			if (sourceFrame.empty())
			{
				return {weave::error::Type::EmptyBuffer, frameID}; // Empty frame
			}
			std::vector<uint8_t> encodedImageBuffer; // TODO Performance improvement potential (maybe making it static)
			// No compression (e.g. with .png) should output a constant encoded frame size. If the input buffer
			// does not use shrink_to_fit at any point in its lifetime, no reallocations will happen when encoding into the buffer.
			const_cast<cv::Mat&>(sourceFrame);
			cv::imencode(_encodingFormat, sourceFrame, encodedImageBuffer, _encodingParameters);
			MessageHeader messageHeader;
			messageHeader.frameID = frameID;
			messageHeader.payloadSize = encodedImageBuffer.size();
			size_t totalMessageSize = encodedImageBuffer.size() + sizeof(MessageHeader);
			if (totalMessageSize > destinationBuffer.size())
			{
				return {weave::error::Type::BufferOverflow, frameID}; // Destination buffer too small for encoded data message
			}
			std::memcpy(destinationBuffer.data(), &messageHeader, sizeof(MessageHeader));
			std::memcpy(destinationBuffer.data() + sizeof(MessageHeader), encodedImageBuffer.data(), encodedImageBuffer.size());
			LOG_DEBUG("Image data encoded: " + std::to_string(messageHeader.payloadSize) + " bytes");
			LOG_DEBUG("Total encoded data: " + std::to_string(totalMessageSize) + " bytes");

			return weave::error::Result::success();
		}
	}
} // test
