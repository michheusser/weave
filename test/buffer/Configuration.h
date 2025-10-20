// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef BUFFERCONFIGURATION_H_2025_08_27_15_56_53
#define BUFFERCONFIGURATION_H_2025_08_27_15_56_53

#include <onnxruntime_c_api.h>

namespace test
{
	namespace buffer
	{
		struct ImageBufferConfiguration
		{
			int frameHeight;
			int frameWidth;
			int frameType;
		};

		struct TensorBufferConfiguration
		{
			int height;
			int width;
			OrtAllocatorType tensorAllocatorType;
			OrtMemType tensorMemoryType;
		};
	}
}

#endif
