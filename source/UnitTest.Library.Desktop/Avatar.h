#pragma once
#include "TypeManager.h"
#include "Entity.h"

namespace UnitTests
{
	class Avatar : public FieaGameEngine::Entity
	{
		RTTI_DECLARATIONS(Avatar, Entity);

	public:

		Avatar();
		Avatar(const Avatar& other) = default;
		Avatar(Avatar&& other) = default;
		Avatar& operator=(const Avatar& other) = default;
		Avatar& operator=(Avatar&& other) = default;
        virtual ~Avatar() = default;

		static const FieaGameEngine::Vector<FieaGameEngine::Signature> Signatures();

        int m_health = 0;
        float m_dps = 0;

	protected:
		virtual gsl::owner<Scope*> Clone() const override;

		
	};

	using FieaGameEngine::Scope;
	ConcreteFactory(Avatar, Scope);
}

