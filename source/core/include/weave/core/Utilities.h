// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef UTILITIES_H_2025_10_08_15_01_16
#define UTILITIES_H_2025_10_08_15_01_16

#include <weave/core/GraphDescriptor.h>
#include <weave/user/Module.h>
#include <weave/user/BufferData.h>

namespace weave
{
	namespace core
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
			using NodesTuple = std::tuple<typename user::Module<typename NodeDescriptors::Tag>::ContextType...>;
			using EdgesTuple = std::tuple<typename user::BufferData<typename EdgeDescriptors::Tag>::ContextType...>;
		};

		//
		template<typename NodeDescriptorType>
		struct ExtractNodeDescriptorParams;

		template<typename ModuleTag, uint64_t NumInputs, uint64_t NumOutputs>
		struct ExtractNodeDescriptorParams<NodeDescriptor<ModuleTag, NumInputs, NumOutputs> >
		{
			using Tag = ModuleTag;
			static constexpr uint64_t inputs = NumInputs;
			static constexpr uint64_t outputs = NumOutputs;
		};

		template<typename NodeDescriptorType>
		struct ExtractEdgeDescriptorParams;

		template<typename BufferDataTag, typename FromModuleTag, typename ToModuleTag>
		struct ExtractEdgeDescriptorParams<EdgeDescriptor<BufferDataTag, FromModuleTag, ToModuleTag> >
		{
			using Tag = BufferDataTag;
			using FromModule = FromModuleTag;
			using ToModule = ToModuleTag;
		};
	}
}

#endif
