// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser
#include <weave/error/Exception.h>
#include <weave/error/Result.h>
#include <weave/logging/Macros.h>

#include "ClientReceiver.h"
#include "Decoder.h"

namespace test
{
	namespace module
	{
		ClientReceiver::ClientReceiver(const Context<constants::ModuleType::ClientReceiver>& context) : _networkClient(context.networkClient), _decoder(context.decoderConfiguration),
		                                                                                                _bufferConfiguration(context.networkBufferConfiguration)
		{}

		void ClientReceiver::initialize()
		{
			_buffer.resize(_bufferConfiguration.bufferSize);
		}

		weave::error::Result ClientReceiver::process(cv::Mat& outputBuffer) noexcept
		{
			weave::error::Result resultReceive = _networkClient->receive(_buffer);
			if (!resultReceive.ok())
			{
				LOG_ERROR("Client Receiver: Error in processing.");
				return {weave::error::Type::Processing, 0};
			}

			weave::error::Result resultDecode = _decoder.decodeBufferIntoFrame(_buffer, outputBuffer);
			if (!resultDecode.ok())
			{
				LOG_ERROR("Client Receiver: Error in processing.");
				return {weave::error::Type::Processing, 0};
			}
			return weave::error::Result::success();
		}
	} // module
} // test
