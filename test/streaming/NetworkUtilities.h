// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser
#ifndef NETWORKUTILITIES_H
#define NETWORKUTILITIES_H

#include <netinet/in.h>
#include <weave/error/Result.h>
#include <weave/logging/Macros.h>
#include <string>
#include <vector>

namespace test
{
	namespace streaming
	{
		namespace TCPIPV4
		{
			sockaddr_in createAddressObject(const std::string& address, const uint16_t port);
			int createSocket();
			void bindSocket(const int32_t localFileDescriptor, const sockaddr_in& localAddressObject);
			void connect(const int32_t localFileDescriptor, const sockaddr_in& remoteAddressObject);
			weave::error::Result send(const int32_t localFileDescriptor, const std::vector<uint8_t>& buffer) noexcept;
			// Important to always passed initialized buffers, and not only reserved their size
			weave::error::Result receive(const int32_t localFileDescriptor, std::vector<uint8_t>& buffer) noexcept;
			void listen(const int32_t localFileDescriptor);
			int accept(const int32_t localFileDescriptor);
			std::string bufferToHex(const std::vector<uint8_t>& buffer, size_t numBytes = 0);
			void close(const int32_t localFileDescriptor);
			std::string errnoToString(int sysErrno);
		}
	}
}
#endif //NETWORKUTILITIES_H
