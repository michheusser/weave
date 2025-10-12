// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser
#include "SessionExecutor.h"

#include "../Application/Client.h"
#include "../Application/Server.h"
#include "../Configuration/ConfigurationGenerator.h"
#include "../Error/Exception.h"
#include "../Logging/Macros.h"
#include "../include/weave/profiling/Macros.h"

namespace weave
{
	namespace profiling
	{
		void SessionExecutor::executeDefaultServerSession()
		{
			try{
				ServerSessionConfiguration serverSessionConfiguration = Configuration::ConfigurationGenerator::generateDefaultServerSessionConfiguration();

				TRACE_INIT(serverSessionConfiguration.sessionName, serverSessionConfiguration.sessionDescription);
				METRICS_INIT(serverSessionConfiguration.sessionName, serverSessionConfiguration.sessionDescription);
				CONFIG_INIT(serverSessionConfiguration.sessionName, serverSessionConfiguration.sessionDescription);

				CONFIG_ADD("serverSessionConfiguration", serverSessionConfiguration);

				Application::Server server(serverSessionConfiguration.serverConfiguration);
				// server.run(serverSessionConfiguration.numCycles); // TODO

				TRACE_DISPLAY();
				METRICS_DISPLAY();
				CONFIG_DISPLAY();
				TRACE_DUMP(serverSessionConfiguration.loggingDirectory);
				METRICS_DUMP(serverSessionConfiguration.loggingDirectory);
				CONFIG_DUMP(serverSessionConfiguration.loggingDirectory);
			}
			catch (error::Exception& exception)
			{
				LOG_ERROR(exception.what());
				throw error::Exception("Failed");
			}
		}

		void SessionExecutor::executeDefaultClientSession()
		{
			try
			{
				ClientSessionConfiguration clientSessionConfiguration = Configuration::ConfigurationGenerator::generateDefaultClientSessionConfiguration();

				TRACE_INIT(clientSessionConfiguration.sessionName, clientSessionConfiguration.sessionDescription);
				METRICS_INIT(clientSessionConfiguration.sessionName, clientSessionConfiguration.sessionDescription);
				CONFIG_INIT(clientSessionConfiguration.sessionName, clientSessionConfiguration.sessionDescription);

				CONFIG_ADD("clientSessionConfiguration", clientSessionConfiguration);

				Application::Client client(clientSessionConfiguration.clientConfiguration);
				// client.run(clientSessionConfiguration.numCycles); // TODO

				TRACE_DISPLAY();
				METRICS_DISPLAY();
				CONFIG_DISPLAY();
				TRACE_DUMP(clientSessionConfiguration.loggingDirectory);
				METRICS_DUMP(clientSessionConfiguration.loggingDirectory);
				CONFIG_DUMP(clientSessionConfiguration.loggingDirectory);
			}
			catch (error::Exception& exception)
			{
				LOG_ERROR(exception.what());
				throw error::Exception("Failed");
			}
		}
	}
}
