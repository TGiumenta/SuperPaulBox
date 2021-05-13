#include "pch.h"
#include "CollisionDefines.h"
namespace FieaGameEngine
{
    uint16_t CollisionUtiltiy::GetCategoryBits(ECollisionChannel collisionChannel)
    {
        size_t index = static_cast<size_t>(collisionChannel);
        if (index < collisionsBits.Size())
        {
            return collisionsBits[index];
        }

        return 1;
    }

    uint16_t CollisionUtiltiy::GetMaskBits(ECollisionChannel collisionChannel)
    {
        size_t index = static_cast<size_t>(collisionChannel);
        if (index < collisionsBits.Size())
        {
            uint16_t MaskBits = 0;
            for (size_t i = 0; i < CollisionMatrix[index].Size(); ++i)
            {
                
                if (CollisionMatrix[index][i] == true)
                {
                    MaskBits = MaskBits | collisionsBits[i];
                }
            }

            return MaskBits;
        }

        return 1;
    }


}