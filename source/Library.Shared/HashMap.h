#pragma once

#include "DefaultEquality.h"
#include "DefaultHash.h"
#include "Vector.h"
#include "SList.h"
#include <utility>

namespace FieaGameEngine
{
    /// <summary>
    /// Template HashMap class that maps TKeys to TValues and offers best case constant time lookups
    /// </summary>
    /// <typeparam name="TKey"> Required type parameter that defines the type of the keys stored in the HashMap </typeparam>
    /// <typeparam name="TValue"> Required type parameter that defines the type of the values store in the HashMap </typeparam>
    /// <typeparam name="HashFunctor"> Optional type parameter that defines what hash function is used to hash keys </typeparam>
    /// <typeparam name="EqualityFunctor"> Optional type parameter that defines how determine equality for keys </typeparam>
    template<typename TKey, typename TValue, typename HashFunctor = DefaultHash<TKey>, typename EqualityFunctor = DefaultEquality<TKey>>
    class HashMap
    {
    public:
        using PairType = std::pair<const TKey, TValue>;
        using value_type = PairType;

        using ChainType = SList<PairType>;
        using BucketType = Vector<ChainType>;

        using ChainIteratorType = typename ChainType::Iterator;
        using ConstChainIteratorType = typename ChainType::ConstIterator;

        using BucketIteratorType = typename BucketType::Iterator;
        using ConstBucketIteratorType = typename BucketType::ConstIterator;

        /// <summary>
        /// Custom Equality functor that compares two pairs based only on their first value
        /// </summary>
        struct KeyEquality final
        {
            size_t operator()(const PairType& lhs, const PairType& rhs) const
            {
                EqualityFunctor eq;
                return eq(lhs.first, rhs.first);
            }
        };

        #pragma region Iterator
        /// <summary>
        /// Iterator class that assists in stepping through the elements in an HashMap
        /// </summary>
        class Iterator final
        {
            friend HashMap;
        public:

            /// <summary>
            /// Default constructor
            /// </summary>
            Iterator() = default;

            /// <summary>
            /// Default copy constructor
            /// </summary>
            /// <param name="other"> The other Iterator to construct this one from </param>
            Iterator(const Iterator& other) = default;

            /// <summary>
            /// Default move constructor
            /// </summary>
            /// <param name="other"> The other Iterator to construct this one from </param>
            Iterator(Iterator&& other) noexcept = default;

            /// <summary>
            /// Default assignment operator
            /// </summary>
            /// <param name="other"> The other Iterator to assign this one to </param>
            /// <returns> A reference to this newly assigned Iterator </returns>
            Iterator& operator=(const Iterator& other) = default;

            /// <summary>
            /// Default move assignment operator
            /// </summary>
            /// <param name="other"> The other Iterator to assign this one to </param>
            Iterator& operator=(Iterator&& other) noexcept = default;

            /// <summary>
            /// Default destructor
            /// </summary>
            ~Iterator() = default;

            /// <summary>
            /// Compares two iterators to see if their owner HashMap is the same and that they point to
            /// the same key-value pair
            /// </summary>
            /// <param name="other"> The other Iterator to compare to </param>
            /// <returns> True if the Iterators are equivalent, false otherwise </returns>
            bool operator==(const Iterator& other) const;

            /// <summary>
            /// Compares two iterators to see if their owner HashMap is the different or that they point to
            /// the different key-value pairs
            /// </summary>
            /// <param name="other"> The other Iterator to compare to </param>
            /// <returns> True if the Iterators are different, false otherwise </returns>
            bool operator!=(const Iterator& other) const;

            /// <summary>
            /// Increments the Iterator so that it points to the next element. Has no effect if the Iterator
            /// is pointing to the end of the HashMap
            /// </summary>
            /// <returns> A reference to this Iterator </returns>
            /// <exception cref="std::runtime_error"> Throws if this Iterator doesn't have an owner </exception>
            Iterator& operator++();

            /// <summary>
            /// Increments the Iterator so that it points to the next element. Has no effect if the Iterator
            /// is pointing to the end of the HashMap.
            /// </summary>
            /// <returns> A copy of the Iterator that hasn't been incremented </returns>
            /// <exception cref="std::runtime_error"> Throws if this Iterator doesn't have an owner </exception>
            Iterator operator++(int);

            /// <summary>
            /// Attempts to deference this Iterator and return the key-value pair it is pointing to.
            /// </summary>
            /// <returns> A reference to the element this Iterator is pointing to </returns>
            /// <exception cref="std::runtime_error"> Throws if this Iterator doesn't have an owner or if
            /// the Iterator is pointing to end() </exception>
            PairType& operator*() const;

            /// <summary>
            /// Attempts to dereference this Iterator and return a pointer to what it is pointing to.
            /// </summary>
            /// <returns> A pointer to the element this Iterator is pointing to </returns>
            /// <exception cref="std::runtime_error"> Throws if this Iterator doesn't have an owner or if
            /// the Iterator is pointing out of bounds of the Vector </exception>
            PairType* operator->();

        private:
            Iterator(HashMap& owner, const BucketIteratorType& bucketIterator, const ChainIteratorType& chainIterator);

            /// <summary>
            /// Moves this iterator to the next empty bucket if it's not already pointing to a non-empty bucket
            /// </summary>
            /// <returns></returns>
            Iterator& GoToNextFilledBucket();

            /// <summary>
            /// The HashMap that owns this Iterator
            /// </summary>
            HashMap* m_owner = nullptr;

            /// <summary>
            /// Inner bucket iterator that defines where in the internal vector we are
            /// </summary>
            BucketIteratorType m_bucketIterator;

            /// <summary>
            /// Inner chain iterator that defines where in the current bucket's chain we are
            /// </summary>
            ChainIteratorType m_chainIterator;
        };
        #pragma endregion

        #pragma region ConstIterator
        /// <summary>
        /// Iterator class that assists in stepping through the elements in an HashMap without
        /// giving the user the ability to modify the HashMap in any way
        /// </summary>
        class ConstIterator final
        {
            friend HashMap;
        public:

            /// <summary>
            /// Default constructor
            /// </summary>
            ConstIterator() = default;

            /// <summary>
            /// Constructor that builds a ConstIterator from an Iterator
            /// </summary>
            /// <param name="other"> The Iterator to build this ConstIterator from </param>
            ConstIterator(const Iterator& other);

            /// <summary>
            /// Default copy constructor
            /// </summary>
            /// <param name="other"> The other ConstIterator to construct this one from </param>
            ConstIterator(const ConstIterator& other) = default;

            /// <summary>
            /// Default move constructor
            /// </summary>
            /// <param name="other"> The other ConstIterator to construct this one from </param>
            ConstIterator(ConstIterator&& other) noexcept = default;

            /// <summary>
            /// Assignment operator that assigns the value of an Iterator to this ConstIterator
            /// </summary>
            /// <param name="other"> The Iterator to assign this ConstIterator to</param>
            /// <returns> A reference to the newly assigned ConstIterator </returns>
            ConstIterator& operator=(const Iterator& other);

            /// <summary>
            /// Default assignment operator
            /// </summary>
            /// <param name="other"> The other ConstIterator to assign this one to </param>
            /// <returns> A reference to this newly assigned ConstIterator </returns>
            ConstIterator& operator=(const ConstIterator& other) = default;

            /// <summary>
            /// Default move assignment operator
            /// </summary>
            /// <param name="other"> The other ConstIterator to assign this one to </param>
            ConstIterator& operator=(ConstIterator&& other) noexcept = default;

            /// <summary>
            /// Default destructor
            /// </summary>
            ~ConstIterator() = default;

            /// <summary>
            /// Compares two ConstIterators to see if their owner HashMap is the same and that they point to
            /// the same key-value pair
            /// </summary>
            /// <param name="other"> The other ConstIterator to compare to </param>
            /// <returns> True if the ConstIterators are equivalent, false otherwise </returns>
            bool operator==(const ConstIterator& other) const;

            /// <summary>
            /// Compares two ConstIterators to see if their owner HashMap is the different or that they point to
            /// the different key-value pairs
            /// </summary>
            /// <param name="other"> The other ConstIterator to compare to </param>
            /// <returns> True if the ConstIterators are different, false otherwise </returns>
            bool operator!=(const ConstIterator& other) const;

            /// <summary>
            /// Increments the ConstIterator so that it points to the next element. Has no effect if the ConstIterator
            /// is pointing to the end of the HashMap
            /// </summary>
            /// <returns> A reference to this ConstIterator </returns>
            /// <exception cref="std::runtime_error"> Throws if this ConstIterator doesn't have an owner </exception>
            ConstIterator& operator++();

            /// <summary>
            /// Increments the ConstIterator so that it points to the next element. Has no effect if the ConstIterator
            /// is pointing to the end of the HashMap.
            /// </summary>
            /// <returns> A copy of the ConstIterator that hasn't been incremented </returns>
            /// <exception cref="std::runtime_error"> Throws if this ConstIterator doesn't have an owner </exception>
            ConstIterator operator++(int);

            /// <summary>
            /// Attempts to deference this ConstIterator and return the key-value pair it is pointing to.
            /// </summary>
            /// <returns> A const reference to the element this ConstIterator is pointing to </returns>
            /// <exception cref="std::runtime_error"> Throws if this ConstIterator doesn't have an owner or if
            /// the ConstIterator is pointing to end() </exception>
            const PairType& operator*() const;

            /// <summary>
            /// Attempts to dereference this ConstIterator and return a pointer to what it is pointing to.
            /// </summary>
            /// <returns> A pointer to the element this ConstIterator is pointing to </returns>
            /// <exception cref="std::runtime_error"> Throws if this ConstIterator doesn't have an owner or if
            /// the ConstIterator is pointing out of bounds of the Vector </exception>
            const PairType* operator->() const;

        private:
            ConstIterator(const HashMap& owner, const ConstBucketIteratorType& bucketIterator, const ConstChainIteratorType& chainIterator);

            /// <summary>
            /// Moves this ConstIterator to the next empty bucket if it's not already pointing to a non-empty bucket
            /// </summary>
            /// <returns></returns>
            ConstIterator& GoToNextFilledBucket();

            /// <summary>
            /// The HashMap that owns this ConstIterator
            /// </summary>
            const HashMap* m_owner = nullptr;

            /// <summary>
            /// Inner bucket iterator that defines where in the internal vector we are
            /// </summary>
            ConstBucketIteratorType m_bucketIterator;

            /// <summary>
            /// Inner chain iterator that defines where in the current bucket's chain we are
            /// </summary>
            ConstChainIteratorType m_chainIterator;
        };
        #pragma endregion

        /// <summary>
        /// Constructor that creates the hashmap with the given capacity, filling the capacity with empty chains
        /// </summary>
        /// <param name="capacity"> Optional parameter to set the capacity (default 11) </param>
        explicit HashMap(size_t capacity = 11);

        /// <summary>
        /// Default copy constructor
        /// </summary>
        /// <param name="other"> The other HashMap to construct this one from </param>
        HashMap(const HashMap& other) = default;

        /// <summary>
        /// Default move constructor
        /// </summary>
        /// <param name="other"> The other HashMap to construct this one from </param>
        HashMap(HashMap&& other) noexcept = default;

        /// <summary>
        /// Default assignment operator
        /// </summary>
        /// <param name="other"> The other HashMap to assign this one to </param>
        HashMap& operator=(const HashMap& other) = default;

        /// <summary>
        /// Default move assignment operator
        /// </summary>
        /// <param name="other"> The other HashMap to assign this one to</param>
        HashMap& operator=(HashMap&& other) noexcept = default;

        /// <summary>
        /// Default destructor
        /// </summary>
        ~HashMap() = default;

        /// <summary>
        /// Returns the current size of the HashMap. This is how many key-value pairs reside in the 
        /// HashMap, not how many "buckets" it has.
        /// </summary>
        /// <returns> The current size of the HashMap </returns>
        size_t Size() const;

        /// <summary>
        /// Removes all key-value pairs from the HashMap.
        /// </summary>
        void Clear();

        /// <summary>
        /// Attempts to insert the given pair into the HashMap. If the key is found to already be in the 
        /// HashMap, it will be unaffected and an Iterator pointing to that pair will be returned.
        /// </summary>
        /// <param name="pair"> The pair to attempt to insert </param>
        /// <returns> A tuple whose first element is an Iterator pointing to either the newly inserted pair
        /// or the pair that was found. The second element of the tuple is a bool that will be true if the 
        /// pair was inserted, false otherwise </returns>
        std::tuple<Iterator, size_t> Insert(const PairType& pair);

        /// <summary>
        /// Attempts to return the value of the pair stored at the given key. If the key isn't found, a new 
        /// key-value pair will be inserted with the default constructed value. 
        /// </summary>
        /// <param name="key"> The key to find the value of (or insert if not found) </param>
        /// <returns> The a reference to the value of the key-value pair found or inserted </returns>
        TValue& operator[](const TKey& key);

        /// <summary>
        /// Tries to find the given key in the HashMap.
        /// </summary>
        /// <param name="key"> The key to try to find </param>
        /// <returns> An Iterator pointing to the found key-value pair. If the key wasn't found, the
        /// Iterator will point to end() </returns>
        Iterator Find(const TKey& key);

        /// <summary>
        /// Tries to find the given key in the HashMap.
        /// </summary>
        /// <param name="key"> The key to try to find </param>
        /// <returns> A ConstIterator pointing to the found key-value pair. If the key wasn't found, the
        /// Iterator will point to end() </returns>
        ConstIterator Find(const TKey& key) const;

        /// <summary>
        /// Attempts to remove the given key from the HashMap.
        /// </summary>
        /// <param name="key"> The key to attempt to remove </param>
        /// <returns> True if the key was found and could be removed, false if the key wasn't in
        /// the HashMap </returns>
        bool Remove(const TKey& key);

        /// <summary>
        /// Checks if the provided key is in the HashMap
        /// </summary>
        /// <param name="key"> The key to look for </param>
        /// <returns> True if the key is in the HashMap, false otherwise </returns>
        bool ContainsKey(const TKey& key);

        /// <summary>
        /// Gets the value at the given key
        /// </summary>
        /// <param name="key"> The key to get the value of </param>
        /// <returns> A reference to the value associated with the key </returns>
        /// <exception cref="std::runtime_error"> Throws if this key is not in the HashMap </exception>
        TValue& At(const TKey& key);

        /// <summary>
        /// Gets the value at the given key
        /// </summary>
        /// <param name="key"> The key to get the value of </param>
        /// <returns> A const reference to the value associated with the key </returns>
        /// <exception cref="std::runtime_error"> Throws if this key is not in the HashMap </exception>
        const TValue& At(const TKey& key) const;

        /// <summary>
        /// Gets an Iterator pointing to the first key-value pair in the HashMap. This is not necessarily
        /// the first key-value pair that was inserted.
        /// </summary>
        /// <returns> An Iterator pointing to the first key-value pair in the HashMap </returns>
        Iterator begin();

        /// <summary>
        /// Gets a ConstIterator pointing to the first key-value pair in the HashMap. This is not necessarily
        /// the first key-value pair that was inserted.
        /// </summary>
        /// <returns> A ConstIterator pointing to the first key-value pair in the HashMap </returns>
        ConstIterator begin() const;

        /// <summary>
        /// Gets a ConstIterator pointing to the first key-value pair in the HashMap. This is not necessarily
        /// the first key-value pair that was inserted.
        /// </summary>
        /// <returns> A ConstIterator pointing to the first key-value pair in the HashMap </returns>
        ConstIterator cbegin() const;

        /// <summary>
        /// Gets an Iterator pointing to the past the end of the HashMap
        /// </summary>
        /// <returns> An Iterator pointing to the past the end of the HashMap </returns>
        Iterator end();

        /// <summary>
        /// Gets a ConstIterator pointing to the past the end of the HashMap
        /// </summary>
        /// <returns> An ConstIterator pointing to the past the end of the HashMap </returns>
        ConstIterator end() const;

        /// <summary>
        /// Gets a ConstIterator pointing to the past the end of the HashMap
        /// </summary>
        /// <returns> An ConstIterator pointing to the past the end of the HashMap </returns>
        ConstIterator cend() const;

    private:
        /// <summary>
        /// Helper function that tries to find a key in the HashMap
        /// </summary>
        /// <param name="key"> The key to find </param>
        /// <returns> A tuple whose first element is a ChainIterator and second element is
        /// a BucketIterator, both of which point to the found key. They will point to end() 
        /// if it wasn't found. </returns>
        std::tuple<ChainIteratorType, BucketIteratorType> FindIt(const TKey& key);

        /// <summary>
        /// Internal Vector used as "buckets". Each bucket contains an SList
        /// </summary>
        BucketType m_buckets;

        /// <summary>
        /// The current number of key-value pairs in the HashMap
        /// </summary>
        size_t m_size = 0;
    };  

}

#include "HashMap.inl"