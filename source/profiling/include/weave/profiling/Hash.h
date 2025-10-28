// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef HASH_H_2025_10_25_14_08_03
#define HASH_H_2025_10_25_14_08_03
#include <cstdint>

#include "constants.h"

namespace weave
{
	namespace profiling
	{
		class Hash
		{
		public:
			/**
			 * We use the FNV1a hashing function https://en.wikipedia.org/wiki/Fowler–Noll–Vo_hash_function for simplicity and compile time hashing
			 * @param input string view
			 * @return hash value of string
			 */
			static constexpr uint64_t hashString(const std::string_view& input)
			{
				uint64_t hash = FNV_OFFSET_BASIS;
				for (int i = 0; i < input.size(); ++i)
				{
					hash = (hash ^ static_cast<uint64_t>(input[i])) * FNV_PRIME;
				}
				return hash;
			}

		private:
			static constexpr uint64_t FNV_OFFSET_BASIS = 14695981039346656037ULL;
			static constexpr uint64_t FNV_PRIME = 1099511628211ULL;
		};
	}
}

#endif
