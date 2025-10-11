// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef EDGE_H_2025_10_05_14_42_57
#define EDGE_H_2025_10_05_14_42_57

namespace weave
{
	namespace core
	{
		template<typename EdgeDescriptorType>
		class Edge
		{
			// TODO MAYBE ITS TIME TO START FRESH AND TRANSPORT EVERYTHING TO NEW REPOSITORY!!!
		public:
			// TODO Move buffer class with generic tags to Graph
			explicit Edge(typename Module<typename ExtractEdgeDescriptorParams<EdgeDescriptorType>::Tag>::ContextType context) : _buffer(context)
			{
				// TODO
			}

			void start()
			{
				// TODO
			}

			// TODO Later substitute by multiplexer with several buffers!
			weave::Buffering::Buffer<ExtractEdgeDescriptorParams<EdgeDescriptorType>::Tag, Buffering::Constants::PolicyType::Lossless> _buffer;
		};
	}
}

#endif
