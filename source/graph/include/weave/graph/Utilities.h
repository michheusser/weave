// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef UTILITIES_H_2025_10_08_15_01_16
#define UTILITIES_H_2025_10_08_15_01_16

#include <weave/graph/GraphDescriptor.h>
#include <weave/user/Module.h>
#include <weave/user/BufferData.h>
#include <weave/graph/Node.h>
#include <weave/graph/Edge.h>

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
			using ContextTuple = std::tuple<typename user::Module<typename NodeDescriptors::Tag>::ContextType...>;
		};

		template<typename... EdgeDescriptors>
		struct DescriptorListToContextTuple<EdgeDescriptorList<EdgeDescriptors...> >
		{
			using ContextTuple = std::tuple<typename user::BufferData<typename EdgeDescriptors::Tag>::ContextType...>;
		};

		// GraphDescriptorToContextTuples
		template<typename GraphDescriptorType>
		struct GraphDescriptorToContextTuples;

		template<typename... NodeDescriptors, typename... EdgeDescriptors>
		struct GraphDescriptorToContextTuples<GraphDescriptor<NodeDescriptorList<NodeDescriptors...>, EdgeDescriptorList<EdgeDescriptors...> > >
		{
			using NodeContextTuple = std::tuple<typename user::Module<typename NodeDescriptors::Tag>::ContextType...>;
			using EdgeContextTuple = std::tuple<typename user::BufferData<typename EdgeDescriptors::Tag>::ContextType...>;
		};

		// GraphDescriptorToPipelineTuples
		template<typename GraphDescriptorType>
		struct GraphDescriptorToGraphTuples;

		template<typename... NodeDescriptors, typename... EdgeDescriptors>
		struct GraphDescriptorToGraphTuples<GraphDescriptor<NodeDescriptorList<NodeDescriptors...>, EdgeDescriptorList<EdgeDescriptors...> > >
		{
			using NodesTuple = std::tuple<Node<NodeDescriptors>...>;
			using EdgesTuple = std::tuple<Edge<EdgeDescriptors>...>;
		};
	}
}

#endif
