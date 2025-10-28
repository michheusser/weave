// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef FUNCTIONSTATISTICS_H_2025_10_25_13_43_37
#define FUNCTIONSTATISTICS_H_2025_10_25_13_43_37
#include <cstdint>
#include <vector>

#include "../../../../../../../../../opt/homebrew/Cellar/opencv/4.11.0_1/include/opencv4/opencv2/core/hal/interface.h"

struct FunctionStatistics
{
	// Metadata
	std::string_view name;
	std::string_view type;
	// We do not store process / thread id's because functions can be theoretically called from different places.

	// Throughput
	uint64_t totalCount = 0;
	uint64_t averageDurationPerUnitNs = 0; // For throughput in actual duration of each count
	uint64_t minStartTimeNs = UINT64_MAX;
	uint64_t maxEndTimeNs = 0;
	uint64_t averageDurationWallClockNs = 0; // For observed throughput across start of first call and end of last call

	// Latency
	uint64_t invocations = 0;
	uint64_t minLatencyNs = UINT64_MAX;
	uint64_t maxLatencyNs = 0;
	uint64_t averageLatencyNs = 0;

	std::vector<uint64_t> latencyReservoir;
	size_t latencyReservoirCount = 0;
};

#endif