// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser
#include <random>
#include <weave/profiling/MetricsCollector.h>
#include <weave/logging//Macros.h>
#include "MetricsPrinter.h"
#include "weave/profiling/constants.h"
#include "weave/profiling/MetricsContext.h"

namespace weave
{
	namespace profiling
	{
		MetricsCollector::MetricsCollector(const std::string& sessionName, const std::string& sessionDescription) : _sessionName(sessionName), _sessionDescription(sessionName)
		{}

		void MetricsCollector::addMetricsSpan(const std::shared_ptr<MetricsSpanData>& metricsSpanData)
		{
			std::unique_lock lock(_mutex);
			if (!_statistics.contains(metricsSpanData->hash))
			{
				_statistics[metricsSpanData->hash] = FunctionStatistics(); // TODO Potentially more effective with emplace/make_pair
				_statistics[metricsSpanData->hash].latencyReservoir.reserve(constants::MAX_RESERVOIR_SIZE);
				_statistics[metricsSpanData->hash].name = metricsSpanData->name;
				_statistics[metricsSpanData->hash].type = metricsSpanData->type;
			}
			FunctionStatistics& curStatistics = _statistics[metricsSpanData->hash];

			// Throughput
			/*
			 * To update the average we use the following relationship:
			 * d_av,n : average duration for the nth call
			 * c_n: count for the nth call
			 *
			 * d_av,n   = 1/(c_1 + ... +c_n)            * (d_1 + d_2 + ... + d_n)
			 * d_av,n+1 = 1/(c_1 + ... + c_n + c_(n+1)) * (d_1 + d_2 + ... + d_n + d_(n+1))
			 * => d_av,n+1 = (c_1 + ... + c_n)/(c_1 + ... + c_n + c_(n+1)) * d_av,n + 1/(c_1 + ... + c_n + c_(n+1))*dn+1
			 *
			 * We use this expression to avoid overflows by using the total sum of duration, which we're anyway using for averages.
			 * However, floating point error can accumulate over time in this way. For now, we'll favor this way to avoid overflows.
			 */
			uint64_t duration = metricsSpanData->endNs - metricsSpanData->startNs;
			uint64_t newTotalCount = curStatistics.totalCount + metricsSpanData->count;
			const double a = static_cast<double>(curStatistics.totalCount) / static_cast<double>(newTotalCount);
			const double b = static_cast<double>(1) / static_cast<double>(newTotalCount);
			curStatistics.averageDurationPerUnitNs = static_cast<uint64_t>(a * static_cast<double>(curStatistics.averageDurationPerUnitNs) + b * static_cast<double>(duration));
			curStatistics.totalCount = newTotalCount;
			curStatistics.minStartTimeNs = std::min(curStatistics.minStartTimeNs, metricsSpanData->startNs);
			curStatistics.maxEndTimeNs = std::max(curStatistics.maxEndTimeNs, metricsSpanData->endNs);
			curStatistics.averageDurationWallClockNs = static_cast<uint64_t>(static_cast<double>(curStatistics.maxEndTimeNs - curStatistics.minStartTimeNs) / static_cast<double>(curStatistics.totalCount));

			// Latency
			uint64_t newInvocations = curStatistics.invocations + 1;
			curStatistics.minLatencyNs = std::min(curStatistics.minLatencyNs, duration);
			curStatistics.maxLatencyNs = std::max(curStatistics.maxLatencyNs, duration);
			const double c = static_cast<double>(curStatistics.invocations) / static_cast<double>(newInvocations);
			const double d = static_cast<double>(1) / static_cast<double>(newInvocations);
			curStatistics.averageLatencyNs = static_cast<uint64_t>(c * static_cast<double>(curStatistics.averageLatencyNs) + d * static_cast<double>(duration));
			curStatistics.invocations = newInvocations;

			// Reservoir update
			if (curStatistics.latencyReservoir.size() < constants::MAX_RESERVOIR_SIZE) // We add the new latency
			{
				curStatistics.latencyReservoir.push_back(duration);
			}
			else
			{
				// TODO can be optimized
				std::mt19937 range(std::random_device{}());
				std::uniform_int_distribution<size_t> dist(0, curStatistics.latencyReservoirCount);
				size_t randomIndex = dist(range);

				if (randomIndex< constants::MAX_RESERVOIR_SIZE)
				{
					curStatistics.latencyReservoir[randomIndex] = duration;
				}
			}
			curStatistics.latencyReservoirCount += 1;
		}

		void MetricsCollector::display()
		{
			std::shared_lock<std::shared_mutex> lock(_mutex);
			std::string informationString = MetricsPrinter::print(_sessionName, _sessionDescription, _statistics);
			LOG_INFO(informationString);
		}

		void MetricsCollector::dump(const std::string& directory)
		{
			std::shared_lock<std::shared_mutex> lock(_mutex);
			const std::string fileName = directory + "/" + _sessionName + "_METRICS.csv";
			MetricsPrinter::dumpToCsv(fileName, _statistics);
		}
	}
}
