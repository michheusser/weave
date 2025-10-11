// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser
#include <weave/error/Exception.h>

weave::error::Exception::Exception(const std::string& message) : _message(message)
{}
weave::error::Exception::Exception(const std::string&& message): Exception(message)
{}

const char* weave::error::Exception::what() const noexcept
{
	return _message.c_str();
}
