// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef CONSTANTS_H_2025_09_20_09_39_36
#define CONSTANTS_H_2025_09_20_09_39_36

namespace weave
{
	worker
	{
		namespace Worker
		{
			namespace Constants
			{
				enum class Type
				{
					Source,
					Process,
					Sink
				};

				enum class Module
				{
					Capturer,
					Displayer,
					ImageProcessor,
					ImageToTensor,
					Inference,
					ClientSender,
					ClientReceiver,
					ServerSender,
					ServerReceiver
				};
			}
		}
	}
}

#endif
