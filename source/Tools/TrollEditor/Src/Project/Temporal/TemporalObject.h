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
#ifndef ___TemporalObject_H___
#define ___TemporalObject_H___

#include "Module_Temporal.h"

#include "Project/Object.h"

BEGIN_TROLL_PROJECT_TEMPORAL_NAMESPACE
{
	enum TemporalObjectType
	{
		ttAnimationGroup,
		ttAnimatedObject,
		ttAnimation,
		ttSequence,
		ttPonctualEvent,
		ttContinuousEvent,
		ttKeyFrame
	};

	class TemporalObject
		: public TROLL_PROJECT_NAMESPACE::Object
	{
	public:
		TemporalObject( const wxString & p_name, const wxString & p_fileName, TemporalObjectType p_type );
		virtual ~TemporalObject();

		inline TemporalObjectType GetTemporalObjectType()const
		{
			return m_temporalObjectType;
		}

	protected:
		TemporalObjectType m_temporalObjectType;
	};
}
END_TROLL_PROJECT_TEMPORAL_NAMESPACE

#endif
