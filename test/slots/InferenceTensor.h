// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef INFERENCETENSOR_H_2025_10_21_16_05_08
#define INFERENCETENSOR_H_2025_10_21_16_05_08

#include <onnxruntime_cxx_api.h>
#include "../buffer/Context.h"

namespace test
{
	namespace slot
	{
		class InferenceTensor
		{
		public:
			using StorageType = Ort::Value;
			using ContextType = buffer::Context<buffer::constants::BufferType::InferenceInputTensor>;
			explicit InferenceTensor();
			void initialize(const ContextType& context);
			StorageType& data();

		private:
			StorageType _data;
			std::vector<float> _internalData;
		};
	} // slot
} // test

#endif
