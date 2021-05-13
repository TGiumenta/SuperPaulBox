#pragma once
#include "DefaultEquality.h"


namespace FieaGameEngine
{
    /// <summary>
    /// SList is a templated linked list class that supports pushing/popping from both the head
    /// and the tail.
    /// </summary>
    template <typename T>
    class SList
    {
    private:
        /// <summary>
        /// Node class used internally by the linked list as containers for the data
        /// </summary>
        struct Node final
        {
            Node(const T& data, Node* next = nullptr) : m_data(data), m_nextNode(next) {};
            T m_data;
            Node* m_nextNode;
        };

    public:
        #pragma region Iterator
        /// <summary>
        /// Iterator class that assists in stepping through the elements in an SList
        /// </summary>
        class Iterator final
        {
            friend SList;
            friend class ConstIterator;

        public:
            Iterator() = default;
            Iterator(const Iterator& other) = default;
            Iterator(Iterator&& other) noexcept = default;
            Iterator& operator=(const Iterator& other) = default;
            Iterator& operator=(Iterator&& other) noexcept = default;
            ~Iterator() = default;

            bool operator==(const Iterator& other) const;
            bool operator!=(const Iterator& other) const;

            Iterator& operator++();
            Iterator operator++(int);

            T& operator*() const;

        private:
            Iterator(const SList& owner, Node* node) : m_owner(&owner), m_nodePtr(node) {};

            const SList* m_owner = nullptr;
            Node* m_nodePtr = nullptr;
        };
        #pragma endregion Iterator

        #pragma region ConstIterator
        /// <summary>
        /// Iterator class that assists in stepping through the elements in an SList without
        /// giving the user the ability to modify the list in any way
        /// </summary>
        class ConstIterator final
        {
            friend SList;

        public:
            ConstIterator() = default;
            ConstIterator(const Iterator& other);
            ConstIterator(const ConstIterator& other) = default;
            ConstIterator(ConstIterator&& other) noexcept = default;
            ConstIterator& operator=(const ConstIterator& other) = default;
            ConstIterator& operator=(const Iterator& other);
            ConstIterator& operator=(ConstIterator&& other) noexcept = default;
            ~ConstIterator() = default;

            bool operator==(const ConstIterator& other) const;
            bool operator!=(const ConstIterator& other) const;

            ConstIterator& operator++();
            ConstIterator operator++(int);

            const T& operator*() const;

        private:
            ConstIterator(const SList& owner, const Node* node) : m_owner(&owner), m_nodePtr(node) {};

            const SList* m_owner = nullptr;
            const Node* m_nodePtr = nullptr;
        };
        #pragma endregion ConstIterator


        SList() = default;
        ~SList();

        /// <summary>
        /// Constructs the list with a copy of the contents of other
        /// </summary>
        /// <param name="other"> Another list to use as a data source </param>
        SList(const SList& other);

        /// <summary>
        /// Copies the contents of other into this list. All data currently stored in this list is detroyed.
        /// </summary>
        /// <param name="other"> Another list to use as a data source </param>
        /// <returns></returns>
        SList& operator=(const SList& other);

        /// <summary>
        /// Constructs the list with the contents of other using move semantics
        /// </summary>
        /// <param name="other"> Another list to use as a data source </param>
        SList(SList&& other) noexcept;

        /// <summary>
        /// Replaces the contents with those of other using move semantics
        /// </summary>
        /// <param name="other"> Another list to use as a data source </param>
        /// <returns> The newly modified list </returns>
        SList& operator=(SList&& other) noexcept;

        /// <summary>
        /// Adds an element to the front of the list. This element becomes the new head
        /// </summary>
        /// <param name="value"> The element to add to the list </param>
        /// <returns> An Iterator to the newly added element </returns>
        Iterator PushFront(const T& value);

        /// <summary>
        /// Adds an element to the end of the list. This element becomes the new tail
        /// </summary>
        /// <param name="value"> The element to add to the list </param>
        /// <returns> An Iterator to the newly added element </returns>
        Iterator PushBack(const T& value);

        /// <summary>
        /// Deletes the first element of the list. The next element becomes the new head
        /// </summary>
        void PopFront();

        /// <summary>
        /// Deletes the last element of the list. The previous element becomes the new tail
        /// </summary>
        void PopBack();

        /// <summary>
        /// Returns the first element of the list
        /// </summary>
        /// <returns> The value of the first element in the list </returns>
        /// <exception cref="std::runtime_error"> Throws if list is empty </exception>
        T& Front();

        /// <summary>
        /// Returns the first element of the list
        /// </summary>
        /// <returns> The value of the first element in the list </returns>
        /// <exception cref="std::runtime_error"> Throws if list is empty </exception>
        const T& Front() const;

        /// <summary>
        /// Returns the last element of the list
        /// </summary>
        /// <returns> The value of the last element in the list </returns>
        /// <exception cref="std::runtime_error"> Throws if list is empty </exception>
        T& Back();

        /// <summary>
        /// Returns the last element of the list
        /// </summary>
        /// <returns> The value of the last element in the list </returns>
        /// <exception cref="std::runtime_error"> Throws if list is empty </exception>
        const T& Back() const;

        /// <summary>
        /// Returns the current size of the list
        /// </summary>
        /// <returns> An int representing the number of elements in the list </returns>
        size_t Size() const;

        /// <summary>
        /// Returns whether or not the list contains no elements
        /// </summary>
        /// <returns> True if the list is empty, false otherwise </returns>
        bool IsEmpty() const;

        /// <summary>
        /// Clears the list of all of it's elements, resetting it's size to 0
        /// </summary>
        void Clear();

        /// <summary>
        /// Used to obtain an Iterator pointing to the first element in the list
        /// </summary>
        /// <returns> An Iterator pointing to the first element in the list </returns>
        Iterator begin();

        /// <summary>
        /// Used to obtain a ConstIterator pointing to the first element in the list
        /// </summary>
        /// <returns> A ConstIterator pointing to the first element in the list </returns>
        ConstIterator begin() const;

        /// <summary>
        /// Used to obtain a ConstIterator pointing to the first element in the list
        /// </summary>
        /// <returns> A ConstIterator pointing to the first element in the list </returns>
        ConstIterator cbegin() const;

        /// <summary>
        /// Used to obtain an Iterator pointing to the last element in the list
        /// </summary>
        /// <returns> An Iterator pointing to the last element in the list </returns>
        Iterator end();

        /// <summary>
        /// Used to obtain a ConstIterator pointing to the last element in the list
        /// </summary>
        /// <returns> A ConstIterator pointing to the last element in the list </returns>
        ConstIterator end() const;

        /// <summary>
        /// Used to obtain a ConstIterator pointing to the last element in the list
        /// </summary>
        /// <returns> A ConstIterator pointing to the last element in the list </returns>
        ConstIterator cend() const;

        /// <summary>
        /// Inserts the given value into the list immediately after the given iterator
        /// </summary>
        /// <param name="it"> The iterator pointing to an element. Value will be inserted after this
        /// iterator's element </param>
        /// <param name="value"> The element to insert into the list </param>
        /// <returns> An Iterator to the newly added element </returns>
        Iterator InsertAfter(const Iterator& it, const T& value);

        /// <summary>
        /// Attempts to find and return the given element in the list
        /// </summary>
        /// <typeparam name="EqualityFunctor"> Optional type parameter that defines how this
        /// function handles checking for equality </typeparam>
        /// <param name="value"> The value of the element to search for in the list </param>
        /// <returns> A ConstIterator pointing to the found element. If the element cannot be
        /// found, returns a ConstIterator pointing past the end of the list. </returns>
        template <typename EqualityFunctor = DefaultEquality<T>>
        ConstIterator Find(const T& value) const;

        /// <summary>
        /// Attempts to find and return the given element in the list
        /// </summary>
        /// <typeparam name="EqualityFunctor"> Optional type parameter that defines how this
        /// function handles checking for equality </typeparam>
        /// <param name="value"> The value of the element to search for in the list </param>
        /// <returns> An Iterator pointing to the found element. If the element cannot be
        /// found, returns an Iterator pointing past the end of the list. </returns>
        template <typename EqualityFunctor = DefaultEquality<T>>
        Iterator Find(const T& value);

        /// <summary>
        /// Attempts to remove an element with the given value from the list. If the value
        /// cannot be found in the list, nothing will happen. If more than one element of value
        /// exists in the list, only the first occurance will be removed.
        /// </summary>
        /// <typeparam name="EqualityFunctor"> Optional type parameter that defines how this
        /// function handles checking for equality </typeparam>
        /// <param name="value"> The value to attempt to remove from the list </param>
        template <typename EqualityFunctor = DefaultEquality<T>>
        void Remove(const T& value);

        /// <summary>
        /// Attempts to remove an element pointed to by the given iterator. If the iterator is pointing past
        /// the end of the list, nothing will happen.
        /// </summary>
        /// <param name="it"> The iterator pointing to the element to remove </param>
        /// <exception cref="std::runtime_error"> Throws if the iterator does not have an owner
        /// or the iterator is owned by a different container </exception>
        void Remove(const Iterator& it);


    private:
        /// <summary>
        /// A pointer to the Node object that is the head of the linked list
        /// </summary>
        Node* m_head = nullptr;

        /// <summary>
        /// A pointer to the Node object that is the tail of the linked list
        /// </summary>
        Node* m_tail = nullptr;

        /// <summary>
        /// The current size of the list
        /// </summary>
        size_t m_size = 0;
    };
    
}

#include "SList.inl"