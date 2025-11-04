// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser
#include "DisplayBridge.h"

#include <opencv2/highgui.hpp>
#include <shared_mutex>
#include <weave/error/Result.h>
#include <weave/logging/Macros.h>
#include <weave/utilities/Executable.h>
#include <weave/utilities/SignalManager.h>

#include "weave/graph/Constants.h"

namespace test
{
	namespace utilities
	{
		std::thread::id DisplayBridge::_mainThreadID;
		std::condition_variable DisplayBridge::conditionVariable;
		std::queue<std::tuple<const cv::Mat&, const std::string, const bool> > DisplayBridge::_frameQueue;
		std::mutex DisplayBridge::_mutex;

		weave::error::Result DisplayBridge::showFrame(const cv::Mat& frameBuffer, const std::string& title, const bool wait) noexcept
		{
			if (std::this_thread::get_id() == _mainThreadID)
			{
				weave::error::Result result = _showFrame(frameBuffer, title, wait);
				if (!result.ok())
				{
					LOG_ERROR("Error showing frame.");
					return {weave::error::Type::EmptyBuffer, 0};
				}
			}
			else
			{
				std::unique_lock<std::mutex> lock(_mutex);
				LOG_TRACE("Show frame requested");
				_frameQueue.push({frameBuffer, title, wait});
				lock.unlock();
				conditionVariable.notify_one();
			}
			return weave::error::Result::success();
		}

		void DisplayBridge::flushFrames()
		{
			while (!weave::utilities::SignalManager::shutdownRequested())
			{
				weave::error::Result result = flushFrame();
				if (!result.ok())
				{
					LOG_ERROR("Failed to show frame.");
					return;
				}
			}
		}

		weave::error::Result DisplayBridge::flushFrame()
		{
			std::unique_lock<std::mutex> lock(_mutex);
			conditionVariable.wait_for(lock, std::chrono::milliseconds(weave::constants::DEFAULT_SHUTDOWN_WAIT_CYCLE_TIMEOUT_MILLISECONDS), []()
			{
				return !_frameQueue.empty() || weave::utilities::SignalManager::shutdownRequested();
			});

			if (_frameQueue.empty())
			{
				return weave::error::Result::success();
			}
			auto& [frame, title, wait] = _frameQueue.front();
			_frameQueue.pop();
			lock.unlock();
			weave::error::Result result = _showFrame(frame, title, wait);
			if (!result.ok())
			{
				LOG_ERROR("Failed to show frame.");
				return {weave::error::Type::Display, 0};
			}
			return weave::error::Result::success();
		}

		weave::error::Result DisplayBridge::_showFrame(const cv::Mat& frameBuffer, const std::string& title, const bool wait) noexcept
		{
			LOG_TRACE("Showing frame");
			if (frameBuffer.empty())
			{
				LOG_ERROR("Cannot display empty frame.");
				return {weave::error::Type::EmptyBuffer, 0};
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
			return weave::error::Result::success();
		}
	}
}
