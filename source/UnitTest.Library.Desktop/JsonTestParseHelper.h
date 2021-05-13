#pragma once
#include "JsonParseCoordinator.h"
#include "IJsonParseHelper.h"
#include "Vector.h"
#include "Scope.h"

namespace UnitTests
{
    class JsonTestParseHelper final : public FieaGameEngine::IJsonParseHelper
    {
        RTTI_DECLARATIONS(JsonTestParseHelper, FieaGameEngine::IJsonParseHelper)
    public:
        class SharedData final : public FieaGameEngine::JsonParseCoordinator::SharedData
        {
            RTTI_DECLARATIONS(SharedData, FieaGameEngine::JsonParseCoordinator::SharedData)
        public:
            //virtual void Initialize() override;
            virtual gsl::owner<SharedData*> Create() const override;

            FieaGameEngine::Scope rootScope;
        };

        virtual gsl::owner<FieaGameEngine::IJsonParseHelper*> Create() const override;
        virtual void Initialize() override;
        virtual bool StartHandler(FieaGameEngine::JsonParseCoordinator::SharedData& sharedData, const std::string& key, const Json::Value& jsonValue, bool isArray) override;
        virtual bool EndHandler(FieaGameEngine::JsonParseCoordinator::SharedData& sharedData, const std::string& key) override;

    private:
        FieaGameEngine::Datum::DatumType getTypeFromString(std::string input);

        std::string readKey = "";
        FieaGameEngine::Datum::DatumType readType = FieaGameEngine::Datum::DatumType::Unknown;

        FieaGameEngine::Vector<FieaGameEngine::Scope*> scopeStack;
        size_t currentDepth = 0;
    };
}

