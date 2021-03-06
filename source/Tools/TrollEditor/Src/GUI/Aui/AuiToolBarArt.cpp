#include "AuiToolBarArt.h"

namespace Troll
{
	namespace GUI
	{
		AuiToolBarArt::AuiToolBarArt()
			: wxAuiDefaultToolBarArt()
		{
		}

		wxAuiToolBarArt * AuiToolBarArt::Clone()
		{
			return new AuiToolBarArt( *this );
		}

		void AuiToolBarArt::DrawBackground( wxDC & p_dc, wxWindow * p_window, wxRect const & p_rect )
		{
#if wxCHECK_VERSION( 2, 9, 0 )

			p_dc.SetPen( wxPen( GuiCommon::InactiveTabColour, 1, wxPENSTYLE_SOLID ) );
			p_dc.SetBrush( wxBrush( GuiCommon::InactiveTabColour, wxBRUSHSTYLE_SOLID ) );

#else

			p_dc.SetPen( wxPen( GuiCommon::InactiveTabColour, 1, wxSOLID ) );
			p_dc.SetBrush( wxBrush( GuiCommon::InactiveTabColour, wxSOLID ) );

#endif

			p_dc.DrawRectangle( p_rect );
		}

		void AuiToolBarArt::DrawPlainBackground( wxDC & p_dc, wxWindow * p_window, wxRect const & p_rect )
		{
#if wxCHECK_VERSION( 2, 9, 0 )

			p_dc.SetPen( wxPen( GuiCommon::InactiveTabColour, 1, wxPENSTYLE_SOLID ) );
			p_dc.SetBrush( wxBrush( GuiCommon::InactiveTabColour, wxBRUSHSTYLE_SOLID ) );

#else

			p_dc.SetPen( wxPen( GuiCommon::InactiveTabColour, 1, wxSOLID ) );
			p_dc.SetBrush( wxBrush( GuiCommon::InactiveTabColour, wxSOLID ) );

#endif

			p_dc.DrawRectangle( p_rect );
		}

		void AuiToolBarArt::DrawSeparator( wxDC & p_dc, wxWindow * p_window, wxRect const & p_rect )
		{
#if wxCHECK_VERSION( 2, 9, 0 )

			p_dc.SetPen( wxPen( GuiCommon::BorderColour, 1, wxPENSTYLE_SOLID ) );

#else

			p_dc.SetPen( wxPen( GuiCommon::BorderColour, 1, wxSOLID ) );

#endif
			p_dc.DrawLine( ( p_rect.GetBottomLeft() + p_rect.GetBottomRight() ) / 2, ( p_rect.GetTopLeft() + p_rect.GetTopRight() ) / 2 );
		}
	}
}
