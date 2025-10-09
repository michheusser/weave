// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef GRAPH_H_2025_10_05_14_42_23
#define GRAPH_H_2025_10_05_14_42_23
#include <vector>

#include "Edge.h"
#include "GraphDescriptor.h"
#include "Node.h"
#include "Utilities.h"

namespace weave
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
		typename GraphDescriptorToGraphTuples<GraphDescriptorType>::Nodes _nodes;
		typename GraphDescriptorToGraphTuples<GraphDescriptorType>::Edges _edges;
	};
}

#endif
