// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser
#include "../streaming/NetworkClient.h"

#include "NetworkUtilities.h"
#include <weave/error/Exception.h>
#include <weave/error/Result.h>
#include <weave/logging/Macros.h>

namespace test
{
	namespace streaming
	{
		NetworkClient::NetworkClient(const NetworkClientConfiguration& networkClientConfiguration) : _localFileDescriptor{-1}, _configuration(networkClientConfiguration)
		{}

		NetworkClient::~NetworkClient()
		{
			TCPIPV4::close(_localFileDescriptor);
		}

		bool NetworkClient::isInitialized() const
		{
			return _localFileDescriptor != -1;
		}

		void NetworkClient::initialize()
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

		void NetworkClient::connect() const
		{
			try
			{
				sockaddr_in remoteAddressObject = TCPIPV4::createAddressObject(_configuration.remoteAddress, _configuration.remotePort);
				TCPIPV4::connect(_localFileDescriptor, remoteAddressObject);
			}
			catch (weave::error::Exception& exception)
			{
				LOG_ERROR(exception.what());
				throw weave::error::Exception("Failed connect");
			}
		}

		weave::error::Result NetworkClient::send(const std::vector<uint8_t>& sendBuffer) const noexcept
		{
			// TRACE_FUNCTION("NetworkClient");
			weave::error::Result result = TCPIPV4::send(_localFileDescriptor, sendBuffer);
			if (!result.ok())
			{
				LOG_ERROR("Error sending on filedescriptor: " + std::to_string(_localFileDescriptor));
				uint32_t context = _localFileDescriptor < 0 ? 0 : static_cast<uint32_t>(_localFileDescriptor);
				return {weave::error::Type::NetworkSend, context};
			}
			return weave::error::Result::success();
		}

		weave::error::Result NetworkClient::receive(std::vector<uint8_t>& receiveBuffer) const noexcept
		{
			// TRACE_FUNCTION("NetworkClient");
			weave::error::Result result = TCPIPV4::receive(_localFileDescriptor, receiveBuffer);
			if (!result.ok())
			{
				LOG_ERROR("Error receiving on filedescriptor: " + std::to_string(_localFileDescriptor));
				uint32_t context = _localFileDescriptor < 0 ? 0 : static_cast<uint32_t>(_localFileDescriptor);
				return {weave::error::Type::NetworkReceive, context};
			}
			return weave::error::Result::success();
		}
	}
}
