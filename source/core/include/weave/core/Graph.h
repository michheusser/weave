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
			using NodesTuple = typename GraphDescriptorToGraphTuples<GraphDescriptorType>::NodesTuple;
			using EdgesTuple = typename GraphDescriptorToGraphTuples<GraphDescriptorType>::EdgesTuple;

			Graph(NodesTuple& nodeContexts, EdgesTuple& edgeContexts) : _nodes(constructNodes(nodeContexts, std::make_index_sequence<std::tuple_size_v<NodesTuple> >{})),
			                                                            _edges(constructEdges(edgeContexts, std::make_index_sequence<std::tuple_size_v<EdgesTuple> >{}))
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
			template<typename ContextTuple, std::size_t... Is>
			static NodesTuple constructNodes(ContextTuple& contexts, std::index_sequence<Is...>)
			{
				NodesTuple nodesTuple = {std::tuple_element_t<Is, NodesTuple>(std::get<Is>(contexts))...};
				return nodesTuple;
			}
			template<typename ContextTuple, std::size_t... Is>
			static EdgesTuple constructEdges(ContextTuple& contexts, std::index_sequence<Is...>)
			{
				EdgesTuple edgesTuple = {std::tuple_element_t<Is, EdgesTuple>(std::get<Is>(contexts))...};
				return edgesTuple;
			}
			NodesTuple _nodes;
			EdgesTuple _edges;
		};
	}
}

#endif
