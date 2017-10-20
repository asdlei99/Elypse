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
#include "TrollEditorPch.h"

#include "GUI/AboutFrame.h"

#include "GUI/MainFrame.h"

#include "xpm/BMPKeumar.xpm"

#include <GradientButton.h>

namespace Troll
{
	namespace GUI
	{
		enum aboutFrameID
		{
			afBack,
			afKeumar,
			afShowKeumar
		};

		BEGIN_EVENT_TABLE( AboutFrame, wxFrame )
			EVT_BUTTON( afBack, AboutFrame::_onBack )
			EVT_BUTTON( afKeumar, AboutFrame::_onCloseKeumar )
			EVT_BUTTON( afShowKeumar, AboutFrame::_onShowKeumar )
		END_EVENT_TABLE()

		AboutFrame::AboutFrame( wxWindow * parent )
			: wxFrame( parent, wxID_ANY, _( "About Troll Editor" ), wxDefaultPosition, wxSize( 240, 155 ), wxFRAME_TOOL_WINDOW | wxCAPTION | wxCLOSE_BOX | wxFRAME_FLOAT_ON_PARENT )
			, m_easterEggFrame( NULL )
		{
			SetBackgroundColour( GuiCommon::PanelBackgroundColour );
			SetForegroundColour( GuiCommon::PanelForegroundColour );

			wxSize l_size = GetClientSize();
			wxPanel * l_panel = new wxPanel( this, wxID_ANY, wxPoint( 0, 0 ), l_size, wxBORDER_NONE );
			l_panel->SetBackgroundColour( GuiCommon::PanelBackgroundColour );
			l_panel->SetForegroundColour( GuiCommon::PanelForegroundColour );
			wxBitmap l_bitmap = wxBITMAP( BMPKeumar );

			if ( l_bitmap.IsOk() )
			{
				m_easterEggFrame = new wxFrame( this, wxID_ANY, wxT( "Keumar le barbare" ), wxPoint( 50, 50 ), wxSize( l_bitmap.GetWidth(), l_bitmap.GetHeight() ), 0 );
				wxBitmapButton * l_button = new wxBitmapButton( m_easterEggFrame, afKeumar, l_bitmap, wxPoint( 0, 0 ), m_easterEggFrame->GetClientSize(), wxBORDER_NONE );
				wxButton * l_keumar = new wxButton( l_panel, afShowKeumar, wxEmptyString, wxPoint( 10, 10 ), wxSize( 10, 10 ), wxBORDER_SIMPLE );
				m_easterEggFrame->Hide();
			}

			wxPoint l_position( 20, 20 );
			wxStaticText * l_line1 = new wxStaticText( l_panel, wxID_ANY, _( "Troll Editor, The Elypse Project Creator" ), l_position );
			l_line1->SetBackgroundColour( GuiCommon::PanelBackgroundColour );
			l_line1->SetForegroundColour( GuiCommon::PanelForegroundColour );
			wxStaticText * l_line2 = new wxStaticText( l_panel, wxID_ANY, wxT( "Version : 0.1.0.0" ), l_position += wxPoint( 0, 40 ) );
			l_line2->SetBackgroundColour( GuiCommon::PanelBackgroundColour );
			l_line2->SetForegroundColour( GuiCommon::PanelForegroundColour );
			m_back = new GuiCommon::GradientButton( l_panel, afBack, TE_OK, wxPoint( ( l_size.x / 2 ) - 35, l_size.y - 25 ), wxSize( 70, 20 ) );
			m_word.clear();
		}

		void AboutFrame::_onBack( wxCommandEvent & p_event )
		{
			Destroy();
		}

		void AboutFrame::_onCloseKeumar( wxCommandEvent & p_event )
		{
			if ( m_easterEggFrame )
			{
				m_easterEggFrame->Hide();
			}
		}

		void AboutFrame::_onShowKeumar( wxCommandEvent & p_event )
		{
			if ( m_easterEggFrame )
			{
				m_easterEggFrame->Show();
			}
		}
	}
}
