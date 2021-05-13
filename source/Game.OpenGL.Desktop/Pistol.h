#pragma once
#include "Weapon.h"
#include "TypeManager.h"
#include "PhysicsEntity.h"

using namespace FieaGameEngine;

class Pistol final : public Weapon
{
	RTTI_DECLARATIONS(Pistol, Weapon);

public:
	Pistol();
	~Pistol() = default;

	static const Vector<Signature> Signatures();

	virtual void SpawnProjectile() override;

private:
	const std::string& m_pistolSound = "generic_shot.mp3";
};

ConcreteFactory(Pistol, Scope);