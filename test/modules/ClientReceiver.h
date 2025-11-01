// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef CLIENTRECEIVER_H_2025_10_20_15_41_16
#define CLIENTRECEIVER_H_2025_10_20_15_41_16

#include <weave/error/Exception.h>
#include <weave/error/Result.h>
#include <weave/logging/Macros.h>

#include "Context.h"
#include "Decoder.h"


namespace test
{
	namespace module
	{
		class ClientReceiver
		{
		public:
			using ContextType = Context<constants::ModuleType::ClientReceiver>;
			explicit ClientReceiver(const ContextType& context);
			void initialize();
			weave::error::Result process(cv::Mat& outputBuffer) noexcept;

		private:
			const std::shared_ptr<streaming::NetworkClient> _networkClient;
			const Decoder _decoder;
			const NetworkBufferConfiguration _bufferConfiguration;
			std::vector<uint8_t> _buffer;
		};
	} // module
} // test

#endif
