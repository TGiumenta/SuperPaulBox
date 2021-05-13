#pragma once

#include "RTTI.h"

namespace FieaGameEngine
{
    class GameTime;

    class GameComponent : public RTTI
    {
        RTTI_DECLARATIONS(GameComponent, RTTI)

    public:
        GameComponent();
        GameComponent(const GameComponent&) = default;
        GameComponent& operator=(const GameComponent&) = default;
        GameComponent(GameComponent&&) = default;
        GameComponent& operator=(GameComponent&&) = default;
        virtual ~GameComponent() = default;

        bool Enabled() const;
        void SetEnabled(bool enabled);

        virtual void Initialize();
        virtual void Shutdown();
        virtual void Update(const GameTime& gameTime);

    protected:
        bool mEnabled;
    };
}
