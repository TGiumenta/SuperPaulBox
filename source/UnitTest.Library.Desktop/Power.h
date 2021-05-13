#pragma once
#include "Attributed.h"
#include "TypeManager.h"
#include "Factory.h"

namespace UnitTests
{
    class Power : public FieaGameEngine::Attributed
    {
        RTTI_DECLARATIONS(Power, FieaGameEngine::Attributed);

    public:
        Power();
        Power(const Power& other) = default;
        Power(Power && other) = default;
        Power& operator=(const Power & other) = default;
        Power& operator=(Power && other) = default;
        ~Power() = default;

        // Should these be here? Since they are defined in JSON
        std::string m_name;
        float m_dps;

        static const FieaGameEngine::Vector<FieaGameEngine::Signature> Signatures();

    protected:
        gsl::owner<Scope*> Clone() const override;
    };

    using namespace FieaGameEngine;
    ConcreteFactory(Power, Scope);
}

