#include "pch.h"
#include "Scope.h"
#include "SList.h"

namespace FieaGameEngine
{
    RTTI_DEFINITIONS(Scope);

#pragma region RuleOf6
    Scope::Scope(size_t capacity)
        :m_map(capacity)
    {
    }

    Scope::Scope(const Scope& other)
    {
        for (PairType* ptr : other.m_order)
        {
            Datum& newDatum = Append(ptr->first);

            if (ptr->second.Type() == Datum::DatumType::Table)
            {
                newDatum.SetType(Datum::DatumType::Table);
                for (size_t i = 0; i < ptr->second.Size(); ++i)
                {
                    Scope* newScope = ptr->second.Get<Scope>(i).Clone();
                    Adopt(*newScope, ptr->first);
                }
            }
            else
            {
                newDatum = ptr->second;
            }
        }
    }

    Scope::Scope(Scope&& other) noexcept
        : m_map(std::move(other.m_map)), m_order(std::move(other.m_order)), m_parent(other.m_parent)
    {
        if (m_parent != nullptr)
        {
            auto [foundDatum, index] = m_parent->FindContainedScope(other);
            foundDatum->Set(this, index);
        }

        // Update the parent pointers of all our new children
        ForEachNestedScopeIn([this](const Scope&, Datum& datum, size_t index)
            {
                datum.Get<Scope>(index).m_parent = this;
                return false;
            });

        other.m_parent = nullptr;
    }

    Scope& Scope::operator=(const Scope& other)
    {
        Clear();
        m_map.Clear();
        m_order.Clear();

        for (PairType* ptr : other.m_order)
        {
            Datum& newDatum = Append(ptr->first);
            if (ptr->second.Type() == Datum::DatumType::Table)
            {
                newDatum.SetType(Datum::DatumType::Table);
                for (size_t i = 0; i < ptr->second.Size(); ++i)
                {
                    Scope* newScope = ptr->second.Get<Scope>(i).Clone(); 
                    Adopt(*newScope, ptr->first);
                }
            }
            else
            {
                newDatum = ptr->second;
            }
        }

        return *this;
    }

    Scope& Scope::operator=(Scope&& other) noexcept
    {
        // Delete all of our children before we override them
        Clear();

        // If we have a parent, orphan ourselves to remove that link
        if (m_parent != nullptr)
        {
            Orphan();
        }

        // Steal the data from the other scope
        m_map = std::move(other.m_map);
        m_order = std::move(other.m_order);
        m_parent = other.m_parent;

        // If the other scope had a parent, replace it's reference with a pointer to this
        if (m_parent != nullptr)
        {
            auto [foundDatum, index] = m_parent->FindContainedScope(other);
            foundDatum->Set(this, index);
        }

        // Update the parent pointers of all our new children
        ForEachNestedScopeIn([this](const Scope&, Datum& datum, size_t index)
            {
                datum.Get<Scope>(index).m_parent = this;
                return false;
            });
        
        other.m_parent = nullptr;

        return *this;
    }

    Scope::~Scope()
    {
        Clear();
    }
#pragma endregion

    bool Scope::Equals(const RTTI* rhs) const
    {
        if (rhs == nullptr)
        {
            return false;
        }

        const Scope* otherScope = rhs->As<const Scope>();

        // If the passed in RTTI pointer isn't a Scope
        if (otherScope == nullptr)
        {
            return false;
        }

        // If the two scopes are different sizes
        if (Size() != otherScope->Size())
        {
            return false;
        }

        for (size_t i = 0; i < Size(); ++i)
        {
            if ((*m_order[i]).first == "this")
            {
                continue;
            }

            if ((*m_order[i]) != (*otherScope->m_order[i]))
            {
                return false;
            }
        }

        return true;
    }

    bool Scope::operator==(const Scope& other) const
    {
        return Equals(&other);
    }

    bool Scope::operator!=(const Scope& other) const
    {
        return !(operator == (other));
    }

    Datum& Scope::operator[](size_t index)
    {
        return m_order[index]->second;
    }

    Datum& Scope::operator[](const std::string key)
    {
        return Append(key);
    }

    Datum& Scope::Append(const std::string& key)
    {
        auto [it, wasInserted] = AppendHelper(key);
        return it->second;
    }

    Scope& Scope::AppendScope(const std::string& key)
    {
        auto [it, wasInserted] = AppendHelper(key);
        if (!wasInserted)
        {
            if (it->second.Type() != Datum::DatumType::Table)
            {
                throw std::runtime_error("Trying to AppendScope to a Datum whose type isn't table!");
            }
        }

        Scope* newScope = new Scope();
        it->second.SetType(Datum::DatumType::Table);
        it->second.PushBack(*newScope);
        newScope->m_parent = this;

        return *newScope;
    }

    void Scope::Adopt(Scope& child, const std::string& key)
    {
        // Guard against self-adoption
        if (this == &child)
        {
            throw std::runtime_error("Trying to adopt yourself!");
        }

        // Guard against adopting ancestor/descendant
        if (IsDescendantOf(child))
        {
            throw std::runtime_error("Trying to adopt your ancestor or descendant!");
        }

        auto [it, wasInserted] = AppendHelper(key);
        if (!wasInserted)
        {
            if (it->second.Type() != Datum::DatumType::Table)
            {
                throw std::runtime_error("Trying to AppendScope to a Datum whose type isn't table!");
            }
        }

        // If the child already has a parent, orphan it first
        if (child.m_parent != nullptr)
        {
            child.Orphan();
        }

        it->second.SetType(Datum::DatumType::Table);
        it->second.PushBack(child);
        child.m_parent = this;
    }

    void Scope::Orphan()
    {
        if (m_parent != nullptr)
        {
            auto [containedDatum, index] = m_parent->FindContainedScope(*this);
            containedDatum->RemoveAt(index);

            m_parent = nullptr;
        }
    }

    Scope* Scope::GetParent()
    {
        return m_parent;
    }

    const Scope* Scope::GetParent() const
    {
        return m_parent;
    }

    bool Scope::IsDescendantOf(const Scope& potentialAncestor) const
    {
        bool isParentOf = (m_parent == &potentialAncestor);
        if (!isParentOf && m_parent != nullptr)
        {
            return m_parent->IsDescendantOf(potentialAncestor);
        }

        return isParentOf;
    }

    bool Scope::IsAncestorOf(const Scope& potentialDescendant) const
    {
        return potentialDescendant.IsDescendantOf(*this);
    }

    size_t Scope::Size() const
    {
        return m_order.Size();
    }

    Datum* Scope::Find(const std::string& key)
    {
        Datum* value = nullptr;
        MapIteratorType it = m_map.Find(key);

        if (it != m_map.end())
        {
            value = &(it->second);
        }

        return value;
    }

    const Datum* Scope::Find(const std::string& key) const
    {
        const Datum* value = nullptr;
        ConstMapIteratorType it = m_map.Find(key);

        if (it != m_map.cend())
        {
            value = &(it->second);
        }

        return value;
    }

    std::tuple<Datum*, size_t> Scope::FindContainedScope(const Scope& scopeToFind)
    {
        Datum* foundDatum = nullptr;
        size_t foundIndex = 0;

        ForEachNestedScopeIn([&scopeToFind, &foundIndex, &foundDatum](const Scope&, Datum& datum, size_t index)
            {
                if (&(datum.Get<Scope>(index)) == &scopeToFind)
                {
                    foundDatum = &datum;
                    foundIndex = index;
                    return true;
                }

                return false;
            });

        return std::make_tuple(foundDatum, foundIndex);
    }

    std::tuple<Datum*, Scope*> Scope::Search(const std::string& key)
    {
        Datum* foundDatum = Find(key);
        if (foundDatum == nullptr && m_parent != nullptr)
        {
            return m_parent->Search(key);
        }

        return std::make_tuple(foundDatum, this);
    }

    std::tuple<const Datum*, const Scope*> Scope::Search(const std::string& key) const
    {
        const Datum* foundDatum = Find(key);
        if (foundDatum == nullptr && m_parent != nullptr)
        {
            return m_parent->Search(key);
        }

        return std::make_tuple(foundDatum, this);
    }

    void Scope::Clear()
    {
        ForEachNestedScopeIn([](const Scope&, Datum& datum, size_t index)
            {
                delete datum.m_data.sc[index];
                return false;
            });

        m_map.Clear();
        m_order.Clear();
    }

    gsl::owner<Scope*> Scope::Clone() const
    {
        return new Scope(*this);
    }

    void Scope::ForEachNestedScopeIn(NestedScopeFunction func) const
    {
        for (const auto& pair : m_order)
        {
            Datum& datum = pair->second;
            if (datum.Type() == Datum::DatumType::Table)
            {
                assert(!datum.m_externalStorage);

                for (size_t i = 0; i < datum.Size(); ++i)
                {
                    if (func(*this, datum, i))
                    {
                        return;
                    }
                }
            }
        }
    }

    std::tuple<Scope::MapIteratorType, bool> Scope::AppendHelper(const std::string& key)
    {
        if (key == "")
        {
            throw std::runtime_error("Trying to AppendScope with an empty string!");
        }

        PairType newPair = std::make_pair(key, Datum());
        auto [it, wasInserted] = m_map.Insert(newPair);
        if (wasInserted)
        {
            m_order.PushBack(&(*it));
        }

        return std::make_tuple(it, wasInserted);
    }
}