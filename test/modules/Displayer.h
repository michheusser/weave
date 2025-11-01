// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef DISPLAYER_H_2025_09_16_15_36_58
#define DISPLAYER_H_2025_09_16_15_36_58
#include <opencv2/videoio.hpp>
#include "Configuration.h"
#include <weave/error/Result.h>

#include "Context.h"

namespace test
{
	namespace module
	{
		class Displayer
		{
		public:
			using ContextType = Context<constants::ModuleType::ImageDisplayer>;
			explicit Displayer(const ContextType& context);
			~Displayer();
			void initialize();
			weave::error::Result process(const cv::Mat& frameBuffer) noexcept;
		private:
			std::string _title;
		};
	}
}

#endif
