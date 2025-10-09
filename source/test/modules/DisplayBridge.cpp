// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser
#include "DisplayBridge.h"

#include <shared_mutex>
#include <opencv2/highgui.hpp>

#include "../Application/SignalManager.h"
#include "../Error/Exception.h"
#include "../Logging/Macros.h"

namespace test
{
	namespace module
	{
		std::thread::id DisplayBridge::_mainThreadID;
		std::condition_variable DisplayBridge::conditionVariable;
		std::queue<std::tuple<const cv::Mat&, const std::string, const uint32_t, const bool>> DisplayBridge::_frameQueue;
		std::mutex DisplayBridge::_mutex;

		Error::Result DisplayBridge::showFrame(const cv::Mat& frameBuffer, const std::string& title, const uint32_t frameID, const bool wait) noexcept
		{
			if (std::this_thread::get_id() == _mainThreadID)
			{
				Error::Result result = _showFrame(frameBuffer, title, frameID, wait);
				if (!result.ok())
				{
					LOG_ERROR("Error showing frame.");
					return {Error::Type::EmptyBuffer, frameID};
				}
			}
			else
			{
				std::unique_lock<std::mutex> lock(_mutex);
				LOG_TRACE("Show frame requested");
				_frameQueue.push({frameBuffer, title, frameID, wait});
				lock.unlock();
				conditionVariable.notify_one();
			}
			return Error::Result::success();
		}

		void DisplayBridge::flushFrames()
		{
			std::unique_lock<std::mutex> lock(_mutex);
			while (!Application::SignalManager::shutdownRequested())
			{
				conditionVariable.wait(lock, []()
				{
					return !_frameQueue.empty();
				});
				auto& [frame, title, frameID, wait] = _frameQueue.front();
				_frameQueue.pop();
				lock.unlock();
				Error::Result result = _showFrame(frame, title, wait);
				if (!result.ok())
				{
					LOG_ERROR("Failed to flush frames.");
					return;
				}
				lock.lock();
			}
		}

		Error::Result DisplayBridge::_showFrame(const cv::Mat& frameBuffer, const std::string& title, const uint32_t frameID, const bool wait) noexcept
		{
			LOG_TRACE("Showing frame");
			if (frameBuffer.empty())
			{
				LOG_ERROR("Cannot display empty frame.");
				return {Error::Type::EmptyBuffer, frameID};
			}
			cv::imshow(title, frameBuffer);
			if (wait)
			{
				cv::waitKey(0); // Displays until a key is pressed
			}
			else
			{
				cv::waitKey(1); // 100ms
			}
			return Error::Result::success();
		}
	} // IO
}
