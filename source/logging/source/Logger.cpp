#include <weave/logging/Logger.h>


#if WEAVE_ENABLE_LOGGING
#include <vector>
#include <iostream>
#include <weave/utilities/Executable.h>
#include <weave/logging/Constants.h>
#endif

namespace weave
{
	namespace logging
	{
		#if WEAVE_ENABLE_LOGGING
		std::shared_ptr<spdlog::logger> Logger::_logger = nullptr;

		void Logger::ensureInitialized()
		{
			if (!_logger)
			{
				initialize();
			}
		}

		void Logger::initialize(bool fileLogging)
		{
			try
			{
				std::vector<spdlog::sink_ptr> sinks;

				auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
				console_sink->set_level(spdlog::level::trace);
				console_sink->set_pattern(DEFAULT_CONSOLE_PATTERN.data());
				sinks.push_back(console_sink);

				if (fileLogging)
				{
					std::string executableName = utilities::getExecutableName();
					std::string loggingFileName = executableName + ".log";
					auto rotating_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(loggingFileName, DEFAULT_FILE_SIZE_IN_BYTES, DEFAULT_MAX_FILES);
					rotating_sink->set_level(spdlog::level::trace);
					rotating_sink->set_pattern(DEFAULT_FILE_PATTERN.data());
					sinks.push_back(rotating_sink);
				}

				_logger = std::make_shared<spdlog::logger>(DEFAULT_LOGGER_NAME.data(), sinks.begin(), sinks.end());
				_logger->set_level(spdlog::level::trace);
				_logger->flush_on(spdlog::level::warn);

				spdlog::register_logger(_logger);
			}
			catch (const std::exception& e)
			{
				std::cerr << "Failed to initialize logging: " << e.what() << std::endl;
				_logger = spdlog::stdout_color_mt("weaveEmergency");
			}
		}

		void Logger::setLevel(Level level)
		{
			ensureInitialized();
			_logger->set_level(convertLevel(level));
		}

		void Logger::shutdown()
		{
			if (_logger)
			{
				_logger->flush();
				spdlog::drop(DEFAULT_LOGGER_NAME.data());
				_logger = nullptr;
			}
		}

		std::shared_ptr<spdlog::logger>& Logger::getInternalLogger()
		{
			ensureInitialized();
			return _logger;
		}

		spdlog::level::level_enum Logger::convertLevel(Level level)
		{
			switch (level)
			{
			case Level::Trace: return spdlog::level::trace;
			case Level::Debug: return spdlog::level::debug;
			case Level::Info: return spdlog::level::info;
			case Level::Warning: return spdlog::level::warn;
			case Level::Error: return spdlog::level::err;
			case Level::Fatal: return spdlog::level::critical;
			default: return spdlog::level::info;
			}
		}
		#endif
	}
}
