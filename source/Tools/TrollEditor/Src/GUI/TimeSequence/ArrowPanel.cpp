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

#include "ArrowPanel.h"

#include "GUI/MainFrame.h"
#include "GUI/TimeSequence/TimePanel.h"

BEGIN_TROLL_GUI_TIME_NAMESPACE
{
	BEGIN_EVENT_TABLE( ArrowPanel, wxPanel )
		EVT_PAINT( ArrowPanel::OnPaint )
		EVT_LEFT_DOWN( ArrowPanel::OnMouseLeftDown )
		EVT_LEFT_UP( ArrowPanel::OnMouseLeftUp )
		EVT_MOTION( ArrowPanel::OnMouseMove )
		EVT_LEAVE_WINDOW( ArrowPanel::OnMouseLeave )
		EVT_KILL_FOCUS( ArrowPanel::OnKillFocus )
	END_EVENT_TABLE()

	ArrowPanel::ArrowPanel( wxWindow * p_parent, wxWindowID p_id, const wxPoint & p_position, const wxSize & p_size )
		: wxPanel( p_parent, p_id, p_position, p_size, wxBORDER_NONE )
		, m_mouseLeftDown( false )
		, m_boundPanel( NULL )
	{
		SetBackgroundColour( wxColour( 0, 255, 0 ) );
	}

	ArrowPanel::~ArrowPanel()
	{
	}

	void ArrowPanel::OnPaint( wxPaintEvent & p_event )
	{
		wxPaintDC l_dc( this );
		wxBrush l_brush( wxColour( 255, 255, 255 ), wxSOLID );
		l_dc.SetBrush( l_brush );
		l_dc.SetBackground( l_brush );
		l_dc.SetBackgroundMode( wxTRANSPARENT );
		wxPen l_pen = *wxWHITE_PEN;
		l_dc.SetPen( l_pen );
		wxSize l_size = GetClientSize();
		l_dc.DrawRectangle( 0, 0, l_size.x, l_size.y );
		l_pen = *wxBLACK_PEN;
		l_dc.SetPen( l_pen );
		l_dc.DrawLine( l_size.x / 2, 0, l_size.x / 2, l_size.y );
		l_dc.DrawLine( l_size.x / 2 - 5, l_size.y - 5, l_size.x / 2, l_size.y );
		l_dc.DrawLine( l_size.x / 2 + 5, l_size.y - 5, l_size.x / 2, l_size.y );
	}

	void ArrowPanel::OnMouseLeftDown( wxMouseEvent & p_event )
	{
		m_mouseLeftDown = true;
		//m_boundPanel->SetMouseLeftDown( true);
	}

	void ArrowPanel::OnMouseLeftUp( wxMouseEvent & p_event )
	{
		m_mouseLeftDown = false;
		//m_boundPanel->SetMouseLeftDown( false);
	}

	void ArrowPanel::OnMouseMove( wxMouseEvent & p_event )
	{
		//if (m_mouseLeftDown && m_boundPanel != NULL && GetPosition().x != -6)
		//{
		//	int l_left;
		//	l_left = p_event.GetX() + GetPosition().x;

		//	SetPosition( wxPoint( l_left - 6, GetPosition().y));
		//	m_boundPanel->SetPosition(wxPoint( l_left, m_boundPanel->GetPosition().y));
		//}
	}

	void ArrowPanel::OnMouseLeave( wxMouseEvent & p_event )
	{
		m_mouseLeftDown = false;
		//m_boundPanel->SetMouseLeftDown( false);
	}

	void ArrowPanel::OnKillFocus( wxFocusEvent & p_event )
	{
		m_mouseLeftDown = false;
		//m_boundPanel->SetMouseLeftDown( false);
	}
}
END_TROLL_GUI_TIME_NAMESPACE
