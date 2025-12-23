#ifndef __GICAME__TCPSOCKET_H__
#define __GICAME__TCPSOCKET_H__


#include <string>
#include <vector>
#include "../common.h"
#include "../utils/NotCopyable.h"
#include "../interfaces/IDataExchanger.h"
#include "./NetworkUtility.h"
#include "./NetworkDefinitions.h"


namespace Gicame {


	class GICAME_API TcpSocket : public IDataExchanger {

		NOT_COPYABLE(TcpSocket)

	private:
		SocketDescriptor sockfd;
		SocketStatus socketStatus;
		short sin_family; // IPv4 (AF_INET) or IPv6 (AF_INET6)
		bool flagReceptionBlocking;

	private:
		TcpSocket(
			const SocketDescriptor sockfd, const SocketStatus socketStatus,
			const short sin_family
		);

	public:
		TcpSocket(const InternetProtocolVersion ipv = InternetProtocolVersion::IPv4);
		TcpSocket(TcpSocket&& other) noexcept;
		~TcpSocket();

		/**
		 * Connect to the host specified by "ip". The host has to listen on port "port"
		 */
		bool connectTo(const std::string& ip, const uint16_t port);

		/**
		 * Connect to the host specified by "ip". The host has to listen on port "port"
		 */
		bool connectTo(const IPv4& ip, const uint16_t port);

		/**
		 * Connect to the host specified by "ip". The host has to listen on port "port"
		 */
		bool connectTo(const IPv6& ip, const uint16_t port);

		virtual size_t send(const void* buffer, const size_t size) override;
		virtual size_t receive(void* buffer, const size_t size) override;

		/**
		 * Close the socket
		 */
		void close();

		/**
		 * Return true if the socket is connected
		 */
		virtual bool isConnected() const override;
		virtual bool isSenderConnected() const override { return isConnected(); }
		virtual bool isReceiverConnected() const override { return isConnected(); }

		/**
		 * Current socket status
		 */
		SocketStatus getSocketStatus() const;

		/**
		 * Get the IPv4 of the remote peer
		 */
		IPv4 getPeerIPv4() const;

		/**
		 * Set the receptionBlocking flag: when receiving, try to receive the exact amount of bytes
		 * specified
		 */
		void setReceptionBlocking(const bool rb);

		/**
		 * Old-style socket options (getters)
		 */
		bool getSockOptions(const int level, const int optname, void* outValue, const uint32_t outValueSize) const;
		template <typename OutType>
		bool getSockOptions(const int level, const int optname, OutType* outValue) const;

		/**
		 * Old-style socket options (setters)
		 */
		bool setSockOptions(const int level, const int optname, const void* inValue, const uint32_t inValueSize);
		template <typename InType>
		bool setSockOptions(const int level, const int optname, const InType inValue);

	public:
		/**
		 * Utility function: convert a socket descriptor to a TcpSocket instance
		 */
		static TcpSocket fromSockfd(SocketDescriptor sockfd, const SocketStatus socketStatus, const short sin_family);

	};


	/*
	 * Inline implementation
	 */

 	inline SocketStatus TcpSocket::getSocketStatus() const {
 		return socketStatus;
 	}

	template <typename OutType>
	inline bool TcpSocket::getSockOptions(const int level, const int optname, OutType* outValue) const {
		return getSockOptions(level, optname, outValue, sizeof(*outValue));
	}

	template <typename InType>
	inline bool TcpSocket::setSockOptions(const int level, const int optname, const InType inValue) {
		return setSockOptions(level, optname, &inValue, sizeof(inValue));
	}

};


#endif
