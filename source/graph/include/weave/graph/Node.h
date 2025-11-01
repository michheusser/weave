// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef NODE_H_2025_10_05_14_42_39
#define NODE_H_2025_10_05_14_42_39

#include <weave/buffer/Channel.h>
#include <weave/graph/Edge.h>
#include <weave/worker/Worker.h>

namespace weave
{
	namespace graph
	{
		template<typename NodeDescriptorType>
		struct ExtractNodeDescriptorParams;
		template<typename NodeTag, typename ModuleType>
		struct ExtractNodeDescriptorParams<NodeDescriptor<NodeTag, ModuleType> >
		{
			using Tag = NodeTag;
			using Module = ModuleType;
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
		template<typename EdgeTag, typename FromNodeTag, typename ToNodeTag, size_t numSlots>
		struct ExtractEdgeParams<Edge<EdgeDescriptor<EdgeTag, FromNodeTag, ToNodeTag, numSlots> >&>
		{
			using Tag = EdgeTag;
			using FromNode = FromNodeTag;
			using ToNode = ToNodeTag;
			static constexpr size_t slots = numSlots;
			using ChannelTag = Edge<EdgeDescriptor<Tag, FromNode, ToNode, slots> >::ChannelTag;
		};

		template<typename EdgeTupleType>
		struct EdgeTupleToChannelTuple;
		template<typename... Edges>
		struct EdgeTupleToChannelTuple<std::tuple<Edges...> >
		{
			using ChannelTuple = std::tuple<buffer::Channel<typename ExtractEdgeParams<Edges>::ChannelTag, Edge<typename ExtractEdgeDescriptor<Edges>::Descriptor>::policy, ExtractEdgeParams<
				Edges>::slots>&...>;
		};

		template<typename T>
		struct ShowType;

		template<typename NodeDescriptorType>
		class Node
		{
		public:
			using NodeTag = typename ExtractNodeDescriptorParams<NodeDescriptorType>::Tag;
			using ModuleType = typename ExtractNodeDescriptorParams<NodeDescriptorType>::Module;
			using WorkerTag = NodeTag;
			using ContextType = typename worker::Worker<WorkerTag, ModuleType>::ContextType;
			explicit Node(ContextType& context) : _worker(context)
			{}

			template<typename InEdgesTupleType, typename OutEdgesTupleType>
			void start(InEdgesTupleType& inEdges, OutEdgesTupleType& outEdges)
			{
				try
				{
					using InChannelTupleType = typename EdgeTupleToChannelTuple<InEdgesTupleType>::ChannelTuple;
					InChannelTupleType inChannels = _constructTuple<InChannelTupleType>(inEdges, std::make_index_sequence<std::tuple_size_v<InEdgesTupleType> >());

					using OutChannelTupleType = typename EdgeTupleToChannelTuple<OutEdgesTupleType>::ChannelTuple;
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
			ChannelTupleType _constructTuple(EdgeTupleType& edges, std::index_sequence<Indices...> sequence) noexcept
			{
				// We pass a dummy object to deduce the indices (not the nicest, but seems to be the standard C++ idiom).
				// Passing it as template parameter requires too many unnecessary helper constructs.
				return ChannelTupleType(std::get<Indices>(edges).getChannel()...);
			}
			worker::Worker<WorkerTag, ModuleType> _worker;
		};
	}
}

#endif
