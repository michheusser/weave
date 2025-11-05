// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef ENCODEDMESSAGEHEADER_H_2025_08_26_10_11_41
#define ENCODEDMESSAGEHEADER_H_2025_08_26_10_11_41

namespace test
{
	namespace module
	{
		struct MessageHeader
		{
			uint32_t payloadSize; // Size of actual data
			// uint32_t type;    // TODO Later frame/result/etc
			//uint64_t timestamp; // for latency measurement
		};
	}
}

#endif
