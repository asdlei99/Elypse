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
#ifndef ___ChatPlugin___
#define ___ChatPlugin___

#include "Common.h"

#include <ElypsePlugin.h>
#include <ElypseClient.h>
#include <ElypseService.h>

namespace Chat
{
	//**************************************************************************

	class ChatTcpService
		: public Elypse::Server::ElypseTcpService
		, public std::enable_shared_from_this< ChatTcpService >
	{
	private:
		ChatPlugin * m_plugin;

	public:
		ChatTcpService( ChatPlugin * p_plugin );
		~ChatTcpService();

	private:
		virtual General::Network::TcpBaseClient * DoCreateClient();
		virtual void DoDestroyClient( General::Network::TcpBaseClient * p_toDelete );
		void DoDeleteClient( General::Network::TcpBaseClient * p_toDelete );
	};

	//**************************************************************************

	class ChatUdpService
		: public Elypse::Server::ElypseUdpService
		, public std::enable_shared_from_this< ChatUdpService >
	{
	public:
		ChatUdpService();
		~ChatUdpService();
	};

	//**************************************************************************

	class ChatPlugin
		: public Elypse::Server::ElypsePlugin
	{
	private:
		Elypse::Server::ElypseServiceArray m_services;
		std::shared_ptr< ChatWorld > m_world;
		ClientIdChatCLientMap m_connectedClients;
		ClientIdStrMap m_clientIDs;
		std::shared_ptr< ChatDatabase > m_database;
		ChatGameStrMap m_games;
		StrUIntIdMap m_gamesNames;

	public:
		ChatPlugin( String const & p_path );
		~ChatPlugin();

		virtual unsigned short GetVersionNo();
		Elypse::Server::ElypseServiceArray GetServices();

		void AddRoom( std::shared_ptr< ChatRoom > p_room );
		std::shared_ptr< ChatRoom > GetRoom( const String & p_roomName );

		ChatTcpClient * GetClient( const String & p_name );
		const String & GetClientName( unsigned int p_id );
		void AddClient( ChatTcpClient * p_client );
		void RemoveClient( ChatTcpClient * p_client );

		std::shared_ptr< ChatGame > AddGame( const String & p_gameName, ChatTcpClient * p_initiator );
		std::shared_ptr< ChatGame > GetGame( const String & p_gameName, int p_initiatorID );
		StrUIntIdMap GetGamesList( const String & p_gameName );
		unsigned int GetGameMaxPLayers( const String & p_gameName );
		void RemoveGame( const String & p_gameName, int p_initiatorID );
		void _loadRooms();
		void _loadGames();

	public:
		inline std::shared_ptr< ChatWorld > GetWorld()
		{
			return m_world;
		}
		inline std::shared_ptr< ChatDatabase > GetDatabase()
		{
			return m_database;
		}
	};

	//**************************************************************************
}

#endif
