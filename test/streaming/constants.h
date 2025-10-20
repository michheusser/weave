// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser
#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <sys/socket.h>
#include <string_view>

namespace test
{
	namespace streaming
	{
		namespace constants
		{
			constexpr int IPv4_FAMILY = AF_INET;
			constexpr int TCP_SOCKET_TYPE = SOCK_STREAM;

			constexpr std::string_view SERVER_LOCAL_ADDRESS = "192.168.1.110";
			constexpr uint16_t SERVER_LOCAL_PORT = 8081;

			constexpr std::string_view CLIENT_LOCAL_ADDRESS = "192.168.1.110";
			constexpr uint16_t CLIENT_LOCAL_PORT = 0;
			constexpr std::string_view CLIENT_REMOTE_ADDRESS = SERVER_LOCAL_ADDRESS;
			constexpr uint16_t CLIENT_REMOTE_PORT = SERVER_LOCAL_PORT;
		}
	}
}
#endif //CONSTANTS_H
