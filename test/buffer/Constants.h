// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef BUFFERCONSTANTS_H_2025_08_27_15_56_46
#define BUFFERCONSTANTS_H_2025_08_27_15_56_46

#include <opencv2/imgcodecs.hpp>

namespace test
{
	namespace buffer
	{
		namespace constants
		{
			enum class BufferType
			{
				Image,
				InferenceInputTensor
			};
		}
	}
}


#endif