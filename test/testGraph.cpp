// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

//#include "Specializations/ModuleSpecialization.h"
//#include "Specializations/ProcessorSpecialization.h"
#include "Specializations/SlotSpecialization.h"
#include "Specializations/RingBufferTraitsSpecialization.h"
#include "Specializations/ChannelTraitsSpecialization.h"
#include "Specializations/EdgeTraitsSpecialization.h"

#include "Specializations/ProcessorTraitsSpecialization.h"
#include "Specializations/ProcessorSpecialization.h"
#include "Specializations/SynchronizerTraitsSpecialization.h"
#include "Specializations/WorkerTraitsSpecialization.h"
#include "Specializations/NodeTraitsSpecialization.h"

#include <weave/graph/Builder.h>


#include "modules/Constants.h"
#include "modules/Configuration.h"
#include "modules/Context.h"

#include "buffer//Constants.h"
#include "buffer/Configuration.h"
#include "buffer/Context.h"

#include "utilities/DisplayBridge.h"



using namespace test;

int main()
{
	// Nodes
	module::Context<module::constants::ModuleType::ImageCapturer> imageCapturerContext;
	imageCapturerContext.configuration.cameraID = module::constants::CAMERA_ID;
	imageCapturerContext.configuration.captureAPI = module::constants::OPENCV_VIDEO_CAPTURE_API;

	module::Context<module::constants::ModuleType::ImageNormalizer> imageNormalizerContext;
	imageNormalizerContext.configuration.destinationWidth = module::constants::FRAME_WIDTH;
	imageNormalizerContext.configuration.destinationHeight = module::constants::FRAME_HEIGHT;

	module::Context<module::constants::ModuleType::ImageDisplayer> imageDisplayerContext;
	imageDisplayerContext.configuration.title = std::string(module::constants::DEFAULT_DISPLAY_TITLE);

	// Edges
	buffer::Context<buffer::constants::BufferType::Image> rawImageBufferContext;
	rawImageBufferContext.configuration.frameHeight = buffer::constants::RAW_FRAME_HEIGHT;
	rawImageBufferContext.configuration.frameWidth = buffer::constants::RAW_FRAME_WIDTH;
	rawImageBufferContext.configuration.frameType = buffer::constants::RAW_FRAME_TYPE;

	buffer::Context<buffer::constants::BufferType::Image> processedImageBufferContext;
	processedImageBufferContext.configuration.frameHeight = buffer::constants::FRAME_HEIGHT;
	processedImageBufferContext.configuration.frameWidth = buffer::constants::FRAME_WIDTH;
	processedImageBufferContext.configuration.frameType = buffer::constants::FRAME_TYPE;

	// Build
	auto pipeline = weave::graph::Builder()
	                .addNode<FirstCapturerNode, 0, 1>(imageCapturerContext)
	                .addNode<FirstNormalizerNode, 1, 1>(imageNormalizerContext)
	                .addNode<FirstDisplayerNode, 1, 0>(imageDisplayerContext)
	                .addEdge<FirstImageEdge, FirstCapturerNode, FirstNormalizerNode>(rawImageBufferContext)
					.addEdge<SecondImageEdge, FirstNormalizerNode, FirstDisplayerNode>(processedImageBufferContext)
	                .build();

	pipeline.start();
	utilities::DisplayBridge::flushFrames();

	return 0;
}
