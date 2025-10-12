// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef GRAPH_H_2025_10_05_14_42_23
#define GRAPH_H_2025_10_05_14_42_23

#include <weave/graph/Utilities.h>

namespace weave
{
	namespace graph
	{
		template<typename GraphDescriptorType>
		class Graph
		{
		public:
			using NodeContextTuple = typename GraphDescriptorToContextTuples<GraphDescriptorType>::NodeContextTuple;
			using EdgeContextTuple = typename GraphDescriptorToContextTuples<GraphDescriptorType>::EdgeContextTuple;
			using NodesTuple = typename GraphDescriptorToGraphTuples<GraphDescriptorType>::NodesTuple;
			using EdgesTuple = typename GraphDescriptorToGraphTuples<GraphDescriptorType>::EdgesTuple;

			Graph(NodeContextTuple& nodeContexts, EdgeContextTuple& edgeContexts) : _nodes(_constructTuple<NodesTuple>(nodeContexts, std::make_index_sequence<std::tuple_size_v<NodesTuple> >{})),
			                                                                        _edges(_constructTuple<EdgesTuple>(edgeContexts, std::make_index_sequence<std::tuple_size_v<EdgesTuple> >{}))
			{}
			void start()
			{
				//_startNodes(std::make_index_sequence<std::tuple_size_v<NodesTuple> >{}); // Arguments: 0, 1, 2, ...
			}

		private:
			template<std::size_t... Is>
			void _startNodes(std::index_sequence<Is...>)
			{
				(std::get<Is>(_nodes).start(), ...);
			}

			template<typename Tuple, typename ContextTuple, std::size_t... Indices>
			static Tuple _constructTuple(ContextTuple& contexts, std::index_sequence<Indices...>)
			{
				Tuple tuple = {std::tuple_element_t<Indices, Tuple>(std::get<Indices>(contexts))...};
				return tuple;
			}
			NodesTuple _nodes;
			EdgesTuple _edges;
		};
	}
}

#endif
