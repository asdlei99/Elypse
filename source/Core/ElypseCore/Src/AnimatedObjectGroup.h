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
#ifndef ___ANIMATED_OBJECT_GROUP_H___
#define ___ANIMATED_OBJECT_GROUP_H___

#include "Module_Animation.h"

namespace Elypse
{
	namespace Animation
	{
		class d_dll_export  AnimatedObjectGroup : public General::Theory::named,
			public General::Theory::owned_by<AnimationManager>,
			d_noncopyable
		{
		protected:
			AnimatedObjectMap m_objects;
			RealMap m_commonAnimationsTime;

		public:
			AnimatedObjectGroup( const String & p_name, AnimationManager * p_owner );
			~AnimatedObjectGroup();

		public:
			void Update( Real p_timeStep );
			AnimatedObject * AddEntity( Ogre::Entity * p_timeStep );

			void StartAnimation( const String & p_animationName );
			void StopAnimation( const String & p_animationName );
			void PauseAnimation( const String & p_animationName );
			void SetTimeAnimation( const String & p_animationName, Real p_timeIndex );
			void SetWeightAnimation( const String & p_animationName, Real p_weight );
			void SetAnimationTimeOut( const String & p_animationName, Real p_timeOut );
			void SetAnimationTimeIn( const String & p_animationName, Real p_timeIn );
			void SetAnimationLooped( const String & p_animationName, bool p_looped );
			void SetAnimationReversed( const String & p_animationName, bool p_reversed );

			void SetTimeScaleAnimation( const String & p_animationName, Real p_timescale );

			void ChainAnimation( const String & p_animName, const String & p_chainedAnimationName );

			void RemoveObject( const String & p_objectName );

			Real GetAnimationTime( const String & p_animationName )const;

			void StopAllAnimations();

			void ChainAllAnimations( bool p_looped );

			void Clear();

			void SetPauseAll( bool p_pause );

		public:
			inline size_t GetNumObjects()const
			{
				return m_objects.size();
			}
			inline AnimatedObject * GetObjectByName( const String & p_objectName )const
			{
				return General::Utils::map::findOrNull( m_objects, p_objectName );
			}
			inline bool HasAnimation( const String & p_name )
			{
				return General::Utils::map::has( m_commonAnimationsTime, p_name );
			}
		};
	}
}

#endif
