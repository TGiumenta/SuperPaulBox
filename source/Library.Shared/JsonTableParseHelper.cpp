#include "pch.h"
#include "JsonTableParseHelper.h"
#include "Factory.h"

namespace FieaGameEngine
{
    RTTI_DEFINITIONS(JsonTableParseHelper);
    RTTI_DEFINITIONS(JsonTableParseHelper::SharedData);

#pragma region SharedData
    JsonTableParseHelper::SharedData::SharedData(Scope& scope)
    {
        rootScope = &scope;
    }

    gsl::owner<JsonTableParseHelper::SharedData*> JsonTableParseHelper::SharedData::Create() const
    {
        return new JsonTableParseHelper::SharedData(*rootScope);
    }
#pragma endregion

    gsl::owner<IJsonParseHelper*> JsonTableParseHelper::Create() const
    {
        return new JsonTableParseHelper();
    }

    bool JsonTableParseHelper::StartHandler(JsonParseCoordinator::SharedData& sharedData, const std::string& key, const Json::Value& jsonValue, bool isArray)
    {
        JsonTableParseHelper::SharedData* customSharedData = sharedData.As<JsonTableParseHelper::SharedData>();
        if (customSharedData == nullptr)
        {
            return false;
        }

        if (key == "type")
        {
            StackFrame& currentStackFrame = m_stack.Top();
            // Add type to current stack frame
            currentStackFrame.m_type = Datum::m_nameToType.at(jsonValue.asString());

        }
        else if (key == "class")
        {
            StackFrame& currentStackFrame = m_stack.Top();
            currentStackFrame.m_className = jsonValue.asString();
        }
        else if (key == "value")
        {
            StackFrame& currentStackFrame = m_stack.Top();

            if (currentStackFrame.m_type == Datum::DatumType::Table)
            {
                if (currentStackFrame.m_className.empty())
                {
                    currentStackFrame.m_className = "Scope";
                }

                Scope* nestedScope = Factory<Scope>::Create(currentStackFrame.m_className);
                if (nestedScope == nullptr)
                {
                    throw std::exception("Trying to construct a class that doesn't exist!");
                }

                currentStackFrame.m_context->Adopt(*nestedScope, currentStackFrame.m_key);

                // If this is an array, push an entire new stack frame, otherwise just update
                // the current context
                if (isArray)
                {
                    m_stack.Push(StackFrame(key, nestedScope));
                }
                else
                {
                    currentStackFrame.m_context = nestedScope;
                }

                // Clear the current class name in case we are holding an old class name and
                // have gotten to a new object. This is specifically for the case of an
                // array of nested scopes with different classes
                currentStackFrame.m_className.clear();
            }
            else
            {
                Datum& nestedDatum = currentStackFrame.m_context->Append(currentStackFrame.m_key);
                nestedDatum.SetType(currentStackFrame.m_type);

                JsonTableParseHelper::JsonToDatumFunction func = m_JSONToDatumFunctions[static_cast<uint32_t>(currentStackFrame.m_type)];
                (this->*func)(nestedDatum, jsonValue, currentStackFrame.m_arrayIndex);

                if (isArray)
                {
                    ++currentStackFrame.m_arrayIndex;
                }
            }
        }
        else
        {
            Scope* context = (m_stack.IsEmpty()) ? customSharedData->rootScope : m_stack.Top().m_context; context;
            m_stack.Push(StackFrame(key, context));
        }

        return true;
    }

    bool JsonTableParseHelper::EndHandler(JsonParseCoordinator::SharedData&, const std::string& key)
    {
        // If the current stack frame is completed, pop it off the stack
        if (&key == &m_stack.Top().m_key)
        {
            m_stack.Pop();     
        }

        return true;
    }

    #pragma region JsonIntoDatumFunctions
    void JsonTableParseHelper::JsonIntToDatum(Datum& datum, const Json::Value& jsonValue, size_t index)
    {
        if (datum.IsExternal())
        {
            datum.Set(jsonValue.asInt(), index);
        }
        else
        {
            datum.PushBack(jsonValue.asInt());
        }
    }

    void JsonTableParseHelper::JsonFloatToDatum(Datum& datum, const Json::Value& jsonValue, size_t index)
    {
        if (datum.IsExternal())
        {
            datum.Set(jsonValue.asFloat(), index);
        }
        else
        {
            datum.PushBack(jsonValue.asFloat());
        }
    }

    void JsonTableParseHelper::JsonStringToDatum(Datum& datum, const Json::Value& jsonValue, size_t index)
    {
        if (datum.IsExternal())
        {
            datum.Set(jsonValue.asString(), index);
        }
        else
        {
            datum.PushBack(jsonValue.asString());
        }
    }

    void JsonTableParseHelper::JsonGenericToDatum(Datum& datum, const Json::Value& jsonValue, size_t index)
    {
        std::string valueString = jsonValue.asString();

        if (datum.IsExternal())
        {
            datum.SetFromString(valueString, index);
        }
        else
        {
            datum.PushBackFromString(valueString);
        }
    }
    #pragma endregion
}