#pragma once
namespace FieaGameEngine
{

    template <typename TKey>
    struct DefaultHash final
    {
        size_t operator()(const TKey& value);
    };
    
}

#include "DefaultHash.inl"