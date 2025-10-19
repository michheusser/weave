// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef BUILDER_H_2025_10_05_15_26_29
#define BUILDER_H_2025_10_05_15_26_29

#include <tuple>
#include <weave/graph/GraphDescriptor.h>
#include <weave/graph/NodeDescriptor.h>
#include <weave/graph/EdgeDescriptor.h>
#include <weave/graph/Graph.h>
#include <weave/user/EdgeTraits.h>

namespace weave
{
	namespace graph
	{
		// AppendToList Metaprogramming Function
		template<typename DescriptorListType, typename NewDescriptor>
		struct AppendToList;

		template<typename... NodeDescriptors, typename NewNodeDescriptor>
		struct AppendToList<NodeDescriptorList<NodeDescriptors...>, NewNodeDescriptor>
		{
			using NewDescriptorListType = NodeDescriptorList<NodeDescriptors..., NewNodeDescriptor>;
		};

		template<typename... EdgeDescriptors, typename NewEdgeDescriptor>
		struct AppendToList<EdgeDescriptorList<EdgeDescriptors...>, NewEdgeDescriptor>
		{
			using NewDescriptorListType = EdgeDescriptorList<EdgeDescriptors..., NewEdgeDescriptor>;
		};

		// DescriptorListToContextTuple Metaprogramming Function
		template<typename DescriptorListType>
		struct DescriptorListToContextTuple;

		template<typename... NodeDescriptors>
		struct DescriptorListToContextTuple<NodeDescriptorList<NodeDescriptors...> >
		{
			using ContextTuple = std::tuple<typename user::NodeTraits<typename NodeDescriptors::Tag>::ContextType...>;
		};

		template<typename... EdgeDescriptors>
		struct DescriptorListToContextTuple<EdgeDescriptorList<EdgeDescriptors...> >
		{
			using ContextTuple = std::tuple<typename user::EdgeTraits<typename EdgeDescriptors::Tag>::ContextType...>;
		};

		// Builder
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
			template<typename NodeTag>
			Builder<typename AppendToList<NodeDescriptorListType, NodeDescriptor<NodeTag> >::NewDescriptorListType, EdgeDescriptorListType> addNode(const typename user::NodeTraits<NodeTag>::ContextType& context)
			{
				Builder<typename AppendToList<NodeDescriptorListType, NodeDescriptor<NodeTag> >::NewDescriptorListType, EdgeDescriptorListType> newBuilder(std::tuple_cat(_nodeContexts, std::make_tuple(context)), _edgeContexts);
				return newBuilder;
			}

			template<typename EdgeTag, typename FromNode, typename ToNode, size_t numSlots>
			Builder<NodeDescriptorListType, typename AppendToList<EdgeDescriptorListType, EdgeDescriptor<EdgeTag, FromNode, ToNode, numSlots> >::NewDescriptorListType> addEdge(const typename user::EdgeTraits<EdgeTag>::ContextType& context)
			{
				Builder<NodeDescriptorListType, typename AppendToList<EdgeDescriptorListType, EdgeDescriptor<EdgeTag, FromNode, ToNode, numSlots> >::NewDescriptorListType> newBuilder(_nodeContexts, std::tuple_cat(_edgeContexts, std::make_tuple(context)));
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
