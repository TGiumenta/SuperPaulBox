#pragma once

#include "Weapon.h"

using namespace FieaGameEngine;

class Revolver final : public Weapon
{
	RTTI_DECLARATIONS(Revolver, Weapon);

public:
	/// <summary>
	/// Constructor for object initialization
	/// </summary>
	Revolver();

	/// <summary>
	/// Defaulted copy constructor 
	/// </summary>
	/// <param name="other"> The revolver to copy </param>
	Revolver(const Revolver& other) = default;

	/// <summary>
	/// Defaulted move constructor
	/// </summary>
	/// <param name="other"> The revolver to move </param>
	Revolver(Revolver&& other) noexcept = default;

	/// <summary>
	/// Defaulted copy assignment operator
	/// </summary>
	/// <param name="other"> The revolver to copy to "this" revolver </param>
	/// <returns> The LHS of the = operator </returns>
	Revolver& operator=(const Revolver& other) = default;

	/// <summary>
	/// Defaulted move assignment operator
	/// </summary>
	/// <param name="other"> The revolver to move to "this" revolver </param>
	/// <returns> The LHS of the = operator </returns>
	Revolver& operator=(Revolver&& other) noexcept = default;

	/// <summary>
	/// Defaulted destructor
	/// </summary>
	~Revolver() = default;

	/// <summary>
	/// Queries the prescribed attributes of the revolver
	/// </summary>
	/// <returns> The prescribed attributes of the revolver </returns>
	static const Vector<Signature> Signatures();

	/// <summary>
	/// The derived spawn projectile method which plays sound and calls the base method with the json file
	/// </summary>
	virtual void SpawnProjectile() override;

private: 
	const std::string& m_revolverSound = "hit.mp3";
};

ConcreteFactory(Revolver, Scope);
