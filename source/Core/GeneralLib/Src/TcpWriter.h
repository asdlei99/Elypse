/*
This source file is part of ElypsePlayer (https://sourceforge.net/projects/elypse/)

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
the program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
*/
#ifndef ___GENERAL_TCP_WRITER_H___
#define ___GENERAL_TCP_WRITER_H___

#include <string>
#include <deque>

#include "Macros.h"
#include "Exception.h"

#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/bind.hpp>

namespace General
{
	namespace Network
	{
		class TcpWriterBase
		{
		protected:
			boost::asio::ip::tcp::socket & m_socket;
			boost::asio::io_service & m_service;
			std::deque <std::string> m_messages;
			bool m_noSend;
			enum
			{
				c_headerLength = sizeof( short ),
				c_maxMessageLength = 8190
			};
			boost::array < char, c_maxMessageLength + c_headerLength > m_buffer;

		public:
			TcpWriterBase( boost::asio::ip::tcp::socket & p_socket, boost::asio::io_service & p_service )
				: m_socket( p_socket ),
					m_service( p_service ),
					m_noSend( false )
			{
			}
			virtual ~TcpWriterBase()
			{
			}
		protected:
			virtual bool CallbackWriterError( const boost::system::error_code & p_err ) = 0;

			void DoEndWrite( const boost::system::error_code & p_err )
			{
				if ( p_err )
				{
					return;
				}

				//if (p_err && CallbackWriterError( p_err))
				//{
				//	return;
				//}

				m_messages.pop_front();

				if ( ! m_messages.empty() )
				{
					DoAsyncWrite();
				}
			}

			void DoAsyncWrite()
			{
				unsigned short l_length = static_cast <unsigned short>( m_messages.front().size() );
				m_buffer.data()[0] = static_cast< unsigned char >( l_length & 0xFF );
				m_buffer.data()[1] = static_cast< unsigned char >( l_length >> 8 );
				//memcpy( m_buffer.data(), &l_length, c_headerLength);
				memcpy( m_buffer.data() + c_headerLength, m_messages.front().c_str(), m_messages.front().size() );
				boost::asio::async_write( m_socket,
											boost::asio::buffer( m_buffer.data(), l_length + c_headerLength ),
											boost::bind( & TcpWriterBase::DoEndWrite,
													this,
													boost::asio::placeholders::error ) );
			}

			void DoAddMessage( const std::string & p_message )
			{
				bool l_noWrite = m_messages.empty();
				m_messages.push_back( p_message );

				if ( l_noWrite )
				{
					DoAsyncWrite();
				}
			}

			void DoAddMessageNoSend( const std::string & p_message )
			{
				m_messages.push_back( p_message );
			}

		public:
			boost::system::error_code BlockingSend( const std::string & p_message )
			{
				boost::system::error_code l_error;
				boost::asio::write( m_socket, boost::asio::buffer( p_message ), boost::asio::transfer_all(), l_error );
				return l_error;
			}

			void AsyncSend( const std::string & p_message )
			{
				if ( m_noSend )
				{
					return;
				}

				m_service.post( boost::bind( & TcpWriterBase::DoAddMessage, this, p_message ) );
			}

			void StoreMessage( const std::string & p_message )
			{
				if ( m_noSend )
				{
					return;
				}

				m_service.post( boost::bind( & TcpWriterBase::DoAddMessageNoSend, this, p_message ) );
			}

			void StartSending()
			{
				if ( !m_messages.empty() )
				{
					DoAsyncWrite();
				}
			}
		};


		// Exemple de TcpWriter très simplifié
		class TcpWriter : public TcpWriterBase
		{
		public:
			TcpWriter( boost::asio::ip::tcp::socket & p_socket, boost::asio::io_service & p_service )
				: TcpWriterBase( p_socket, p_service )
			{
			}
			virtual ~TcpWriter()
			{
			}

		protected:
			virtual bool CallbackWriterError( const boost::system::error_code & p_err )
			{
				//std::cout << "TcpWriter -> error : " + p_err.message() << std::endl;
				GENLIB_EXCEPTION( "TcpWriter -> error : " + p_err.message() );
				return false;
			}
		};
	}
}

#endif
