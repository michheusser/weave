// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef DISPLAYBRIDGE_H_2025_09_22_18_17_22
#define DISPLAYBRIDGE_H_2025_09_22_18_17_22
#include <shared_mutex>
#include <thread>
#include <opencv2/core/mat.hpp>
#include <weave/error/Constants.h>

#include "weave/error/Result.h"

namespace test
{
	namespace utilities
	{
		// TODO Race conditions. I'm passing reference to a buffer that is constantly modified!
		class DisplayBridge
		{
		public:
			static void initMainThread()
			{
				_mainThreadID = std::this_thread::get_id();
			}

			static weave::error::Result showFrame(const cv::Mat& frameBuffer, const std::string& title, const uint32_t frameID, const bool wait = false) noexcept;
			static void flushFrames();
			static weave::error::Result flushFrame();
		private:
			static weave::error::Result _showFrame(const cv::Mat& frameBuffer, const std::string& title, const uint32_t frameID, const bool wait = false) noexcept;
			static std::thread::id _mainThreadID;
			static std::queue<std::tuple<const cv::Mat&, const std::string, const uint32_t, const bool>> _frameQueue;
			static std::condition_variable conditionVariable;
			static std::mutex _mutex;
		};
	}
}

#endif
