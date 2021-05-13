#pragma once
#include "Weapon.h"
#include "TypeManager.h"
#include "PhysicsEntity.h"

using namespace FieaGameEngine;

class MachineGun final : public Weapon
{
	RTTI_DECLARATIONS(MachineGun, Weapon);

public:
	MachineGun();
	~MachineGun() = default;

	static const Vector<Signature> Signatures();

	virtual void SpawnProjectile() override;

private:
	const std::string& m_machingGunSound = "ak.mp3";
};

ConcreteFactory(MachineGun, Scope);