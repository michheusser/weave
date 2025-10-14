// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef NODE_H_2025_10_05_14_42_39
#define NODE_H_2025_10_05_14_42_39

#include <weave/user/Module.h>

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

		template<typename NodeDescriptorType>
		class Node
		{
		public:
			// TODO Move worker class with generic tags to Graph
			explicit Node(typename user::Module<typename ExtractNodeDescriptorParams<NodeDescriptorType>::Tag>::ContextType context) //: _worker(context) // TODO
			{
				// TODO
			}

			template<typename InEdgesTuple, typename OutEdgesTuple>
			void start(InEdgesTuple& inEdges, OutEdgesTuple& outEdges)
			{
				// TODO
			}
			//weave::worker::Worker::Worker<ExtractNodeDescriptorParams<NodeDescriptorType>::Tag> _worker;
		};
	}
}

#endif
