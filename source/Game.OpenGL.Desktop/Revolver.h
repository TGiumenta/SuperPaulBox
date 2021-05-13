#pragma once

#include "Weapon.h"

using namespace FieaGameEngine;

class Revolver final : public Weapon
{
	RTTI_DECLARATIONS(Revolver, Weapon);

public:
	Revolver();
	~Revolver() = default;

	static const Vector<Signature> Signatures();

	virtual void SpawnProjectile() override;

private: 
	const std::string& m_revolverSound = "hit.mp3";
};

ConcreteFactory(Revolver, Scope);
