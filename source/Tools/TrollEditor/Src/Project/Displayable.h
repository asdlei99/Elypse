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
#ifndef ___TEDISPLAYABLE_H___
#define ___TEDISPLAYABLE_H___

#include "Module_Project.h"

#include <wx/treebase.h>

#include "GUI/ObjectProperties/Module_Properties.h"

BEGIN_TROLL_PROJECT_NAMESPACE
{
	class Displayable
	{
	public:
		Displayable();
		virtual ~Displayable();

		virtual int BuildPanel( wxWindow * p_parent, int p_width ) = 0;
		virtual void ShowProperties();
		virtual void HideProperties();

		inline wxTreeItemId	GetTreeItemId()const
		{
			return m_treeItemID;
		}
		inline wxPanel * GetPanel()const
		{
			return reinterpret_cast <wxPanel *>( m_propertiesPanel );
		}
		inline void SetTreeItemId( wxTreeItemId p_id )
		{
			m_treeItemID = p_id;
		}

	protected:
		TROLL_GUI_PROPERTIES_NAMESPACE::ObjectProperties * m_propertiesPanel;
		wxTreeItemId m_treeItemID;
	};
}
END_TROLL_PROJECT_NAMESPACE

#endif
