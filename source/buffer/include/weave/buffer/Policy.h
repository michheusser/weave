// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef POLICY_H_2025_09_29_16_42_01
#define POLICY_H_2025_09_29_16_42_01

#include <cstdint>
#include <weave/buffer/Constants.h>

namespace weave
{
	namespace buffer
	{
		// TODO Deal with the following dilemma: Is policy only concerning either reader/writer? There seems to be a coupling by making the policy part of the buffer, but
		//		reader/writer policy might mean different things?
		template <constants::PolicyType type>
		struct Policy; // Never instantiated as generic

		template <>
		struct Policy<constants::PolicyType::Lossless> // Creates backpressure, but everything gets processed eventually
		{
			static constexpr constants::CongestionStrategy congestion = constants::CongestionStrategy::Backpressure;
			static constexpr constants::TimeoutStrategy timeout = constants::TimeoutStrategy::Infinite;
		};

		template <>
		struct Policy<constants::PolicyType::Realtime> // Drops old, "stale" data
		{
			static constexpr constants::CongestionStrategy congestion = constants::CongestionStrategy::DropOldest;
			static constexpr constants::TimeoutStrategy timeout = constants::TimeoutStrategy::Timed;
		};

		template <>
		struct Policy<constants::PolicyType::Attempts> // Drops old, "stale" data with attempts
		{
			static constexpr constants::CongestionStrategy congestion = constants::CongestionStrategy::DropOldest;
			static constexpr constants::TimeoutStrategy timeout = constants::TimeoutStrategy::Attempts;
			static constexpr uint64_t timeoutMS = 100;
			static constexpr std::uint64_t numAttempts = 1;
		};

		template <>
		struct Policy<constants::PolicyType::LiveStream> // Drops old, "stale" data but timed
		{
			static constexpr constants::CongestionStrategy congestion = constants::CongestionStrategy::DropOldest;
			static constexpr constants::TimeoutStrategy timeout = constants::TimeoutStrategy::Timed;
			static constexpr uint64_t timeoutMS = 100;
		};

		template <>
		struct Policy<constants::PolicyType::Throttled> // Respects "committed" data and rejects new incoming one
		{
			static constexpr constants::CongestionStrategy congestion = constants::CongestionStrategy::DropNewest;
			static constexpr constants::TimeoutStrategy timeout = constants::TimeoutStrategy::Timed;
		};
	}
}

#endif
