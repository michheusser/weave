// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef ERRORCONSTANTS_H_2025_09_27_17_11_17
#define ERRORCONSTANTS_H_2025_09_27_17_11_17

namespace weave
{
	namespace error
	{
		enum class Type
		{
			None,
			Unknown,
			NetworkSend,
			NetworkReceive,
			Capture,
			EmptyBuffer,
			Display,
			BufferOverflow,
			Decoding,
			Encoding,
			Processing,
			Cycle
		};

	}
}

#endif
