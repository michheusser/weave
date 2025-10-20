// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#include <weave/error/Exception.h>
#include <weave/error/Result.h>
#include <weave/logging/Macros.h>

#include "ServerReceiver.h"

#include "Decoder.h"

namespace test
{
	namespace module
	{
		ServerReceiver::ServerReceiver(const Context<constants::ModuleType::ServerReceiver>& context) : _networkServer(context.networkServer),
			_decoder(context.decoderConfiguration), _bufferConfiguration(context.networkBufferConfiguration)
		{}

		void ServerReceiver::initialize()
		{
			_buffer.resize(_bufferConfiguration.bufferSize);
		}

		weave::error::Result ServerReceiver::process(cv::Mat& outputBuffer) noexcept
		{
			weave::error::Result resultReceive = _networkServer->receive(_buffer);
			if (!resultReceive.ok())
			{
				LOG_ERROR("Server Receiver: Error in processing.");
				return {weave::error::Type::Processing, 0};
			}

			weave::error::Result resultDecode = _decoder.decodeBufferIntoFrame(_buffer, outputBuffer);
			if (!resultDecode.ok())
			{
				LOG_ERROR("Server Receiver: Error in processing.");
				return {weave::error::Type::Processing, 0};
			}
			return weave::error::Result::success();
		}
	} // module
} // weave
