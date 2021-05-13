#include "pch.h"
#include "JsonParseCoordinator.h"
#include "IJsonParseHelper.h"
#include <fstream>
#include <iostream>


namespace FieaGameEngine
{
    RTTI_DEFINITIONS(JsonParseCoordinator::SharedData);
    RTTI_DEFINITIONS(JsonParseCoordinator);

#pragma region Shared Data
    JsonParseCoordinator* JsonParseCoordinator::SharedData::GetJsonParseCoordinator()
    {
        return m_parseCoordinator;
    }

    void JsonParseCoordinator::SharedData::IncrementDepth()
    {
        ++m_depth;
    }

    void JsonParseCoordinator::SharedData::DecrementDepth()
    {
        --m_depth;
    }

    size_t JsonParseCoordinator::SharedData::Depth()
    {
        return m_depth;
    }

    void JsonParseCoordinator::SharedData::SetJsonParseCoordinator(JsonParseCoordinator* parseCoordinator)
    {
        m_parseCoordinator = parseCoordinator;
    }
#pragma endregion

#pragma region RuleOf6
    JsonParseCoordinator::JsonParseCoordinator(SharedData& sharedData)
    {
        m_sharedData = &sharedData;
        sharedData.SetJsonParseCoordinator(this);
    }

    JsonParseCoordinator::JsonParseCoordinator(JsonParseCoordinator&& other) noexcept
        : m_helpers(std::move(other.m_helpers)), m_sharedData(other.m_sharedData), m_filename(other.m_filename), m_isClone(other.m_isClone)
    {
        other.m_sharedData = nullptr;
        other.m_filename = "";
    }

    JsonParseCoordinator& JsonParseCoordinator::operator=(JsonParseCoordinator&& other) noexcept
    {
        m_helpers = std::move(other.m_helpers);
        m_sharedData = other.m_sharedData;
        m_filename = other.m_filename;
        m_isClone = other.m_isClone;

        other.m_sharedData = nullptr;
        other.m_filename = "";
        
        return *this;
    }

    JsonParseCoordinator::~JsonParseCoordinator()
    {
        // If this is a clone, it owns its helpers and shared data and must delete them
        if (m_isClone)
        {
            delete m_sharedData;
            
            for (IJsonParseHelper* helper : m_helpers)
            {
                delete helper;
            }

            m_helpers.Clear();
        }
    }

    JsonParseCoordinator* JsonParseCoordinator::Clone() const
    {
        SharedData* sharedDataCopy = m_sharedData->Create();
        JsonParseCoordinator* jpc = new JsonParseCoordinator(*sharedDataCopy);

        for (IJsonParseHelper* helper : m_helpers)
        {
            jpc->AddHelper(*(helper->Create()));
        }

        jpc->m_isClone = true;

        return jpc;
    }
#pragma endregion

#pragma region Parsing
    void JsonParseCoordinator::Parse(const std::string& input)
    {
        std::istringstream inputStream(input);
        Parse(inputStream);
    }

    void JsonParseCoordinator::Parse(std::istream& inputStream)
    {
        Initialize();
        Json::Value root;
        inputStream >> root;
        ParseNestedFiles(root);
        ParseValue(root, false);
    }

    void JsonParseCoordinator::ParseFromFile(const std::string& filename)
    {
        std::ifstream inputFile(filename, std::ifstream::binary);
        if (!inputFile.good())
        {
            throw std::exception("Failed to read file!");
        }
        Parse(inputFile);
        m_filename = filename;        
    }

    void JsonParseCoordinator::ParseValue(const Json::Value& jsonVal, bool isArray)
    { 
        if (jsonVal.size() > 0)
        {
            for (std::string& valueKey : jsonVal.getMemberNames())
            {
                ParseKeyValuePair(valueKey, jsonVal[valueKey], isArray);
            }
        }
    }

    void JsonParseCoordinator::ParseKeyValuePair(const std::string& key, const Json::Value& jsonVal, bool isArray)
    {
        if (jsonVal.isObject())
        {
            m_sharedData->IncrementDepth();

            for (IJsonParseHelper* const helper : m_helpers)
            {
                if (helper->StartHandler(*m_sharedData, key, jsonVal, isArray))
                {
                    ParseValue(jsonVal, isArray);
                    helper->EndHandler(*m_sharedData, key);
                    break;
                }
            }

            m_sharedData->DecrementDepth();
        }
        else if (jsonVal.isArray())
        {
            for (const Json::Value& element : jsonVal)
            {
                if (element.isObject())
                {
                    m_sharedData->IncrementDepth();
                    ParseValue(element, true);
                    m_sharedData->DecrementDepth();
                }
                else
                {
                    ParseKeyValuePair(key, element, true);
                }
            }
        }
        else
        {
            for (IJsonParseHelper* const helper : m_helpers)
            {
                if (helper->StartHandler(*m_sharedData, key, jsonVal, isArray))
                {
                    helper->EndHandler(*m_sharedData, key);
                    break;
                }
            }
        }
    }

    void JsonParseCoordinator::InsertNestedFile(Json::Value& context)
    {
        std::ifstream inputFile(context["file"].asString(), std::ifstream::binary);
        if (!inputFile.good())
        {
            throw std::exception("Failed to read file!");
        }
        Json::Value nestedJsonVal;
        inputFile >> nestedJsonVal;

        // Recursively fill this value in case it has its own nested files
        ParseNestedFiles(nestedJsonVal);

        context = nestedJsonVal;
    }
    
    void JsonParseCoordinator::ParseNestedFiles(Json::Value& jsonVal)
    {
        if (jsonVal.size() > 0)
        {
            for (std::string& valueKey : jsonVal.getMemberNames())
            {
                if (valueKey == "file")
                {
                    InsertNestedFile(jsonVal);
                }
                else if (jsonVal[valueKey].isObject())
                {
                    ParseNestedFiles(jsonVal[valueKey]);
                }
                else if (jsonVal[valueKey].isArray())
                {
                    for (Json::Value& element : jsonVal[valueKey])
                    {
                        if (element.isObject())
                        {
                            ParseNestedFiles(element);
                        }
                    }
                }
            }
        }


    }
#pragma endregion

    void JsonParseCoordinator::AddHelper(IJsonParseHelper& helper)
    {
        if (m_isClone)
        {
            throw std::exception("Trying to add a helper to a cloned coordinator!");
        }

        m_helpers.PushBack(&helper);
    }

    void JsonParseCoordinator::RemoveHelper(IJsonParseHelper& helper)
    {
        if (m_isClone)
        {
            throw std::exception("Trying to remove a helper from a cloned coordinator!");
        }

        m_helpers.Remove(&helper);
    }

    JsonParseCoordinator::SharedData* JsonParseCoordinator::GetSharedData() const
    {
        return m_sharedData;
    }

    void JsonParseCoordinator::SetSharedData(SharedData* sharedData)
    {
        m_sharedData = sharedData;
        sharedData->SetJsonParseCoordinator(this);
    }

    std::string JsonParseCoordinator::GetFileName() const
    {
        return m_filename;
    }

    void JsonParseCoordinator::Initialize()
    {
        for (IJsonParseHelper* helper : m_helpers)
        {
            helper->Initialize();
        }
    }
}