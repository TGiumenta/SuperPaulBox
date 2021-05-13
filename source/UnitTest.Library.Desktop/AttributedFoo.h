#pragma once
#include "Attributed.h"
#include "TypeManager.h"

namespace UnitTests
{
    class AttributedFoo final: public FieaGameEngine::Attributed
    {
        RTTI_DECLARATIONS(AttributedFoo, FieaGameEngine::Attributed);

    public:
        static const std::size_t ArraySize = 5;

        AttributedFoo();
        AttributedFoo(const AttributedFoo& other) = default;
        AttributedFoo(AttributedFoo&& other) = default;
        AttributedFoo& operator=(const AttributedFoo& other) = default;
        AttributedFoo& operator=(AttributedFoo&& other) = default;
        ~AttributedFoo() = default;

        int ExternalInteger = 0;
        float ExternalFloat = 0;
        std::string ExternalString;
        glm::vec4 ExternalVector;
        glm::mat4 ExternalMatrix;

        int ExternalIntegerArray[ArraySize];
        float ExternalFloatArray[ArraySize];
        std::string ExternalStringArray[ArraySize];
        glm::vec4 ExternalVectorArray[ArraySize];
        glm::mat4 ExternalMatrixArray[ArraySize];

        static const FieaGameEngine::Vector<FieaGameEngine::Signature> Signatures();

    protected:
        gsl::owner<Scope*> Clone() const override;

    };
}