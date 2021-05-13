#pragma once
#include "HashMap.h"
#include <cassert>

namespace FieaGameEngine
{
    // ================== CONSTRUCTOR ==================
    template<typename TKey, typename TValue, typename HashFunctor, typename EqualityFunctor>
    inline HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::HashMap(size_t capacity)
        : m_buckets(capacity)
    {
        m_buckets.Resize(capacity);
    }

    // ================== SIZE ==================
    template<typename TKey, typename TValue, typename HashFunctor, typename EqualityFunctor>
    inline size_t HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::Size() const
    {
        return m_size;
    }

    // ================== CLEAR ==================
    template<typename TKey, typename TValue, typename HashFunctor, typename EqualityFunctor>
    inline void HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::Clear()
    {
        for (BucketIteratorType it = m_buckets.begin(); it != m_buckets.end(); ++it)
        {
            it->Clear();
        }

        m_size = 0;
    }

    // ================== INSERT ==================
    template<typename TKey, typename TValue, typename HashFunctor, typename EqualityFunctor>
    std::tuple<typename HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::Iterator, size_t> HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::Insert(const PairType& pair)
    {
        bool inserted = false;
        auto [chainIt, bucketIt] = FindIt(pair.first);

        // If the key isn't there, push this key to the end of the chain
        if (chainIt == bucketIt->end())
        {
            chainIt = bucketIt->PushBack(pair);
            ++m_size;
            inserted = true;
        }

        return std::make_tuple(Iterator(*this, bucketIt, chainIt), inserted);
    }

    // ================== [ ] OPERATOR ==================
    template<typename TKey, typename TValue, typename HashFunctor, typename EqualityFunctor>
    inline TValue& HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::operator[](const TKey& key)
    {
        auto [chainIt, bucketIt] = FindIt(key);

        // If we couldn't find the element, create an entry
        if (chainIt == bucketIt->end())
        {
            chainIt = bucketIt->PushBack(std::make_pair(key, TValue()));
            ++m_size;
        }

        return (*chainIt).second;
    }

    // ================== FIND ==================
    template<typename TKey, typename TValue, typename HashFunctor, typename EqualityFunctor>
    typename HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::Iterator HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::Find(const TKey& key)
    {
        auto [chainIt, bucketIt] = FindIt(key);

        // If we couldn't find the element, return end
        if (chainIt == bucketIt->end())
        {
            return end();
        }

        return Iterator(*this, bucketIt, chainIt);
    }

    // ================== FIND (CONST) ==================
    template<typename TKey, typename TValue, typename HashFunctor, typename EqualityFunctor>
    typename HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::ConstIterator HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::Find(const TKey& key) const
    {
        HashMap* hash = const_cast<HashMap*>(this);
        return ConstIterator(hash->Find(key));
    }

    // ================== REMOVE ==================
    template<typename TKey, typename TValue, typename HashFunctor, typename EqualityFunctor>
    inline bool HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::Remove(const TKey& key)
    {
        auto [chainIt, bucketIt] = FindIt(key);

        // We found the element and can remove it
        if (chainIt != bucketIt->end())
        {
            bucketIt->Remove(chainIt);
            --m_size;
            return true;
        }

        return false;
    }

    // ================== CONTAINS KEY ==================
    template<typename TKey, typename TValue, typename HashFunctor, typename EqualityFunctor>
    inline bool HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::ContainsKey(const TKey& key)
    {
        return (Find(key) != end());
    }

    // ================== AT ==================
    template<typename TKey, typename TValue, typename HashFunctor, typename EqualityFunctor>
    inline TValue& HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::At(const TKey& key)
    {
        auto it = Find(key);

        if (it == end())
        {
            throw std::runtime_error("Key did not exist in the hashmap!");
        }

        return (*it).second;
    }

    // ================== AT (CONST) ==================
    template<typename TKey, typename TValue, typename HashFunctor, typename EqualityFunctor>
    inline const TValue& HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::At(const TKey& key) const
    {
        ConstIterator it = (Find(key));

        if (it == end())
        {
            throw std::runtime_error("Key did not exist in the hashmap!");
        }

        return (*it).second;
    }

    // ================== BEGIN ==================
    template<typename TKey, typename TValue, typename HashFunctor, typename EqualityFunctor>
    typename HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::Iterator HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::begin()
    {
        if (m_size == 0)
        {
            return end();
        }

        return Iterator(*this, m_buckets.begin(), ChainIteratorType()).GoToNextFilledBucket();
    }

    // ================== BEGIN (CONST) ==================
    template<typename TKey, typename TValue, typename HashFunctor, typename EqualityFunctor>
    typename HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::ConstIterator HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::begin() const
    {
        return ConstIterator(*this, m_buckets.begin(), ChainIteratorType()).GoToNextFilledBucket();
    }

    // ================== CBEGIN ==================
    template<typename TKey, typename TValue, typename HashFunctor, typename EqualityFunctor>
    typename HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::ConstIterator HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::cbegin() const
    {
        return ConstIterator(*this, m_buckets.begin(), ChainIteratorType()).GoToNextFilledBucket();
    }

    // ================== END ==================
    template<typename TKey, typename TValue, typename HashFunctor, typename EqualityFunctor>
    typename HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::Iterator HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::end()
    {
        return Iterator(*this, m_buckets.end(), ChainIteratorType());
    }

    // ================== END (CONST) ==================
    template<typename TKey, typename TValue, typename HashFunctor, typename EqualityFunctor>
    typename HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::ConstIterator HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::end() const
    {
        return ConstIterator(*this, m_buckets.end(), ChainIteratorType());
    }

    // ================== CEND ==================
    template<typename TKey, typename TValue, typename HashFunctor, typename EqualityFunctor>
    typename HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::ConstIterator HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::cend() const
    {
        return ConstIterator(*this, m_buckets.end(), ChainIteratorType());
    }

    // ================== FINDIT ==================
    template<typename TKey, typename TValue, typename HashFunctor, typename EqualityFunctor>
    std::tuple<typename HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::ChainIteratorType, typename HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::BucketIteratorType> 
        HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::FindIt(const TKey& key)
    {
        HashFunctor hashFunc;
        size_t index = hashFunc(key) % m_buckets.Size();

        PairType pair = std::make_pair(key, TValue());

        BucketIteratorType bucketIt = m_buckets.begin() + index;
        ChainIteratorType chainIt = bucketIt->Find<KeyEquality>(pair);

        return std::make_tuple(chainIt, bucketIt);
    }

#pragma region Iterator
    // ================== CONSTRUCTOR ==================
    template<typename TKey, typename TValue, typename HashFunctor, typename EqualityFunctor>
    inline HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::Iterator::Iterator(HashMap& owner, const BucketIteratorType& bucketIterator,  const ChainIteratorType& chainIterator)
        : m_owner(&owner), m_bucketIterator(bucketIterator), m_chainIterator(chainIterator)
    {

    }

    // ================== GOTONEXTFILLEDBUCKET ==================
    template<typename TKey, typename TValue, typename HashFunctor, typename EqualityFunctor>
    typename HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::Iterator& HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::Iterator::GoToNextFilledBucket()
    {
        while (m_bucketIterator != m_owner->end().m_bucketIterator && m_bucketIterator->Size() == 0)
        {
            ++m_bucketIterator;
        }

        if (m_bucketIterator == m_owner->end().m_bucketIterator)
        {
            m_chainIterator = ChainIteratorType();
        }
        else
        {
            m_chainIterator = m_bucketIterator->begin();
        }
        
        return *this;
    }

    // ================== OPERATOR== ==================
    template<typename TKey, typename TValue, typename HashFunctor, typename EqualityFunctor>
    bool HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::Iterator::operator==(const Iterator& other) const
    {
        // TODO: check for diff owners

        bool bucketsEqual = (m_bucketIterator == other.m_bucketIterator);
        bool chainsEqual = (m_chainIterator == other.m_chainIterator);
        return bucketsEqual && chainsEqual;
    }

    // ================== OPERATOR!= ==================
    template<typename TKey, typename TValue, typename HashFunctor, typename EqualityFunctor>
    bool HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::Iterator::operator!=(const Iterator& other) const
    {
        return !(operator == (other));
    }

    // ================== ++OPERATOR ==================
    template<typename TKey, typename TValue, typename HashFunctor, typename EqualityFunctor>
    typename HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::Iterator& HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::Iterator::operator++()
    {
        if (m_owner == nullptr)
        {
            throw std::runtime_error("You should not increment an uninitialized Iterator!");
        }

        // If we are not at the end of our buckets
        if (m_bucketIterator != m_owner->end().m_bucketIterator)
        {
            // Move down the linked list
            ++m_chainIterator;

            // If we reached the end of the linked list
            if (m_chainIterator == m_bucketIterator->end())
            {

                ++m_bucketIterator;
                GoToNextFilledBucket();
            }
        }

        return *this;
    }

    // ================== OPERATOR++ ==================
    template<typename TKey, typename TValue, typename HashFunctor, typename EqualityFunctor>
    typename HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::Iterator HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::Iterator::operator++(int)
    {
        Iterator tempIterator = *this;
        ++(*this);
        return tempIterator;
    }

    // ================== OPERATOR* ==================
    template<typename TKey, typename TValue, typename HashFunctor, typename EqualityFunctor>
    typename HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::PairType& HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::Iterator::operator*() const
    {
        // TODO check if chain_iterator out of bounds? or maybe chain iterator will handle this

        return (*m_chainIterator);
    }

    // ================== OPERATOR-> ==================
    template<typename TKey, typename TValue, typename HashFunctor, typename EqualityFunctor>
    typename HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::PairType* HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::Iterator::operator->()
    {
        // TODO check if chain_iterator out of bounds? or maybe chain iterator will handle this

        return &(*m_chainIterator);
    }
    
#pragma endregion

#pragma region ConstIterator
    // ================== CONSTRUCTOR ==================
    template<typename TKey, typename TValue, typename HashFunctor, typename EqualityFunctor>
    inline HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::ConstIterator::ConstIterator(const Iterator& other)
        : m_owner(other.m_owner), m_bucketIterator(other.m_bucketIterator), m_chainIterator(other.m_chainIterator)
    {
    }

    // ================== CONSTRUCTOR ==================
    template<typename TKey, typename TValue, typename HashFunctor, typename EqualityFunctor>
    inline HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::ConstIterator::ConstIterator(const HashMap& owner, const ConstBucketIteratorType& bucketIterator, const ConstChainIteratorType& chainIterator)
        : m_owner(&owner), m_bucketIterator(bucketIterator), m_chainIterator(chainIterator)
    {
    }

    // ================== COPY ASSIGNMENT ==================
    template<typename TKey, typename TValue, typename HashFunctor, typename EqualityFunctor>
    typename HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::ConstIterator& HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::ConstIterator::operator=(const Iterator& other)
    {
        m_owner = other.m_owner;
        m_bucketIterator = other.m_bucketIterator;
        m_chainIterator = other.m_chainIterator;

        return *this;
    }

    // ================== GOTONEXTFILLEDBUCKET ==================
    template<typename TKey, typename TValue, typename HashFunctor, typename EqualityFunctor>
    typename HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::ConstIterator& HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::ConstIterator::GoToNextFilledBucket()
    {
        while (m_bucketIterator != m_owner->end().m_bucketIterator && m_bucketIterator->Size() == 0)
        {
            ++m_bucketIterator;
        }

        if (m_bucketIterator == m_owner->end().m_bucketIterator)
        {
            m_chainIterator = ConstChainIteratorType();
        }
        else
        {
            m_chainIterator = m_bucketIterator->begin();
        }

        return *this;
    }

    // ================== OPERATOR== ==================
    template<typename TKey, typename TValue, typename HashFunctor, typename EqualityFunctor>
    bool HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::ConstIterator::operator==(const ConstIterator& other) const
    {
        // TODO: check for diff owners

        bool bucketsEqual = (m_bucketIterator == other.m_bucketIterator);
        bool chainsEqual = (m_chainIterator == other.m_chainIterator);
        return bucketsEqual && chainsEqual;
    }

    // ================== OPERATOR!= ==================
    template<typename TKey, typename TValue, typename HashFunctor, typename EqualityFunctor>
    bool HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::ConstIterator::operator!=(const ConstIterator& other) const
    {
        return !(operator == (other));
    }

    // ================== ++OPERATOR ==================
    template<typename TKey, typename TValue, typename HashFunctor, typename EqualityFunctor>
    typename HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::ConstIterator& HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::ConstIterator::operator++()
    {
        if (m_owner == nullptr)
        {
            throw std::runtime_error("You should not increment an uninitialized Iterator!");
        }

        // If we are not at the end of our buckets
        if (m_bucketIterator != m_owner->end().m_bucketIterator)
        {
            // Move down the linked list
            ++m_chainIterator;

            // If we reached the end of the linked list
            if (m_chainIterator == m_bucketIterator->end())
            {
                ++m_bucketIterator;
                GoToNextFilledBucket();
            }
        }

        return *this;
    }

    // ================== OPERATOR++ ==================
    template<typename TKey, typename TValue, typename HashFunctor, typename EqualityFunctor>
    typename HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::ConstIterator HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::ConstIterator::operator++(int)
    {
        ConstIterator tempIterator = *this;
        ++(*this);
        return tempIterator;
    }

    // ================== OPERATOR* ==================
    template<typename TKey, typename TValue, typename HashFunctor, typename EqualityFunctor>
    const typename HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::PairType& HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::ConstIterator::operator*() const
    {
        return (*m_chainIterator);
    }
    
    // ================== OPERATOR-> ==================
    template<typename TKey, typename TValue, typename HashFunctor, typename EqualityFunctor>
    inline const typename HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::PairType* HashMap<TKey, TValue, HashFunctor, EqualityFunctor>::ConstIterator::operator->() const
    {
        return &(*m_chainIterator);
    }
#pragma endregion
}