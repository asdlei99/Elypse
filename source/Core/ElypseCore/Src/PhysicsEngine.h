/*
See LICENSE file in root folder
*/
#ifndef ___PHYSICS_ENGINE_H___
#define ___PHYSICS_ENGINE_H___

#include "Module_Physics.h"

#include <PreciseTimer.h>

#include <Manager.h>
#include <ode/contact.h>

namespace Elypse
{
	namespace Physics
	{
		class d_dll_export PhysicsEngine : d_noncopyable
		{
		private:
			PhysicsSimulationMap m_universes;
			PhysicsMaterialMap m_materials;

		public:
			PhysicsEngine();
			~PhysicsEngine();

		public:
			void ClearAllUniverses() noexcept;
			void Update();
			void Render();

			PhysicsSimulation * CreateSimulation( String const & p_name );
			bool DestroySimulation( String const & p_name );

			PhysicsMaterial * CreateMaterial( String const & p_name );
			bool DestroyMaterial( String const & p_name );

			static void CollisionCallback( void * p_data, Ode::dGeomID p_geomA, Ode::dGeomID p_geomB );
			static void RollingFrictionCallback( PhysicsObject * p_objA, PhysicsObject * p_objB, Ode::dContact * p_contacts, uint32_t p_numContacts );

			inline PhysicsSimulation * GetSimulation( String const & p_name ) const
			{
				return General::Utils::map::findOrNull( m_universes, p_name );
			}
			inline PhysicsMaterial * GetMaterial( String const & p_name ) const
			{
				return General::Utils::map::findOrNull( m_materials, p_name );
			}
		};
	}
}

#endif
