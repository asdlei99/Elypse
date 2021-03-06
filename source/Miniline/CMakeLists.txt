option( BUILD_MINILINE_SERVER "Build Miniline server" TRUE )

function( MinilineInit )
	set( ElpsServr "no (Not wanted)" PARENT_SCOPE )
	set( ChatSvPlg "no (Not wanted)" PARENT_SCOPE )
	set( ElpsSvBin "no (Not wanted)" PARENT_SCOPE )
	set( ClientTst "no (Not wanted)" PARENT_SCOPE )
endfunction( MinilineInit )

function( MinilineBuild )
	if ( BUILD_MINILINE_SERVER )
		if (NOT MINGW)
			set( Boost_USE_STATIC_LIBS ON)
			set( BOOST_THREAD_USE_LIB ON)
		endif()

		if( NOT WIN32 )
			find_package( X11 )
		endif()

		if ( Boost_FOUND )

			if ( WIN32 )
				add_definitions( -D_WINSOCK_DEPRECATED_NO_WARNINGS )
			endif ()
			
			#X11 Libs
			set( x11Libraries "" )
			foreach( Lib ${X11_LIBRARIES} )
				if( x11Libraries )
					set( x11Libraries "${x11Libraries}|${Lib}" )
				else()
					set( x11Libraries "${Lib}" )
				endif()
			endforeach()
			
			set( Build ${ElpsServr} )
			add_subdirectory( Miniline/ElypseServer )
			set( ElpsServr ${Build} PARENT_SCOPE )
			
			set( Build ${ChatSvPlg} )
			add_subdirectory( Miniline/ChatServerPlugin )
			set( ChatSvPlg ${Build} PARENT_SCOPE )
			
			set( Build ${ElpsSvBin} )
			add_subdirectory( Miniline/ElypseServerBin )
			set( ElpsSvBin ${Build} PARENT_SCOPE )
			
			set( Build ${ClientTst} )
			add_subdirectory( Miniline/ClientTest )
			set( ClientTst ${Build} PARENT_SCOPE )
		else()
			set( ElpsServr "No (missing: Boost library)" )
			set( ChatSvPlg "No (missing: Boost library)" )
			set( ElpsSvBin "No (missing: Boost library)" )
			set( ClientTst "No (missing: Boost library)" )
		endif ()
	endif ()
endfunction( MinilineBuild )

function( MinilineSummary msg )
	set( msg_tmp "\nMiniline :" )
	set( msg_tmp "${msg_tmp}\n    ElypseServer       ${ElpsServr}" )
	set( msg_tmp "${msg_tmp}\n    ChatServerPlugin   ${ChatSvPlg}" )
	set( msg_tmp "${msg_tmp}\n    ElypseServerBin    ${ElpsSvBin}" )
	set( msg_tmp "${msg_tmp}\n    ClientTest         ${ClientTst}" )
	SET( msg "${msg}${msg_tmp}" PARENT_SCOPE )
endfunction( MinilineSummary )

function( MinilineSetup )
	cpack_add_component_group( Miniline
		EXPANDED
		DISPLAY_NAME "Miniline tools"
		DESCRIPTION "Server tools to use Miniline"
	)

	cpack_add_component( ElypseServer
		DISPLAY_NAME "ElypseServer library"
		DESCRIPTION "Server common library to plugins and server application"
		REQUIRED
		GROUP Miniline
		DEPENDS ElypseCore
		INSTALL_TYPES Minimal Developer Full
	)
	cpack_add_component( ElypseServer_dev
		DISPLAY_NAME "ElypseServer library development files"
		DESCRIPTION "Used to develop a plugin based on ElypseServer library"
		REQUIRED
		GROUP Miniline
		DEPENDS ElypseCore ElypseServer
		INSTALL_TYPES Developer Full
	)
	cpack_add_component( ChatServerPlugin
		DISPLAY_NAME "Chat server plugin"
		DESCRIPTION "Plugin used to extend server base functionalities to Miniline"
		GROUP Miniline
		DEPENDS ElypseCore ElypseServer
		INSTALL_TYPES Developer Full
	)
	cpack_add_component( ElypseServerBin
		DISPLAY_NAME "Elypse server Application"
		DESCRIPTION "Main server application"
		GROUP Miniline
		DEPENDS ElypseCore ElypseServer
		INSTALL_TYPES Developer Full
	)
	cpack_add_component( ClientTest
		DISPLAY_NAME "Client test Application"
		DESCRIPTION "A client test application"
		GROUP Miniline
		DEPENDS ElypseCore ElypseServer
		INSTALL_TYPES Developer
	)
endfunction( MinilineSetup )

