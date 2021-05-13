#pragma once
#include <initializer_list>
#include "DefaultEquality.h"
#include "DefaultIncrement.h"

namespace FieaGameEngine
{
    /// <summary>
    /// Template Vector class that holds elements and supports dynamically scaling in size. 
    /// </summary>
    /// <typeparam name="T"> Required typeparameter that defines the type of elements this
    /// Vector stores </typeparam>
    template <typename T>
    class Vector
    {
    public:
        using value_type = T;

        #pragma region Iterator
        /// <summary>
        /// Iterator class that assists in stepping through the elements in an Vector
        /// </summary>
        class Iterator final
        {
            friend Vector;
            friend class ConstIterator;

        public:
            using size_type = std::size_t;
            using difference_type = std::ptrdiff_t;
            using value_type = T;
            using reference = T;
            using pointer = T*;
            using iterator_category = std::random_access_iterator_tag;

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
            /// Compares two iterators to see if their owner Vector and index are the same
            /// </summary>
            /// <param name="other"> The other Iterator to compare to </param>
            /// <returns> True if the Iterators are equivalent, false otherwise </returns>
            bool operator==(const Iterator& other) const;

            /// <summary>
            /// Compares two iterators to see if either their owner Vector or index are different
            /// </summary>
            /// <param name="other"> The other Iterator to compare to </param>
            /// <returns> True if the Iterators aren't equivalent, false otherwise </returns>
            bool operator!=(const Iterator& other) const;

            /// <summary>
            /// Increments the Iterator so that it points to the next element. Has no effect if the Iterator
            /// is pointing to the end of list.
            /// </summary>
            /// <returns> A reference to this Iterator </returns>
            /// <exception cref="std::runtime_error"> Throws if this Iterator doesn't have an owner </exception>
            Iterator& operator++();

            /// <summary>
            /// Increments the Iterator so that it points to the next element. Has no effect if the Iterator
            /// is pointing to the end of list.
            /// </summary>
            /// <returns> A copy of the Iterator that hasn't been incremented </returns>
            /// <exception cref="std::runtime_error"> Throws if this Iterator doesn't have an owner </exception>
            Iterator operator++(int);

            /// <summary>
            /// Decrements the Iterator so that it points to the previous element. Has no effect if the Iterator
            /// is pointing to the beginning of the list
            /// </summary>
            /// <returns> A reference to this Iterator </returns>
            /// <exception cref="std::runtime_error"> Throws if this Iterator doesn't have an owner </exception>
            Iterator& operator--();

            /// <summary>
            /// Decrements the Iterator so that it points to the previous element. Has no effect if the Iterator
            /// is pointing to the beginning of the list
            /// </summary>
            /// <returns> A copy of the Iterator that hasn't been decremented </returns>
            /// <exception cref="std::runtime_error"> Throws if this Iterator doesn't have an owner </exception>
            Iterator operator--(int);

            /// <summary>
            /// Attempts to add the given value to the Iterator. If the current index plus the value exceed the
            /// size of the Vector, the Iterator returned will be the end of the Vector.
            /// </summary>
            /// <param name="value"> The value to add </param>
            /// <returns> A copy of the Iterator with value added </returns>
            /// <exception cref="std::runtime_error"> Throws if this Iterator doesn't have an owner </exception>
            Iterator operator+(size_t value) const;

            /// <summary>
            /// Attempts to subtract the given value from the Iterator. If the current index minus the value are
            /// less than 0, the Iterator returned will be the beginning of the Vector.
            /// </summary>
            /// <param name="value"> The value to subtract </param>
            /// <returns> A copy of the Iterator with value subtracted </returns>
            /// <exception cref="std::runtime_error"> Throws if this Iterator doesn't have an owner </exception>
            Iterator operator-(size_t value) const;

            std::ptrdiff_t operator-(const Iterator& other) const;

            /// <summary>
            /// Attempts to add the given value to the Iterator and set this Iterator to that value. If the current 
            /// index plus the value exceed the size of the Vector, the Iterator will be set to the end of the Vector.
            /// </summary>
            /// <param name="value"> The value to add </param>
            /// <returns> A reference to this Iterator </returns>
            /// <exception cref="std::runtime_error"> Throws if this Iterator doesn't have an owner </exception>
            Iterator& operator+=(size_t value);

            /// <summary>
            /// Attempts to subtract the given value from the Iterator and set this Iterator to that value. If the current 
            /// index minus the value are less than 0, the Iterator will be set to the beginning of the Vector.
            /// </summary>
            /// <param name="value"> The value to subtract </param>
            /// <returns> A reference to this Iterator </returns>
            /// <exception cref="std::runtime_error"> Throws if this Iterator doesn't have an owner </exception>
            Iterator& operator-=(size_t value);

            /// <summary>
            /// Attempts to see if this Iterator has a smaller index than the provided Iterator
            /// </summary>
            /// <param name="other"> The Iterator to compare this one to </param>
            /// <returns> True if this Iterator has a smaller index, false otherwise </returns>
            /// <exception cref="std::runtime_error"> Throws if either Iterator doesn't have an owner or
            /// if their owners aren't the same </exception>
            bool operator<(const Iterator & other) const;

            /// <summary>
            /// Attempts to see if this Iterator has a bigger index than the provided Iterator
            /// </summary>
            /// <param name="other"> The Iterator to compare this one to </param>
            /// <returns> True if this Iterator has a bigger index, false otherwise </returns>
            /// <exception cref="std::runtime_error"> Throws if either Iterator doesn't have an owner or
            /// if their owners aren't the same </exception>
            bool operator>(const Iterator & other) const;

            /// <summary>
            /// Attempts to deference this Iterator and return the value it is pointing to.
            /// </summary>
            /// <returns> A reference to the element this Iterator is pointing to </returns>
            /// <exception cref="std::runtime_error"> Throws if this Iterator doesn't have an owner or if
            /// the Iterator is pointing out of bounds of the Vector </exception>
            T& operator*() const;

            /// <summary>
            /// Attempts to obtain the element at index in the Vector that owns this iterator
            /// </summary>
            /// <param name="index"> The index of the element to get </param>
            /// <returns> A reference to the element at index </returns>
            /// <exception cref="std::runtime_error"> Throws if the index is out of bounds or if this iterator
            /// does not have an owner </exception>
            T& operator[](size_t index) const;

            /// <summary>
            /// Attempts to dereference this Iterator and return a pointer to what it is pointing to.
            /// </summary>
            /// <returns> A pointer to the element this Iterator is pointing to </returns>
            /// <exception cref="std::runtime_error"> Throws if this Iterator doesn't have an owner or if
            /// the Iterator is pointing out of bounds of the Vector </exception>
            T* operator->();

        private:
            Iterator(Vector& owner, size_t index) : m_owner(&owner), m_index(index) {};

            /// <summary>
            /// A pointer to the Vector that owns this Iterator
            /// </summary>
            Vector* m_owner = nullptr;

            /// <summary>
            /// The current index of the Vector that this Iterator is pointing to. This should never exceed the
            /// size of the Vector
            /// </summary>
            size_t m_index = 0;
        };
        #pragma endregion Iterator

        #pragma region ConstIterator
        /// <summary>
        /// Iterator class that assists in stepping through the elements in an Vector without
        /// giving the user the ability to modify the list in any way
        /// </summary>
        class ConstIterator final
        {
            friend Vector;

        public:
            using size_type = std::size_t;
            using difference_type = std::ptrdiff_t;
            using value_type = T;
            using reference = T;
            using pointer = T*;
            using iterator_category = std::random_access_iterator_tag;

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
            /// Default assignment operator
            /// </summary>
            /// <param name="other"> The other ConstIterator to assign this one to </param>
            /// <returns> A reference to this newly assigned ConstIterator </returns>
            ConstIterator& operator=(const ConstIterator& other) = default;

            /// <summary>
            /// Assignment operator that assigns the value of an Iterator to this ConstIterator
            /// </summary>
            /// <param name="other"> The Iterator to assign this ConstIterator to</param>
            /// <returns> A reference to the newly assigned ConstIterator </returns>
            ConstIterator& operator=(const Iterator& other);

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
            /// Compares two ConstIterators to see if their owner Vector and index are the same
            /// </summary>
            /// <param name="other"> The other ConstIterator to compare to </param>
            /// <returns> True if the ConstIterators are equivalent, false otherwise </returns>
            bool operator==(const ConstIterator& other) const;

            /// <summary>
            /// Compares two ConstIterators to see if either their owner Vector or index are different
            /// </summary>
            /// <param name="other"> The other ConstIterator to compare to </param>
            /// <returns> True if the ConstIterators aren't equivalent, false otherwise </returns>
            bool operator!=(const ConstIterator& other) const;

            /// <summary>
            /// Increments the ConstIterator so that it points to the next element. Has no effect if the ConstIterator
            /// is pointing to the end of list.
            /// </summary>
            /// <returns> A reference to this ConstIterator </returns>
            /// <exception cref="std::runtime_error"> Throws if this ConstIterator doesn't have an owner </exception>
            ConstIterator& operator++();

            /// <summary>
            /// Increments the ConstIterator so that it points to the next element. Has no effect if the ConstIterator
            /// is pointing to the end of list.
            /// </summary>
            /// <returns> A copy of the ConstIterator that hasn't been incremented </returns>
            /// <exception cref="std::runtime_error"> Throws if this ConstIterator doesn't have an owner </exception>
            ConstIterator operator++(int);

            /// <summary>
            /// Decrements the ConstIterator so that it points to the previous element. Has no effect if the ConstIterator
            /// is pointing to the beginning of the list
            /// </summary>
            /// <returns> A reference to this ConstIterator </returns>
            /// <exception cref="std::runtime_error"> Throws if this ConstIterator doesn't have an owner </exception>
            ConstIterator& operator--();

            /// <summary>
            /// Decrements the ConstIterator so that it points to the previous element. Has no effect if the ConstIterator
            /// is pointing to the beginning of the list
            /// </summary>
            /// <returns> A copy of the ConstIterator that hasn't been decremented </returns>
            /// <exception cref="std::runtime_error"> Throws if this ConstIterator doesn't have an owner </exception>
            ConstIterator operator--(int);

            /// <summary>
            /// Attempts to add the given value to the ConstIterator. If the current index plus the value exceed the
            /// size of the Vector, the ConstIterator returned will be the end of the Vector.
            /// </summary>
            /// <param name="value"> The value to add </param>
            /// <returns> A copy of the ConstIterator with value added </returns>
            /// <exception cref="std::runtime_error"> Throws if this ConstIterator doesn't have an owner </exception>
            ConstIterator operator+(size_t value) const;

            /// <summary>
            /// Attempts to subtract the given value from the ConstIterator. If the current index minus the value are
            /// less than 0, the ConstIterator returned will be the beginning of the Vector.
            /// </summary>
            /// <param name="value"> The value to subtract </param>
            /// <returns> A copy of the ConstIterator with value subtracted </returns>
            /// <exception cref="std::runtime_error"> Throws if this ConstIterator doesn't have an owner </exception>
            ConstIterator operator-(size_t value) const;

            /// <summary>
            /// Attempts to add the given value to the ConstIterator and set this ConstIterator to that value. If the current 
            /// index plus the value exceed the size of the Vector, the ConstIterator will be set to the end of the Vector.
            /// </summary>
            /// <param name="value"> The value to add </param>
            /// <returns> A reference to this ConstIterator </returns>
            /// <exception cref="std::runtime_error"> Throws if this ConstIterator doesn't have an owner </exception>
            ConstIterator& operator+=(size_t value);

            /// <summary>
            /// Attempts to subtract the given value from the ConstIterator and set this ConstIterator to that value. If the current 
            /// index minus the value are less than 0, the ConstIterator will be set to the beginning of the Vector.
            /// </summary>
            /// <param name="value"> The value to subtract </param>
            /// <returns> A reference to this ConstIterator </returns>
            /// <exception cref="std::runtime_error"> Throws if this ConstIterator doesn't have an owner </exception>
            ConstIterator& operator-=(size_t value);

            /// <summary>
            /// Attempts to see if this ConstIterator has a smaller index than the provided ConstIterator
            /// </summary>
            /// <param name="other"> The ConstIterator to compare this one to </param>
            /// <returns> True if this ConstIterator has a smaller index, false otherwise </returns>
            /// <exception cref="std::runtime_error"> Throws if either ConstIterator doesn't have an owner or
            /// if their owners aren't the same </exception>
            bool operator<(const ConstIterator & other) const;

            /// <summary>
            /// Attempts to see if this ConstIterator has a bigger index than the provided ConstIterator
            /// </summary>
            /// <param name="other"> The ConstIterator to compare this one to </param>
            /// <returns> True if this ConstIterator has a bigger index, false otherwise </returns>
            /// <exception cref="std::runtime_error"> Throws if either ConstIterator doesn't have an owner or
            /// if their owners aren't the same </exception>
            bool operator>(const ConstIterator & other) const;

            /// <summary>
            /// Attempts to deference this ConstIterator and return the value it is pointing to.
            /// </summary>
            /// <returns> A const reference to the element this ConstIterator is pointing to </returns>
            /// <exception cref="std::runtime_error"> Throws if this ConstIterator doesn't have an owner or if
            /// the ConstIterator is pointing out of bounds of the Vector </exception>
            const T& operator*() const;

            /// <summary>
            /// Attempts to obtain the element at index in the Vector that owns this ConstIterator
            /// </summary>
            /// <param name="index"> The index of the element to get </param>
            /// <returns> A reference to the element at index </returns>
            /// <exception cref="std::runtime_error"> Throws if the index is out of bounds or if this ConstIterator
            /// does not have an owner </exception>
            const T& operator[](size_t index) const;

            /// <summary>
            /// Attempts to dereference this ConstIterator and return a pointer to what it is pointing to.
            /// </summary>
            /// <returns> A pointer to the element this ConstIterator is pointing to </returns>
            /// <exception cref="std::runtime_error"> Throws if this ConstIterator doesn't have an owner or if
            /// the ConstIterator is pointing out of bounds of the Vector </exception>
            const T* operator->();

        private:
            ConstIterator(const Vector& owner, size_t index) : m_owner(&owner), m_index(index) {};

            /// <summary>
            /// A pointer to the Vector that owns this ConstIterator
            /// </summary>
            const Vector* m_owner = nullptr;

            /// <summary>
            /// The current index of the Vector that this ConstIterator is pointing to. This should never exceed the
            /// size of the Vector
            /// </summary>
            size_t m_index = 0;
        };
        #pragma endregion ConstIterator

        /// <summary>
        /// Constructs an empty Vector
        /// </summary>
        /// <param name="capacity"> Optional parameter to give the Vector an initial capacity</param>
        Vector(size_t capacity = 0);

        /// <summary>
        /// Constructs a Vector with a copy of the contents of other
        /// </summary>
        /// <param name="other"> Another vector to use as a data source </param>
        Vector(const Vector& other);

        /// <summary>
        /// Copies the contents of other into this Vector. All data currently stored in this Vector will be destroyed.
        /// </summary>
        /// <param name="other"> Another Vector to use as a data source </param>
        /// <returns> A reference to this Vector </returns>
        Vector& operator=(const Vector& other);

        /// <summary>
        /// Constructs a Vector with the contents of other using move semantics
        /// </summary>
        /// <param name="other"> Another Vector to use as a data source </param>
        Vector(Vector&& other) noexcept;

        /// <summary>
        /// Replaces the contents with those of other using move semantics. All data currently in the list will be destroyed.
        /// </summary>
        /// <param name="other"> Another Vector to use as a data source </param>
        /// <returns> A reference to this Vector </returns>
        Vector& operator=(Vector&& other) noexcept;

        /// <summary>
        /// Constructs a Vector using the given initializer list
        /// </summary>
        /// <param name="list"> The initializer list to construct the Vector with </param>
        Vector(std::initializer_list<T> list);
        
        /// <summary>
        /// Deletes all of the elements in the Vector and clears all allocated memory
        /// </summary>
        ~Vector();

        /// <summary>
        /// Accesses the element at the provided index and returns a reference to the element
        /// </summary>
        /// <param name="index"> The index of the element to retrieve </param>
        /// <returns> A reference to the element </returns>
        /// <exception cref="std::runtime_error"> Throws if the index is out of bounds </exception>
        T& operator[](size_t index);

        /// <summary>
        /// Accesses the element at the provided index and returns a const reference to the element
        /// </summary>
        /// <param name="index"> The index of the element to retrieve </param>
        /// <returns> A const reference to the element </returns>
        /// <exception cref="std::runtime_error"> Throws if the index is out of bounds </exception>
        const T& operator[](size_t index) const;

        /// <summary>
        /// Accesses the element at the provided index and returns a reference to the element
        /// </summary>
        /// <param name="index"> The index of the element to retrieve </param>
        /// <returns> A reference to the element </returns>
        /// <exception cref="std::runtime_error"> Throws if the index is out of bounds </exception>
        T& At(size_t index);

        /// <summary>
        /// Accesses the element at the provided index and returns a const reference to the element
        /// </summary>
        /// <param name="index"> The index of the element to retrieve </param>
        /// <returns> A const reference to the element </returns>
        /// <exception cref="std::runtime_error"> Throws if the index is out of bounds </exception>
        const T& At(size_t index) const;

        /// <summary>
        /// Returns the first element in the Vector
        /// </summary>
        /// <returns> A reference to the first element in the list </returns>
        /// <exception cref="std::runtime_error"> Throws if the Vector is empty </exception>
        T& Front();

        /// <summary>
        /// Returns the first element in the Vector
        /// </summary>
        /// <returns> A reference to the first element in the list </returns>
        /// <exception cref="std::runtime_error"> Throws if the Vector is empty </exception>
        const T& Front() const;

        /// <summary>
        /// Returns the last element of the Vector
        /// </summary>
        /// <returns> A reference to the last element in the Vector </returns>
        /// <exception cref="std::runtime_error"> Throws if the Vector is empty </exception>
        T& Back();

        /// <summary>
        /// Returns the last element of the Vector
        /// </summary>
        /// <returns> A reference to the last element in the Vector </returns>
        /// <exception cref="std::runtime_error"> Throws if the Vector is empty </exception>
        const T& Back() const;

        /// <summary>
        /// Used to obtain an Iterator pointing to the first element in the Vector
        /// </summary>
        /// <returns> An Iterator pointing to the first element in the Vector </returns>
        Iterator begin();

        /// <summary>
        /// Used to obtain a ConstIterator pointing to the first element in the Vector
        /// </summary>
        /// <returns> A ConstIterator pointing to the first element in the Vector </returns>
        ConstIterator begin() const;

        /// <summary>
        /// Used to obtain a ConstIterator pointing to the first element in the Vector
        /// </summary>
        /// <returns> A ConstIterator pointing to the first element in the Vector </returns>
        ConstIterator cbegin() const;

        /// <summary>
        /// Used to obtain an Iterator pointing to the last element in the Vector
        /// </summary>
        /// <returns> An Iterator pointing to the last element in the Vector </returns>
        Iterator end();

        /// <summary>
        /// Used to obtain a ConstIterator pointing to the last element in the Vector
        /// </summary>
        /// <returns> A ConstIterator pointing to the last element in the Vector </returns>
        ConstIterator end() const;

        /// <summary>
        /// Used to obtain a ConstIterator pointing to the last element in the Vector
        /// </summary>
        /// <returns> A ConstIterator pointing to the last element in the Vector </returns>
        ConstIterator cend() const;

        /// <summary>
        /// Adds an element to the end of the list. This will allocate more space if there is none left
        /// </summary>
        /// <typeparam name="IncrementFunctor"> Optional type parameter that defines how much space
        /// this function will allocate if there's no room for the new element (The functor should return
        /// the value to set the capacity to, not how much new space to allocate). Default is to space for
        /// the one element that is being pushed on. </typeparam>
        /// <param name="value"> The element to add to the Vector </param>
        /// <returns> An Iterator pointing to the newly added element </returns>
        /// <exception cref="std::runtime_error"> Throws if the provided IncrementFunctor fails to
        /// allocate enough new space for the element </exception>
        template <typename IncrementFunctor = DefaultIncrement>
        Iterator PushBack(const T& value);

        /// <summary>
        /// Adds an element to the end of the list using move semantics. This will allocate more space 
        /// if there is none left
        /// </summary>
        /// <typeparam name="IncrementFunctor"> Optional type parameter that defines how much space
        /// this function will allocate if there's no room for the new element (The functor should return
        /// the value to set the capacity to, not how much new space to allocate). Default is to space for
        /// the one element that is being pushed on. </typeparam>
        /// <param name="value"> The element to add to the Vector </param>
        /// <returns> An Iterator pointing to the newly added element </returns>
        /// <exception cref="std::runtime_error"> Throws if the provided IncrementFunctor fails to
        /// allocate enough new space for the element </exception>
        template <typename IncrementFunctor = DefaultIncrement>
        Iterator PushBack(T&& value);

        /// <summary>
        /// Deletes the last element of the Vector. This does not affect the capacity at all.
        /// </summary>
        void PopBack();

        /// <summary>
        /// Sets the capacity to the given value and reserves space for that many elements. 
        /// Has no effect if the given capacity is less than or equal to the current capacity.
        /// </summary>
        /// <param name="capacity"> The desired capacity </param>
        void Reserve(size_t capacity);

        /// <summary>
        /// Shrinks the Vector's capacity so that it is the same as the size. If the capacity
        /// equals the size, this has no effect.
        /// </summary>
        void ShrinkToFit();

        void Resize(size_t size);

        /// <summary>
        /// Gets the current capacity of the Vector. This is not the number of elements but rather
        /// the amount of space allocated for this Vector.
        /// </summary>
        /// <returns> The current capacity of the Vector </returns>
        size_t Capacity() const;

        /// <summary>
        /// Gets the current size of the Vector 
        /// </summary>
        /// <returns> The current size of the Vector </returns>
        size_t Size() const;

        /// <summary>
        /// Returns whether or not this Vector has no elements (size is 0)
        /// </summary>
        /// <returns> True if the Vector is empty, false otherwise </returns>
        bool IsEmpty() const;

        /// <summary>
        /// Clears the Vector of all elements, destroying them and setting the size to 0. This does not
        /// effect the capacity.
        /// </summary>
        void Clear();

        /// <summary>
        /// Attempts to find the given element in the Vector
        /// </summary>
        /// <typeparam name="EqualityFunctor"> Optional type parameter that defines how this
        /// function handles checking for equality </typeparam>
        /// <param name="value"> The value to search for </param>
        /// <returns> An Iterator pointing to the first found element. If the element could not be
        /// located, returns an Iterator pointing past the end of the Vector </returns>
        template <typename EqualityFunctor = DefaultEquality<T>>
        Iterator Find(const T& value);

        /// <summary>
        /// Attempts to find the given element in the Vector
        /// </summary>
        /// <typeparam name="EqualityFunctor"> Optional type parameter that defines how this
        /// function handles checking for equality </typeparam>
        /// <param name="value"> The value to search for </param>
        /// <returns> A ConstIterator pointing to the first found element. If the element could not be
        /// located, returns an ConstIterator pointing past the end of the Vector </returns>
        template <typename EqualityFunctor = DefaultEquality<T>>
        ConstIterator Find(const T& value) const;

        /// <summary>
        /// Attempts to remove the given element from the Vector
        /// </summary>
        /// <typeparam name="EqualityFunctor"> Optional type parameter that defines how this
        /// function handles checking for equality </typeparam>
        /// <param name="value"> The value to attempt to remove </param>
        /// <returns> True if an element was successfully removed, false otherwise </returns>
        template <typename EqualityFunctor = DefaultEquality<T>>
        bool Remove(const T& value);

        /// <summary>
        /// Attempts to remove an element pointed to by the given iterator. If the iterator is out of bounds,
        /// nothing will happen.
        /// </summary>
        /// <param name="it"> The Iterator pointing to the element to remove </param>
        /// <returns> True if the element was successfully removed, false otherwise </returns>
        bool Remove(const Iterator& it);

        /// <summary>
        /// Attempts to remove the elements between the startIterator (inclusive) and the endIterator (exclusive)
        /// </summary>
        /// <param name="startIterator"> An Iterator pointing at the start of the elements to remove </param>
        /// <param name="endIterator"> An Iterator pointing at the end of the elements to remove</param>
        /// <returns> True if the elements were successfully removed, false otherwise </returns>
        /// <exception cref="std::runtime_error"> Throws if the provided start Iterator points to a later
        /// position in the list than the end Iterator </exception>
        bool Remove(const Iterator& startIterator, const Iterator& endIterator);

    private:
        /// <summary>
        /// The underlying data array of the Vector
        /// </summary>
        T* m_data = nullptr;

        /// <summary>
        /// The current size (number of actual elements) of the Vector
        /// </summary>
        size_t m_size = 0;

        /// <summary>
        /// The current capacity (amount of space reserved) for the Vector
        /// </summary>
        size_t m_capacity = 0;
    };
    
}

#include "Vector.inl"