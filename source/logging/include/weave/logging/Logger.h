#ifndef LOGGER_H
#define LOGGER_H

#include <weave/logging/Definitions.h>

#if WEAVE_ENABLE_LOGGING
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <memory>
#include <weave/logging/Constants.h>
#endif

namespace weave
{
	namespace logging
	{
		class Logger
		{
		public:
			enum class Level
			{
				Trace,
				Debug,
				Info,
				Warning,
				Error,
				Fatal
			};

			#if WEAVE_ENABLE_LOGGING
			static void initialize(bool fileLogging = DEFAULT_FILE_LOGGING_ENABLED);
			static void setLevel(Level level);
			static void shutdown();
			static std::shared_ptr<spdlog::logger>& getInternalLogger();

		private:
			static std::shared_ptr<spdlog::logger> _logger;
			static spdlog::level::level_enum convertLevel(Level level);
			static void ensureInitialized();
			#else
            static constexpr void initialize(const std::string& = "", bool = true) noexcept {}
            static constexpr void initializeWithRotation(const std::string& = "", size_t = 0, size_t = 0, bool = true) noexcept {}
            static constexpr void setLevel(Level) noexcept {}
            static constexpr void shutdown() noexcept {}
			#endif
		};
	}
}

#endif // LOGGER_H
