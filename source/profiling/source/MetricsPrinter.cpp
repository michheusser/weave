// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser
#include "MetricsPrinter.h"

#include <fstream>
#include <iomanip>
#include <map>
#include <weave/logging/Macros.h>
#include <weave/error/Exception.h>

namespace weave
{
	namespace profiling
	{
		std::string MetricsPrinter::print(const std::string& sessionName, const std::string& sessionDescription, std::vector<std::shared_ptr<MetricsSpanData>>& metricsSpanTrees)
		{
			std::ostringstream report;

			// Session header
			report << "\n=== " << sessionName << " ===\n";
			report << sessionDescription << "\n\n";

			// Group data by processID
			std::unordered_map<pid_t, std::vector<std::shared_ptr<MetricsSpanData>>> processBuckets;
			for (const auto& span : metricsSpanTrees)
			{
				processBuckets[span->processID].push_back(span);
			}

			// Process each PID's data
			for (const auto& [processID, spans] : processBuckets)
			{
				report << "Process ID: " << processID << "\n";
				report << std::string(80, '-') << "\n";

				// Aggregate metrics by (name, type) key
				struct AggregatedMetrics
				{
					uint64_t totalCount = 0;
					uint64_t totalDurationNs = 0;
					uint64_t minStartTimeNs = UINT64_MAX;
					uint64_t maxEndTimeNs = 0;
				};

				std::map<std::pair<std::string, std::string>, AggregatedMetrics> aggregated;

				for (const auto& span : spans)
				{
					auto key = std::make_pair(span->name, span->type);
					auto& metrics = aggregated[key];

					uint64_t spanDuration = span->endInNanoseconds - span->startInNanoseconds;

					metrics.totalCount += span->count;
					metrics.totalDurationNs += spanDuration;
					metrics.minStartTimeNs = std::min(metrics.minStartTimeNs, span->startInNanoseconds);
					metrics.maxEndTimeNs = std::max(metrics.maxEndTimeNs, span->endInNanoseconds);
				}

				// Table header
				report << std::left
					<< std::setw(60) << "Name"
					<< std::setw(20) << "Type"
					<< std::setw(20) << "Total Count"
					<< std::setw(20) << "Avg ms/count"
					<< std::setw(20) << "Avg ms/count"
					<< std::setw(20) << "Avg count/s"
					<< std::setw(20) << "Avg count/s"
					<< "\n"
					<< std::setw(60) << ""
					<< std::setw(20) << ""
					<< std::setw(20) << ""
					<< std::setw(20) << "Theor./Local TP"
					<< std::setw(20) << "Obser./Overall TP"
					<< std::setw(20) << "Theor./Local TP"
					<< std::setw(20) << "Obser./Overall TP"
					<< "\n";
				report << std::string(93, '-') << "\n";

				// Table rows
				for (const auto& [key, metrics] : aggregated)
				{
					const std::string& name = key.first;
					const std::string& type = key.second;

					constexpr int NANOSECONDS_PER_MILLISECONDS = 1e6;
					constexpr int NANOSECONDS_PER_SECOND = 1e9;
					double avgMilisecondsPerCountInModule = metrics.totalCount > 0 ? (static_cast<double>(metrics.totalDurationNs) / metrics.totalCount) / NANOSECONDS_PER_MILLISECONDS : 0.0;
					double avgMilisecondsPerCountOverall = metrics.totalCount > 0
						                                       ? (static_cast<double>(metrics.maxEndTimeNs - metrics.minStartTimeNs) / metrics.totalCount) / NANOSECONDS_PER_MILLISECONDS
						                                       : 0.0;
					double avgCountPerSecondInModule = metrics.totalDurationNs > 0 ? static_cast<double>(metrics.totalCount) * NANOSECONDS_PER_SECOND / (metrics.totalDurationNs) : 0.0;
					double avgCountPerSecondOverall = metrics.maxEndTimeNs - metrics.minStartTimeNs > 0
						                                  ? static_cast<double>(metrics.totalCount) * NANOSECONDS_PER_SECOND / (metrics.maxEndTimeNs - metrics.minStartTimeNs)
						                                  : 0.0;

					report << std::left
						<< std::setw(60) << (name.length() > (70 - 1) ? name.substr(0, (70 - 1 - 3)) + "..." : name)
						<< std::setw(20) << (type.length() > (20 - 1) ? type.substr(0, (20 - 1 - 3)) + "..." : type)
						<< std::setw(20) << metrics.totalCount
						<< std::setw(20) << std::fixed << std::setprecision(3) << avgMilisecondsPerCountInModule
						<< std::setw(20) << std::fixed << std::setprecision(3) << avgMilisecondsPerCountOverall
						<< std::setw(20) << std::fixed << std::setprecision(2) << avgCountPerSecondInModule
						<< std::setw(20) << std::fixed << std::setprecision(2) << avgCountPerSecondOverall
						<< "\n";
				}

				report << "\n";
			}

			return report.str();
		}

		void MetricsPrinter::dumpToCsv(const std::string& filename, std::vector<std::shared_ptr<MetricsSpanData>>& metricsSpanTrees)
		{
			try
			{
				std::filesystem::path filepath(filename);
				std::filesystem::path parent_dir = filepath.parent_path();

				// Create all intermediate directories
				if (!parent_dir.empty()) {
					std::error_code ec;
					std::filesystem::create_directories(parent_dir, ec);
					if (ec) {
						throw error::Exception("Failed to create directories: " + ec.message());
					}
				}

				std::ofstream csvFile(filename);
				if (!csvFile.is_open())
				{
					throw error::Exception("Failed to open file: " + filename);
				}

				// CSV Header
				csvFile << "name,thread_id,process_id,start_ns,end_ns,duration_ns,type,count\n";

				// Data rows
				for (const auto& span : metricsSpanTrees)
				{
					uint64_t duration = span->endInNanoseconds - span->startInNanoseconds;

					csvFile << "\"" << span->name << "\","
						<< span->threadID << ","
						<< span->processID << ","
						<< span->startInNanoseconds << ","
						<< span->endInNanoseconds << ","
						<< duration << ","
						<< "\"" << span->type << "\","
						<< span->count << "\n";
				}

				csvFile.close();
			}
			catch (error::Exception& exception)
			{
				LOG_WARN(exception.what());
				// No re-throwing
			}
		}
	}
}
