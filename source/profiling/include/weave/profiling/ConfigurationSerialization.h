// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef CONFIGURATIONSERIALIZATION_H_2025_09_13_20_32_05
#define CONFIGURATIONSERIALIZATION_H_2025_09_13_20_32_05
#include <iosfwd>
#include <vector>
#include <iostream>

namespace weave
{
	namespace profiling
	{
		inline std::ostream& operator<<(std::ostream& os, const std::vector<int>& vec) {
			os << "["; for (size_t i = 0; i < vec.size(); ++i) { if (i > 0) os << ","; os << vec[i]; } os << "]"; return os;
		}

		// std::tuple<double, double, double>
		inline std::ostream& operator<<(std::ostream& os, const std::tuple<double, double, double>& t) {
			return os << "(" << std::get<0>(t) << "," << std::get<1>(t) << "," << std::get<2>(t) << ")";
		}
	}
}

#endif
