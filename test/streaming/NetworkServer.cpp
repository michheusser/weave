#include "NetworkServer.h"

#include <unistd.h>

#include "NetworkUtilities.h"
#include <weave/error/Exception.h>
#include <weave/error/Result.h>
#include <weave/logging/Macros.h>

namespace test
{
	namespace streaming
	{
		NetworkServer::NetworkServer(const NetworkServerConfiguration& serverConfiguration) : _localFileDescriptor{-1}, _remoteFileDescriptor{-1}, _configuration(serverConfiguration)
		{}

		NetworkServer::~NetworkServer()
		{
			TCPIPV4::close(_localFileDescriptor);
			TCPIPV4::close(_remoteFileDescriptor);
		}

		bool NetworkServer::isInitialized() const
		{
			return _localFileDescriptor != -1 && _remoteFileDescriptor != -1;
		}

		void NetworkServer::initialize()
		{
			try
			{
				_localFileDescriptor = TCPIPV4::createSocket();
				sockaddr_in localAddressObject = TCPIPV4::createAddressObject(_configuration.localAddress, _configuration.localPort);
				TCPIPV4::bindSocket(_localFileDescriptor, localAddressObject);
			}
			catch (weave::error::Exception& exception)
			{
				LOG_ERROR(exception.what());
				throw weave::error::Exception("Failed _initialize");
			}
		}

		void NetworkServer::listen() const
		{
			try
			{
				TCPIPV4::listen(_localFileDescriptor);
			}
			catch (weave::error::Exception& exception)
			{
				LOG_ERROR(exception.what());
				throw weave::error::Exception("Failed listen");
			}
		}

		void NetworkServer::accept()
		{
			try
			{
				_remoteFileDescriptor = TCPIPV4::accept(_localFileDescriptor);
			}
			catch (weave::error::Exception& exception)
			{
				LOG_ERROR(exception.what());
				throw weave::error::Exception("Failed accept");
			}
		}

		weave::error::Result NetworkServer::send(const std::vector<uint8_t>& sendBuffer) const noexcept
		{
			// TRACE_FUNCTION("NetworkServer");
			weave::error::Result result = TCPIPV4::send(_remoteFileDescriptor, sendBuffer);
			if (!result.ok())
			{
				LOG_ERROR("Error sending on filedescriptor: " + std::to_string(_remoteFileDescriptor));
				uint32_t context = _remoteFileDescriptor < 0 ? 0 : static_cast<uint32_t>(_remoteFileDescriptor);
				return {weave::error::Type::NetworkSend, context};
			}
			return weave::error::Result::success();
		}

		weave::error::Result NetworkServer::receive(std::vector<uint8_t>& receiveBuffer) const noexcept
		{
			// TRACE_FUNCTION("NetworkServer");
			weave::error::Result result = TCPIPV4::receive(_remoteFileDescriptor, receiveBuffer);
			if (!result.ok())
			{
				LOG_ERROR("Error receiving on filedescriptor: " + std::to_string(_remoteFileDescriptor));
				uint32_t context = _remoteFileDescriptor < 0 ? 0 : static_cast<uint32_t>(_remoteFileDescriptor);
				return {weave::error::Type::NetworkReceive, context};
			}
			return weave::error::Result::success();
		}
	}
} // test
