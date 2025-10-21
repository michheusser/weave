// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser
#include "Image.h"

#include <opencv2/core.hpp>
#include <weave/logging/Macros.h>
#include <weave/error/Exception.h>

namespace test
{
	namespace slot
	{
		Image::Image()
		{

		}

		void Image::initialize(const ContextType& context)
		{
			try
			{
				_data.create(context.configuration.frameHeight, context.configuration.frameWidth, context.configuration.frameType);
			}
			catch (cv::Exception& exception)
			{
				LOG_ERROR(exception.what());
				throw weave::error::Exception("Failed.");
			}
		}

		Image::StorageType& Image::data()
		{
			return _data;
		}

	} // slots
} // test
