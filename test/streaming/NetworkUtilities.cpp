// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser
#include "NetworkUtilities.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sstream>
#include <iomanip>
#include <vector>
#include <algorithm>

#include "constants.h"
#include <weave/error/Exception.h>
#include <weave/error/Result.h>
#include <weave/logging/Macros.h>

namespace test
{
	namespace streaming
	{
		namespace TCPIPV4
		{
			sockaddr_in createAddressObject(const std::string& address, const uint16_t port)
			{
				try
				{
					sockaddr_in addressObject{};
					addressObject.sin_family = constants::IPv4_FAMILY;
					addressObject.sin_port = htons(port);
					int clientAddressRval = ::inet_pton(constants::IPv4_FAMILY, address.c_str(), &addressObject.sin_addr);
					if (clientAddressRval == 0)
					{
						throw weave::error::Exception("Invalid IP Address");
					}
					if (clientAddressRval == -1)
					{
						throw weave::error::Exception("System Error" + errnoToString(errno));
					}
					return addressObject;
				}
				catch (weave::error::Exception& exception)
				{
					LOG_ERROR(exception.what());
					throw weave::error::Exception("Failed createAddressObject");
				}
			}

			int createSocket()
			{
				try
				{
					int fileDescriptor = ::socket(constants::IPv4_FAMILY, constants::TCP_SOCKET_TYPE, 0);
					if (fileDescriptor == -1)
					{
						throw weave::error::Exception("System Error" + errnoToString(errno));
					}
					return fileDescriptor;
				}
				catch (weave::error::Exception& exception)
				{
					LOG_ERROR(exception.what());
					throw weave::error::Exception("Failed createSocket");
				}
			}

			void bindSocket(const int32_t localFileDescriptor, const sockaddr_in& localAddressObject)
			{
				try
				{
					int bindValue = ::bind(localFileDescriptor, reinterpret_cast<const sockaddr*>(&localAddressObject), sizeof(localAddressObject));
					if (bindValue == -1)
					{
						throw weave::error::Exception("System Error" + errnoToString(errno));
					}
				}
				catch (weave::error::Exception& exception)
				{
					LOG_ERROR(exception.what());
					throw weave::error::Exception("Failed bindSocket");
				}
			}

			void connect(const int32_t localFileDescriptor, const sockaddr_in& remoteAddressObject)
			{
				try
				{
					int connectRval = ::connect(localFileDescriptor,
					                            reinterpret_cast<const sockaddr*>(&remoteAddressObject),
					                            sizeof(remoteAddressObject));
					if (connectRval == -1)
					{
						throw weave::error::Exception("System Error" + errnoToString(errno));
					}
					LOG_DEBUG("Connected Successfully.");
				}
				catch (weave::error::Exception& exception)
				{
					LOG_ERROR(exception.what());
					throw weave::error::Exception("Failed connect");
				}
			}

			weave::error::Result send(const int32_t localFileDescriptor, const std::vector<uint8_t>& buffer) noexcept
			{
				// TODO Currently sends the whole buffer, even if the encoded data occupies a smaller amount. Performance optimization here.
				const size_t totalBytesToSend = buffer.size();
				size_t totalBytesSent = 0;
				const uint8_t* bufferStart = buffer.data();
				LOG_DEBUG("Bytes to be sent: " + std::to_string(totalBytesToSend));
				while (totalBytesSent < totalBytesToSend)
				{
					size_t curBytesSent = ::send(localFileDescriptor, bufferStart + totalBytesSent, totalBytesToSend - totalBytesSent, 0);
					if (curBytesSent == -1)
					{
						LOG_ERROR("System Error: " + errnoToString(errno));
						uint32_t context = localFileDescriptor < 0 ? 0 : static_cast<uint32_t>(localFileDescriptor);
						return {weave::error::Type::NetworkSend, context};
					}
					totalBytesSent += curBytesSent;
					LOG_TRACE("Successfully sent: " + std::to_string(curBytesSent) + " bytes. Total sent: " + std::to_string(totalBytesSent) + " of " + std::to_string(totalBytesToSend));
				}
				// METRICS_BYTES("NetworkUtilities", totalBytesSent);
				LOG_DEBUG("All bytes sent: " + std::to_string(totalBytesSent));
				return weave::error::Result::success();
			}

			weave::error::Result receive(const int32_t localFileDescriptor, std::vector<uint8_t>& buffer) noexcept
			{
				// TODO Currently receives the whole length of the buffer, even if the encoded data occupies a smaller amount. Performance optimization here.
				// The buffer needs to be exactly the size of the sending message
				const size_t totalBytesToReceive = buffer.size();
				size_t totalBytesReceived = 0;
				const uint8_t* bufferStart = buffer.data();
				LOG_DEBUG("Bytes to be received: " + std::to_string(totalBytesToReceive));

				while (totalBytesReceived < totalBytesToReceive)
				{
					size_t curBytesReceived = ::recv(localFileDescriptor, buffer.data() + totalBytesReceived, buffer.size() - totalBytesReceived, 0);
					if (curBytesReceived == 0)
					{
						LOG_ERROR("No bytes received, remote server has closed connection");
						uint32_t context = localFileDescriptor < 0 ? 0 : static_cast<uint32_t>(localFileDescriptor);
						return {weave::error::Type::NetworkReceive, context};
					}
					if (curBytesReceived == -1)
					{
						LOG_ERROR("System Error: " + errnoToString(errno));
						uint32_t context = localFileDescriptor < 0 ? 0 : static_cast<uint32_t>(localFileDescriptor);
						return {weave::error::Type::NetworkReceive, context};
					}
					totalBytesReceived += curBytesReceived;
					LOG_TRACE("Successfully received: " + std::to_string(curBytesReceived) + " bytes. Total received: " + std::to_string(totalBytesReceived) + " of " + std::to_string(totalBytesToReceive));
				}
				// METRICS_BYTES("NetworkUtilities", totalBytesReceived);
				LOG_DEBUG("All bytes received: " + std::to_string(totalBytesReceived));
				return weave::error::Result::success();
			}

			void listen(const int32_t localFileDescriptor)
			{
				try
				{
					int backlog = SOMAXCONN;
					LOG_DEBUG("Entering listening mode...");
					int listenRval = ::listen(localFileDescriptor, backlog);
					if (listenRval == -1)
					{
						throw weave::error::Exception("System Error: " + errnoToString(errno));
					}
					LOG_DEBUG("Awaiting connections...");
				}
				catch (weave::error::Exception& exception)
				{
					LOG_ERROR(exception.what());
					throw weave::error::Exception("Failed listen");
				}
			}

			int accept(const int32_t localFileDescriptor)
			{
				try
				{
					/* The local file descriptor is not a communication endpoint like it is in the client.
					 * It is a passive socket used only to accept incoming connection requests.
					 * The idea here is that we can accept multiple connections, and thus we need a socket
					 * exclusively for accepting those and not for data-transfer.
					*/
					sockaddr_in remoteAddressObject{}; // TODO Unused
					socklen_t remoteAddressObjectLength = sizeof(remoteAddressObject);
					int remoteFileDescriptor = accept(localFileDescriptor, reinterpret_cast<sockaddr*>(&remoteAddressObject), &remoteAddressObjectLength);
					if (remoteFileDescriptor == -1)
					{
						throw weave::error::Exception("System Error: " + errnoToString(errno));
					}
					LOG_DEBUG("Connected successfully.");
					return remoteFileDescriptor;
				}
				catch (weave::error::Exception& exception)
				{
					LOG_ERROR(exception.what());
					throw weave::error::Exception("Failed acceptConnections()");
				}
			}

			std::string bufferToHex(const std::vector<uint8_t>& buffer, size_t numBytes)
			{
				if (numBytes == 0) numBytes = buffer.size();
				numBytes = std::min(numBytes, buffer.size());

				std::ostringstream oss;
				oss << std::hex << std::setfill('0');

				for (size_t i = 0; i < numBytes; ++i)
				{
					oss << std::setw(2) << static_cast<int>(buffer[i]);
				}

				return oss.str();
			}

			void close(const int32_t localFileDescriptor)
			{
				if (localFileDescriptor >= 0)
				{
					shutdown(localFileDescriptor, SHUT_RDWR);
					::close(localFileDescriptor);
				}
				// TODO Check if errors (best industry practices)
			}

			std::string errnoToString(int sysErrno)
			{
				std::string description;
				switch (sysErrno)
				{
				case EACCES:
					description = "EACCES - Permission denied";
					break;
				case EADDRINUSE:
					description = "EADDRINUSE - Address already in use";
					break;
				case EADDRNOTAVAIL:
					description = "EADDRNOTAVAIL - Address not available";
					break;
				case EAFNOSUPPORT:
					description = "EAFNOSUPPORT - Address family not supported";
					break;
				case EAGAIN:
					description = "EAGAIN - Resource temporarily unavailable / EWOULDBLOCK - Operation would block";
					break;
				case EALREADY:
					description = "EALREADY - Connection already in progress";
					break;
				case EBADF:
					description = "EBADF - Bad file descriptor";
					break;
				case ECONNABORTED:
					description = "ECONNABORTED - Connection aborted";
					break;
				case ECONNREFUSED:
					description = "ECONNREFUSED - Remote refused connection";
					break;
				case ECONNRESET:
					description = "ECONNRESET - Connection reset by peer";
					break;
				case EDESTADDRREQ:
					description = "EDESTADDRREQ - Destination address required";
					break;
				case EFAULT:
					description = "EFAULT - Bad address";
					break;
				case EHOSTUNREACH:
					description = "EHOSTUNREACH - No route to host";
					break;
				case EINPROGRESS:
					description = "EINPROGRESS - Operation in progress";
					break;
				case EINTR:
					description = "EINTR - Interrupted system call";
					break;
				case EINVAL:
					description = "EINVAL - Invalid argument";
					break;
				case EISCONN:
					description = "EISCONN - Socket is already connected";
					break;
				case EMFILE:
					description = "EMFILE - Too many open files";
					break;
				case EMSGSIZE:
					description = "EMSGSIZE - Message too long";
					break;
				case ENAMETOOLONG:
					description = "ENAMETOOLONG - Name too long";
					break;
				case ENETDOWN:
					description = "ENETDOWN - Network is down";
					break;
				case ENETRESET:
					description = "ENETRESET - Connection reset by network";
					break;
				case ENETUNREACH:
					description = "ENETUNREACH - Network unreachable";
					break;
				case ENOBUFS:
					description = "ENOBUFS - No buffer space available";
					break;
				case ENOMEM:
					description = "ENOMEM - Out of memory";
					break;
				case ENOTCONN:
					description = "ENOTCONN - Socket not connected";
					break;
				case ENOTSOCK:
					description = "ENOTSOCK - Not a socket";
					break;
				case EOPNOTSUPP:
					description = "EOPNOTSUPP - Operation not supported";
					break;
				case EPERM:
					description = "EPERM - Operation not permitted";
					break;
				case EPIPE:
					description = "EPIPE - Broken pipe";
					break;
				case EPROTO:
					description = "EPROTO - Protocol error";
					break;
				case EPROTONOSUPPORT:
					description = "EPROTONOSUPPORT - Protocol not supported";
					break;
				case ETIMEDOUT:
					description = "ETIMEDOUT - Connection timed out";
					break;
				default:
					description = "UNKNOWN_ERRNO - Unknown error code";
					break;
				}
				return description;
			}
		}
	}
}
