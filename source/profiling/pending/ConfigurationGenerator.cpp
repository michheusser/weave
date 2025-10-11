// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser
#include "ConfigurationGenerator.h"
#include "../Application/Configuration.h"
#include "../Processing/Constants.h"
#include "../Streaming/Constants.h"
#include "../IO/Constants.h"

namespace LatencyLens
{
	namespace Configuration
	{
		Profiling::ClientSessionConfiguration ConfigurationGenerator::generateDefaultClientSessionConfiguration()
		{
			Profiling::ClientSessionConfiguration clientSessionConfiguration;
			clientSessionConfiguration.sessionName = Profiling::Constants::DEFAULT_CLIENT_SESSION_NAME;
			clientSessionConfiguration.sessionDescription = Profiling::Constants::DEFAULT_CLIENT_SESSION_DESCRIPTION;
			clientSessionConfiguration.numCycles = Profiling::Constants::DEFAULT_NUM_CYCLES;
			clientSessionConfiguration.loggingDirectory = std::string(Profiling::Constants::MONITORING_REPORTS_LOCATION) + "/" + _getDateString();
			clientSessionConfiguration.clientConfiguration = generateDefaultClientConfiguration();
			return clientSessionConfiguration;
		}

		Profiling::ServerSessionConfiguration ConfigurationGenerator::generateDefaultServerSessionConfiguration()
		{
			Profiling::ServerSessionConfiguration serverSessionConfiguration;
			serverSessionConfiguration.sessionName = Profiling::Constants::DEFAULT_SERVER_SESSION_NAME;
			serverSessionConfiguration.sessionDescription = Profiling::Constants::DEFAULT_SERVER_SESSION_DESCRIPTION;
			serverSessionConfiguration.numCycles = Profiling::Constants::DEFAULT_NUM_CYCLES;
			serverSessionConfiguration.loggingDirectory = std::string(Profiling::Constants::MONITORING_REPORTS_LOCATION) + "/" + _getDateString();
			serverSessionConfiguration.serverConfiguration = generateDefaultServerConfiguration();
			return serverSessionConfiguration;
		}

		Application::ClientConfiguration ConfigurationGenerator::generateDefaultClientConfiguration()
		{
			Application::ClientConfiguration clientConfiguration;

			// IO
			clientConfiguration.capturerConfiguration.cameraID = IO::Constants::CAMERA_ID;
			clientConfiguration.capturerConfiguration.captureAPI = IO::Constants::OPENCV_VIDEO_CAPTURE_API;

			clientConfiguration.displayerConfiguration.title = IO::Constants::DEFAULT_DISPLAY_TITLE;

			// Buffers
			clientConfiguration.clientOrchestratorConfiguration.encodedSendFrameBufferConfiguration.bufferSize = Processing::Constants::BUFFER_SIZE;
			clientConfiguration.clientOrchestratorConfiguration.encodedReceiveFrameBufferConfiguration.bufferSize = Processing::Constants::BUFFER_SIZE;

			// Encoder
			clientConfiguration.clientOrchestratorConfiguration.encoderConfiguration.encodingFormat = std::string(Processing::Constants::ENCODING_FORMAT);
			clientConfiguration.clientOrchestratorConfiguration.encoderConfiguration.encodingParameters = {
				Processing::Constants::COMPRESSION_IMWRITE_PARAM, Processing::Constants::COMPRESSION_LEVEL
			};

			// Decoder
			clientConfiguration.clientOrchestratorConfiguration.decoderConfiguration.flags = Processing::Constants::DECODER_FLAGS;

			clientConfiguration.clientOrchestratorConfiguration.pipelineConfiguration.bufferConfiguration.inputFrameBufferConfiguration.frameHeight = Processing::Constants::RAW_FRAME_HEIGHT;
			clientConfiguration.clientOrchestratorConfiguration.pipelineConfiguration.bufferConfiguration.inputFrameBufferConfiguration.frameWidth = Processing::Constants::RAW_FRAME_WIDTH;
			clientConfiguration.clientOrchestratorConfiguration.pipelineConfiguration.bufferConfiguration.inputFrameBufferConfiguration.frameType = Processing::Constants::RAW_FRAME_TYPE;

			clientConfiguration.clientOrchestratorConfiguration.pipelineConfiguration.bufferConfiguration.sendFrameBufferConfiguration.frameHeight = Processing::Constants::FRAME_HEIGHT;
			clientConfiguration.clientOrchestratorConfiguration.pipelineConfiguration.bufferConfiguration.sendFrameBufferConfiguration.frameWidth = Processing::Constants::FRAME_WIDTH;
			clientConfiguration.clientOrchestratorConfiguration.pipelineConfiguration.bufferConfiguration.sendFrameBufferConfiguration.frameType = Processing::Constants::FRAME_TYPE;


			clientConfiguration.clientOrchestratorConfiguration.pipelineConfiguration.bufferConfiguration.receiveFrameBufferConfiguration.frameHeight = Processing::Constants::FRAME_HEIGHT;
			clientConfiguration.clientOrchestratorConfiguration.pipelineConfiguration.bufferConfiguration.receiveFrameBufferConfiguration.frameWidth = Processing::Constants::FRAME_WIDTH;
			clientConfiguration.clientOrchestratorConfiguration.pipelineConfiguration.bufferConfiguration.receiveFrameBufferConfiguration.frameType = Processing::Constants::FRAME_TYPE;

			clientConfiguration.clientOrchestratorConfiguration.pipelineConfiguration.bufferConfiguration.outputFrameBufferConfiguration.frameHeight = Processing::Constants::RAW_FRAME_HEIGHT;
			clientConfiguration.clientOrchestratorConfiguration.pipelineConfiguration.bufferConfiguration.outputFrameBufferConfiguration.frameWidth = Processing::Constants::RAW_FRAME_WIDTH;
			clientConfiguration.clientOrchestratorConfiguration.pipelineConfiguration.bufferConfiguration.outputFrameBufferConfiguration.frameType = Processing::Constants::RAW_FRAME_TYPE;

			// Modules
			// Preprocessor
			clientConfiguration.clientOrchestratorConfiguration.pipelineConfiguration.moduleConfiguration.preprocessorConfiguration.destinationHeight = Processing::Constants::FRAME_HEIGHT;
			clientConfiguration.clientOrchestratorConfiguration.pipelineConfiguration.moduleConfiguration.preprocessorConfiguration.destinationWidth = Processing::Constants::FRAME_WIDTH;

			// Postprocessor
			clientConfiguration.clientOrchestratorConfiguration.pipelineConfiguration.moduleConfiguration.postprocessorConfiguration.destinationHeight = Processing::Constants::RAW_FRAME_HEIGHT;
			clientConfiguration.clientOrchestratorConfiguration.pipelineConfiguration.moduleConfiguration.postprocessorConfiguration.destinationWidth = Processing::Constants::RAW_FRAME_WIDTH;

			// Streaming Configuration
			// Client
			clientConfiguration.clientOrchestratorConfiguration.streamingConfiguration.localAddress = Streaming::Constants::CLIENT_LOCAL_ADDRESS;
			clientConfiguration.clientOrchestratorConfiguration.streamingConfiguration.localPort = Streaming::Constants::CLIENT_LOCAL_PORT;
			clientConfiguration.clientOrchestratorConfiguration.streamingConfiguration.remoteAddress = Streaming::Constants::CLIENT_REMOTE_ADDRESS;
			clientConfiguration.clientOrchestratorConfiguration.streamingConfiguration.remotePort = Streaming::Constants::CLIENT_REMOTE_PORT;

			return clientConfiguration;
		}

		Application::ServerConfiguration ConfigurationGenerator::generateDefaultServerConfiguration()
		{
			Application::ServerConfiguration serverConfiguration;

			// Buffers
			serverConfiguration.serverOrchestratorConfiguration.encodedReceiveFrameBufferConfiguration.bufferSize = Processing::Constants::BUFFER_SIZE;
			serverConfiguration.serverOrchestratorConfiguration.encodedSendFrameBufferConfiguration.bufferSize = Processing::Constants::BUFFER_SIZE;
			// Decoder
			serverConfiguration.serverOrchestratorConfiguration.decoderConfiguration.flags = Processing::Constants::DECODER_FLAGS;
			// Encoder
			serverConfiguration.serverOrchestratorConfiguration.encoderConfiguration.encodingFormat = std::string(Processing::Constants::ENCODING_FORMAT);
			serverConfiguration.serverOrchestratorConfiguration.encoderConfiguration.encodingParameters = {
				Processing::Constants::COMPRESSION_IMWRITE_PARAM, Processing::Constants::COMPRESSION_LEVEL
			};

			serverConfiguration.serverOrchestratorConfiguration.pipelineConfiguration.bufferConfiguration.receiveFrameBufferConfiguration.frameHeight = Processing::Constants::FRAME_HEIGHT;
			serverConfiguration.serverOrchestratorConfiguration.pipelineConfiguration.bufferConfiguration.receiveFrameBufferConfiguration.frameWidth = Processing::Constants::FRAME_WIDTH;
			serverConfiguration.serverOrchestratorConfiguration.pipelineConfiguration.bufferConfiguration.receiveFrameBufferConfiguration.frameType = Processing::Constants::FRAME_TYPE;

			serverConfiguration.serverOrchestratorConfiguration.pipelineConfiguration.bufferConfiguration.inferenceInputTensorBufferConfiguration.height = Processing::Constants::TENSOR_HEIGHT;
			serverConfiguration.serverOrchestratorConfiguration.pipelineConfiguration.bufferConfiguration.inferenceInputTensorBufferConfiguration.width = Processing::Constants::TENSOR_WIDTH;
			serverConfiguration.serverOrchestratorConfiguration.pipelineConfiguration.bufferConfiguration.inferenceInputTensorBufferConfiguration.tensorAllocatorType =
				Processing::Constants::TENSOR_ALLOCATOR_TYPE;
			serverConfiguration.serverOrchestratorConfiguration.pipelineConfiguration.bufferConfiguration.inferenceInputTensorBufferConfiguration.tensorMemoryType =
				Processing::Constants::TENSOR_MEMORY_TYPE;


			serverConfiguration.serverOrchestratorConfiguration.pipelineConfiguration.bufferConfiguration.sendFrameBufferConfiguration.frameHeight = Processing::Constants::FRAME_HEIGHT;
			serverConfiguration.serverOrchestratorConfiguration.pipelineConfiguration.bufferConfiguration.sendFrameBufferConfiguration.frameWidth = Processing::Constants::FRAME_WIDTH;
			serverConfiguration.serverOrchestratorConfiguration.pipelineConfiguration.bufferConfiguration.sendFrameBufferConfiguration.frameType = Processing::Constants::FRAME_TYPE;


			serverConfiguration.serverOrchestratorConfiguration.pipelineConfiguration.bufferConfiguration.receiveFrameBufferConfiguration.frameHeight = Processing::Constants::FRAME_HEIGHT;
			serverConfiguration.serverOrchestratorConfiguration.pipelineConfiguration.bufferConfiguration.receiveFrameBufferConfiguration.frameWidth = Processing::Constants::FRAME_WIDTH;
			serverConfiguration.serverOrchestratorConfiguration.pipelineConfiguration.bufferConfiguration.receiveFrameBufferConfiguration.frameType = Processing::Constants::FRAME_TYPE;

			// Modules

			// InferenceInputProcessor
			serverConfiguration.serverOrchestratorConfiguration.pipelineConfiguration.moduleConfiguration.inferenceInputProcessorConfiguration.rtype = Processing::Constants::INPUT_FRAME_RTYPE;
			serverConfiguration.serverOrchestratorConfiguration.pipelineConfiguration.moduleConfiguration.inferenceInputProcessorConfiguration.alpha = Processing::Constants::INPUT_FRAME_ALPHA;
			serverConfiguration.serverOrchestratorConfiguration.pipelineConfiguration.moduleConfiguration.inferenceInputProcessorConfiguration.inputWidth = Processing::Constants::INPUT_FRAME_WIDTH;
			serverConfiguration.serverOrchestratorConfiguration.pipelineConfiguration.moduleConfiguration.inferenceInputProcessorConfiguration.inputHeight = Processing::Constants::INPUT_FRAME_HEIGHT;
			serverConfiguration.serverOrchestratorConfiguration.pipelineConfiguration.moduleConfiguration.inferenceInputProcessorConfiguration.mean = Processing::Constants::INPUT_FRAME_MEAN;
			serverConfiguration.serverOrchestratorConfiguration.pipelineConfiguration.moduleConfiguration.inferenceInputProcessorConfiguration.std = Processing::Constants::INPUT_FRAME_STD;

			// Inference Model
			serverConfiguration.serverOrchestratorConfiguration.pipelineConfiguration.moduleConfiguration.inferenceModelConfiguration.loggingLevel = Processing::Constants::ONNX_ENV_LOGGING_LEVEL;
			serverConfiguration.serverOrchestratorConfiguration.pipelineConfiguration.moduleConfiguration.inferenceModelConfiguration.logid = Processing::Constants::ONNX_ENV_LOGID;
			serverConfiguration.serverOrchestratorConfiguration.pipelineConfiguration.moduleConfiguration.inferenceModelConfiguration.modelPath = Processing::Constants::ONNX_MODEL_PATH;
			serverConfiguration.serverOrchestratorConfiguration.pipelineConfiguration.moduleConfiguration.inferenceModelConfiguration.modelFile = Processing::Constants::ONNX_MODEL_FILE;
			serverConfiguration.serverOrchestratorConfiguration.pipelineConfiguration.moduleConfiguration.inferenceModelConfiguration.outputImageType = Processing::Constants::OUTPUT_IMAGE_TYPE;
			serverConfiguration.serverOrchestratorConfiguration.pipelineConfiguration.moduleConfiguration.inferenceModelConfiguration.width = Processing::Constants::OUTPUT_IMAGE_WIDTH;
			serverConfiguration.serverOrchestratorConfiguration.pipelineConfiguration.moduleConfiguration.inferenceModelConfiguration.height = Processing::Constants::OUTPUT_IMAGE_HEIGHT;
			serverConfiguration.serverOrchestratorConfiguration.pipelineConfiguration.moduleConfiguration.inferenceModelConfiguration.normalizedAlpha = Processing::Constants::OUTPUT_NORM_ALPHA;
			serverConfiguration.serverOrchestratorConfiguration.pipelineConfiguration.moduleConfiguration.inferenceModelConfiguration.normalizedBeta = Processing::Constants::OUTPUT_NORM_BETA;
			serverConfiguration.serverOrchestratorConfiguration.pipelineConfiguration.moduleConfiguration.inferenceModelConfiguration.normalizeType = Processing::Constants::OUTPUT_NORM_TYPE;
			serverConfiguration.serverOrchestratorConfiguration.pipelineConfiguration.moduleConfiguration.inferenceModelConfiguration.normalizeDType = Processing::Constants::OUTPUT_NORM_DTYPE;

			// Streaming Configuration
			// Server
			serverConfiguration.serverOrchestratorConfiguration.streamingConfiguration.localAddress = Streaming::Constants::SERVER_LOCAL_ADDRESS;
			serverConfiguration.serverOrchestratorConfiguration.streamingConfiguration.localPort = Streaming::Constants::SERVER_LOCAL_PORT;

			return serverConfiguration;
		}

		std::string ConfigurationGenerator::_getDateString()
		{
			auto now = std::chrono::system_clock::now();
			auto time_t = std::chrono::system_clock::to_time_t(now);

			std::ostringstream oss;
			oss << std::put_time(std::localtime(&time_t), "%Y_%m_%d_%H_%M_%S");

			return oss.str();
		}
	} // Configuration
} // LatencyLens
