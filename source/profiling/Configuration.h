// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef CONFIGURATION_H_2025_09_14_16_25_32
#define CONFIGURATION_H_2025_09_14_16_25_32

#include "../Application/Configuration.h"

namespace weave{
	namespace profiling
	{
		struct ServerSessionConfiguration
		{
			std::string sessionName;
			std::string sessionDescription;
			uint64_t numCycles;
			std::string loggingDirectory;
			Application::ServerConfiguration serverConfiguration;

			CONFIG_MAKE_DATA(CONFIG_PAIR("sessionName",sessionName), CONFIG_PAIR("description",sessionDescription), CONFIG_PAIR("numCycles",numCycles), CONFIG_PAIR("loggingDirectory",loggingDirectory), CONFIG_PAIR("serverConfiguration",serverConfiguration))
		};

		struct ClientSessionConfiguration
		{
			std::string sessionName;
			std::string sessionDescription;
			uint64_t numCycles;
			std::string loggingDirectory;
			Application::ClientConfiguration clientConfiguration;

			CONFIG_MAKE_DATA(CONFIG_PAIR("sessionName",sessionName), CONFIG_PAIR("description",sessionDescription), CONFIG_PAIR("numCycles",numCycles), CONFIG_PAIR("loggingDirectory",loggingDirectory), CONFIG_PAIR("clientConfiguration",clientConfiguration))
		};

	}

}

#endif