// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef NODE_H_2025_10_05_14_42_39
#define NODE_H_2025_10_05_14_42_39

#include <weave/buffer/Channel.h>
#include <weave/graph/Edge.h>
#include <weave/worker/Worker.h>
#include <weave/user/NodeTraits.h>

namespace weave
{
	namespace graph
	{
		template<typename NodeDescriptorType>
		struct ExtractNodeDescriptorParams;
		template<typename NodeTag, uint64_t NumInputs, uint64_t NumOutputs>
		struct ExtractNodeDescriptorParams<NodeDescriptor<NodeTag, NumInputs, NumOutputs> >
		{
			using Tag = NodeTag;
			static constexpr uint64_t inputs = NumInputs;
			static constexpr uint64_t outputs = NumOutputs;
		};

		template<typename Edge>
		struct ExtractEdgeDescriptor;
		template<typename EdgeDescriptorType>
		struct ExtractEdgeDescriptor<Edge<EdgeDescriptorType>&>
		{
			using Descriptor = EdgeDescriptorType;
		};

		template<typename EdgeType>
		struct ExtractEdgeParams;
		template<typename EdgeTag, typename FromNodeTag, typename ToNodeTag>
		struct ExtractEdgeParams<Edge<EdgeDescriptor<EdgeTag, FromNodeTag, ToNodeTag> >&>
		{
			using Tag = EdgeTag;
			using FromNode = FromNodeTag;
			using ToNode = ToNodeTag;
		};

		template<typename EdgeTupleType>
		struct EdgeTupleToChannelTuple;
		template<typename... Edges>
		struct EdgeTupleToChannelTuple<std::tuple<Edges...> >
		{
			using ChannelTuple = std::tuple<buffer::Channel<typename user::EdgeTraits<typename ExtractEdgeParams<Edges>::Tag>::ChannelTag, Edge<typename ExtractEdgeDescriptor<Edges>::Descriptor>::policy>&...>;
		};

		template<typename T>
		struct ShowType;

		template<typename NodeDescriptorType>
		class Node
		{
		public:
			// TODO Move worker class with generic tags to Graph
			using NodeTag = typename ExtractNodeDescriptorParams<NodeDescriptorType>::Tag;
			using WorkerTag = typename user::NodeTraits<NodeTag>::WorkerTag;
			explicit Node(typename user::NodeTraits<NodeTag>::ContextType& context) : _worker(context)
			{}

			template<typename InEdgesTupleType, typename OutEdgesTupleType>
			void start(InEdgesTupleType& inEdges, OutEdgesTupleType& outEdges)
			{
				using InChannelTupleType = typename EdgeTupleToChannelTuple<InEdgesTupleType>::ChannelTuple;
				using OutChannelTupleType = typename EdgeTupleToChannelTuple<OutEdgesTupleType>::ChannelTuple;

				try
				{
					InChannelTupleType inChannels = _constructTuple<InChannelTupleType>(inEdges, std::make_index_sequence<std::tuple_size_v<InEdgesTupleType> >());
					OutChannelTupleType outChannels = _constructTuple<OutChannelTupleType>(outEdges, std::make_index_sequence<std::tuple_size_v<OutEdgesTupleType> >());
					_worker.start(inChannels, outChannels);
				}
				catch (error::Exception& exception)
				{
					LOG_ERROR(exception.what());
					throw error::Exception("Failed");
				}
			}

		private:
			template<typename ChannelTupleType, typename EdgeTupleType, std::size_t... Indices>
			ChannelTupleType _constructTuple(EdgeTupleType& edges, std::index_sequence<Indices...> sequence)
			{
				// We pass a dummy object to deduce the indices (not the nicest, but seems to be used a lot). Passing it as template parameter requires too many helper constructs.
				ChannelTupleType tuple = {std::tuple_element_t<Indices, ChannelTupleType>(std::get<Indices>(edges).getChannel())...}; // TODO NEED to expose getChannel?
				return tuple; // Move is not needed here, because we're passing tuple of references!
			}
			worker::Worker<WorkerTag> _worker;
		};
	}
}

#endif
