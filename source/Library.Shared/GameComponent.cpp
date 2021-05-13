#include "pch.h"
#include "GameComponent.h"

namespace FieaGameEngine
{
    RTTI_DEFINITIONS(GameComponent)

        GameComponent::GameComponent() :
        mEnabled(true)
    {
    }

    bool GameComponent::Enabled() const
    {
        return mEnabled;
    }

    void GameComponent::SetEnabled(bool enabled)
    {
        mEnabled = enabled;
    }

    void GameComponent::Initialize()
    {
    }

    void GameComponent::Shutdown()
    {
    }

    void GameComponent::Update(const GameTime&)
    {
    }
}