// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#include "../Logging/Macros.h"
#include "../Error/Exception.h"
#include "../Profiling/SessionExecutor.h"

int main()
{
	try
	{
		weave::Profiling::SessionExecutor::executeDefaultClientSession();
		return 0;
	}
	catch (weave::error::Exception& exception)
	{
		LOG_ERROR(exception.what());
		return 1;
	}
}
