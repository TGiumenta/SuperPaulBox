#include "pch.h"
#include "JsonIntegerParseHelper.h"


namespace UnitTests
{
    RTTI_DEFINITIONS(JsonIntegerParseHelper);
    RTTI_DEFINITIONS(JsonIntegerParseHelper::SharedData);

#pragma region SharedData
    gsl::owner<JsonIntegerParseHelper::SharedData*> JsonIntegerParseHelper::SharedData::Create() const 
    {
        return new JsonIntegerParseHelper::SharedData();
    }
#pragma endregion

    gsl::owner<FieaGameEngine::IJsonParseHelper*> UnitTests::JsonIntegerParseHelper::Create() const
    {
        return new JsonIntegerParseHelper();
    }

    void JsonIntegerParseHelper::Initialize()
    {
        IJsonParseHelper::Initialize();
        m_parsingData = false;
    }

    bool UnitTests::JsonIntegerParseHelper::StartHandler(FieaGameEngine::JsonParseCoordinator::SharedData& sharedData, const std::string& key, const Json::Value& jsonValue, bool)
    {
        JsonIntegerParseHelper::SharedData* customSharedData = sharedData.As<JsonIntegerParseHelper::SharedData>();
        if (customSharedData == nullptr)
        {
            return false;
        }

        if (key != IntegerKey)
        {
            return false;
        }

        if (m_parsingData)
        {
            throw std::runtime_error("Received another start element while already parsing an integer!");
        }

        m_parsingData = true;

        customSharedData->m_data.PushBack(jsonValue.asInt());

        return true;
    }

    bool UnitTests::JsonIntegerParseHelper::EndHandler(FieaGameEngine::JsonParseCoordinator::SharedData& sharedData, const std::string& key)
    {
        JsonIntegerParseHelper::SharedData* customSharedData = sharedData.As<JsonIntegerParseHelper::SharedData>();
        if (customSharedData == nullptr || key != IntegerKey || m_parsingData == false)
        {
            return false;
        }

        m_parsingData = false;

        return true;
    }
}


