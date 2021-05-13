#include "pch.h"
#include "Number.h"

#include "pch.h"
#include "Weapon.h"
#include "Player.h"
#include "Game.h"
#include "Projectile.h"

RTTI_DEFINITIONS(Number)

using namespace FieaGameEngine;

Number::Number() :
	Entity(Number::TypeIdClass())
{

}

void Number::Update(WorldState* worldState)
{
	UNREFERENCED_PARAMETER(worldState);
}

const Vector<Signature> Number::Signatures()
{
	return Vector<Signature>
	{

	};
}