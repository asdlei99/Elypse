#ifndef __GENLIB_TCP_SERVER_H___
#define __GENLIB_TCP_SERVER_H___

#include "TcpAcceptor.h"

#include <General/TcpWriter.h>
#include <General/TcpReader.h>
#include <General/StlMacros.h>
#include <General/Macros.h>

namespace General
{ namespace MultiThreading
{
	class ChatClient : public TcpBaseClient
	{
	public:
		TcpWriter m_writer;
		TcpQueuedReader m_reader;

	public:
		ChatClient( asio::io_service & p_service)
			:	TcpBaseClient( p_service),
				m_writer( m_socket, p_service),
				m_reader( m_socket, p_service)
		{
		}

	public:
		virtual void Start() { m_reader.StartAsyncRead(); }
	};

	class TcpServer : public TcpAcceptor
	{
	public:
		TcpServer( unsigned short p_port)
			:	TcpAcceptor( p_port)
		{
		}

		~TcpServer()
		{
		}

	public:
		virtual TcpBaseClient * _createNewClient()
		{
			return new ChatClient( m_service);
		}
	};
}
}

#endif