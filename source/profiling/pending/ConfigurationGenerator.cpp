// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser
#include "ConfigurationGenerator.h"
#include "../Application/Configuration.h"
#include "../Processing/Constants.h"
#include "../Streaming/Constants.h"
#include "../IO/Constants.h"

namespace weave
{
	namespace Configuration
	{
		profiling::ClientSessionConfiguration ConfigurationGenerator::generateDefaultClientSessionConfiguration()
		{
			profiling::ClientSessionConfiguration clientSessionConfiguration;
			clientSessionConfiguration.sessionName = profiling::Constants::DEFAULT_CLIENT_SESSION_NAME;
			clientSessionConfiguration.sessionDescription = profiling::Constants::DEFAULT_CLIENT_SESSION_DESCRIPTION;
			clientSessionConfiguration.numCycles = profiling::Constants::DEFAULT_NUM_CYCLES;
			clientSessionConfiguration.loggingDirectory = std::string(profiling::Constants::MONITORING_REPORTS_LOCATION) + "/" + _getDateString();
			clientSessionConfiguration.clientConfiguration = generateDefaultClientConfiguration();
			return clientSessionConfiguration;
		}

		profiling::ServerSessionConfiguration ConfigurationGenerator::generateDefaultServerSessionConfiguration()
		{
			profiling::ServerSessionConfiguration serverSessionConfiguration;
			serverSessionConfiguration.sessionName = profiling::Constants::DEFAULT_SERVER_SESSION_NAME;
			serverSessionConfiguration.sessionDescription = profiling::Constants::DEFAULT_SERVER_SESSION_DESCRIPTION;
			serverSessionConfiguration.numCycles = profiling::Constants::DEFAULT_NUM_CYCLES;
			serverSessionConfiguration.loggingDirectory = std::string(profiling::Constants::MONITORING_REPORTS_LOCATION) + "/" + _getDateString();
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
			clientConfiguration.clientOrchestratorConfiguration.encodedSendFrameBufferConfiguration.bufferSize = worker::Constants::BUFFER_SIZE;
			clientConfiguration.clientOrchestratorConfiguration.encodedReceiveFrameBufferConfiguration.bufferSize = worker::Constants::BUFFER_SIZE;

			// Encoder
			clientConfiguration.clientOrchestratorConfiguration.encoderConfiguration.encodingFormat = std::string(worker::Constants::ENCODING_FORMAT);
			clientConfiguration.clientOrchestratorConfiguration.encoderConfiguration.encodingParameters = {
				worker::Constants::COMPRESSION_IMWRITE_PARAM, worker::Constants::COMPRESSION_LEVEL
			};

			// Decoder
			clientConfiguration.clientOrchestratorConfiguration.decoderConfiguration.flags = worker::Constants::DECODER_FLAGS;

			clientConfiguration.clientOrchestratorConfiguration.pipelineConfiguration.bufferConfiguration.inputFrameBufferConfiguration.frameHeight = worker::Constants::RAW_FRAME_HEIGHT;
			clientConfiguration.clientOrchestratorConfiguration.pipelineConfiguration.bufferConfiguration.inputFrameBufferConfiguration.frameWidth = worker::Constants::RAW_FRAME_WIDTH;
			clientConfiguration.clientOrchestratorConfiguration.pipelineConfiguration.bufferConfiguration.inputFrameBufferConfiguration.frameType = worker::Constants::RAW_FRAME_TYPE;

			clientConfiguration.clientOrchestratorConfiguration.pipelineConfiguration.bufferConfiguration.sendFrameBufferConfiguration.frameHeight = worker::Constants::FRAME_HEIGHT;
			clientConfiguration.clientOrchestratorConfiguration.pipelineConfiguration.bufferConfiguration.sendFrameBufferConfiguration.frameWidth = worker::Constants::FRAME_WIDTH;
			clientConfiguration.clientOrchestratorConfiguration.pipelineConfiguration.bufferConfiguration.sendFrameBufferConfiguration.frameType = worker::Constants::FRAME_TYPE;


			clientConfiguration.clientOrchestratorConfiguration.pipelineConfiguration.bufferConfiguration.receiveFrameBufferConfiguration.frameHeight = worker::Constants::FRAME_HEIGHT;
			clientConfiguration.clientOrchestratorConfiguration.pipelineConfiguration.bufferConfiguration.receiveFrameBufferConfiguration.frameWidth = worker::Constants::FRAME_WIDTH;
			clientConfiguration.clientOrchestratorConfiguration.pipelineConfiguration.bufferConfiguration.receiveFrameBufferConfiguration.frameType = worker::Constants::FRAME_TYPE;

			clientConfiguration.clientOrchestratorConfiguration.pipelineConfiguration.bufferConfiguration.outputFrameBufferConfiguration.frameHeight = worker::Constants::RAW_FRAME_HEIGHT;
			clientConfiguration.clientOrchestratorConfiguration.pipelineConfiguration.bufferConfiguration.outputFrameBufferConfiguration.frameWidth = worker::Constants::RAW_FRAME_WIDTH;
			clientConfiguration.clientOrchestratorConfiguration.pipelineConfiguration.bufferConfiguration.outputFrameBufferConfiguration.frameType = worker::Constants::RAW_FRAME_TYPE;

			// Modules
			// Preprocessor
			clientConfiguration.clientOrchestratorConfiguration.pipelineConfiguration.moduleConfiguration.preprocessorConfiguration.destinationHeight = worker::Constants::FRAME_HEIGHT;
			clientConfiguration.clientOrchestratorConfiguration.pipelineConfiguration.moduleConfiguration.preprocessorConfiguration.destinationWidth = worker::Constants::FRAME_WIDTH;

			// Postprocessor
			clientConfiguration.clientOrchestratorConfiguration.pipelineConfiguration.moduleConfiguration.postprocessorConfiguration.destinationHeight = worker::Constants::RAW_FRAME_HEIGHT;
			clientConfiguration.clientOrchestratorConfiguration.pipelineConfiguration.moduleConfiguration.postprocessorConfiguration.destinationWidth = worker::Constants::RAW_FRAME_WIDTH;

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
			serverConfiguration.serverOrchestratorConfiguration.encodedReceiveFrameBufferConfiguration.bufferSize = worker::Constants::BUFFER_SIZE;
			serverConfiguration.serverOrchestratorConfiguration.encodedSendFrameBufferConfiguration.bufferSize = worker::Constants::BUFFER_SIZE;
			// Decoder
			serverConfiguration.serverOrchestratorConfiguration.decoderConfiguration.flags = worker::Constants::DECODER_FLAGS;
			// Encoder
			serverConfiguration.serverOrchestratorConfiguration.encoderConfiguration.encodingFormat = std::string(worker::Constants::ENCODING_FORMAT);
			serverConfiguration.serverOrchestratorConfiguration.encoderConfiguration.encodingParameters = {
				worker::Constants::COMPRESSION_IMWRITE_PARAM, worker::Constants::COMPRESSION_LEVEL
			};

			serverConfiguration.serverOrchestratorConfiguration.pipelineConfiguration.bufferConfiguration.receiveFrameBufferConfiguration.frameHeight = worker::Constants::FRAME_HEIGHT;
			serverConfiguration.serverOrchestratorConfiguration.pipelineConfiguration.bufferConfiguration.receiveFrameBufferConfiguration.frameWidth = worker::Constants::FRAME_WIDTH;
			serverConfiguration.serverOrchestratorConfiguration.pipelineConfiguration.bufferConfiguration.receiveFrameBufferConfiguration.frameType = worker::Constants::FRAME_TYPE;

			serverConfiguration.serverOrchestratorConfiguration.pipelineConfiguration.bufferConfiguration.inferenceInputTensorBufferConfiguration.height = worker::Constants::TENSOR_HEIGHT;
			serverConfiguration.serverOrchestratorConfiguration.pipelineConfiguration.bufferConfiguration.inferenceInputTensorBufferConfiguration.width = worker::Constants::TENSOR_WIDTH;
			serverConfiguration.serverOrchestratorConfiguration.pipelineConfiguration.bufferConfiguration.inferenceInputTensorBufferConfiguration.tensorAllocatorType =
				worker::Constants::TENSOR_ALLOCATOR_TYPE;
			serverConfiguration.serverOrchestratorConfiguration.pipelineConfiguration.bufferConfiguration.inferenceInputTensorBufferConfiguration.tensorMemoryType =
				worker::Constants::TENSOR_MEMORY_TYPE;


			serverConfiguration.serverOrchestratorConfiguration.pipelineConfiguration.bufferConfiguration.sendFrameBufferConfiguration.frameHeight = worker::Constants::FRAME_HEIGHT;
			serverConfiguration.serverOrchestratorConfiguration.pipelineConfiguration.bufferConfiguration.sendFrameBufferConfiguration.frameWidth = worker::Constants::FRAME_WIDTH;
			serverConfiguration.serverOrchestratorConfiguration.pipelineConfiguration.bufferConfiguration.sendFrameBufferConfiguration.frameType = worker::Constants::FRAME_TYPE;


			serverConfiguration.serverOrchestratorConfiguration.pipelineConfiguration.bufferConfiguration.receiveFrameBufferConfiguration.frameHeight = worker::Constants::FRAME_HEIGHT;
			serverConfiguration.serverOrchestratorConfiguration.pipelineConfiguration.bufferConfiguration.receiveFrameBufferConfiguration.frameWidth = worker::Constants::FRAME_WIDTH;
			serverConfiguration.serverOrchestratorConfiguration.pipelineConfiguration.bufferConfiguration.receiveFrameBufferConfiguration.frameType = worker::Constants::FRAME_TYPE;

			// Modules

			// InferenceInputProcessor
			serverConfiguration.serverOrchestratorConfiguration.pipelineConfiguration.moduleConfiguration.inferenceInputProcessorConfiguration.rtype = worker::Constants::INPUT_FRAME_RTYPE;
			serverConfiguration.serverOrchestratorConfiguration.pipelineConfiguration.moduleConfiguration.inferenceInputProcessorConfiguration.alpha = worker::Constants::INPUT_FRAME_ALPHA;
			serverConfiguration.serverOrchestratorConfiguration.pipelineConfiguration.moduleConfiguration.inferenceInputProcessorConfiguration.inputWidth = worker::Constants::INPUT_FRAME_WIDTH;
			serverConfiguration.serverOrchestratorConfiguration.pipelineConfiguration.moduleConfiguration.inferenceInputProcessorConfiguration.inputHeight = worker::Constants::INPUT_FRAME_HEIGHT;
			serverConfiguration.serverOrchestratorConfiguration.pipelineConfiguration.moduleConfiguration.inferenceInputProcessorConfiguration.mean = worker::Constants::INPUT_FRAME_MEAN;
			serverConfiguration.serverOrchestratorConfiguration.pipelineConfiguration.moduleConfiguration.inferenceInputProcessorConfiguration.std = worker::Constants::INPUT_FRAME_STD;

			// Inference Model
			serverConfiguration.serverOrchestratorConfiguration.pipelineConfiguration.moduleConfiguration.inferenceModelConfiguration.loggingLevel = worker::Constants::ONNX_ENV_LOGGING_LEVEL;
			serverConfiguration.serverOrchestratorConfiguration.pipelineConfiguration.moduleConfiguration.inferenceModelConfiguration.logid = worker::Constants::ONNX_ENV_LOGID;
			serverConfiguration.serverOrchestratorConfiguration.pipelineConfiguration.moduleConfiguration.inferenceModelConfiguration.modelPath = worker::Constants::ONNX_MODEL_PATH;
			serverConfiguration.serverOrchestratorConfiguration.pipelineConfiguration.moduleConfiguration.inferenceModelConfiguration.modelFile = worker::Constants::ONNX_MODEL_FILE;
			serverConfiguration.serverOrchestratorConfiguration.pipelineConfiguration.moduleConfiguration.inferenceModelConfiguration.outputImageType = worker::Constants::OUTPUT_IMAGE_TYPE;
			serverConfiguration.serverOrchestratorConfiguration.pipelineConfiguration.moduleConfiguration.inferenceModelConfiguration.width = worker::Constants::OUTPUT_IMAGE_WIDTH;
			serverConfiguration.serverOrchestratorConfiguration.pipelineConfiguration.moduleConfiguration.inferenceModelConfiguration.height = worker::Constants::OUTPUT_IMAGE_HEIGHT;
			serverConfiguration.serverOrchestratorConfiguration.pipelineConfiguration.moduleConfiguration.inferenceModelConfiguration.normalizedAlpha = worker::Constants::OUTPUT_NORM_ALPHA;
			serverConfiguration.serverOrchestratorConfiguration.pipelineConfiguration.moduleConfiguration.inferenceModelConfiguration.normalizedBeta = worker::Constants::OUTPUT_NORM_BETA;
			serverConfiguration.serverOrchestratorConfiguration.pipelineConfiguration.moduleConfiguration.inferenceModelConfiguration.normalizeType = worker::Constants::OUTPUT_NORM_TYPE;
			serverConfiguration.serverOrchestratorConfiguration.pipelineConfiguration.moduleConfiguration.inferenceModelConfiguration.normalizeDType = worker::Constants::OUTPUT_NORM_DTYPE;

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
	}
}
