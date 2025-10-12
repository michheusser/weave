// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef EDGE_H_2025_10_05_14_42_57
#define EDGE_H_2025_10_05_14_42_57

namespace weave
{
	namespace graph
	{
		template<typename NodeDescriptorType>
		struct ExtractEdgeDescriptorParams;

		template<typename EdgeTag, typename FromNodeTag, typename ToNodeTag>
		struct ExtractEdgeDescriptorParams<EdgeDescriptor<EdgeTag, FromNodeTag, ToNodeTag> >
		{
			using Tag = EdgeTag;
			using FromModule = FromNodeTag;
			using ToModule = ToNodeTag;
		};

		template<typename EdgeDescriptorType>
		class Edge
		{
			// TODO MAYBE ITS TIME TO START FRESH AND TRANSPORT EVERYTHING TO NEW REPOSITORY!!!
		public:
			// TODO Move buffer class with generic tags to Graph
			explicit Edge(typename user::BufferData<typename ExtractEdgeDescriptorParams<EdgeDescriptorType>::Tag>::ContextType context)// : _buffer(context)
			{
				// TODO
			}

			void start()
			{
				// TODO
			}

			// TODO Later substitute by multiplexer with several buffers!
			//weave::buffer::Buffer<ExtractEdgeDescriptorParams<EdgeDescriptorType>::Tag, buffer::Constants::PolicyType::Lossless> _buffer;
		};
	}
}

#endif
