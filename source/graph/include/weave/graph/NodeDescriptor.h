// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef NODEDESCRIPTOR_H_2025_10_06_15_52_36
#define NODEDESCRIPTOR_H_2025_10_06_15_52_36

#include <cstdint>

namespace weave
{
	namespace graph
	{
		template<typename NodeTag, typename ModuleType>
		class NodeDescriptor
		{
		public:
			using Tag = NodeTag;
		};
	}
}

#endif
