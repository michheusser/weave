// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef GRAPHDESCRIPTOR_H_2025_10_06_15_53_03
#define GRAPHDESCRIPTOR_H_2025_10_06_15_53_03

namespace weave
{
	namespace graph
	{
		template<typename... NodeDescriptorTypes>
		struct NodeDescriptorList
		{};

		template<typename... EdgeDescriptorTypes>
		struct EdgeDescriptorList
		{};

		template<typename NodeDescriptorListType, typename EdgeDescriptorListType>
		class GraphDescriptor
		{};
	}
}

#endif
