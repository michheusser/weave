// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef IMAGE_H_2025_10_21_16_04_28
#define IMAGE_H_2025_10_21_16_04_28

#include <opencv2/core/mat.hpp>
#include "../buffer/Context.h"

namespace test
{
	namespace slot
	{
		class Image
		{
		public:
			using StorageType = cv::Mat;
			using ContextType = buffer::Context<buffer::constants::BufferType::Image>;
			explicit Image();
			void initialize(const ContextType& context);
			StorageType& data();

		private:
			StorageType _data;
		};
	} // slots
} // test

#endif
