// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser
#include <weave/utilities/Executable.h>
#include <string>
#include <filesystem>

#ifdef __APPLE__
#include <mach-o/dyld.h>
#elif __linux__
#include <unistd.h>
#endif

namespace weave
{
	namespace utilities
	{
		std::string getExecutableName()
		{
			std::string executablePath;

			#ifdef __APPLE__
			char pathBuffer[1024];
			uint32_t size = sizeof(pathBuffer);
			if (_NSGetExecutablePath(pathBuffer, &size) == 0)
			{
				executablePath = pathBuffer;
			}

			// TODO Check for Linux
			#elif __linux__
			char pathBuffer[1024];
			ssize_t len = readlink("/proc/self/exe", pathBuffer, sizeof(pathBuffer) - 1);
			if (len != -1) {
				pathBuffer[len] = '\0';
				executablePath = pathBuffer;
			}
			#endif

			if (executablePath.empty())
			{
				return "unknown";
			}

			// Extract just the filename without path and extension
			std::filesystem::path path(executablePath);
			return path.stem().string();
		}
	}
}
