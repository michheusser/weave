// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef MODULECONFIGURATION_H_2025_08_27_15_56_53
#define MODULECONFIGURATION_H_2025_08_27_15_56_53

#include <string>
#include <vector>
#include <onnxruntime_c_api.h>

namespace test
{
	namespace module
	{
		struct CapturerConfiguration
		{
			uint32_t cameraID;
			uint32_t captureAPI;
		};

		struct NormalizerConfiguration
		{
			int destinationWidth;
			int destinationHeight;
		};

		struct DisplayerConfiguration
		{
			std::string title;
		};

		struct EncoderConfiguration
		{
			std::string encodingFormat;
			std::vector<int> encodingParameters;
		};

		struct DecoderConfiguration
		{
			int flags;
		};

		struct InferenceInputProcessorConfiguration
		{
			int rtype;
			double alpha;
			int inputWidth;
			int inputHeight;
			std::tuple<double, double, double> mean;
			std::tuple<double, double, double> std;
		};

		struct InferenceModelConfiguration
		{
			OrtLoggingLevel loggingLevel;
			std::string logid;
			std::string modelPath;
			std::string modelFile;
			int outputImageType;
			int width;
			int height;
			int normalizedAlpha;
			int normalizedBeta;
			int normalizeType;
			int normalizeDType;
		};
	}
}

#endif
