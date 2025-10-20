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
	// FirstCapturerNode
	module::Context<module::constants::ModuleType::ImageCapturer> firtImageCapturerContext;
	firtImageCapturerContext.configuration.cameraID = module::constants::CAMERA_ID;
	firtImageCapturerContext.configuration.captureAPI = module::constants::OPENCV_VIDEO_CAPTURE_API;

	// RawImageEdge
	buffer::Context<buffer::constants::BufferType::Image> rawImageBufferContext;
	rawImageBufferContext.configuration.frameHeight = module::constants::RAW_FRAME_HEIGHT;
	rawImageBufferContext.configuration.frameWidth = module::constants::RAW_FRAME_WIDTH;
	rawImageBufferContext.configuration.frameType = module::constants::RAW_FRAME_TYPE;

	// FirstNormalizerNode
	module::Context<module::constants::ModuleType::ImageNormalizer> firstImageNormalizerContext;
	firstImageNormalizerContext.configuration.destinationWidth = module::constants::FRAME_WIDTH;
	firstImageNormalizerContext.configuration.destinationHeight = module::constants::FRAME_HEIGHT;

	// NormalizedImageEdge
	buffer::Context<buffer::constants::BufferType::Image> firstNormalizedImageContext;
	firstNormalizedImageContext.configuration.frameHeight = module::constants::FRAME_HEIGHT;
	firstNormalizedImageContext.configuration.frameWidth = module::constants::FRAME_WIDTH;
	firstNormalizedImageContext.configuration.frameType = module::constants::FRAME_TYPE;

	// FirstInferenceInputProcessorNode
	module::Context<module::constants::ModuleType::InferenceInputProcessor> firstInferenceInputProcessorContext;
	firstInferenceInputProcessorContext.configuration.rtype = module::constants::INPUT_FRAME_RTYPE;
	firstInferenceInputProcessorContext.configuration.alpha = module::constants::INPUT_FRAME_ALPHA;
	firstInferenceInputProcessorContext.configuration.inputWidth = module::constants::INPUT_FRAME_WIDTH;
	firstInferenceInputProcessorContext.configuration.inputHeight = module::constants::INPUT_FRAME_HEIGHT;
	firstInferenceInputProcessorContext.configuration.mean = module::constants::INPUT_FRAME_MEAN;
	firstInferenceInputProcessorContext.configuration.std = module::constants::INPUT_FRAME_STD;

	// InferenceInputTensorEdge
	buffer::Context<buffer::constants::BufferType::InferenceInputTensor> inferenceInputTensorContext;
	inferenceInputTensorContext.configuration.height = module::constants::TENSOR_HEIGHT;
	inferenceInputTensorContext.configuration.width = module::constants::TENSOR_WIDTH;
	inferenceInputTensorContext.configuration.tensorAllocatorType = module::constants::TENSOR_ALLOCATOR_TYPE;
	inferenceInputTensorContext.configuration.tensorMemoryType = module::constants::TENSOR_MEMORY_TYPE;

	// FirstInferenceModelNode
	module::Context<module::constants::ModuleType::InferenceModel> firstInferenceModelContext;
	firstInferenceModelContext.configuration.loggingLevel = module::constants::ONNX_ENV_LOGGING_LEVEL;
	firstInferenceModelContext.configuration.logid = module::constants::ONNX_ENV_LOGID;
	firstInferenceModelContext.configuration.modelPath = module::constants::ONNX_MODEL_PATH;
	firstInferenceModelContext.configuration.modelFile = module::constants::ONNX_MODEL_FILE;
	firstInferenceModelContext.configuration.outputImageType = module::constants::OUTPUT_IMAGE_TYPE;
	firstInferenceModelContext.configuration.width = module::constants::OUTPUT_IMAGE_WIDTH;
	firstInferenceModelContext.configuration.height = module::constants::OUTPUT_IMAGE_HEIGHT;
	firstInferenceModelContext.configuration.normalizedAlpha = module::constants::OUTPUT_NORM_ALPHA;
	firstInferenceModelContext.configuration.normalizedBeta = module::constants::OUTPUT_NORM_BETA;
	firstInferenceModelContext.configuration.normalizeType = module::constants::OUTPUT_NORM_TYPE;
	firstInferenceModelContext.configuration.normalizeDType = module::constants::OUTPUT_NORM_DTYPE;

	// InferenceOutputImageEdge
	buffer::Context<buffer::constants::BufferType::Image> inferenceOutputImageContext;
	inferenceOutputImageContext.configuration.frameHeight = module::constants::FRAME_HEIGHT;
	inferenceOutputImageContext.configuration.frameWidth = module::constants::FRAME_WIDTH;
	inferenceOutputImageContext.configuration.frameType = module::constants::FRAME_TYPE;

	// SecondNormalizerNode
	module::Context<module::constants::ModuleType::ImageNormalizer> secondImageNormalizerContext;
	secondImageNormalizerContext.configuration.destinationWidth = module::constants::RAW_FRAME_WIDTH;
	secondImageNormalizerContext.configuration.destinationHeight = module::constants::RAW_FRAME_HEIGHT;

	// SecondNormalizedImageEdge
	buffer::Context<buffer::constants::BufferType::Image> secondNormalizedImageContext;
	secondNormalizedImageContext.configuration.frameHeight = module::constants::RAW_FRAME_HEIGHT;
	secondNormalizedImageContext.configuration.frameWidth = module::constants::RAW_FRAME_WIDTH;
	secondNormalizedImageContext.configuration.frameType = module::constants::RAW_FRAME_TYPE;

	// FirstDisplayerNode
	module::Context<module::constants::ModuleType::ImageDisplayer> firstImageDisplayerContext;
	firstImageDisplayerContext.configuration.title = std::string(module::constants::DEFAULT_DISPLAY_TITLE);

	// Build
	auto pipeline = weave::graph::Builder()
		.addNode<FirstCapturerNode>(firtImageCapturerContext).addEdge<FirstRawImageEdge, FirstCapturerNode, FirstNormalizerNode, 16>(rawImageBufferContext)
		.addNode<FirstNormalizerNode>(firstImageNormalizerContext)
		.addEdge<FirstNormalizedImageEdge, FirstNormalizerNode, FirstInferenceInputProcessorNode, 16>(firstNormalizedImageContext)
		.addNode<FirstInferenceInputProcessorNode>(firstInferenceInputProcessorContext)
		.addEdge<FirstInferenceInputTensorEdge, FirstInferenceInputProcessorNode, FirstInferenceModelNode,16>(inferenceInputTensorContext)
		.addNode<FirstInferenceModelNode>(firstInferenceModelContext)
		.addEdge<FirstInferenceOutputImageEdge, FirstInferenceModelNode, SecondNormalizerNode,16>(inferenceOutputImageContext)
		.addNode<SecondNormalizerNode>(secondImageNormalizerContext)
		.addEdge<SecondNormalizedImageEdge, SecondNormalizerNode, FirstDisplayerNode,16>(secondNormalizedImageContext)
		.addNode<FirstDisplayerNode>(firstImageDisplayerContext)
		.build();

	pipeline.start();
	utilities::DisplayBridge::flushFrames();

	return 0;
}
