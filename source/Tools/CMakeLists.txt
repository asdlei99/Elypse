option( BUILD_TOOLS_ELYPSE_BINARY "Build ElypseBinary" TRUE )
option( BUILD_TOOLS_MUSE_READER "Build MuseReader" TRUE )
option( BUILD_TOOLS_MUSE_WRITER "Build MuseWriter" TRUE )
option( BUILD_TOOLS_TROLL_EDITOR "Build TrollEditor" TRUE )

function( ToolsInit )
	set( GuiCommon "no (Not wanted)" PARENT_SCOPE )
	set( ElpsBinry "no (Not wanted)" PARENT_SCOPE )
	set( MuseReadr "no (Not wanted)" PARENT_SCOPE )
	set( MuseWritr "no (Not wanted)" PARENT_SCOPE )
	set( TrollEdtr "no (Not wanted)" PARENT_SCOPE )
endfunction( ToolsInit )

function( ToolsBuild )
	set( wxWidgets_USE_STATIC OFF)

	if (WIN32)
		find_package( wxWidgets COMPONENTS core base stc aui adv propgrid net scintilla )
	else ()
		find_package( wxWidgets COMPONENTS core base stc aui adv propgrid net )
	endif()

	if ( wxWidgets_FOUND AND GTK2_FOUND )
		message( STATUS "+ Found wxWidgets" )
		include_directories( ${wxWidgets_INCLUDE_DIRS} )

		set( wxWidgetsLibraries "" )
		FOREACH( Lib ${wxWidgets_LIBRARIES} )
			if ( wxWidgetsLibraries )
				set( wxWidgetsLibraries "${wxWidgetsLibraries}|${Lib}" )
			else()
				set( wxWidgetsLibraries "${Lib}" )
			endif()
		ENDFOREACH()

		#Few configuration for wxWidgets
		string( COMPARE EQUAL "${wxWidgets_USE_DEBUG}" "ON" IsWxDebug )
		add_definitions( -D_FILE_OFFSET_BITS=64 -D_LARGE_FILES )
		if ( NOT WIN32 )
			add_definitions( -D__WXGTK__ )
		endif ()
		if ( IsWxDebug )
			add_definitions( -D__WXDEBUG__ )
		endif ()
		
		set( BinariesLibraries
			${wxWidgetsLibraries}
		)
		
		if ( BUILD_TOOLS_ELYPSE_BINARY OR BUILD_TOOLS_MUSE_READER OR BUILD_TOOLS_MUSE_WRITER OR BUILD_TOOLS_TROLL_EDITOR )
			set( Build ${GuiCommon} )
			add_subdirectory( Tools/GuiCommon )
			set( GuiCommon ${Build} PARENT_SCOPE )
		endif ()
		
		if ( BUILD_TOOLS_ELYPSE_BINARY )
			set( Build ${ElpsBinry} )
			add_subdirectory( Tools/ElypseBinary )
			set( ElpsBinry ${Build} PARENT_SCOPE )
		endif ()
		
		if ( BUILD_TOOLS_MUSE_READER )
			set( Build ${MuseReadr} )
			add_subdirectory( Tools/MuseReader )
			set( MuseReadr ${Build} PARENT_SCOPE )
		endif ()
		
		if ( BUILD_TOOLS_MUSE_WRITER )
			set( Build ${MuseWritr} )
			add_subdirectory( Tools/MuseWriter )
			set( MuseWritr ${Build} PARENT_SCOPE )
		endif ()
		
		if ( BUILD_TOOLS_TROLL_EDITOR )
			set( Build ${TrollEdtr} )
			add_subdirectory( Tools/TrollEditor )
			set( TrollEdtr ${Build} PARENT_SCOPE )
		endif ()
	else()
		set( _TXT "No (missing: " )
		set( SEP "" )
		if ( NOT wxWidgets_FOUND )
			set( _TXT "${SEP}${_TXT}wxWidgets library " )
			set( SEP ", " )
		endif()
		if ( NOT GTK_FOUND )
			set( _TXT "${SEP}${_TXT}GTK library" )
			set( SEP ", " )
		endif()
		if ( NOT OPENGL_FOUND )
			set( _TXT "${SEP}${_TXT}OpenGL library" )
			set( SEP ", " )
		endif()
		set( GuiCommon "${_TXT})" )
		set( ElpsBinry "${_TXT})" )
		set( MuseReadr "${_TXT})" )
		set( TrollEdtr "${_TXT})" )
	endif ()
endfunction( ToolsBuild )

function( ToolsSummary msg )
	set( msg_tmp "\nTools :" )
	set( msg_tmp "${msg_tmp}\n    GuiCommon          ${GuiCommon}" )
	set( msg_tmp "${msg_tmp}\n    ElypseBinary       ${ElpsBinry}" )
	set( msg_tmp "${msg_tmp}\n    MuseReader         ${MuseReadr}" )
	set( msg_tmp "${msg_tmp}\n    MuseWriter         ${MuseWritr}" )
	set( msg_tmp "${msg_tmp}\n    TrollEditor        ${TrollEdtr}" )
	set( msg "${msg}${msg_tmp}" PARENT_SCOPE )
endfunction( ToolsSummary )

function( ToolsSetup )
	cpack_add_component_group( Tools
		EXPANDED
		DISPLAY_NAME "Binary tools"
		DESCRIPTION "Tools used to develop new scenes using ${MAIN_PROJECT_NAME}"
	)

	cpack_add_component( ElypseBinary
		DISPLAY_NAME "ElypseBinary Application"
		DESCRIPTION "Simple executable that can run scenes in a single muse file"
		REQUIRED
		GROUP Tools
		DEPENDS ElypseCore
		INSTALL_TYPES Minimal Developer Full
	)
	cpack_add_component( MuseReader
		DISPLAY_NAME "MuseReader Application"
		DESCRIPTION "Application used to read the content of a muse file"
		GROUP Tools
		DEPENDS ElypseCore
		INSTALL_TYPES Developer Full
	)
	cpack_add_component( MuseWriter
		DISPLAY_NAME "MuseWriter Application"
		DESCRIPTION "Application used to builf muse files from scene files, script files and folders"
		GROUP Tools
		DEPENDS ElypseCore
		INSTALL_TYPES Developer Full
	)
	cpack_add_component( TrollEditor
		DISPLAY_NAME "TrollEditor Application"
		DESCRIPTION "Application used to create new scenes"
		GROUP Tools
		DEPENDS ElypseCore
		INSTALL_TYPES Developer Full
	)
	cpack_add_component( GuiCommon
		DISPLAY_NAME "Common Elypse/wxWigets library"
		DESCRIPTION "Used to develop wxWidgets applications using Elypse"
		GROUP Tools
		DEPENDS ElypseCore
		INSTALL_TYPES Developer Full
	)
	cpack_add_component( GuiCommon_dev
		DISPLAY_NAME "Common Elypse/wxWigets library SDK"
		DESCRIPTION "Used to develop wxWidgets applications using Elypse"
		GROUP Development
		DEPENDS GuiCommon
		INSTALL_TYPES Developer Full
	)
endfunction( ToolsSetup )
