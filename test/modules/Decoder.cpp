// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser
#include "Decoder.h"
#include "Constants.h"
#include <weave/error/Exception.h>
#include <weave/error/Result.h>
#include <weave/logging/Macros.h>
#include "MessageHeader.h"

namespace test
{
	namespace module
	{
		Decoder::Decoder(const DecoderConfiguration& configuration) : _configuration(configuration)
		{}

		Decoder::~Decoder()
		{}

		weave::error::Result Decoder::decodeBufferIntoFrame(const std::vector<uint8_t>& sourceBuffer, cv::Mat& destinationFrame) const noexcept
		{
			const int flags = _configuration.flags;
			// This overload reuses the cv::Mat as a buffer, sparing re-allocations when image size doesn't change
			MessageHeader messageHeader;
			std::memcpy(&messageHeader, sourceBuffer.data(), sizeof(MessageHeader));
			size_t expectedTotalMessageSize = sizeof(MessageHeader) + messageHeader.payloadSize;
			if (sourceBuffer.size() < messageHeader.payloadSize + sizeof(MessageHeader))
			{
				return {weave::error::Type::BufferOverflow, 0}; // Source buffer too small for expected payload size.
			}
			LOG_DEBUG("Total data size: " + std::to_string(expectedTotalMessageSize) + " bytes");
			LOG_DEBUG("Image data to decode: " + std::to_string(messageHeader.payloadSize) + " bytes");
			std::vector<uint8_t> encodedImageBuffer(messageHeader.payloadSize); // TODO Performance improvement potential (make it member variable for pre-allocation, probably moving the whole module logic into the "processor")
			std::memcpy(encodedImageBuffer.data(), sourceBuffer.data() + sizeof(MessageHeader), messageHeader.payloadSize);
			cv::imdecode(encodedImageBuffer, flags, &destinationFrame);
			if (destinationFrame.empty())
			{
				return {weave::error::Type::Decoding, 0};
			}
			return weave::error::Result::success();
		}
	} // Video
} // test
