// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef CONFIGURATIONGENERATOR_H_2025_09_14_16_01_49
#define CONFIGURATIONGENERATOR_H_2025_09_14_16_01_49

#include "../Application/Configuration.h"
#include "../profiling/Configuration.h"

namespace weave
{
	namespace Configuration
	{
		class ConfigurationGenerator
		{
		public:
			static profiling::ClientSessionConfiguration generateDefaultClientSessionConfiguration();
			static profiling::ServerSessionConfiguration generateDefaultServerSessionConfiguration();
			static Application::ClientConfiguration generateDefaultClientConfiguration();
			static Application::ServerConfiguration generateDefaultServerConfiguration();
		private:
			static std::string _getDateString();
		};
	}
}

#endif
