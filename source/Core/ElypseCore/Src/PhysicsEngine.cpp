/*
See LICENSE file in root folder
*/
#include "PrecompiledHeader.h"

#include "PhysicsEngine.h"
#include "PhysicsSimulation.h"
#include "PhysicsMaterial.h"
#include "Space.h"
#include "BoundingShape.h"

#include <ode/collision.h>
#include <ode/contact.h>
#include <ode/objects.h>

PhysicsEngine::PhysicsEngine()
{
	CreateSimulation( "Main" );
	CreateMaterial( "Default" );
}

PhysicsEngine::~PhysicsEngine()
{
	ClearAllUniverses();
	General::Utils::map::deleteAll( m_materials );
}

void PhysicsEngine::ClearAllUniverses() noexcept
{
	General::Utils::map::deleteAll( m_universes );
}

PhysicsSimulation * PhysicsEngine::CreateSimulation( String const & p_name )
{
	genlib_assert( ! p_name.empty() );
	return General::Utils::map::insert( m_universes, p_name, p_name );
}

bool PhysicsEngine::DestroySimulation( String const & p_name )
{
	return General::Utils::map::deleteValue( m_universes, p_name );
}

PhysicsMaterial * PhysicsEngine::CreateMaterial( String const & p_name )
{
	genlib_assert( ! p_name.empty() );
	return General::Utils::map::insert( m_materials, p_name, p_name );
}

bool PhysicsEngine::DestroyMaterial( String const & p_name )
{
	return General::Utils::map::deleteValue( m_materials, p_name );
}

void PhysicsEngine::Update()
{
	PhysicsSimulationMap::iterator i = m_universes.begin();
	const PhysicsSimulationMap::iterator & iend = m_universes.end();

	for ( ; i != iend ; ++ i )
	{
		if ( i->second->IsEnabled() && i->second->IsAutoUpdated() )
		{
			i->second->Update();
		}
	}
}

void PhysicsEngine::Render()
{
	PhysicsSimulationMap::iterator i = m_universes.begin();
	const PhysicsSimulationMap::iterator & iend = m_universes.end();

	for ( ; i != iend ; ++ i )
	{
		if ( i->second->IsEnabled() )
		{
			i->second->Render();
		}
	}
}

void PhysicsEngine::CollisionCallback( void *, dGeomID p_geomA, dGeomID p_geomB )
{
	bool l_aIsSpace = dGeomIsSpace( p_geomA ) != 0;
	bool l_bIsSpace = dGeomIsSpace( p_geomB ) != 0;
	void * l_ptrA = dGeomGetData( p_geomA );
	void * l_ptrB = dGeomGetData( p_geomB );

	if ( l_aIsSpace  || l_bIsSpace )
	{
		if ( l_aIsSpace && l_bIsSpace )
		{
			static_cast <Space *>( l_ptrA )->CollideSpace( static_cast <Space *>( l_ptrB ) );
		}
		else if ( l_aIsSpace )
		{
			static_cast <Space *>( l_ptrA )->CollideShape( static_cast <BoundingShape *>( l_ptrB ) );
		}
		else
		{
			static_cast <Space *>( l_ptrB )->CollideShape( static_cast <BoundingShape *>( l_ptrA ) );
		}

		if ( l_aIsSpace )
		{
			static_cast <Space *>( l_ptrA )->CollideInternal();
		}

		if ( l_bIsSpace )
		{
			static_cast <Space *>( l_ptrB )->CollideInternal();
		}
	}
	else
	{
		static_cast <BoundingShape *>( l_ptrA )->Collide( static_cast <BoundingShape *>( l_ptrB ) );
	}
}

void PhysicsEngine::RollingFrictionCallback( PhysicsObject * p_objA, PhysicsObject * p_objB, dContact * p_contact, uint32_t p_numContacts )
{
	//TODO
	if ( p_objA->IsStatic() )
	{
		PhysicsObject * l_temp = p_objA;
		p_objA = p_objB;
		p_objB = l_temp;
	}

	if ( p_objA->GetMaterial()->m_rollFrictionCoeff1 == 0.0 && p_objA->GetMaterial()->m_rollFrictionDamping == 0.0 )
	{
		return;
	}

	if ( p_objB->GetMaterial()->m_rollFrictionCoeff1 == 0.0 && p_objB->GetMaterial()->m_rollFrictionDamping == 0.0 )
	{
		return;
	}

	Vector3 l_totalLocalSpeedA = Vector3::ZERO;
	Vector3 l_totalLocalSpeedB = Vector3::ZERO;
	Vector3 l_normal = Vector3::ZERO;
	Vector3 l_position = Vector3::ZERO;

	for ( uint32_t i = 0 ; i < p_numContacts ; i ++ )
	{
		dVector3 l_result;
		l_position += Vector3( p_contact[i].geom.pos[0], p_contact[i].geom.pos[1], p_contact[i].geom.pos[2] );
		dBodyGetPointVel( p_objA->GetBodyId(), p_contact[i].geom.pos[0], p_contact[i].geom.pos[1], p_contact[i].geom.pos[2], l_result );
		l_totalLocalSpeedA += Vector3( l_result[0], l_result[1], l_result[2] );
		dBodyGetPointVel( p_objB->GetBodyId(), p_contact[i].geom.pos[0], p_contact[i].geom.pos[1], p_contact[i].geom.pos[2], l_result );
		l_totalLocalSpeedB += Vector3( l_result[0], l_result[1], l_result[2] );
		l_normal += Vector3( p_contact[i].geom.normal[0], p_contact[i].geom.normal[1], p_contact[i].geom.normal[2] );
	}

	l_totalLocalSpeedA /= Real( p_numContacts );
	l_totalLocalSpeedB /= Real( p_numContacts );
	l_position /= Real( p_numContacts );
	l_normal.normalise();
	l_totalLocalSpeedA -= l_normal * l_totalLocalSpeedA.dotProduct( l_normal );
	l_totalLocalSpeedB -= l_normal * l_totalLocalSpeedB.dotProduct( l_normal );
	Vector3 l_diff = l_totalLocalSpeedB - l_totalLocalSpeedA;
	l_diff = l_diff * p_objA->GetMaterial()->m_rollFrictionCoeff1;

	if ( ! p_objA->IsStatic() )
	{
		p_objA->AddForce( l_diff );
		const dReal * v = dBodyGetAngularVel( p_objA->GetBodyId() );
		dBodyAddTorque( p_objA->GetBodyId(), v[0] * p_objA->GetMaterial()->m_rollFrictionDamping, v[1] * p_objA->GetMaterial()->m_rollFrictionDamping, v[2] * p_objA->GetMaterial()->m_rollFrictionDamping );
	}
}
