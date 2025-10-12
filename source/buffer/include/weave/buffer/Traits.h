// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef TRAITS_H_2025_09_17_13_01_39
#define TRAITS_H_2025_09_17_13_01_39

#include <opencv2/core/mat.hpp>

#include <weave/buffer/Constants.h>
#include "../../../onnxruntime/include/onnxruntime_cxx_api.h"
#include "../Processing/Configuration.h"

namespace weave
{
	namespace buffer
	{
		template <Constants::BufferType bufferType>
		struct Traits; // Prevents use of generic/non-specialized template

		template <>
		struct Traits<Constants::BufferType::Image>
		{
			using StorageType = cv::Mat;
			using ConfigurationType = worker::ImageBufferConfiguration; // TODO Later Configuration<Constants::BufferType::Image>;
			static constexpr std::string_view name = "Image";
		};

		template <>
		struct Traits<Constants::BufferType::Network>
		{
			using StorageType = std::vector<uint8_t>;
			using ConfigurationType = worker::RingBufferConfiguration;
			static constexpr std::string_view name = "Network";
		};

		template <>
		struct Traits<Constants::BufferType::Inference>
		{
			using StorageType = Ort::Value;
			using ConfigurationType = worker::TensorBufferConfiguration;
			static constexpr std::string_view name = "Inference";
		};
	}
}

#endif
