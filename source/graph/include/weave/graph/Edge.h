// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef EDGE_H_2025_10_05_14_42_57
#define EDGE_H_2025_10_05_14_42_57

#include <weave/buffer/Channel.h>
#include <weave/buffer/Constants.h>

namespace weave
{
	namespace graph
	{
		template<typename EdgeDescriptorType>
		struct ExtractEdgeDescriptorParams;
		template<typename EdgeTag, typename FromNodeTag, typename ToNodeTag, size_t numSlots>
		struct ExtractEdgeDescriptorParams<EdgeDescriptor<EdgeTag, FromNodeTag, ToNodeTag, numSlots> >
		{
			using Tag = EdgeTag;
			using FromNode = FromNodeTag;
			using ToNode = ToNodeTag;
			static constexpr size_t slots = numSlots;
		};

		template<typename T>
		struct ShowType;

		template<typename EdgeDescriptorType>
		class Edge
		{
		public:
			static constexpr size_t numSlots = ExtractEdgeDescriptorParams<EdgeDescriptorType>::slots;
			static constexpr buffer::constants::PolicyType policy = buffer::constants::PolicyType::Lossless;

			using EdgeTag = typename ExtractEdgeDescriptorParams<EdgeDescriptorType>::Tag;
			using ChannelTag = EdgeTag;
			using ContextType = typename buffer::Channel<ChannelTag, policy, numSlots>::ContextType;
			explicit Edge(ContextType& context) : _channel(context)
			{}

			// Edges contain channels, which contain synchronization artifacts and thus not copyable/movable. Even though the compiler deletes per default, we delete here explicitly to be more expressive and clear.
			Edge(const Edge&) = delete;
			Edge& operator=(const Edge&) = delete;
			Edge(Edge&&) = delete;
			Edge& operator=(Edge&&) = delete;

			buffer::Channel<ChannelTag, policy, numSlots>& getChannel() noexcept // TODO Decide if this is the best solution
			{
				return _channel;
			}

			void start()
			{
				// TODO Decide if initialization should happen here or in constructor of Channel
			}

			// TODO Later substitute by multiplexer with several buffers!
			buffer::Channel<ChannelTag, policy, numSlots> _channel;
		};
	}
}

#endif
