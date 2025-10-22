// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef REFLECTION_H_2025_10_22_14_12_39
#define REFLECTION_H_2025_10_22_14_12_39
#include <string_view>

namespace weave
{
	namespace utilities
	{
		template<typename T>
		constexpr std::string_view typeName() noexcept
		{
			std::string_view name;

#if defined(__clang__) || defined(__GNUC__)
			name = __PRETTY_FUNCTION__;
			// Clang: "std::string_view type_name() [T = MyType]"
			// GCC:   "constexpr std::string_view type_name() [with T = MyType]"

			std::size_t start = name.find('=') + 2; // Skip "= "
			std::size_t end = name.find_last_of(']');
			name = name.substr(start, end - start);

#elif defined(_MSC_VER) // TODO NOT TESTED
		name = __FUNCSIG__;
		// MSVC: "std::string_view __cdecl type_name<MyType>(void)"

		std::size_t start = name.find('<') + 1;
		std::size_t end = name.find_last_of('>');
		name = name.substr(start, end - start);

#else
		name = "unknown";
#endif

			return name;
		}
	} // utilities
} // weave

#endif
