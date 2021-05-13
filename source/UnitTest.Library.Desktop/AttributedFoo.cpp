#include "pch.h"
#include "AttributedFoo.h"

using namespace FieaGameEngine;

namespace UnitTests
{
    RTTI_DEFINITIONS(AttributedFoo);



    AttributedFoo::AttributedFoo()
        : Attributed(AttributedFoo::TypeIdClass())
    {
    }

    const FieaGameEngine::Vector<FieaGameEngine::Signature> AttributedFoo::Signatures()
    {
        return Vector<Signature>
        {
            { "ExternalInteger", Datum::DatumType::Integer, 1, offsetof(AttributedFoo, ExternalInteger) },
            { "ExternalFloat", Datum::DatumType::Float, 1, offsetof(AttributedFoo, ExternalFloat) },
            { "ExternalString", Datum::DatumType::String, 1, offsetof(AttributedFoo, ExternalString) },
            { "ExternalVector", Datum::DatumType::Vector, 1, offsetof(AttributedFoo, ExternalVector) },
            { "ExternalMatrix", Datum::DatumType::Matrix, 1, offsetof(AttributedFoo, ExternalMatrix) },
            { "ExternalIntegerArray", Datum::DatumType::Integer, ArraySize, offsetof(AttributedFoo, ExternalIntegerArray)},
            { "ExternalFloatArray", Datum::DatumType::Float, ArraySize, offsetof(AttributedFoo, ExternalFloatArray) },
            { "ExternalStringArray", Datum::DatumType::String, ArraySize, offsetof(AttributedFoo, ExternalStringArray) },
            { "ExternalVectorArray", Datum::DatumType::Vector, ArraySize, offsetof(AttributedFoo, ExternalVectorArray) },
            { "ExternalMatrixArray", Datum::DatumType::Matrix, ArraySize, offsetof(AttributedFoo, ExternalMatrixArray) },
            { "NestedScope", Datum::DatumType::Table, 0, 0},
            { "NestedScopeArray", Datum::DatumType::Table, 5, 0}
        };
    }

    gsl::owner<Scope*> AttributedFoo::Clone() const
    {
        return new AttributedFoo(*this);;
    }

}