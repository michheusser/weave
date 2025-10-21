// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#include "specializations/ProcessorTraitsSpecialization.h"
#include "specializations/SlotSpecialization.h"
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
	/***** NETWORK *****/
	streaming::NetworkClientConfiguration networkClientConfiguration;
	networkClientConfiguration.localAddress = streaming::constants::CLIENT_LOCAL_ADDRESS;
	networkClientConfiguration.localPort = streaming::constants::CLIENT_LOCAL_PORT;
	networkClientConfiguration.remoteAddress = streaming::constants::CLIENT_REMOTE_ADDRESS;
	networkClientConfiguration.remotePort = streaming::constants::CLIENT_REMOTE_PORT;
	std::shared_ptr<streaming::NetworkClient> networkClient = std::make_shared<streaming::NetworkClient>(networkClientConfiguration);


	streaming::NetworkServerConfiguration networkServerConfiguration;
	networkServerConfiguration.localAddress = streaming::constants::SERVER_LOCAL_ADDRESS;
	networkServerConfiguration.localPort = streaming::constants::SERVER_LOCAL_PORT;
	std::shared_ptr<streaming::NetworkServer> networkServer = std::make_shared<streaming::NetworkServer>(networkServerConfiguration);

	/******* CLIENT *****/
	module::Context<module::constants::ModuleType::ImageCapturer> clientImageCapturerContext;
	clientImageCapturerContext.configuration.cameraID = module::constants::CAMERA_ID;
	clientImageCapturerContext.configuration.captureAPI = module::constants::OPENCV_VIDEO_CAPTURE_API;

	buffer::Context<buffer::constants::BufferType::Image> clientRawImageBufferContext;
	clientRawImageBufferContext.configuration.frameHeight = module::constants::RAW_FRAME_HEIGHT;
	clientRawImageBufferContext.configuration.frameWidth = module::constants::RAW_FRAME_WIDTH;
	clientRawImageBufferContext.configuration.frameType = module::constants::RAW_FRAME_TYPE;

	module::Context<module::constants::ModuleType::ImageNormalizer> clientFirstImageNormalizerContext;
	clientFirstImageNormalizerContext.configuration.destinationWidth = module::constants::FRAME_WIDTH;
	clientFirstImageNormalizerContext.configuration.destinationHeight = module::constants::FRAME_HEIGHT;

	buffer::Context<buffer::constants::BufferType::Image> clientImageSendBufferContext;
	clientImageSendBufferContext.configuration.frameHeight = module::constants::FRAME_HEIGHT;
	clientImageSendBufferContext.configuration.frameWidth = module::constants::FRAME_WIDTH;
	clientImageSendBufferContext.configuration.frameType = module::constants::FRAME_TYPE;

	module::Context<module::constants::ModuleType::ClientSender> clientImageSenderContext;
	clientImageSenderContext.networkBufferConfiguration.bufferSize = module::constants::BUFFER_SIZE;
	clientImageSenderContext.encoderConfiguration.encodingFormat = std::string(module::constants::ENCODING_FORMAT);
	clientImageSenderContext.encoderConfiguration.encodingParameters = {module::constants::COMPRESSION_IMWRITE_PARAM, module::constants::COMPRESSION_LEVEL};
	clientImageSenderContext.networkClient = networkClient;

	module::Context<module::constants::ModuleType::ClientReceiver> clientImageReceiverContext;
	clientImageReceiverContext.networkBufferConfiguration.bufferSize = module::constants::BUFFER_SIZE;
	clientImageReceiverContext.decoderConfiguration.flags = module::constants::DECODER_FLAGS;
	clientImageReceiverContext.networkClient = networkClient;

	buffer::Context<buffer::constants::BufferType::Image> clientImageReceiveBufferContext;
	clientImageReceiveBufferContext.configuration.frameHeight = module::constants::FRAME_HEIGHT;
	clientImageReceiveBufferContext.configuration.frameWidth = module::constants::FRAME_WIDTH;
	clientImageReceiveBufferContext.configuration.frameType = module::constants::FRAME_TYPE;

	module::Context<module::constants::ModuleType::ImageNormalizer> clientSecondImageNormalizerContext;
	clientSecondImageNormalizerContext.configuration.destinationWidth = module::constants::RAW_FRAME_WIDTH;
	clientSecondImageNormalizerContext.configuration.destinationHeight = module::constants::RAW_FRAME_HEIGHT;

	buffer::Context<buffer::constants::BufferType::Image> clientDisplayImageBufferContext;
	clientDisplayImageBufferContext.configuration.frameHeight = module::constants::RAW_FRAME_HEIGHT;
	clientDisplayImageBufferContext.configuration.frameWidth = module::constants::RAW_FRAME_WIDTH;
	clientDisplayImageBufferContext.configuration.frameType = module::constants::RAW_FRAME_TYPE;

	module::Context<module::constants::ModuleType::ImageDisplayer> clientImageDisplayerContext;
	clientImageDisplayerContext.configuration.title = std::string(module::constants::DEFAULT_DISPLAY_TITLE);



	/****** SERVER *******/

	module::Context<module::constants::ModuleType::ServerReceiver> serverImageReceiverContext;
	serverImageReceiverContext.networkBufferConfiguration.bufferSize = module::constants::BUFFER_SIZE;
	serverImageReceiverContext.decoderConfiguration.flags = module::constants::DECODER_FLAGS;
	serverImageReceiverContext.networkServer = networkServer;

	buffer::Context<buffer::constants::BufferType::Image> serverImageReceiveBufferContext;
	serverImageReceiveBufferContext.configuration.frameHeight = module::constants::FRAME_HEIGHT;
	serverImageReceiveBufferContext.configuration.frameWidth = module::constants::FRAME_WIDTH;
	serverImageReceiveBufferContext.configuration.frameType = module::constants::FRAME_TYPE;

	// FirstInferenceInputProcessorNode
	module::Context<module::constants::ModuleType::InferenceInputProcessor> serverInferenceInputProcessorContext;
	serverInferenceInputProcessorContext.configuration.rtype = module::constants::INPUT_FRAME_RTYPE;
	serverInferenceInputProcessorContext.configuration.alpha = module::constants::INPUT_FRAME_ALPHA;
	serverInferenceInputProcessorContext.configuration.inputWidth = module::constants::INPUT_FRAME_WIDTH;
	serverInferenceInputProcessorContext.configuration.inputHeight = module::constants::INPUT_FRAME_HEIGHT;
	serverInferenceInputProcessorContext.configuration.mean = module::constants::INPUT_FRAME_MEAN;
	serverInferenceInputProcessorContext.configuration.std = module::constants::INPUT_FRAME_STD;

	// InferenceInputTensorEdge
	buffer::Context<buffer::constants::BufferType::InferenceInputTensor> serverInferenceInputTensorContext;
	serverInferenceInputTensorContext.configuration.height = module::constants::TENSOR_HEIGHT;
	serverInferenceInputTensorContext.configuration.width = module::constants::TENSOR_WIDTH;
	serverInferenceInputTensorContext.configuration.tensorAllocatorType = module::constants::TENSOR_ALLOCATOR_TYPE;
	serverInferenceInputTensorContext.configuration.tensorMemoryType = module::constants::TENSOR_MEMORY_TYPE;

	// FirstInferenceModelNode
	module::Context<module::constants::ModuleType::InferenceModel> serverInferenceModelContext;
	serverInferenceModelContext.configuration.loggingLevel = module::constants::ONNX_ENV_LOGGING_LEVEL;
	serverInferenceModelContext.configuration.logid = module::constants::ONNX_ENV_LOGID;
	serverInferenceModelContext.configuration.modelPath = module::constants::ONNX_MODEL_PATH;
	serverInferenceModelContext.configuration.modelFile = module::constants::ONNX_MODEL_FILE;
	serverInferenceModelContext.configuration.outputImageType = module::constants::OUTPUT_IMAGE_TYPE;
	serverInferenceModelContext.configuration.width = module::constants::OUTPUT_IMAGE_WIDTH;
	serverInferenceModelContext.configuration.height = module::constants::OUTPUT_IMAGE_HEIGHT;
	serverInferenceModelContext.configuration.normalizedAlpha = module::constants::OUTPUT_NORM_ALPHA;
	serverInferenceModelContext.configuration.normalizedBeta = module::constants::OUTPUT_NORM_BETA;
	serverInferenceModelContext.configuration.normalizeType = module::constants::OUTPUT_NORM_TYPE;
	serverInferenceModelContext.configuration.normalizeDType = module::constants::OUTPUT_NORM_DTYPE;

	// InferenceOutputImageEdge
	buffer::Context<buffer::constants::BufferType::Image> serverImageSendBufferContext;
	serverImageSendBufferContext.configuration.frameHeight = module::constants::FRAME_HEIGHT;
	serverImageSendBufferContext.configuration.frameWidth = module::constants::FRAME_WIDTH;
	serverImageSendBufferContext.configuration.frameType = module::constants::FRAME_TYPE;

	module::Context<module::constants::ModuleType::ServerSender> serverImageSenderContext;
	serverImageSenderContext.networkBufferConfiguration.bufferSize = module::constants::BUFFER_SIZE;
	serverImageSenderContext.encoderConfiguration.encodingFormat = std::string(module::constants::ENCODING_FORMAT);
	serverImageSenderContext.encoderConfiguration.encodingParameters = {module::constants::COMPRESSION_IMWRITE_PARAM, module::constants::COMPRESSION_LEVEL};
	serverImageSenderContext.networkServer = networkServer;

	// Build
	auto clientPipeline = weave::graph::Builder()
		.addNode<ClientImageCapturer>(clientImageCapturerContext)
		.addEdge<ClientRawImageEdge, ClientImageCapturer, ClientFirstImageNormalizer, 16>(clientRawImageBufferContext)
		.addNode<ClientFirstImageNormalizer>(clientFirstImageNormalizerContext)
		.addEdge<ClientImageSendEdge, ClientFirstImageNormalizer, ClientImageSender, 16>(clientImageSendBufferContext)
		.addNode<ClientImageSender>(clientImageSenderContext)
		.addNode<ClientImageReceiver>(clientImageReceiverContext)
		.addEdge<ClientImageReceiveEdge, ClientImageReceiver, ClientSecondImageNormalizer, 16>(clientImageReceiveBufferContext)
		.addNode<ClientSecondImageNormalizer>(clientSecondImageNormalizerContext)
		.addEdge<ClientDisplayImageEdge, ClientSecondImageNormalizer, ClientImageDisplayer,16>(clientDisplayImageBufferContext)
		.addNode<ClientImageDisplayer>(clientImageDisplayerContext)
		.build();

	auto serverPipeline = weave::graph::Builder()
		.addNode<ServerImageReceiver>(serverImageReceiverContext)
		.addEdge<ServerImageReceiveEdge, ServerImageReceiver, ServerInferenceInputProcessor, 16>(serverImageReceiveBufferContext)
		.addNode<ServerInferenceInputProcessor>(serverInferenceInputProcessorContext)
		.addEdge<ServerInferenceInputTensorEdge, ServerInferenceInputProcessor, ServerInferenceModel,16>(serverInferenceInputTensorContext)
		.addNode<ServerInferenceModel>(serverInferenceModelContext)
		.addEdge<ServerImageSendEdge, ServerInferenceModel, ServerImageSender,16>(serverImageSendBufferContext)
		.addNode<ServerImageSender>(serverImageSenderContext)
		.build();

	networkServer->initialize();
	networkClient->initialize();

	networkServer->listen();
	networkClient->connect();
	networkServer->accept();
	serverPipeline.start();
	clientPipeline.start();
	utilities::DisplayBridge::flushFrames();
	return 0;
}
