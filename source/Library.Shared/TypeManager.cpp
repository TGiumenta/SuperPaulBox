#include "pch.h"
#include "TypeManager.h"


namespace FieaGameEngine
{
    HashMap<RTTI::IdType, TypeManager::TypeInfo> TypeManager::m_types;

    const Vector<Signature> TypeManager::GetSignatures(RTTI::IdType typeId)
    {
        auto it = m_types.Find(typeId);

        if (it == m_types.end())
        {
            throw std::exception("Trying to get signatures for a type that isn't registered!");
        }

        Vector<Signature> signatures;

        // If our parent type is in the type manager, recursively get all of the inherited members first
        if (ContainsType(it->second.m_parentType))
        {
            signatures = GetSignatures(it->second.m_parentType);
        }

        // Now append all of our signatures to that list 
        for (Signature& signature : it->second.m_signatures)
        {
            signatures.PushBack(signature);
        }

        return signatures;
    }

    bool TypeManager::ContainsType(RTTI::IdType typeId)
    {
        return m_types.ContainsKey(typeId);
    }

    bool TypeManager::AddType(RTTI::IdType typeId, const Vector<Signature>& signatures, RTTI::IdType parentType)
    {
        if (typeId == parentType)
        {
            throw std::exception("Trying to add a type whose parent type is itself!");
        }

        auto [it, wasInserted] = m_types.Insert(std::make_pair(typeId, TypeInfo()));

        if (wasInserted)
        {
            it->second.m_signatures = signatures;
            it->second.m_parentType = parentType;
        }

        return wasInserted;
    }

    bool TypeManager::RemoveType(RTTI::IdType typeId)
    {
        return m_types.Remove(typeId);
    }

    void TypeManager::Clear()
    {
        if (Size() > 0)
        {
            m_types.Clear();
        } 
    }

    size_t TypeManager::Size()
    {
        return m_types.Size();
    }
}