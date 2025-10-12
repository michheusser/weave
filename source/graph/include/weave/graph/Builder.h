// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef BUILDER_H_2025_10_05_15_26_29
#define BUILDER_H_2025_10_05_15_26_29

#include <cstdlib>
#include <tuple>
#include <weave/graph/GraphDescriptor.h>
#include <weave/graph/NodeDescriptor.h>
#include <weave/graph/EdgeDescriptor.h>
#include <weave/graph/Graph.h>
#include <weave/graph/Utilities.h>
#include <weave/user/Module.h>
#include <weave/user/BufferData.h>

namespace weave
{
	namespace graph
	{
		template<typename NodeDescriptorListType = NodeDescriptorList<>, typename EdgeDescriptorListType = EdgeDescriptorList<> >
		class Builder
		{
		public:
			explicit Builder()
			{}

			explicit Builder(typename DescriptorListToContextTuple<NodeDescriptorListType>::ContextTuple nodeContexts,
			                 typename DescriptorListToContextTuple<EdgeDescriptorListType>::ContextTuple edgeContexts) : _nodeContexts(nodeContexts), _edgeContexts(edgeContexts)
			{}

			// TODO Optimize for moving contexts instead of copying it everytime.
			template<typename NodeTag, uint64_t numInputs, uint64_t numOutputs>
			Builder<typename AppendToList<NodeDescriptorListType, NodeDescriptor<NodeTag, numInputs, numOutputs> >::NewDescriptorListType, EdgeDescriptorListType> addNode(const typename user::Module<NodeTag>::ContextType& context)
			{
				Builder<typename AppendToList<NodeDescriptorListType, NodeDescriptor<NodeTag, numInputs, numOutputs> >::NewDescriptorListType, EdgeDescriptorListType> newBuilder(std::tuple_cat(_nodeContexts, std::make_tuple(context)), _edgeContexts);
				return newBuilder;
			}

			template<typename EdgeTag, typename FromNode, typename ToNode>
			Builder<NodeDescriptorListType, typename AppendToList<EdgeDescriptorListType, EdgeDescriptor<EdgeTag, FromNode, ToNode> >::NewDescriptorListType> addEdge(const typename user::BufferData<EdgeTag>::ContextType& context)
			{
				Builder<NodeDescriptorListType, typename AppendToList<EdgeDescriptorListType, EdgeDescriptor<EdgeTag, FromNode, ToNode> >::NewDescriptorListType> newBuilder(_nodeContexts, std::tuple_cat(_edgeContexts, std::make_tuple(context)));
				return newBuilder;
			}

			Graph<GraphDescriptor<NodeDescriptorListType, EdgeDescriptorListType> > build()
			{
				return Graph<GraphDescriptor<NodeDescriptorListType, EdgeDescriptorListType> >(_nodeContexts, _edgeContexts);
			}

		private:
			typename DescriptorListToContextTuple<NodeDescriptorListType>::ContextTuple _nodeContexts;
			typename DescriptorListToContextTuple<EdgeDescriptorListType>::ContextTuple _edgeContexts;
		};
	}
}
#endif
