#include "pch.h"
#include "Power.h"

using namespace FieaGameEngine;

namespace UnitTests
{
	RTTI_DEFINITIONS(Power);

	Power::Power()
		: Attributed(Power::TypeIdClass())
	{
	}

	const FieaGameEngine::Vector<FieaGameEngine::Signature> Power::Signatures()
	{
        return Vector<Signature>
        {
        };
	}

	gsl::owner<Scope*> Power::Clone() const
	{
		return gsl::owner<Scope*>();
	}
}
