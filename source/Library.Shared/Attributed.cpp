#include "pch.h"
#include "Attributed.h"
#include "TypeManager.h"
#include "RTTI.h"

namespace FieaGameEngine
{
    RTTI_DEFINITIONS(Attributed);

    Attributed::Attributed(const Attributed& other)
        : Scope(other)
    {
        UpdateExternalStorage(other);
    }

    Attributed::Attributed(Attributed&& other) noexcept
        : Scope(std::move(other))
    {
        UpdateExternalStorage(other);
    }

    Attributed& Attributed::operator=(const Attributed& other)
    {
        Scope::operator=(other);
        UpdateExternalStorage(other);
        return *this;
    }

    Attributed& Attributed::operator=(Attributed&& other) noexcept
    {
        Scope::operator=(std::move(other));
        UpdateExternalStorage(other);
        return *this;
    }

    Attributed::Attributed(RTTI::IdType type)
    {
        (*this)["this"] = this;
        PopulateScope(type);
    }

    bool Attributed::IsAttribute(const std::string& key) const
    {
        return (Find(key) != nullptr);
    }

    bool Attributed::IsPrescribedAttribute(const std::string& key) const
    {
        if (key == "this")
        {
            return true;
        }

        Vector<Signature> signatures = TypeManager::GetSignatures(TypeIdInstance());
        for (Signature& signature : signatures)
        {
            if (key == signature.m_name)
            {
                return true;
            }
        }

        return false;
    }

    bool Attributed::IsAuxiliaryAttribute(const std::string& key) const
    {
        return (IsAttribute(key) && !IsPrescribedAttribute(key));
    }

    Datum& Attributed::AppendAuxiliaryAttribute(const std::string& key)
    {
        if (IsPrescribedAttribute(key))
        {
            throw std::exception("Cannot append an auxiliary attribute into a prescribed attribute!");
        }

        return Append(key);
    }

    const Vector<Scope::PairType*>& Attributed::GetAttributes() const
    {
        return m_order;
    }

    Vector<Scope::PairType*> Attributed::GetPrescribedAttributes() const
    {
        Vector<Scope::PairType*> prescribedAttributes;
        for (PairType* ptr : m_order)
        {
            if (IsPrescribedAttribute(ptr->first))
            {
                prescribedAttributes.PushBack(ptr);
            }
        }

        return prescribedAttributes;
    }

    Vector<Scope::PairType*> Attributed::GetAuxiliaryAttributes() const
    {
        Vector<Scope::PairType*> auxiliaryAttributes;
        for (PairType* ptr : m_order)
        {
            if (!IsPrescribedAttribute(ptr->first))
            {
                auxiliaryAttributes.PushBack(ptr);
            }
        }

        return auxiliaryAttributes;
    }

    void Attributed::PopulateScope(RTTI::IdType type)
    {
        // Go through all the signatures for this type and add them to our scope structure
        Vector<Signature> signatures = TypeManager::GetSignatures(type);
        for (Signature& signature : signatures)
        {
            Datum& appendedDatum = Append(signature.m_name);

            if (signature.m_type == Datum::DatumType::Table)
            {
                appendedDatum.SetType(signature.m_type);
                for (size_t i = 0; i < signature.m_size; ++i)
                {
                    AppendScope(signature.m_name);
                } 
            }
            else
            {
                void* data = reinterpret_cast<std::uint8_t*>(this) + signature.m_storageOffset;
                appendedDatum.SetStorage(data, signature.m_size);
            }
        }
    }

    void Attributed::UpdateExternalStorage(const Attributed& other)
    {
        (*this)["this"] = this;

        Vector<Signature> signatures = TypeManager::GetSignatures(other.TypeIdInstance());
        for (Signature& signature : signatures)
        {
            if (signature.m_type != Datum::DatumType::Table)
            {
                Datum* foundDatum = Find(signature.m_name);
                void* data = reinterpret_cast<std::uint8_t*>(this) + signature.m_storageOffset;
                foundDatum->m_data.vp = data;
            }
        }
    }
}