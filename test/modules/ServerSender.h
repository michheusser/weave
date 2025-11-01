// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef SERVERSENDER_H_2025_10_20_15_41_31
#define SERVERSENDER_H_2025_10_20_15_41_31

#include <weave/error/Exception.h>
#include <weave/error/Result.h>
#include <weave/logging/Macros.h>

#include "Context.h"
#include "Encoder.h"

namespace test
{
	namespace module
	{
		class ServerSender
		{
		public:
			using ContextType = Context<constants::ModuleType::ServerSender>;
			explicit ServerSender(const ContextType& context);
			void initialize();
			weave::error::Result process(const cv::Mat& inputBuffer) noexcept;

		private:
			const std::shared_ptr<streaming::NetworkServer> _networkServer;
			const Encoder _encoder;
			const NetworkBufferConfiguration _bufferConfiguration;
			std::vector<uint8_t> _buffer;
		};
	} // module
} // test

#endif
