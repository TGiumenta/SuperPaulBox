#pragma once
#include "DefaultHash.h"

namespace FieaGameEngine
{

    const size_t HashPrime = 33;

    inline size_t AdditiveHash(const std::uint8_t* data, size_t size)
    {
        size_t hash = 0;
        for (size_t i = 0; i < size; ++i)
        {
            hash += HashPrime * data[i];
        }

        return hash;
    }

    template<typename TKey>
    inline size_t DefaultHash<TKey>::operator()(const TKey& value)
    {
        const std::uint8_t* data = reinterpret_cast<const std::uint8_t*>(&value);

        return AdditiveHash(data, sizeof(TKey));
    }

    template<>
    struct DefaultHash<const char*>
    {
        inline size_t operator()(const char* key) const
        {
            const std::uint8_t* data = reinterpret_cast<const std::uint8_t*>(key);
            return AdditiveHash(data, strlen(key));
        }
    };

    template<>
    struct DefaultHash<std::string>
    {
        inline size_t operator()(const std::string& key) const
        {
            const std::uint8_t* data = reinterpret_cast<const std::uint8_t*>(key.c_str());
            return AdditiveHash(data, key.length());
        }
    };

    template<>
    struct DefaultHash<const std::string>
    {
        inline size_t operator()(const std::string& key) const
        {
            const std::uint8_t* data = reinterpret_cast<const std::uint8_t*>(key.c_str());
            return AdditiveHash(data, key.length());
        }
    };

    template<>
    struct DefaultHash<int>
    {
        inline size_t operator()(int key) const
        {
            return key;
        }
    };

}