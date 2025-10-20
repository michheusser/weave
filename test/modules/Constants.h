// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef MODULECONSTANTS_H_2025_08_27_15_56_46
#define MODULECONSTANTS_H_2025_08_27_15_56_46

#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio.hpp>

namespace test
{
	namespace module
	{
		namespace constants
		{
			enum class ModuleType
			{
				ImageCapturer,
				ImageNormalizer,
				ImageDisplayer,
				ClientSender,
				ClientReceiver,
				ServerSender,
				ServerReceiver,
				InferenceInputProcessor,
				InferenceModel
			};

			// TODO PUT IN DEDICATED CONSTANTS FILE
			constexpr int CAMERA_ID = 0;
			constexpr int OPENCV_VIDEO_CAPTURE_API = cv::CAP_ANY; // Autodetect

			constexpr std::string_view DEFAULT_DISPLAY_TITLE = "Frame";

			constexpr int FRAME_BUFFER_SIZE = 0;
			constexpr std::string_view ENCODING_FORMAT = ".png";
			constexpr int COMPRESSION_IMWRITE_PARAM = cv::IMWRITE_PNG_COMPRESSION;
			constexpr int COMPRESSION_LEVEL = 0;

			constexpr int RAW_FRAME_HEIGHT = 1080;
			constexpr int RAW_FRAME_WIDTH = 1920;
			constexpr int RAW_FRAME_TYPE = CV_8UC3; // RGB, 8-bit

			constexpr int FRAME_HEIGHT = 256;
			constexpr int FRAME_WIDTH = 256;
			constexpr int FRAME_TYPE = CV_8UC3; // RGB, 8-bit

			constexpr int BUFFER_SIZE = FRAME_HEIGHT * FRAME_WIDTH * 3 * 1.1;

			constexpr int DECODER_FLAGS = cv::IMREAD_ANYCOLOR;

			//
			constexpr int INPUT_FRAME_RTYPE = CV_32F;
			constexpr double INPUT_FRAME_ALPHA = 1.0 / 255.0;
			constexpr int INPUT_FRAME_WIDTH = FRAME_WIDTH;
			constexpr int INPUT_FRAME_HEIGHT = FRAME_HEIGHT;
			constexpr std::tuple<double, double, double> INPUT_FRAME_MEAN = {0.485, 0.456, 0.406};
			constexpr std::tuple<double, double, double> INPUT_FRAME_STD = {0.229, 0.224, 0.225};

			constexpr int TENSOR_WIDTH = FRAME_WIDTH;
			constexpr int TENSOR_HEIGHT = FRAME_HEIGHT;
			constexpr OrtAllocatorType TENSOR_ALLOCATOR_TYPE = OrtAllocatorType::OrtArenaAllocator;
			constexpr OrtMemType TENSOR_MEMORY_TYPE = OrtMemType::OrtMemTypeDefault;

			//
			constexpr OrtLoggingLevel ONNX_ENV_LOGGING_LEVEL = OrtLoggingLevel::ORT_LOGGING_LEVEL_WARNING;
			constexpr std::string_view ONNX_ENV_LOGID = "style_transfer";
			constexpr std::string_view ONNX_MODEL_PATH = WEAVE_MODELS_LOCATION;
			constexpr std::string_view ONNX_MODEL_FILE = "candy_nc16_256x256_onnxjs013.onnx";
			constexpr int OUTPUT_IMAGE_TYPE = CV_32FC3;        // Model outputs float32
			constexpr int OUTPUT_IMAGE_WIDTH = FRAME_WIDTH;    // 384
			constexpr int OUTPUT_IMAGE_HEIGHT = FRAME_HEIGHT;  // 384
			constexpr int OUTPUT_NORM_ALPHA = 0;               // Clip min
			constexpr int OUTPUT_NORM_BETA = 255;              // Clip max
			constexpr int OUTPUT_NORM_TYPE = cv::NORM_MINMAX;
			constexpr int OUTPUT_NORM_DTYPE = CV_8UC3;         // Final RGB output
		}
	}
}


#endif