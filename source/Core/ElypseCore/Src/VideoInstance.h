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
#ifndef ___VIDEOINSTANCE_H___
#define ___VIDEOINSTANCE_H___

#include "Module_Media.h"

namespace Elypse
{
	namespace Media
	{
		class d_dll_export VideoInstance : public owned_by <VideoObject>
		{
		private:
			Entity * m_entity;
			short m_noSubMaterial;
			String m_oldMaterialName;
			bool m_delayLoad;

		public:
			VideoInstance( VideoObject * p_owner, Entity * p_entity );
			~VideoInstance();
			void Initialise( const String & p_videoTextureName );
			void Reset();
			void SetNoSubMaterial( unsigned short p_noSubMat );

		public:
			inline Entity 	*	GetEntity()const
			{
				return m_entity;
			}
			inline short		GetNoSubMaterial()const
			{
				return m_noSubMaterial;
			}
			inline bool			IsLoadDelayed()const
			{
				return m_delayLoad;
			}

			inline void		DelayLoad()
			{
				m_delayLoad = true;
			}

		};
	}
}

#endif
