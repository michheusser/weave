// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#include "MetricsPrinter.h"

#include <fstream>
#include <iomanip>
#include <algorithm>
#include <filesystem>
#include <weave/logging/Macros.h>
#include <weave/error/Exception.h>

namespace weave
{
	namespace profiling
	{
		namespace
		{

		}

		std::string MetricsPrinter::print(const std::string& sessionName, const std::string& sessionDescription, std::unordered_map<uint64_t, FunctionStatistics>& statistics)
		{
			// Column width configuration
			constexpr int COL_FUNCTION = 50;
			constexpr int COL_TYPE = 10;
			constexpr int COL_INVOCATIONS = 10;
			constexpr int COL_TOTAL_COUNT = 12;
			constexpr int COL_THROUGHPUT = 14;
			constexpr int COL_LATENCY = 10;

			constexpr int TOTAL_WIDTH = COL_FUNCTION + COL_TYPE + COL_INVOCATIONS + COL_TOTAL_COUNT + 2 * COL_THROUGHPUT + 6 * COL_LATENCY;

			std::ostringstream report;

			// Session header
			report << "\n" << std::string(TOTAL_WIDTH, '=') << "\n";
			report << "=== " << sessionName << " ===\n";
			report << sessionDescription << "\n";
			report << std::string(TOTAL_WIDTH, '=') << "\n\n";

			// Table header - Line 1
			report << std::left << std::setw(COL_FUNCTION) << "Function" << std::setw(COL_TYPE) << "Type" << std::setw(COL_INVOCATIONS) << "Calls" << std::setw(COL_TOTAL_COUNT) << "Total" <<
				std::setw(COL_THROUGHPUT) << "Throughput" << std::setw(COL_THROUGHPUT) << "Throughput" << std::setw(COL_LATENCY) << "" << std::setw(COL_LATENCY) << "Latency (ms)" <<
				std::setw(COL_LATENCY) << "" << std::setw(COL_LATENCY) << "" << std::setw(COL_LATENCY) << "" << std::setw(COL_LATENCY) << "" << "\n";

			// Table header - Line 2
			report << std::left << std::setw(COL_FUNCTION) << "" << std::setw(COL_TYPE) << "" << std::setw(COL_INVOCATIONS) << "" << std::setw(COL_TOTAL_COUNT) << "Count" << std::setw(COL_THROUGHPUT)
				<< "Theor.(c/s)" << std::setw(COL_THROUGHPUT) << "Obs.(c/s)" << std::setw(COL_LATENCY) << "Min" << std::setw(COL_LATENCY) << "P50" << std::setw(COL_LATENCY) << "P95" <<
				std::setw(COL_LATENCY) << "P99" << std::setw(COL_LATENCY) << "Max" << std::setw(COL_LATENCY) << "Mean" << "\n";

			report << std::string(TOTAL_WIDTH, '-') << "\n";

			// Table rows
			for (const auto& [hash, stats]: statistics)
			{
				// Throughput calculations
				const uint64_t theoreticalDuration = stats.averageDurationPerUnitNs * stats.totalCount;
				const uint64_t observedDuration = stats.maxEndTimeNs - stats.minStartTimeNs;

				const double throughputTheoretical = _computeThroughput(stats.totalCount, theoreticalDuration);
				const double throughputObserved = _computeThroughput(stats.totalCount, observedDuration);

				// Latency calculations
				const double minLatency = _nsToMs(stats.minLatencyNs);
				const double maxLatency = _nsToMs(stats.maxLatencyNs);
				const double meanLatency = _nsToMs(stats.averageLatencyNs);

				// Percentiles from reservoir
				auto reservoir = stats.latencyReservoir; // Copy for sorting
				const double p50 = _nsToMs(_computePercentile(reservoir, 0.50));
				const double p95 = _nsToMs(_computePercentile(reservoir, 0.95));
				const double p99 = _nsToMs(_computePercentile(reservoir, 0.99));

				// Convert string_view to string for manipulation
				const std::string nameStr(stats.name);
				const std::string typeStr(stats.type);

				// Truncate if too long
				const std::string displayName = (nameStr.length() > COL_FUNCTION - 1) ? nameStr.substr(0, COL_FUNCTION - 4) + "..." : nameStr;
				const std::string displayType = (typeStr.length() > COL_TYPE - 1) ? typeStr.substr(0, COL_TYPE - 4) + "..." : typeStr;

				report << std::left << std::setw(COL_FUNCTION) << displayName << std::setw(COL_TYPE) << displayType << std::setw(COL_INVOCATIONS) << stats.invocations << std::setw(COL_TOTAL_COUNT) <<
					stats.totalCount << std::setw(COL_THROUGHPUT) << std::fixed << std::setprecision(2) << throughputTheoretical << std::setw(COL_THROUGHPUT) << std::fixed << std::setprecision(2) <<
					throughputObserved << std::setw(COL_LATENCY) << std::fixed << std::setprecision(2) << minLatency << std::setw(COL_LATENCY) << std::fixed << std::setprecision(2) << p50 <<
					std::setw(COL_LATENCY) << std::fixed << std::setprecision(2) << p95 << std::setw(COL_LATENCY) << std::fixed << std::setprecision(2) << p99 << std::setw(COL_LATENCY) << std::fixed
					<< std::setprecision(2) << maxLatency << std::setw(COL_LATENCY) << std::fixed << std::setprecision(2) << meanLatency << "\n";
			}

			report << std::string(TOTAL_WIDTH, '-') << "\n\n";
			return report.str();
		}

		void MetricsPrinter::dumpToCsv(const std::string& filename, std::unordered_map<uint64_t, FunctionStatistics>& statistics)
		{
			try
			{
				std::filesystem::path filepath(filename);
				std::filesystem::path parentDir = filepath.parent_path();

				// Create all intermediate directories
				if (!parentDir.empty())
				{
					std::error_code ec;
					std::filesystem::create_directories(parentDir, ec);
					if (ec)
					{
						throw error::Exception("Failed to create directories: " + ec.message());
					}
				}

				std::ofstream csvFile(filename);
				if (!csvFile.is_open())
				{
					throw error::Exception("Failed to open file: " + filename);
				}

				// CSV Header
				csvFile << "function_name,type," << "invocations,total_count," << "throughput_theoretical_cps,throughput_observed_cps," <<
					"min_latency_ms,p50_latency_ms,p95_latency_ms,p99_latency_ms," << "max_latency_ms,mean_latency_ms\n";

				// Data rows
				for (const auto& [hash, stats]: statistics)
				{
					// Throughput calculations
					const uint64_t theoreticalDuration = stats.averageDurationPerUnitNs * stats.totalCount;
					const uint64_t observedDuration = stats.maxEndTimeNs - stats.minStartTimeNs;

					const double throughputTheoretical = _computeThroughput(stats.totalCount, theoreticalDuration);
					const double throughputObserved = _computeThroughput(stats.totalCount, observedDuration);

					// Latency calculations
					const double minLatency = _nsToMs(stats.minLatencyNs);
					const double maxLatency = _nsToMs(stats.maxLatencyNs);
					const double meanLatency = _nsToMs(stats.averageLatencyNs);

					// Percentiles
					auto reservoir = stats.latencyReservoir; // Copy for sorting
					const double p50 = _nsToMs(_computePercentile(reservoir, 0.50));
					const double p95 = _nsToMs(_computePercentile(reservoir, 0.95));
					const double p99 = _nsToMs(_computePercentile(reservoir, 0.99));

					csvFile << "\"" << stats.name << "\"," << "\"" << stats.type << "\"," << stats.invocations << "," << stats.totalCount << "," << std::fixed << std::setprecision(2) <<
						throughputTheoretical << "," << std::fixed << std::setprecision(2) << throughputObserved << "," << std::fixed << std::setprecision(3) << minLatency << "," << std::fixed <<
						std::setprecision(3) << p50 << "," << std::fixed << std::setprecision(3) << p95 << "," << std::fixed << std::setprecision(3) << p99 << "," << std::fixed << std::setprecision(3)
						<< maxLatency << "," << std::fixed << std::setprecision(3) << meanLatency << "\n";
				}

				csvFile.close();
				LOG_INFO("Metrics dumped to: {}", filename);
			}
			catch (error::Exception& exception)
			{
				LOG_WARN(exception.what());
				// No re-throwing
			}
		}

		// Helper: Compute percentile from sorted reservoir
		uint64_t MetricsPrinter::_computePercentile(std::vector<uint64_t> samples, double percentile)
		{
			if (samples.empty())
				return 0;

			std::sort(samples.begin(), samples.end());
			const size_t index = static_cast<size_t>(percentile * (samples.size() - 1));
			return samples[index];
		}

		// Helper: Convert nanoseconds to milliseconds
		double MetricsPrinter::_nsToMs(uint64_t ns)
		{
			constexpr double NS_PER_MS = 1e6;
			return static_cast<double>(ns) / NS_PER_MS;
		}

		// Helper: Convert nanoseconds to seconds
		double MetricsPrinter::_nsToS(uint64_t ns)
		{
			constexpr double NS_PER_S = 1e9;
			return static_cast<double>(ns) / NS_PER_S;
		}

		// Helper: Compute throughput (count per second)
		double MetricsPrinter::_computeThroughput(uint64_t count, uint64_t durationNs)
		{
			if (durationNs == 0)
				return 0.0;
			constexpr double NS_PER_S = 1e9;
			return static_cast<double>(count) * NS_PER_S / static_cast<double>(durationNs);
		}
	}
}
