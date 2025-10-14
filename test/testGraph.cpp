// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

//#include "Specializations/BufferDataSpecializations.h" // TODO Remove
#include "Specializations/ModuleSpecializations.h"
#include "Specializations/SlotSpecialization.h"
#include "Specializations/RingBufferTraitsSpecialization.h"
#include "Specializations/EdgeTraitsSpecialization.h"

#include <weave/graph/Builder.h>


#include "modules/Constants.h"
#include "modules/Configuration.h"
#include "modules/Context.h"

#include "buffer//Constants.h"
#include "buffer/Configuration.h"
#include "buffer/Context.h"



using namespace test;

int main()
{
	// Nodes
	module::Context<module::constants::ModuleType::ImageCapturer> imageCapturerContext;
	imageCapturerContext.capturerConfiguration.cameraID = module::constants::CAMERA_ID;
	imageCapturerContext.capturerConfiguration.captureAPI = module::constants::OPENCV_VIDEO_CAPTURE_API;

	module::Context<module::constants::ModuleType::ImageNormalizer> imageNormalizerContext;
	imageNormalizerContext.processorConfiguration.destinationWidth = module::constants::FRAME_WIDTH;
	imageNormalizerContext.processorConfiguration.destinationHeight = module::constants::FRAME_HEIGHT;

	module::Context<module::constants::ModuleType::ImageDisplayer> imageDisplayerContext;
	imageDisplayerContext.displayerConfiguration.title = std::string(module::constants::DEFAULT_DISPLAY_TITLE);

	// Edges
	buffer::Context<buffer::constants::BufferType::Image> rawImageBufferContext;
	rawImageBufferContext.configuration.frameHeight = buffer::constants::RAW_FRAME_HEIGHT;
	rawImageBufferContext.configuration.frameWidth = buffer::constants::RAW_FRAME_WIDTH;
	rawImageBufferContext.configuration.frameType = buffer::constants::RAW_FRAME_TYPE;

	buffer::Context<buffer::constants::BufferType::Image> processedImageBufferContext;
	rawImageBufferContext.configuration.frameHeight = buffer::constants::FRAME_HEIGHT;
	rawImageBufferContext.configuration.frameWidth = buffer::constants::FRAME_WIDTH;
	rawImageBufferContext.configuration.frameType = buffer::constants::FRAME_TYPE;

	// Build
	auto pipeline = weave::graph::Builder()
	                .addNode<FirstImageCapturer, 0, 1>(imageCapturerContext)
	                .addNode<FirstImageProcessor, 1, 1>(imageNormalizerContext)
	                .addNode<FirstImageDisplayer, 1, 0>(imageDisplayerContext)
	                .addEdge<FirstImageEdge, FirstImageCapturer, FirstImageProcessor>(rawImageBufferContext)
					.addEdge<SecondImageEdge, FirstImageProcessor, FirstImageDisplayer>(processedImageBufferContext)
	                .build();

	pipeline.start();
	return 0;
}
