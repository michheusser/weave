// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef WORKER_H_2025_09_20_13_44_55
#define WORKER_H_2025_09_20_13_44_55


namespace weave
{
	namespace Worker
	{
		template<Constants::Module module>
		class Worker
		{
		public:
			explicit Worker(const typename Module::Context<module>& context) : _synchronizer(context)
			{}

			~Worker()
			{
				if (_thread.joinable())
				{
					_thread.join();
				}
				LOG_INFO("Module: " + std::string(Module::Traits<module>::name) + ", shutdown.");
			}

			template<typename... BufferRefs>
			void start(BufferRefs&... buffers)
			{
				LOG_INFO("Module: " + std::string(Module::Traits<module>::name) + ", starting...");
				_thread = std::thread(&Worker::_run<BufferRefs...>, this, std::ref(buffers)...);
			}

		private:
			template<typename... BufferRefs>
			void _run(BufferRefs&... buffers)
			{
				try
				{
					_synchronizer.initialize();
					Error::Result result = _cycle(buffers...);
					if (!result.ok())
					{
						throw Error::Exception("Error in cycle");
					}
				}
				catch (Error::Exception& exception)
				{
					LOG_ERROR(exception.what());
				}
			}

			template<typename... BufferRefs>
			Error::Result _cycle(BufferRefs&... buffers) noexcept
			{
				while (!Application::SignalManager::shutdownRequested())
				{
					Error::Result result = _synchronizer.cycle(buffers...);
					if (!result.ok())
					{
						LOG_ERROR("Cycle: Error in cycle.");
						return {Error::Type::Cycle, 0}; // Right now a failed iteration shuts down the worker. Strategy might change in the future
					}
				}
				return Error::Result::success();
			}

		private:
			std::thread _thread;
			Synchronizer<module> _synchronizer;
		};
	}
}

#endif
