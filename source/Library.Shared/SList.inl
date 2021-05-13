#include "SList.h"
#include <stdexcept>

namespace FieaGameEngine
{

#pragma region SList
    template<typename T>
    inline SList<T>::SList(const SList& other)
    {
        if (!other.IsEmpty())
        {
            Node* otherPtr = other.m_head;

            while (otherPtr != nullptr)
            {
                PushBack(otherPtr->m_data);
                otherPtr = otherPtr->m_nextNode;
            }
        }
    }

    template<typename T>
    inline SList<T>::~SList()
    {
        Clear();
    }

    template<typename T>
    inline SList<T>& SList<T>::operator=(const SList& other)
    {
        if (this != &other)
        {
            Clear();

            if (!other.IsEmpty())
            {
                Node* otherPtr = other.m_head;

                while (otherPtr != nullptr)
                {
                    PushBack(otherPtr->m_data);
                    otherPtr = otherPtr->m_nextNode;
                }
            }
        }
      
        return *this;
    }

    template<typename T>
    inline SList<T>::SList(SList&& other) noexcept
        : m_head(other.m_head), m_tail(other.m_tail), m_size(other.m_size)
    {
        other.m_head = nullptr;
        other.m_tail = nullptr;
        other.m_size = 0;
    }

    template<typename T>
    inline SList<T>& SList<T>::operator=(SList&& other) noexcept
    {
        if (this != &other)
        {
            Clear();

            m_head = other.m_head;
            m_tail = other.m_tail;
            m_size = other.m_size;

            other.m_head = nullptr;
            other.m_tail = nullptr;
            other.m_size = 0;
        }
        
        return *this;
    }
 
    template<typename T>
    inline typename SList<T>::Iterator SList<T>::PushFront(const T& value)
    {
        m_head = new Node(value, m_head);

        ++m_size;

        // If this is the first item in the list, it is also the tail
        if (m_size == 1)
        {
            m_tail = m_head;
        }

        return Iterator(*this, m_head);
    }

    template<typename T>
    inline typename SList<T>::Iterator SList<T>::PushBack(const T& value)
    {
        Node* newTail = new Node(value);

        if (IsEmpty())
        {
            m_head = newTail;
        }
        else
        {
            m_tail->m_nextNode = newTail;
            
        }

        m_tail = newTail;

        ++m_size;

        return Iterator(*this, m_tail);
    }

    template<typename T>
    inline void SList<T>::PopFront()
    {
        if (!IsEmpty())
        {
            Node* newHead = m_head->m_nextNode;
            delete m_head;
            m_head = newHead;

            --m_size;

            if (IsEmpty())
            {
                m_tail = nullptr;
            }
        }
    }

    template<typename T>
    inline void SList<T>::PopBack()
    {
        if (IsEmpty())
        { 
            return; 
        }

        if (Size() == 1)
        {
            delete m_tail;
            m_tail = m_head = nullptr;
            --m_size;
            return;
        }

        Node* newTailPtr = m_head;

        while (newTailPtr->m_nextNode != m_tail)
        {
            newTailPtr = newTailPtr->m_nextNode;
        }

        newTailPtr->m_nextNode = nullptr;
        delete m_tail;
        m_tail = newTailPtr;

        --m_size;
    }

    template<typename T>
    inline T& SList<T>::Front()
    {
        if (IsEmpty())
        {
            throw std::runtime_error("You should not call Front() on an empty SList!");
        }

        return m_head->m_data;
    }

    template<typename T>
    inline const T& SList<T>::Front() const
    {
        if (IsEmpty())
        {
            throw std::runtime_error("You should not call Front() on an empty SList!");
        }

        return m_head->m_data;
    }

    template<typename T>
    inline T& SList<T>::Back()
    {
        if (IsEmpty())
        {
            throw std::runtime_error("You should not call Back() on an empty SList!");
        }

        return m_tail->m_data;
    }

    template<typename T>
    inline const T& SList<T>::Back() const
    {
        if (IsEmpty())
        {
            throw std::runtime_error("You should not call Back() on an empty SList!");
        }

        return m_tail->m_data;
    }

    template<typename T>
    inline size_t SList<T>::Size() const
    {
        return m_size;
    }

    template<typename T>
    inline bool SList<T>::IsEmpty() const
    {
        return (m_head == nullptr);
    }

    template<typename T>
    inline void SList<T>::Clear()
    {
        Node* currentNode = m_head;

        while (currentNode != nullptr)
        {
            Node* nodeToDelete = currentNode;
            currentNode = currentNode->m_nextNode;

            delete nodeToDelete;
        }

        m_size = 0;
        m_head = m_tail = nullptr;
    }

#pragma endregion SList

#pragma region SList Iterator
    template<typename T>
    inline typename SList<T>::Iterator SList<T>::begin()
    {
        return Iterator(*this, m_head);
    }

    template<typename T>
    inline typename SList<T>::ConstIterator SList<T>::begin() const
    {
        return ConstIterator(*this, m_head);
    }

    template<typename T>
    inline typename SList<T>::ConstIterator SList<T>::cbegin() const
    {
        return ConstIterator(*this, m_head);
    }

    template<typename T>
    inline typename SList<T>::Iterator SList<T>::end()
    {
        return Iterator(*this, nullptr);
    }

    template<typename T>
    inline typename SList<T>::ConstIterator SList<T>::end() const
    {
        return ConstIterator(*this, nullptr);
    }

    template<typename T>
    inline typename SList<T>::ConstIterator SList<T>::cend() const
    {
        return ConstIterator(*this, nullptr);
    }

    template<typename T>
    inline typename SList<T>::Iterator SList<T>::InsertAfter(const Iterator& it, const T& value)
    {
        if (it.m_owner != this)
        {
            throw std::runtime_error("Invalid iterator; owned by a different container");
        }

        if (it.m_nodePtr == nullptr)
        {
            return PushBack(value);
        }

        Node* oldNext = it.m_nodePtr->m_nextNode;
        it.m_nodePtr->m_nextNode = new Node(value, oldNext);
        ++m_size;

        // If we are inserting after the tail, this is the new tail
        if (oldNext == nullptr)
        {
            m_tail = it.m_nodePtr->m_nextNode;
        }

        return Iterator(*this, it.m_nodePtr->m_nextNode);
    }

    template<typename T>
    template<typename EqualityFunctor>
    inline typename SList<T>::ConstIterator SList<T>::Find(const T& value) const
    {
        EqualityFunctor eq;
        Node* listPtr = m_head;

        while (listPtr != nullptr)
        {
            if (eq(listPtr->m_data, value))
            {
                break;
            }

            listPtr = listPtr->m_nextNode;
        }

        return ConstIterator(*this, listPtr);
    }

    template<typename T>
    template<typename EqualityFunctor>
    inline typename SList<T>::Iterator SList<T>::Find(const T& value)
    {
        EqualityFunctor eq;
        Node* listPtr = m_head;

        while (listPtr != nullptr)
        {
            if (eq(listPtr->m_data,value))
            {
                break;
            }

            listPtr = listPtr->m_nextNode;
        }

        return Iterator(*this, listPtr);
    }

    template<typename T>
    template<typename EqualityFunctor>
    inline void SList<T>::Remove(const T& value)
    {
        Remove(Find<EqualityFunctor>(value));
    }

    template<typename T>
    inline void SList<T>::Remove(const Iterator& it)
    {
        if (it.m_owner != this)
        {
            throw std::runtime_error("Invalid iterator; owned by a different container");
        }

        if (it != end())
        {
            if (it.m_nodePtr == m_tail)
            {
                PopBack();
            }
            else
            {
                Node* next = it.m_nodePtr->m_nextNode;

                it.m_nodePtr->m_data.~T();
                new(&it.m_nodePtr->m_data)T(std::move(next->m_data));
                it.m_nodePtr->m_nextNode = next->m_nextNode;

                /*it.m_nodePtr->m_data = std::move(next->m_data);
                it.m_nodePtr->m_nextNode = next->m_nextNode;*/

                if (next->m_nextNode == nullptr)
                {
                    m_tail = it.m_nodePtr;
                }

                delete next;
                --m_size;   
            }
        }
    }

#pragma endregion SList Iterator

#pragma region Iterator

    template<typename T>
    inline bool SList<T>::Iterator::operator==(const Iterator& other) const
    {
        return !(operator != (other));
    }

    template<typename T>
    inline bool SList<T>::Iterator::operator!=(const Iterator& other) const
    {
        // Make sure the owner list AND the node pointer are the same otherwise they aren't equal
        return ((m_owner != other.m_owner) || (m_nodePtr != other.m_nodePtr));
    }

    template<typename T>
    inline typename SList<T>::Iterator& SList<T>::Iterator::operator++()
    {
        if (m_owner == nullptr)
        {
            throw std::runtime_error("You should not increment an uninitialized Iterator!");
        }

        if (m_nodePtr != nullptr)
        {
            m_nodePtr = m_nodePtr->m_nextNode;
        }

        return *this;
    }

    template<typename T>
    inline typename SList<T>::Iterator SList<T>::Iterator::operator++(int)
    {
        // We make a copy of the iterator so we can return an unchanged version
        // but still update the actual object
        Iterator tempIterator = *this;

        ++(*this);
        return tempIterator;
    }

    template<typename T>
    inline T& SList<T>::Iterator::operator*() const
    {
        if (m_nodePtr == nullptr)
        {
            throw std::runtime_error("Trying to derefence a null pointer! Does this iterator == end()?");
        }

        return m_nodePtr->m_data;
    }

#pragma endregion Iterator

#pragma region ConstIterator
    template<typename T>
    inline SList<T>::ConstIterator::ConstIterator(const Iterator& other)
        : m_owner(other.m_owner), m_nodePtr(other.m_nodePtr)
    {
    }

    template<typename T>
    inline typename SList<T>::ConstIterator& SList<T>::ConstIterator::operator=(const Iterator& other)
    {
        m_owner = other.m_owner;
        m_nodePtr = other.m_nodePtr;

        return *this;
    }

    template<typename T>
    inline bool SList<T>::ConstIterator::operator==(const ConstIterator& other) const
    {
        return !(operator != (other));
    }

    template<typename T>
    inline bool SList<T>::ConstIterator::operator!=(const ConstIterator& other) const
    {
        return ((m_owner != other.m_owner) || (m_nodePtr != other.m_nodePtr));
    }

    template<typename T>
    inline typename SList<T>::ConstIterator& SList<T>::ConstIterator::operator++()
    {
        if (m_owner == nullptr)
        {
            throw std::runtime_error("You should not increment an uninitialized Iterator!");
        }

        if (m_nodePtr != nullptr)
        {
            m_nodePtr = m_nodePtr->m_nextNode;
        }

        return *this;
    }

    template<typename T>
    inline typename SList<T>::ConstIterator SList<T>::ConstIterator::operator++(int)
    {
        ConstIterator tempIterator = *this;

        ++(*this);
        return tempIterator;
    }

    template<typename T>
    inline const T& SList<T>::ConstIterator::operator*() const
    {
        if (m_nodePtr == nullptr)
        {
            throw std::runtime_error("Trying to derefence a null pointer! Does this iterator == end()?");
        }

        return m_nodePtr->m_data;
    }

#pragma endregion ConstIterator
}