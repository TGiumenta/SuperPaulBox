#pragma once
namespace FieaGameEngine
{
    struct DefaultIncrement final
    {
        size_t operator()(size_t /*size*/, size_t capacity) const;
    };
}


