// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#include "specializations/SlotSpecialization.h"
#include "specializations/RingBufferTraitsSpecialization.h"
#include "specializations/ChannelTraitsSpecialization.h"
#include "specializations/EdgeTraitsSpecialization.h"

#include "specializations/ProcessorTraitsSpecialization.h"
#include "specializations/SynchronizerTraitsSpecialization.h"
#include "specializations/WorkerTraitsSpecialization.h"
#include "specializations/NodeTraitsSpecialization.h"

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
	                .addNode<FirstCapturerNode>(imageCapturerContext)
	                .addNode<FirstNormalizerNode>(imageNormalizerContext)
	                .addNode<FirstDisplayerNode>(imageDisplayerContext)
	                .addEdge<FirstImageEdge, FirstCapturerNode, FirstNormalizerNode, 16>(rawImageBufferContext)
					.addEdge<SecondImageEdge, FirstNormalizerNode, FirstDisplayerNode, 16>(processedImageBufferContext)
	                .build();

	pipeline.start();
	utilities::DisplayBridge::flushFrames();

	return 0;
}
