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

#include "SequencePanel.h"

#include "PonctualEventPanel.h"
#include "ContinuousEventPanel.h"
#include "LinePanel.h"
#include "Project/Temporal/Sequence.h"
#include "Project/Temporal/PonctualEvent.h"
#include "Project/Temporal/ContinuousEvent.h"
#include "GUI/MainFrame.h"

BEGIN_TROLL_GUI_TIME_NAMESPACE
{
	enum SequencePanelIDs
	{
		addPonctualEvent,
		addContinuousEvent
	};

	BEGIN_EVENT_TABLE( SequencePanel, wxPanel )
		EVT_LEFT_UP( SequencePanel::OnLeftMouseUp )
		EVT_RIGHT_UP( SequencePanel::OnRightMouseUp )
		EVT_MENU( addPonctualEvent, SequencePanel::OnAddPonctualEvent )
		EVT_MENU( addContinuousEvent, SequencePanel::OnAddContinuousEvent )
	END_EVENT_TABLE()

	SequencePanel::SequencePanel( wxWindow * p_parent, wxWindowID p_id, const wxPoint & p_position, const wxSize & p_size )
		: wxPanel( p_parent, p_id, p_position, p_size, wxBORDER_NONE )
		, m_currentLeft( 20 )
		, m_maxTop( 30 )
	{
		Hide();
		m_sequencePanel = new LinePanel( this, wxID_ANY, wxPoint( 0, 0 ), wxSize( 10, 20 ) );
		m_sequencePanel->Hide();

		for ( unsigned int i = 0 ; i < 40 ; i++ )
		{
			ContinuousEventPanel * l_cpanel = new ContinuousEventPanel( this, wxID_ANY, wxPoint( 0, 0 ), wxSize( 10, 20 ) );
			l_cpanel->Hide();
			m_continuousEvents.push_back( l_cpanel );
			PonctualEventPanel * l_ppanel = new PonctualEventPanel( this, wxID_ANY, wxPoint( 0, 0 ), wxSize( 10, 20 ) );
			l_ppanel->Hide();
			m_ponctualEvents.push_back( l_ppanel );
		}

		SetBackgroundColour( wxColour( 255, 255, 255 ) );
	}

	SequencePanel::~SequencePanel()
	{
	}

	TROLL_PROJECT_TEMPORAL_NAMESPACE::TrollSequence * SequencePanel::SetSequence( TROLL_PROJECT_TEMPORAL_NAMESPACE::TrollSequence * p_sequence )
	{
		if ( p_sequence != NULL )
		{
			m_sequence = p_sequence;
			wxSize l_size;
			l_size.x = int( p_sequence->GetTotalLength() * 100 );
			m_sequencePanel->SetSequence( p_sequence );
			m_sequencePanel->SetSize( l_size.x, 20 );
			m_sequencePanel->Show();

			for ( auto && l_ponctualIt : p_sequence->GetPonctualEvents() )
			{
				_addPonctualEvent( l_ponctualIt.second, l_ponctualIt.first );
			}

			for ( auto && l_continuousIt : p_sequence->GetContinuousEvents() )
			{
				_addContinuousEvent( l_continuousIt.second );
			}

			l_size.x += 20;
			l_size.y = m_maxTop + 20;
			SetSize( l_size );
			Show();
		}

		return p_sequence;
	}

	void SequencePanel::AddPonctualEvent( TROLL_PROJECT_TEMPORAL_NAMESPACE::TrollPonctualEvent * p_event, Real p_time )
	{
		if ( m_ponctualEvents.size() == 0 )
		{
			return;
		}

		PonctualEventPanel * l_eventPanel = m_ponctualEvents[0];
		m_ponctualEvents.erase( m_ponctualEvents.begin() );
		l_eventPanel->SetPonctualEvent( p_event, p_time );
		int l_ancientMax = m_maxTop;
		int l_top = _getTopForPonctualEvent( p_event );

		if ( m_maxTop > l_ancientMax )
		{
			SetSize( GetSize().x, m_maxTop + 20 );
		}

		l_eventPanel->SetTop( l_top );
	}

	void SequencePanel::AddContinuousEvent( TROLL_PROJECT_TEMPORAL_NAMESPACE::TrollContinuousEvent * p_event )
	{
		if ( m_continuousEvents.size() == 0 )
		{
			return;
		}

		ContinuousEventPanel * l_eventPanel = m_continuousEvents[0];
		m_continuousEvents.erase( m_continuousEvents.begin() );
		l_eventPanel->SetContinuousEvent( p_event );
		int l_ancientMax = m_maxTop;
		int l_top = _getTopForContinuousEvent( p_event );

		if ( m_maxTop > l_ancientMax )
		{
			SetSize( GetSize().x, m_maxTop + 20 );
		}

		l_eventPanel->SetTop( l_top );
	}

	void SequencePanel::_addPonctualEvent( TROLL_PROJECT_TEMPORAL_NAMESPACE::TrollPonctualEvent * p_event, Real p_time )
	{
		if ( m_ponctualEvents.size() == 0 )
		{
			return;
		}

		PonctualEventPanel * l_eventPanel = m_ponctualEvents[0];
		m_ponctualEvents.erase( m_ponctualEvents.begin() );
		l_eventPanel->SetPonctualEvent( p_event, p_time );
		l_eventPanel->SetTop( _getTopForPonctualEvent( p_event ) );
	}

	void SequencePanel::_addContinuousEvent( TROLL_PROJECT_TEMPORAL_NAMESPACE::TrollContinuousEvent * p_event )
	{
		if ( m_continuousEvents.size() == 0 )
		{
			return;
		}

		ContinuousEventPanel * l_eventPanel = m_continuousEvents[0];
		m_continuousEvents.erase( m_continuousEvents.begin() );
		l_eventPanel->SetContinuousEvent( p_event );
		l_eventPanel->SetTop( _getTopForContinuousEvent( p_event ) );
	}

	void SequencePanel::RemovePonctualEvent( TROLL_PROJECT_TEMPORAL_NAMESPACE::TrollPonctualEvent * p_event )
	{
		PonctualEventPanel * l_eventPanel = p_event->GetEventPanel();
		l_eventPanel->SetPonctualEvent( NULL, 0.0 );
		m_ponctualEvents.push_back( l_eventPanel );
	}

	void SequencePanel::RemoveContinuousEvent( TROLL_PROJECT_TEMPORAL_NAMESPACE::TrollContinuousEvent * p_event )
	{
		ContinuousEventPanel * l_eventPanel = p_event->GetEventPanel();
		l_eventPanel->SetContinuousEvent( NULL );
		m_continuousEvents.push_back( l_eventPanel );
	}

	int SequencePanel::_getTopForContinuousEvent( TROLL_PROJECT_TEMPORAL_NAMESPACE::TrollContinuousEvent * p_event )
	{
		return _getTop( int( p_event->GetStartTime() * 100 ), int( p_event->GetLength() * 100 ) );
	}

	int SequencePanel::_getTopForPonctualEvent( TROLL_PROJECT_TEMPORAL_NAMESPACE::TrollPonctualEvent * p_event )
	{
		return _getTop( int( p_event->GetFireTime() * 100 ), 20 );
	}

	int SequencePanel::_getTop( int p_left, int p_width )
	{
		if ( m_addedPanelsDimensions.size() == 0 )
		{
			IntIMultiMap l_map;
			l_map.insert( std::make_pair( p_left, p_width ) );
			m_addedPanelsDimensions.insert( std::make_pair( 30, l_map ) );
			return 30;
		}

		DimensionMap::iterator l_it = m_addedPanelsDimensions.begin();
		IntIMultiMap::iterator l_it2;
		bool l_found = false;
		int l_left, l_dim;

		while ( l_it != m_addedPanelsDimensions.end() )
		{
			l_it2 = l_it->second.begin();

			while ( l_it2 != l_it->second.end() && ! l_found )
			{
				l_left = l_it2->first;
				l_dim = l_left + l_it2->second;

				if ( ( l_left <= p_left && l_dim > p_left )
						|| ( l_left < p_left + p_width && l_dim >= p_left + p_width )
						|| ( l_left >= p_left && l_dim <= p_left + p_width ) )
				{
					l_found = true;
				}

				++l_it2;
			}

			if ( l_found )
			{
				l_found = false;
				++l_it;
			}
			else
			{
				l_it->second.insert( std::make_pair( p_left, p_width ) );
				return l_it->first;
			}
		}

		if ( l_it == m_addedPanelsDimensions.end() )
		{
			IntIMultiMap l_map;
			l_map.insert( std::make_pair( p_left, p_width ) );
			m_maxTop += 30;
			m_addedPanelsDimensions.insert( std::make_pair( m_maxTop, l_map ) );
			return m_maxTop;
		}
		else
		{
			return l_it->first;
		}
	}

	void SequencePanel::_showContextMenu( const wxPoint & p_pos )
	{
		wxMenu l_menu;
		l_menu.Append( addPonctualEvent, TE_ADD_PONCTUAL_EVENT );
		l_menu.Append( addContinuousEvent, TE_ADD_CONTINUOUS_EVENT );
		m_currentLeft = p_pos.x;
		int l_modulo = m_currentLeft % 10;

		if ( l_modulo >= 5 )
		{
			m_currentLeft += 10 - l_modulo;
		}
		else
		{
			m_currentLeft -= l_modulo;
		}

		PopupMenu( & l_menu, p_pos.x, p_pos.y );
	}

	void SequencePanel::OnAddPonctualEvent( wxCommandEvent & p_event )
	{
		GUI::MainFrame::GetInstance()->AddPonctualEvent( m_sequencePanel->GetSequence(), m_currentLeft / 100.0f );
	}

	void SequencePanel::OnAddContinuousEvent( wxCommandEvent & p_event )
	{
		std::cout << "SequencePanel::OnAddContinuousEvent - " << m_currentLeft << " - " << m_currentLeft / 100.0 << std::endl;
		GUI::MainFrame::GetInstance()->AddContinuousEvent( m_sequencePanel->GetSequence(), m_currentLeft / 100.0f );
	}

	void SequencePanel::OnLeftMouseUp( wxMouseEvent & p_event )
	{
	}

	void SequencePanel::OnRightMouseUp( wxMouseEvent & p_event )
	{
		_showContextMenu( p_event.GetPosition() );
	}
}
END_TROLL_GUI_TIME_NAMESPACE