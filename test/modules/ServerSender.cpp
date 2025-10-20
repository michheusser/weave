// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#include <weave/error/Exception.h>
#include <weave/error/Result.h>
#include <weave/logging/Macros.h>

#include "ServerSender.h"
#include "Encoder.h"

namespace test
{
	namespace module
	{
		ServerSender::ServerSender(const Context<constants::ModuleType::ServerSender>& context) : _networkServer(context.networkServer), _encoder(context.encoderConfiguration),
		                                                                                          _bufferConfiguration(context.networkBufferConfiguration)
		{}

		void ServerSender::initialize()
		{
			_buffer.resize(_bufferConfiguration.bufferSize);
		}

		weave::error::Result ServerSender::process(const cv::Mat& inputBuffer) noexcept
		{
			weave::error::Result resultEncode = _encoder.encodeFrameIntoBuffer(inputBuffer, _buffer);
			if (!resultEncode.ok())
			{
				LOG_ERROR("Server Sender: Error in processing.");
				return {weave::error::Type::Processing, 0};
			}

			weave::error::Result resultSend = _networkServer->send(_buffer);

			if (!resultSend.ok())
			{
				LOG_ERROR("Server Sender: Error in processing.");
				return {weave::error::Type::NetworkSend, 0};
			}
			return weave::error::Result::success();
		}
	} // module
} // test
