/*
See LICENSE file in root folder
*/
#ifndef ___PORTAL_H___
#define ___PORTAL_H___

#include "Module_Zone.h"
#include "Module_Physics.h"

namespace Elypse
{
	namespace Scene
	{
		class d_dll_export Portal : public named, noncopyable
		{
		private:
			Zone * m_target;
			bool m_active;

			PhysicsObject * m_trigger;

			String m_targetName;

		public:
			Portal( String const & p_name );
			~Portal();

		public:
			void SetSpace( Space * p_space );

		public:
			inline Zone		*	GetTargetZone()const
			{
				return m_target;
			}
			inline String const &	GetTargetName()const
			{
				return m_targetName;
			}
			inline bool				IsActive()const
			{
				return m_active;
			}
			inline PhysicsObject *	GetTrigger()const
			{
				return m_trigger;
			}

			inline void				SetActive( bool p_active )
			{
				m_active = p_active;
			}
			inline void 			SetTargetZone( Zone * p_targetZone )
			{
				m_target = p_targetZone;
			}
			inline void 			SetTargetName( String const & p_name )
			{
				m_targetName = p_name;
			}
			inline void				SetTrigger( PhysicsObject * p_object )
			{
				m_trigger = p_object;
			}
		};
	}
}

#endif
