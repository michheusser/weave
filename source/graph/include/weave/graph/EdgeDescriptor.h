// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef EDGEDESCRIPTOR_H_2025_10_06_15_53_24
#define EDGEDESCRIPTOR_H_2025_10_06_15_53_24

namespace weave
{
	namespace graph
	{
		template<typename EdgeTag, typename FromNodeTag, typename ToNodeTag, size_t numSlots>
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
