// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef BUILDER_H_2025_10_05_15_26_29
#define BUILDER_H_2025_10_05_15_26_29

#include <cstdlib>
#include <tuple>
#include <weave/core/GraphDescriptor.h>
#include <weave/core/NodeDescriptor.h>
#include <weave/core/EdgeDescriptor.h>
#include <weave/core/Graph.h>
#include <weave/core/Utilities.h>
#include <weave/user/Module.h>
#include <weave/user/BufferData.h>

namespace weave
{
	namespace core
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
			template<typename ModuleTag, uint64_t numInputs, uint64_t numOutputs>
			Builder<typename AppendToList<NodeDescriptorListType, NodeDescriptor<ModuleTag, numInputs, numOutputs> >::NewDescriptorListType, EdgeDescriptorListType> addNode(const typename user::Module<ModuleTag>::ContextType& context)
			{
				Builder<typename AppendToList<NodeDescriptorListType, NodeDescriptor<ModuleTag, numInputs, numOutputs> >::NewDescriptorListType, EdgeDescriptorListType> newBuilder(std::tuple_cat(_nodeContexts, std::make_tuple(context)), _edgeContexts);
				return newBuilder;
			}

			template<typename BufferDataTag, typename FromModule, typename ToModule>
			Builder<NodeDescriptorListType, typename AppendToList<EdgeDescriptorListType, EdgeDescriptor<BufferDataTag, FromModule, ToModule> >::NewDescriptorListType> addEdge(const typename user::BufferData<BufferDataTag>::ContextType& context)
			{
				Builder<NodeDescriptorListType, typename AppendToList<EdgeDescriptorListType, EdgeDescriptor<BufferDataTag, FromModule, ToModule> >::NewDescriptorListType> newBuilder(_nodeContexts, std::tuple_cat(_edgeContexts, std::make_tuple(context)));
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
