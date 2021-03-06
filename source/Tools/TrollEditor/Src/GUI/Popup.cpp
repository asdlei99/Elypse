/*
See LICENSE file in root folder
*/
#include "TrollEditorPch.h"

#include "GUI/Popup.h"

#include "GUI/MainFrame.h"

#include "xpm/troll_editor.xpm"

#include <GradientButton.h>

namespace Troll
{
	namespace GUI
	{
		enum
		{
			Minimal_Quit = wxID_EXIT,
			Minimal_About = wxID_ABOUT,
			Minimal_TestDialog,
			Minimal_StartSimplePopup,
			Minimal_StartScrolledPopup,
			Minimal_LogWindow,
			Minimal_PopupButton,
			Minimal_PopupSpinctrl
		};

		BEGIN_EVENT_TABLE( SimpleTransientPopup, wxPopupTransientWindow )
			EVT_MOUSE_EVENTS( SimpleTransientPopup::OnMouse )
			EVT_SIZE( SimpleTransientPopup::OnSize )
			EVT_SET_FOCUS( SimpleTransientPopup::OnSetFocus )
			EVT_KILL_FOCUS( SimpleTransientPopup::OnKillFocus )
			EVT_BUTTON( Minimal_PopupButton, SimpleTransientPopup::OnButton )
			EVT_SPINCTRL( Minimal_PopupSpinctrl, SimpleTransientPopup::OnSpinCtrl )
		END_EVENT_TABLE()

		SimpleTransientPopup::SimpleTransientPopup( wxWindow * p_parent )
			: wxPopupTransientWindow( p_parent )
		{
			SetBackgroundColour( GuiCommon::PanelBackgroundColour );
			SetForegroundColour( GuiCommon::PanelForegroundColour );

			m_panel = new wxScrolledWindow( this, wxID_ANY );
			m_panel->SetBackgroundColour( GuiCommon::PanelBackgroundColour );
			m_panel->SetForegroundColour( GuiCommon::PanelForegroundColour );
			// Keep this code to verify if mouse events work, they're required if
			// you're making a control like a combobox where the items are highlighted
			// under the cursor, the m_panel is set focus in the Popup() function
			m_panel->Connect( wxEVT_MOTION, wxMouseEventHandler( SimpleTransientPopup::OnMouse ), NULL, this );
			wxStaticText * text = new wxStaticText( m_panel, wxID_ANY,
				wxT( "wx.PopupTransientWindow is a\n" )
				wxT( "wx.PopupWindow which disappears\n" )
				wxT( "automatically when the user\n" )
				wxT( "clicks the mouse outside it or if it\n" )
				wxT( "(or its first child) loses focus in \n" )
				wxT( "any other way." ) );
			m_button = new GuiCommon::GradientButton( m_panel, Minimal_PopupButton, wxT( "Press Me" ) );
			m_spinCtrl = new wxSpinCtrl( m_panel, Minimal_PopupSpinctrl, wxT( "Hello" ) );
			m_spinCtrl->SetBackgroundColour( GuiCommon::PanelBackgroundColour );
			m_spinCtrl->SetForegroundColour( GuiCommon::PanelForegroundColour );
			m_mouseText = new wxStaticText( m_panel
				, wxID_ANY
				, wxT( "<- Test Mouse ->" ) );
			m_mouseText->SetBackgroundColour( GuiCommon::PanelBackgroundColour );
			m_mouseText->SetForegroundColour( GuiCommon::PanelForegroundColour );
			wxBoxSizer * topSizer = new wxBoxSizer( wxVERTICAL );
			topSizer->Add( text, 0, wxALL, 5 );
			topSizer->Add( m_button, 0, wxALL, 5 );
			topSizer->Add( m_spinCtrl, 0, wxALL, 5 );
			topSizer->Add( m_mouseText, 0, wxCENTRE | wxALL, 5 );
			m_panel->SetAutoLayout( true );
			m_panel->SetSizer( topSizer );
			topSizer->Fit( m_panel );
			topSizer->Fit( this );
		}

		SimpleTransientPopup::~SimpleTransientPopup()
		{
		}

		void SimpleTransientPopup::Popup( wxWindow * p_focus )
		{
			LogDebug( wxT( "0x%lx SimpleTransientPopup::Popup" ) );
			wxPopupTransientWindow::Popup( p_focus ? p_focus : m_panel );
		}

		void SimpleTransientPopup::OnDismiss()
		{
			LogDebug( wxT( "0x%lx SimpleTransientPopup::OnDismiss" ) );
			wxPopupTransientWindow::OnDismiss();
		}

		bool SimpleTransientPopup::ProcessLeftDown( wxMouseEvent & p_event )
		{
			LogDebug( wxT( "0x%lx SimpleTransientPopup::ProcessLeftDown" ) );
			return wxPopupTransientWindow::ProcessLeftDown( p_event );
		}

		bool SimpleTransientPopup::Show( bool show )
		{
			LogDebug( wxT( "0x%lx SimpleTransientPopup::Show" ) );
			return wxPopupTransientWindow::Show( show );
		}

		void SimpleTransientPopup::OnSize( wxSizeEvent & p_event )
		{
			LogDebug( wxT( "0x%lx SimpleTransientPopup::OnSize" ) );
			p_event.Skip();
		}

		void SimpleTransientPopup::OnSetFocus( wxFocusEvent & p_event )
		{
			LogDebug( wxT( "SimpleTransientPopup::OnSetFocus" ) );
			p_event.Skip();
		}

		void SimpleTransientPopup::OnKillFocus( wxFocusEvent & p_event )
		{
			LogDebug( wxT( "SimpleTransientPopup::OnKillFocus" ) );
			p_event.Skip();
		}

		void SimpleTransientPopup::OnMouse( wxMouseEvent & p_event )
		{
			wxRect rect( m_mouseText->GetRect() );
			rect.SetX( -100000 );
			rect.SetWidth( 1000000 );
			wxColour colour( *wxLIGHT_GREY );

			if ( rect.Contains( p_event.GetPosition() ) )
			{
				colour = wxSystemSettings::GetColour( wxSYS_COLOUR_HIGHLIGHT );
				LogDebug( wxT( "SimpleTransientPopup::OnMouse" ) );
			}

			if ( colour != m_mouseText->GetBackgroundColour() )
			{
				m_mouseText->SetBackgroundColour( colour );
				m_mouseText->Refresh();
			}

			p_event.Skip();
		}

		void SimpleTransientPopup::OnButton( wxCommandEvent & p_event )
		{
			LogDebug( wxT( "SimpleTransientPopup::OnButton" ) );
			wxButton * button = wxDynamicCast( p_event.GetEventObject(), wxButton );

			if ( button->GetLabel() == wxT( "Press Me" ) )
			{
				button->SetLabel( wxT( "Pressed" ) );
			}
			else
			{
				button->SetLabel( wxT( "Press Me" ) );
			}

			p_event.Skip();
		}

		void SimpleTransientPopup::OnSpinCtrl( wxSpinEvent & p_event )
		{
			LogDebug( wxT( "SimpleTransientPopup::OnSpinCtrl" ) );
			p_event.Skip();
		}
	}
}
