#pragma once

#include "Weapon.h"

using namespace FieaGameEngine;

class Shotgun final : public Weapon
{
	RTTI_DECLARATIONS(Shotgun, Weapon);

public:
	Shotgun();
	~Shotgun() = default;

	static const Vector<Signature> Signatures();

	virtual void SpawnProjectile() override;

private:
	const std::string& m_shotgunShoot = "shotty.mp3";
	const std::string& m_shotgunReload = "shotty reload.mp3";

};

ConcreteFactory(Shotgun, Scope);
