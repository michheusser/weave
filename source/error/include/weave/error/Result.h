// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef ERRORCODE_H_2025_09_27_17_10_37
#define ERRORCODE_H_2025_09_27_17_10_37

#include <cstdint>

#include <weave/error/Constants.h>

namespace weave
{
	namespace error
	{
		// Nodiscard in class definition applies it to all functions returning this type
		// Inline function definitions to avoid function call overhead in hot-paths!
		struct [[nodiscard]] Result
		{
			Type type;
			uint32_t context; // e.g. frame
			Result() = delete;

			constexpr Result(Type typeInput, uint32_t contextInput) noexcept : type{typeInput}, context{contextInput}
			{}

			[[nodiscard]] static constexpr Result success() noexcept
			{
				return {Type::None, 0}; // General result class: success doesn't need context
			}

			[[nodiscard]] constexpr bool ok() const noexcept
			{
				return type == Type::None;
			}
		};
	}
}

#endif
