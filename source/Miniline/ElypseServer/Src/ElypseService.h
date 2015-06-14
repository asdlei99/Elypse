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
#ifndef ___ELYPSESERVICE_H___
#define ___ELYPSESERVICE_H___

#include <TcpAcceptor.h>
#include <TcpWriter.h>
#include <TcpReader.h>

#include <Named.h>
#include <memory>

namespace Elypse
{
	namespace Server
	{
		enum TypeService
		{
			UdpService = 0,
			TcpService = 1
		};

		class ElypseService
			: public General::Theory::named
		{
		protected:
			ElypseService( const std::string & p_name )
				: General::Theory::named( p_name )
			{
			}

		public:
			virtual ~ElypseService()
			{
			}

		public:
			virtual unsigned short GetPortNo() const = 0;
			virtual TypeService GetTypeService() const = 0;
		};

		class ElypseTcpService
			: public ElypseService
			, public General::Network::TcpAcceptor
		{
		protected:
			ElypseTcpService( const std::string & p_name, unsigned short p_port )
				: ElypseService( p_name )
				, General::Network::TcpAcceptor( p_port )
			{
				std::clog << "ElypseTCPService(" << p_name << ") created" << std::endl;
			}

		public:
			virtual ~ElypseTcpService()
			{
				std::clog << "~ElypseTCPService(" << m_name << ") deleted" << std::endl;
			}

		public:
			inline TypeService GetTypeService() const
			{
				return TcpService;
			}
			inline unsigned short GetPortNo() const
			{
				return m_acceptor.local_endpoint().port();
			}
		};

		class ElypseUdpService
			: public ElypseService
		{
		private:
			unsigned short m_port;

		protected:
			ElypseUdpService( const std::string & p_name, unsigned short p_port )
				: ElypseService( p_name )
				, m_port( p_port )
			{
				std::clog << "ElypseUDPService(" << p_name << ") created" << std::endl;
			}

		public:
			virtual ~ElypseUdpService()
			{
				std::clog << "~ElypseUDPService(" << m_name << ") deleted" << std::endl;
			}

		public:
			inline TypeService GetTypeService() const
			{
				return UdpService;
			}
			inline unsigned short GetPortNo() const
			{
				return m_port;
			}
		};

		typedef std::vector< std::shared_ptr< ElypseService > > ElypseServiceArray;
	}
}

#endif
