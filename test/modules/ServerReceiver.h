// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef SERVERRECEIVER_H_2025_10_20_15_41_43
#define SERVERRECEIVER_H_2025_10_20_15_41_43

#include <weave/error/Exception.h>
#include <weave/error/Result.h>
#include <weave/logging/Macros.h>

#include "Constants.h"
#include "Context.h"
#include "Decoder.h"

namespace test
{
	namespace module
	{
		class ServerReceiver
		{
		public:
			explicit ServerReceiver(const Context<constants::ModuleType::ServerReceiver>& context);
			void initialize();
			weave::error::Result process(cv::Mat& outputBuffer) noexcept;

		private:
			const std::shared_ptr<streaming::NetworkServer> _networkServer;
			const Decoder _decoder;
			const NetworkBufferConfiguration _bufferConfiguration;
			std::vector<uint8_t> _buffer;
		};
	} // module
} // weave

#endif
