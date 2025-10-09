// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef CONFIGURATION_H_2025_08_27_15_56_53
#define CONFIGURATION_H_2025_08_27_15_56_53
#include "../Profiling/Macros.h"

namespace test
{
	namespace module
	{
		struct CapturerConfiguration
		{
			uint32_t cameraID;
			uint32_t captureAPI;

			CONFIG_MAKE_DATA(CONFIG_PAIR("cameraID",cameraID), CONFIG_PAIR("captureAPI",captureAPI))
		};

		struct DisplayerConfiguration
		{
			std::string title;

			CONFIG_MAKE_DATA(CONFIG_PAIR("title",title))
		};
	}
}

#endif
