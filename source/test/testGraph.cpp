// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#include "../Builder.h"

#include "../../Processing/Worker/Constants.h"
#include "../../IO/Constants.h"
#include "../../Processing//Modules/Preprocessor.h"
#include "../../Error/Exception.h"
#include "../../Processing/Worker/Worker.h"

#include "UserSpecializations.h"

using namespace test;

int main()
{
	// Nodes
	IO::CapturerConfiguration capturerConfiguration;
	capturerConfiguration.cameraID = IO::Constants::CAMERA_ID;
	capturerConfiguration.captureAPI = IO::Constants::OPENCV_VIDEO_CAPTURE_API;
	Processing::Worker::Module::Context<Processing::Worker::Constants::Module::Capturer> capturerContext = {capturerConfiguration};

	Processing::PreprocessorConfiguration preprocessorConfiguration;
	preprocessorConfiguration.destinationWidth = Processing::Constants::FRAME_WIDTH;
	preprocessorConfiguration.destinationHeight = Processing::Constants::FRAME_HEIGHT;
	Processing::Worker::Module::Context<Processing::Worker::Constants::Module::ImageProcessor> preprocessorContext = {preprocessorConfiguration};

	IO::DisplayerConfiguration displayerConfiguration;
	displayerConfiguration.title = std::string(IO::Constants::DEFAULT_DISPLAY_TITLE);
	Processing::Worker::Module::Context<Processing::Worker::Constants::Module::Displayer> displayerContext = {displayerConfiguration};


	// Edges
	Processing::ImageBufferConfiguration rawImageBufferConfiguration;
	rawImageBufferConfiguration.frameHeight = Processing::Constants::RAW_FRAME_HEIGHT;
	rawImageBufferConfiguration.frameWidth = Processing::Constants::RAW_FRAME_WIDTH;
	rawImageBufferConfiguration.frameType = Processing::Constants::RAW_FRAME_TYPE;

	Processing::ImageBufferConfiguration processedImageBufferConfiguration;
	processedImageBufferConfiguration.frameHeight = Processing::Constants::FRAME_HEIGHT;
	processedImageBufferConfiguration.frameWidth = Processing::Constants::FRAME_WIDTH;
	processedImageBufferConfiguration.frameType = Processing::Constants::FRAME_TYPE;

	// Build
	auto pipeline = Graph::Builder()
		                .addNode<ImageCapturer, 0, 1>(capturerContext)
		                .addNode<ImageProcessor, 1, 1>(preprocessorContext)
		                .addNode<ImageDisplayer, 1, 0>(displayerContext)
		                .addEdge<ImageData, ImageCapturer, ImageProcessor>(rawImageBufferConfiguration)
						.addEdge<ImageData, ImageProcessor, ImageDisplayer>(processedImageBufferConfiguration)
		                .build();

	//pipeline.start();
	return 0;
}
