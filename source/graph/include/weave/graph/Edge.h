// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef EDGE_H_2025_10_05_14_42_57
#define EDGE_H_2025_10_05_14_42_57

// #include <weave/user/BufferData.h> // TODO Remove
#include <weave/buffer/Channel.h>
#include <weave/buffer/Constants.h>
#include <weave/user/EdgeTraits.h>

namespace weave
{
	namespace graph
	{
		template<typename EdgeDescriptorType>
		struct ExtractEdgeDescriptorParams;

		template<typename EdgeTag, typename FromNodeTag, typename ToNodeTag>
		struct ExtractEdgeDescriptorParams<EdgeDescriptor<EdgeTag, FromNodeTag, ToNodeTag> >
		{
			using Tag = EdgeTag;
			using FromNode = FromNodeTag;
			using ToNode = ToNodeTag;
		};

		template<typename T>
		struct ShowType;

		template<typename EdgeDescriptorType>
		class Edge
		{
		public:
			using EdgeTag = typename ExtractEdgeDescriptorParams<EdgeDescriptorType>::Tag;
			using ChannelTag = typename user::EdgeTraits<EdgeTag>::ChannelTag;
			explicit Edge(typename user::EdgeTraits<typename ExtractEdgeDescriptorParams<EdgeDescriptorType>::Tag>::ContextType& context) : _channel(context)
			{}

			void start()
			{
				// TODO
			}

			// TODO Later substitute by multiplexer with several buffers!
			buffer::Channel<ChannelTag, buffer::constants::PolicyType::Lossless> _channel;
		};
	}
}

#endif
