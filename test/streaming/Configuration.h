// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef CONFIGURATION_H_2025_08_16_15_49_51
#define CONFIGURATION_H_2025_08_16_15_49_51
#include <cstdint>
#include <string>

namespace test
{
	namespace streaming
	{
		struct NetworkClientConfiguration
		{
			std::string localAddress;
			uint16_t localPort;
			std::string remoteAddress;
			uint16_t remotePort;
		};

		struct NetworkServerConfiguration
		{
			std::string localAddress;
			uint16_t localPort;
		};
	}
}

#endif
