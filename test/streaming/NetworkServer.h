#ifndef SERVER_H
#define SERVER_H

#include <string>
#include <array>
#include <vector>

#include "constants.h"
#include "Configuration.h"
#include <weave/error/Exception.h>
#include <weave/error/Result.h>
#include <weave/logging/Macros.h>

namespace test
{
	namespace streaming
	{
		class NetworkServer
		{
		public:
			explicit NetworkServer(const NetworkServerConfiguration& serverConfiguration);
			NetworkServer() = delete;
			NetworkServer(const NetworkServer& server) = delete;
			NetworkServer(NetworkServer&& server) = delete;
			NetworkServer& operator=(const NetworkServer& server) = delete;
			NetworkServer& operator=(NetworkServer&& server) = delete;
			virtual ~NetworkServer();

			// API
			bool isInitialized() const;
			void initialize();
			void listen() const;
			void accept();
			weave::error::Result send(const std::vector<uint8_t>& sendBuffer) const noexcept;
			weave::error::Result receive(std::vector<uint8_t>& receiveBuffer) const noexcept;
		private:
			int32_t _localFileDescriptor;
			int32_t _remoteFileDescriptor;
			// TODO Rfor later emote file descriptor should be a list?
			NetworkServerConfiguration _configuration;
		};
	} // test
}

#endif //SERVER_H
