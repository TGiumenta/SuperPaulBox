#include "pch.h"
#include "JsonTestParseHelper.h"
#include "Datum.h"

using FieaGameEngine::Datum;
using FieaGameEngine::Scope;

namespace UnitTests
{
    RTTI_DEFINITIONS(JsonTestParseHelper);
    RTTI_DEFINITIONS(JsonTestParseHelper::SharedData);

#pragma region SharedData
    gsl::owner<JsonTestParseHelper::SharedData*> JsonTestParseHelper::SharedData::Create() const
    {
        return new JsonTestParseHelper::SharedData();
    }
#pragma endregion

    gsl::owner<FieaGameEngine::IJsonParseHelper*> JsonTestParseHelper::Create() const
    {
        return new JsonTestParseHelper();
    }

    void JsonTestParseHelper::Initialize()
    {
    }

    bool JsonTestParseHelper::StartHandler(FieaGameEngine::JsonParseCoordinator::SharedData& sharedData, const std::string& key, const Json::Value& jsonValue, bool isArray)
    {
        JsonTestParseHelper::SharedData* customSharedData = sharedData.As<JsonTestParseHelper::SharedData>();
        if (customSharedData == nullptr)
        {
            return false;
        }

        // If this is the first time we get in here
        if (scopeStack.IsEmpty())
        {
            scopeStack.PushBack(&customSharedData->rootScope);
        }

        // If we haven't read a key yet, read in the key
        if (readKey.empty())
        {
            readKey = key;
        }
        // Otherwise, if we are reading a type
        else if (readType == FieaGameEngine::Datum::DatumType::Unknown)
        {
            // If the key isn't type or the value isn't a string, the grammar is wrong
            if (key != "type" || !jsonValue.isString())
            {
                return false;
            }

            readType = getTypeFromString(jsonValue.asString());
        }
        // Otherwise we are reading a value
        else
        {
            if (key != "value")
            {
                return false;
            }

            // If this is an object, we create a nested scope
            if (readType == Datum::DatumType::Table)
            {
                Scope& newScope = scopeStack.Back()->AppendScope(readKey); newScope;
                // Probably gonna need a stack to store this info

            }
            // Otherwise this is plain-old-data and we should create a normal datum
            else
            {
                Datum& newDatum = scopeStack.Back()->Append(readKey);
                newDatum.SetType(readType);
                std::string valueString = jsonValue.toStyledString();
                if (newDatum.IsExternal())
                {
                    newDatum.SetFromString(valueString);
                }
                else
                {
                    newDatum.PushBackFromString(valueString);
                }
            }
        }

        sharedData; key; jsonValue; isArray;
        return true;
    }

    bool JsonTestParseHelper::EndHandler(FieaGameEngine::JsonParseCoordinator::SharedData& sharedData, const std::string& key)
    {
        sharedData; key;
        return false;
    }

    FieaGameEngine::Datum::DatumType JsonTestParseHelper::getTypeFromString(std::string input)
    {
        if (input == "string")
        {
            return Datum::DatumType::String;
        }

        if (input == "integer")
        {
            return Datum::DatumType::Integer;
        }

        if (input == "float")
        {
            return Datum::DatumType::Float;
        }

        if (input == "matrix")
        {
            return Datum::DatumType::Matrix;
        }

        if (input == "vector")
        {
            return Datum::DatumType::Vector;
        }

        if (input == "table")
        {
            return Datum::DatumType::Table;
        }

        return Datum::DatumType::Unknown;
        
    }
}


