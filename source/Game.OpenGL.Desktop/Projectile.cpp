#include "pch.h"
#include "Projectile.h"
#include "Game.h"
#include "Event.h"

RTTI_DEFINITIONS(Projectile)

Projectile::Projectile() : 
	PhysicsEntity(Projectile::TypeIdClass())
{
}

void Projectile::OnCollisionStart(PhysicsEntity* other)
{
	if (other->GetTag() == "LevelWall" || other->GetTag() == "LevelGeometry")
	{
		Game::GetGame()->GetWorldState().DestroyEntity(this);
	}
}

