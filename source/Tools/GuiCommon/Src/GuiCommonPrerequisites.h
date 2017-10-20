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
#ifndef ___GUICOMMON_PREREQUISITES_H___
#define ___GUICOMMON_PREREQUISITES_H___

#include <memory>

#pragma warning( push )
#pragma warning( disable:4996 )
#include <wx/wx.h>
#pragma warning( pop )

#define dSINGLE
#include <Elypse.h>

namespace GuiCommon
{
	class wxElypsePlugin;

	typedef std::unique_ptr< wxElypsePlugin > wxElypsePluginUPtr;
	typedef std::shared_ptr< wxElypsePlugin > wxElypsePluginSPtr;
	typedef std::weak_ptr< wxElypsePlugin > wxElypsePluginWPtr;

	inline wxString const & make_wxString( wxString const & p_value )
	{
		return p_value;
	}
	wxString make_wxString( String const & p_value );
	wxString make_wxString( char const * p_value );
	String make_string( wxString const & p_in );

	bool GetReal( wxString const & p_value, float & p_res );
	bool GetReal( wxString const & p_value, float p_min, float p_max, float & p_res );
	bool GetInt( wxString const & p_value, int & p_res );
	bool GetInt( wxString const & p_value, int p_min, int p_max, int & p_res );
	bool GetReal( String const & p_value, float & p_res );
	bool GetReal( String const & p_value, float p_min, float p_max, float & p_res );
	bool GetInt( String const & p_value, int & p_res );
	bool GetInt( String const & p_value, int p_min, int p_max, int & p_res );

	static const wxColour PanelBackgroundColour = wxColour( 30, 30, 30 );
	static const wxColour PanelForegroundColour = wxColour( 220, 220, 220 );
	static const wxColour BorderColour = wxColour( 90, 90, 90 );
	static const wxColour InactiveTabColour = wxColour( 60, 60, 60 );
	static const wxColour InactiveTextColour = wxColour( 200, 200, 200 );
	static const wxColour ActiveTabColour = wxColour( 51, 153, 255, 255 );
	static const wxColour ActiveTextColour = wxColour( 255, 255, 255, 255 );
}

#endif
