// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef TRACESPANDATANODE_H_2025_09_04_17_40_16
#define TRACESPANDATANODE_H_2025_09_04_17_40_16
#include <cstdint>
#include <string>
#include <thread>

namespace weave
{
	namespace profiling
	{
		struct TraceSpanDataNode
		{
			std::string name;
			std::thread::id threadID; // Obfuscated type since it differs depending on OS (has overloaded ostream operator)
			pid_t processID;
			uint64_t startInNanoseconds;
			uint64_t endInNanoseconds;
			std::weak_ptr<TraceSpanDataNode> parent;
			std::vector<std::shared_ptr<TraceSpanDataNode>> children;
		};
	}
}

#endif
