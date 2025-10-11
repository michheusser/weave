// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser
#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <exception>
#include <string>

namespace weave
{
	namespace error
	{
		class Exception : public std::exception
		{
		public:
			explicit Exception(const std::string& message);
			explicit Exception(const std::string&& message);
			const char* what() const noexcept override;

		private:
			std::string _message;
		};
	}
}

/**
 * The noexcept philosophy: it's a declaration that either this function genuinely cannot throw, or any exception
 * represents such a fundamental system failure that immediate termination is the appropriate response. This is
 * particularly cogent in systems programming where certain invariants must hold for the program to be meaningful.
 */

#endif //EXCEPTION_H
