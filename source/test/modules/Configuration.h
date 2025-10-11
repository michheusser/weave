// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef MODULECONFIGURATION_H_2025_08_27_15_56_53
#define MODULECONFIGURATION_H_2025_08_27_15_56_53

#include <string>

namespace test
{
	namespace module
	{
		struct CapturerConfiguration
		{
			uint32_t cameraID;
			uint32_t captureAPI;
		};

		struct PreprocessorConfiguration
		{
			int destinationWidth;
			int destinationHeight;
		};

		struct DisplayerConfiguration
		{
			std::string title;
		};
	}
}

#endif
