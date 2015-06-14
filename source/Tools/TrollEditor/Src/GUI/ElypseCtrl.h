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
#ifndef ___ElypseCtrl___
#define ___ElypseCtrl___

#include <wx/control.h>
#include <wx/cursor.h>
#include <wx/timer.h>

#include "GUI/TrollEditorGuiPrerequisites.h"

#include "Elypse/TrollInstance.h"
#include "Elypse/TrollSceneFileParser.h"

#include "Project/Module_Project.h"

BEGIN_TROLL_GUI_NAMESPACE
{
	class ElypseCtrl
		: public wxControl
	{
		friend class ElypseFrame;
		friend class MainFrame;

	public:
		ElypseCtrl( wxWindow * p_parent, TROLL_PROJECT_NAMESPACE::ProjectSPtr p_project, MainFrame * p_mainFrame, bool p_edit = false );
		~ElypseCtrl();

		const String GetWHandle();

		void Init();
		void SetCtrlFocus();
		void KillCtrlFocus();

		inline float GetMuseWidth()const
		{
			return m_width;
		}
		inline float GetMuseHeight()const
		{
			return m_height;
		}
		inline TROLL_ELYPSE_NAMESPACE::TrollInstance * GetTrollInstance()const
		{
			return m_emuse;
		}
		inline void	SetNewOverlayFrame( NewOverlayFrame * p_frame )
		{
			m_newOverlayFrame = p_frame;
		}

	private:
		void _initialise();
		void _showContextMenu( const wxPoint & p_position );
		void _setCustomFocus( bool p_hasFocus );
		void DoClose();
		
		DECLARE_EVENT_TABLE()
		void OnSetFocus( wxFocusEvent & p_event );
		void OnKillFocus( wxFocusEvent & p_event );
		void OnShow( wxShowEvent & p_event );
		void OnActivate( wxActivateEvent & p_event );
		void OnIconise( wxIconizeEvent & p_event );
		void OnButtonLeftDown( wxMouseEvent & p_event );
		void OnButtonLeftUp( wxMouseEvent & p_event );
		void OnButtonMiddleDown( wxMouseEvent & p_event );
		void OnButtonMiddleUp( wxMouseEvent & p_event );
		void OnButtonRightDown( wxMouseEvent & p_event );
		void OnButtonRightUp( wxMouseEvent & p_event );
		void OnMouseWheel( wxMouseEvent & p_event );
		void OnMouseMove( wxMouseEvent & p_event );
		void OnMouseEnter( wxMouseEvent & p_event );
		void OnMouseLeave( wxMouseEvent & p_event );
		void OnKeyDown( wxKeyEvent & p_event );
		void OnKeyUp( wxKeyEvent & p_event );
		void OnChar( wxKeyEvent & p_event );
		void OnClose( wxCloseEvent & p_event );
		void OnResize( wxCommandEvent & p_event );
		void OnReplace( wxCommandEvent & p_event );
		void OnNew( wxCommandEvent & p_event );
		void OnDelete( wxCommandEvent & p_event );
		inline void OnPaint( wxPaintEvent & p_event )
		{
			p_event.Skip();
		}
		inline void OnEraseBackground( wxEraseEvent & p_event )
		{
			p_event.Skip();
		}
		inline void OnRenderTimer( wxTimerEvent & p_event )
		{
			p_event.Skip();
		}

		static wxSize GetClientSize( wxWindow * p_window )
		{
			int l_height = 0;
			int l_width = 0;
			p_window->GetClientSize( & l_width, & l_height );
			return wxSize( l_width, l_height );
		}

	private:
		TROLL_ELYPSE_NAMESPACE::TrollInstance * m_emuse;
		COMMON_GUI_NAMESPACE::wxElypsePlugin * m_plugin;
		TROLL_ELYPSE_NAMESPACE::TrollSceneFileParser * m_parser;
		NewOverlayFrame * m_newOverlayFrame;

		Real m_width;
		Real m_height;

		unsigned int m_fsaa;

		bool m_rightButton;
		bool m_leftButton;
		bool m_middleButton;

		bool m_focused;
		bool m_primary;
		bool m_deleteAll;
		bool m_ogreActive;
		bool m_isVista;
		bool m_fucked;
		bool m_destroyed;
		bool m_oneTimePostSetFocus;
		bool m_editionMode;

		String m_baseDirectory;
		String m_filename;
		String m_installPath;
		String m_path;
		String m_museFile;

		TROLL_PROJECT_NAMESPACE::Displayable * m_displayedObject;
		TROLL_PROJECT_NAMESPACE::ProjectSPtr m_project;
		TROLL_PROJECT_NAMESPACE::Scene * m_testedScene;
		MainFrame * m_mainFrame;

		int m_cursorType;
	};
}
END_TROLL_GUI_NAMESPACE
#endif
