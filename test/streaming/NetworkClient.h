// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser
#ifndef NETWORKENTITY_H
#define NETWORKENTITY_H
#include <string>
#include <array>
#include <vector>

#include "constants.h"
#include "Configuration.h"
#include <weave/logging/Macros.h>
#include <weave/error/Exception.h>
#include <weave/error/Result.h>
#include <weave/logging/Macros.h>

namespace test
{
	namespace streaming
	{
		class NetworkClient
		{
		public:
			explicit NetworkClient(const NetworkClientConfiguration& networkClientConfiguration);
			NetworkClient() = delete;
			NetworkClient(const NetworkClient& client) = delete;
			NetworkClient(NetworkClient&& client) = delete;
			NetworkClient& operator=(const NetworkClient& client) = delete;
			NetworkClient& operator=(NetworkClient&& client) = delete;
			virtual ~NetworkClient();

			// API
			bool isInitialized() const;
			void initialize();
			void connect() const;
			weave::error::Result send(const std::vector<uint8_t>& sendBuffer) const noexcept;
			weave::error::Result receive(std::vector<uint8_t>& receiveBuffer) const noexcept;

		private:
			int32_t _localFileDescriptor;
			NetworkClientConfiguration _configuration;
		};
	}
} // test

#endif //NETWORKENTITY_H
