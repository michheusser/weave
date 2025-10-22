// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#include "../logging/Macros.h"
#include "../error/Exception.h"
#include "../Profiling/SessionExecutor.h"

int main()
{
	try
	{
		weave::profiling::SessionExecutor::executeDefaultServerSession();
		return 0;
	}
	catch (weave::error::Exception& exception)
	{
		LOG_ERROR(exception.what());
		return 1;
	}
}
