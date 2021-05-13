#pragma once
#include "Entity.h"
#include "Vector.h"
#include "TypeManager.h"

using namespace FieaGameEngine;

class Number : public Entity
{
	RTTI_DECLARATIONS(Number, Entity)

public:
	Number();
	virtual ~Number() = default;

	void Update(WorldState* worldState);

	static const Vector<Signature> Signatures();


protected:


private:

};

ConcreteFactory(Number, Scope);