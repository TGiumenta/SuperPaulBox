#pragma once
#include "Factory.h"

namespace FieaGameEngine
{
    template<typename T>
    inline gsl::owner<T*> Factory<T>::Create(const std::string& className)
    {
        auto it = m_factories.Find(className);

        if (it == m_factories.end())
        {
            return nullptr;
        }

        return it->second->Create();
    }

    template<typename T>
    inline size_t Factory<T>::Size()
    {
        return m_factories.Size();
    }

    template<typename T>
    inline bool Factory<T>::IsEmpty()
    {
        return (Size() == 0);
    }

    template<typename T>
    inline const Factory<T>* const Factory<T>::Find(const std::string& className)
    {
        auto it = m_factories.Find(className);

        return (it == m_factories.end()) ? nullptr : it->second;
    }

    template<typename T>
    inline void Factory<T>::Add(const Factory& factory)
    {
        auto [it, wasInserted] = m_factories.Insert(std::make_pair(factory.ClassName(), &factory));

        if (!wasInserted)
        {
            throw std::exception("Trying to add a factory whose name already exists!");
        }
    }

    template<typename T>
    inline void Factory<T>::Remove(const Factory& factory)
    {
        m_factories.Remove(factory.ClassName());
    }


}