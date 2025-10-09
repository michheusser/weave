// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef NODE_H_2025_10_05_14_42_39
#define NODE_H_2025_10_05_14_42_39

#include "Constants.h"
#include "../Processing/Worker/Worker.h"
#include "Utilities.h"
#include "Module.h"

namespace weave
{
		// TODO MAYBE ITS TIME TO START FRESH AND TRANSPORT EVERYTHING TO NEW REPOSITORY!!!
		template<typename NodeDescriptorType>
		class Node
		{
		public:
			// TODO Move worker class with generic tags to Graph
			explicit Node(typename Module<typename ExtractNodeDescriptorParams<NodeDescriptorType>::Tag>::ContextType context) : _worker(context)
			{
				// TODO
			}
			void start()
			{
				// TODO
			}
			weave::Processing::Worker::Worker<ExtractNodeDescriptorParams<NodeDescriptorType>::Tag> _worker;
		};
	}

#endif
