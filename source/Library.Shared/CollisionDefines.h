#pragma once
#include "Vector.h"
#include <cstdint>
#include <stdint.h>

namespace FieaGameEngine
{
    enum class ECollisionChannel
    {
        PLAYER = 0,
        CRATES = 1,
        ENEMIES = 2,
        PROJECTILES = 3,
        MAP = 4
    };

    static const Vector<uint16_t> collisionsBits =
    {
        0b00001,
        0b00010,
        0b00100,
        0b01000,
        0b10000
    };

    static const Vector<Vector<bool>> CollisionMatrix
    {
        //                  PLAYER          CRATES          ENEMIES         PROJECTILES     MAP
        /* PLAYER       */{ true,           true,           true,           true,           true },
        /* CRATES       */{ true,           true,           true,           true,           true },
        /* ENEMIES      */{ true,           false,          false,          true,           true },
        /* PROJECTILES  */{ true,           true,           true,           true,           true },
        /* MAP          */{ true,           true,           true,           true,           true }
    };

    struct CollisionUtiltiy
    {
        static uint16_t GetCategoryBits(ECollisionChannel collisionChannel);

        static uint16_t GetMaskBits(ECollisionChannel collisionChannel);
    };
}