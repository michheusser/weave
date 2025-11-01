// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef EDGEDESCRIPTOR_H_2025_10_06_15_53_24
#define EDGEDESCRIPTOR_H_2025_10_06_15_53_24
#include "weave/buffer/Constants.h"

namespace weave
{
	namespace graph
	{
		template<typename EdgeTag, typename FromNodeTag, typename ToNodeTag, typename SlotDataType, size_t numSlots, buffer::constants::PolicyType policyType>
		class EdgeDescriptor
		{
		public:
			using Tag = EdgeTag;
			using FromNode = FromNodeTag;
			using ToNode = ToNodeTag;
			static constexpr size_t slots = numSlots;
		};
	}
}

#endif
