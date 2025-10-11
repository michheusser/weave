// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef GRAPH_H_2025_10_05_14_42_23
#define GRAPH_H_2025_10_05_14_42_23

#include <weave/core/Utilities.h>

namespace weave
{
	namespace core
	{
		template<typename GraphDescriptorType>
		class Graph
		{
		public:
			Graph(typename GraphDescriptorToContextTuples<GraphDescriptorType>::NodeContextTuple& nodeContexts,
			      typename GraphDescriptorToContextTuples<GraphDescriptorType>::EdgeContextTuple& edgeContexts)
			{}
			void initialize()
			{
				// TODO Later
			}
			void start()
			{
				// TODO Later
			}

		private:
			typename GraphDescriptorToGraphTuples<GraphDescriptorType>::NodesTuple _nodes;
			typename GraphDescriptorToGraphTuples<GraphDescriptorType>::EdgesTuple _edges;
		};
	}
}

#endif
