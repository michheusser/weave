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
		template<typename ModuleTag, uint64_t numInputs, uint64_t numOutputs>
		class NodeDescriptor
		{
		public:
			using Tag = ModuleTag;
			static constexpr uint64_t inputs = numInputs;
			static constexpr uint64_t outputs = numOutputs;
		};
	}
}

#endif
