#pragma once
#include "Vector.h"
#include <cassert>
#include <cstdlib>
#include <utility>
#include <cstdlib>
#include <algorithm>

namespace FieaGameEngine
{
    template<typename T>
    Vector<T>::Vector(size_t capacity)
    {
        if (capacity > 0)
        {
            Reserve(capacity);
        }
    }

    template<typename T>
    Vector<T>::Vector(const Vector& other)
    {
        Reserve(other.m_capacity);
        for (const T& value : other)
        {
            PushBack(value);
        }
    }

    template<typename T>
    Vector<T>& Vector<T>::operator=(const Vector& other)
    {
        if (this != &other)
        {
            Clear();

            // Maybe need to put some logic in here to shrink this if other has
            // a smaller capacity? This will only make it grow if needed
            Reserve(other.m_capacity);

            for (const T& value : other)
            {
                // maybe just use placement new here instead of pushback?
                PushBack(value);
            }

        }

        return *this;
    }

    template<typename T>
    Vector<T>::Vector(Vector&& other) noexcept
       : m_data(other.m_data), m_size(other.m_size), m_capacity(other.m_capacity)
    {
        other.m_data = nullptr;
        other.m_size = 0;
        other.m_capacity = 0;
    }

    template<typename T>
    Vector<T>& Vector<T>::operator=(Vector&& other) noexcept
    {
        if (this != &other)
        {
            Clear();
            free(m_data);

            m_data = other.m_data;
            m_size = other.m_size;
            m_capacity = other.m_capacity;

            other.m_data = nullptr;
            other.m_size = 0;
            other.m_capacity = 0;
        }

        return *this;
    }

    template<typename T>
    Vector<T>::Vector(std::initializer_list<T> list)
    {
        Reserve(list.size());
        for (const auto& value : list)
        {
            new(m_data + m_size++) T(value);
        }
    }

    template<typename T>
    inline Vector<T>::~Vector()
    {
        Clear();
        free(m_data);
    }

    template<typename T>
    inline T& Vector<T>::operator[](size_t index)
    {
        if (m_size <= index)
        {
            throw std::runtime_error("Attempting to index out of bounds of the vector's size!");
        }

        return m_data[index];
    }

    template<typename T>
    inline const T& Vector<T>::operator[](size_t index) const
    {
        if (m_size <= index)
        {
            throw std::runtime_error("Attempting to index out of bounds of the vector's size!");
        }

        return m_data[index];
    }

    template<typename T>
    inline T& Vector<T>::At(size_t index)
    {
        if (m_size <= index)
        {
            throw std::runtime_error("Calling At() with an out of bounds index!");
        }

        return m_data[index];
    }

    template<typename T>
    inline const T& Vector<T>::At(size_t index) const
    {
        if (m_size <= index)
        {
            throw std::runtime_error("Calling At() with an out of bounds index!");
        }

        return m_data[index];
    }

    template<typename T>
    inline T& Vector<T>::Front()
    {
        if (m_size == 0)
        {
            throw std::runtime_error("Calling Front() on an empty vector!");
        }

        return m_data[0];
    }

    template<typename T>
    inline const T& Vector<T>::Front() const
    {
        if (m_size == 0)
        {
            throw std::runtime_error("Calling Front() on an empty vector!");
        }

        return m_data[0];
    }

    template<typename T>
    inline T& Vector<T>::Back()
    {
        if (m_size == 0)
        {
            throw std::runtime_error("Calling Back() on an empty vector!");
        }

        return m_data[m_size - 1];
    }

    template<typename T>
    inline const T& Vector<T>::Back() const
    {
        if (m_size == 0)
        {
            throw std::runtime_error("Calling Back() on an empty vector!");
        }

        return m_data[m_size - 1];
    }

    template<typename T>
    inline typename Vector<T>::Iterator Vector<T>::begin()
    {
        return Iterator(*this, 0);
    }

    template<typename T>
    inline typename Vector<T>::ConstIterator Vector<T>::begin() const
    {
        return ConstIterator(*this, 0);
    }

    template<typename T>
    inline typename Vector<T>::ConstIterator Vector<T>::cbegin() const
    {
        return ConstIterator(*this, 0);
    }

    template<typename T>
    inline typename Vector<T>::Iterator Vector<T>::end()
    {
        return Iterator(*this, m_size);
    }

    template<typename T>
    inline typename Vector<T>::ConstIterator Vector<T>::end() const
    {
        return ConstIterator(*this, m_size);
    }

    template<typename T>
    inline typename Vector<T>::ConstIterator Vector<T>::cend() const
    {
        return ConstIterator(*this, m_size);
    }

    template<typename T>
    template<typename IncrementFunctor>
    typename Vector<T>::Iterator Vector<T>::PushBack(const T& value)
    {
        IncrementFunctor inc{};

        if (m_size == m_capacity)
        {
            size_t capacity = std::max(inc(m_size, m_capacity), m_capacity + 1);
            Reserve(capacity);
        }

        new(m_data + m_size) T(value);

        return Iterator(*this, m_size++);
    }

    template<typename T>
    template<typename IncrementFunctor>
    typename Vector<T>::Iterator Vector<T>::PushBack(T&& value)
    {
        IncrementFunctor inc{};

        if (m_size == m_capacity)
        {
            size_t capacity = std::max(inc(m_size, m_capacity), m_capacity + 1);
            Reserve(capacity);
        }

        new(m_data + m_size) T(std::forward<T>(value));
 
        return Iterator(*this, m_size++);
    }

    template<typename T>
    void Vector<T>::PopBack()
    {
        if (m_size > 0)
        {
            m_data[m_size - 1].~T();
            --m_size;
        }
    }

    template<typename T>
    void Vector<T>::Reserve(size_t capacity)
    {
        if (capacity > m_capacity)
        {
            T* data = reinterpret_cast<T*>(realloc(m_data, sizeof(T) * capacity));
            assert(data != nullptr);
            m_data = data;
            m_capacity = capacity;
        }
    }

    template<typename T>
    void Vector<T>::ShrinkToFit()
    {
        if (m_capacity > m_size)
        {
            T* data = reinterpret_cast<T*>(realloc(m_data, sizeof(T) * m_size));
            assert(data != nullptr);
            m_data = data;
            m_capacity = m_size;
        }
    }

    template<typename T>
    inline void Vector<T>::Resize(size_t size)
    {
        if (size < m_size)
        {
            for (size_t i = size; i < m_size; ++i)
            {
                m_data[i].~T();
            }
        }
        else
        {
            Reserve(size);
            for (size_t i = m_size; i < size; ++i)
            {
                new(m_data + i)T();
            }
        }

        m_size = size;
    }

    template<typename T>
    inline size_t Vector<T>::Capacity() const
    {
        return m_capacity;
    }

    template<typename T>
    inline size_t Vector<T>::Size() const
    {
        return m_size;
    }

    template<typename T>
    inline bool Vector<T>::IsEmpty() const
    {
        return (m_size == 0);
    }

    template<typename T>
    void Vector<T>::Clear()
    {
        for (size_t i = 0; i < m_size; ++i)
        {
            m_data[i].~T();
        }

        m_size = 0;
    }

    template<typename T>
    template<typename EqualityFunctor>
    typename Vector<T>::Iterator Vector<T>::Find(const T& value)
    {
        EqualityFunctor eq{};

        auto it = begin();
        for (; it != end(); ++it)
        {
            if (eq(*it, value))
            {
                break;
            }
        }

        return it;
    }

    template<typename T>
    template<typename EqualityFunctor>
    typename Vector<T>::ConstIterator Vector<T>::Find(const T& value) const
    {
        EqualityFunctor eq{};

        auto it = begin();
        for (; it != end(); ++it)
        {
            if (eq(*it, value))
            {
                break;
            }
        }

        return it;
    }

    template<typename T>
    template<typename EqualityFunctor>
    inline bool Vector<T>::Remove(const T& value)
    {
        return Remove(Find<EqualityFunctor>(value));
    }

    template<typename T>
    bool Vector<T>::Remove(const Iterator& it)
    {
        if (it.m_owner == this && it.m_index < m_size)
        {
            // Get the number of bytes that we're gonna have to move
            size_t numBytes = (m_size - 1 - it.m_index) * sizeof(T);

            // Delete the object that we are removing
            m_data[it.m_index].~T();

            T* elementToRemove = &m_data[it.m_index];
            T* startOfData = &m_data[it.m_index + 1];

            memmove(elementToRemove, startOfData, numBytes);

            --m_size;

            return true;
        }
        
        return false;
    }

    template<typename T>
    bool Vector<T>::Remove(const Iterator& startIterator, const Iterator& endIterator)
    {
        // Guard against the start iterator being after the end iterator
        if (startIterator.m_index >= endIterator.m_index)
        {
            throw std::runtime_error("The start iterator is after the end iterator!");
        }

        if (startIterator.m_owner == this && endIterator.m_owner == this)
        {
            if (startIterator.m_index < m_size && endIterator.m_index <= m_size)
            {
                for (auto it = startIterator; it != endIterator; ++it)
                {
                    m_data[it.m_index].~T();
                }

                size_t numBytesToMove = (m_size - endIterator.m_index) * sizeof(T);
                T* dataToMove = &m_data[endIterator.m_index];
                T* destination = &m_data[startIterator.m_index];

                memmove(destination, dataToMove, numBytesToMove);

                m_size -= (endIterator.m_index - startIterator.m_index);

                return true;
            }
        }
       
        return false;
    }

#pragma region Iterator

    template<typename T>
    inline bool Vector<T>::Iterator::operator==(const Iterator& other) const
    {
        return !(operator != (other));
    }

    template<typename T>
    inline bool Vector<T>::Iterator::operator!=(const Iterator& other) const
    {
        // Make sure the owner list AND the index are the same, otherwise they aren't equal
        return ((m_owner != other.m_owner) || (m_index != other.m_index));
    }

    template<typename T>
    typename Vector<T>::Iterator& Vector<T>::Iterator::operator++()
    {
        if (m_owner == nullptr)
        {
            throw std::runtime_error("You should not increment an uninitialized Iterator!");
        }

        if (m_index < m_owner->Size())
        {
            ++m_index;
        }

        return *this;
    }

    template<typename T>
    typename Vector<T>::Iterator Vector<T>::Iterator::operator++(int)
    {
        Iterator tempIterator = *this;
        ++(*this);
        return tempIterator;
    }

    template<typename T>
    typename Vector<T>::Iterator& Vector<T>::Iterator::operator--()
    {
        if (m_owner == nullptr)
        {
            throw std::runtime_error("You should not decrement an uninitialized Iterator!");
        }

        if (m_index > 0)
        {
            --m_index;
        }

        return *this;
    }

    template<typename T>
    typename Vector<T>::Iterator Vector<T>::Iterator::operator--(int)
    {
        Iterator tempIterator = *this;
        --(*this);
        return tempIterator;
    }

    template<typename T>
    typename Vector<T>::Iterator Vector<T>::Iterator::operator+(size_t value) const
    {
        Iterator tempIterator = *this;
        tempIterator += value;
        return tempIterator;
    }

    template<typename T>
    typename Vector<T>::Iterator Vector<T>::Iterator::operator-(size_t value) const
    {
        Iterator tempIterator = *this;
        tempIterator -= value;
        return tempIterator;
    }

    template<typename T>
    inline std::ptrdiff_t Vector<T>::Iterator::operator-(const Iterator& other) const
    {
        int diffCount = 0;
        auto itCopy = *this;

        while (itCopy != other && itCopy != m_owner->end())
        {
            ++itCopy;
            ++diffCount;
        }

        return diffCount;
    }

    template<typename T>
    typename Vector<T>::Iterator& Vector<T>::Iterator::operator+=(size_t value)
    {
        if (m_owner == nullptr)
        {
            throw std::runtime_error("Trying to add to an iterator with no owner!");
        }

        size_t newIndex = m_index + value;
        m_index = (newIndex > m_owner->m_size) ? m_owner->m_size : newIndex;

        return *this;
    }

    template<typename T>
    bool Vector<T>::Iterator::operator<(const Iterator& other) const
    {
        if (m_owner == nullptr || other.m_owner == nullptr)
        {
            throw std::runtime_error("Trying to compare an iterator with no owner!");
        }

        if (m_owner != other.m_owner)
        {
            throw std::runtime_error("Trying to compare two iterators with different owners!");
        }

        return m_index < other.m_index;
    }

    template<typename T>
    bool Vector<T>::Iterator::operator>(const Iterator& other) const
    {
        if (m_owner == nullptr || other.m_owner == nullptr)
        {
            throw std::runtime_error("Trying to compare an iterator with no owner!");
        }

        if (m_owner != other.m_owner)
        {
            throw std::runtime_error("Trying to compare two iterators with different owners!");
        }

        return m_index > other.m_index;
    }

    template<typename T>
    typename Vector<T>::Iterator& Vector<T>::Iterator::operator-=(size_t value)
    {
        if (m_owner == nullptr)
        {
            throw std::runtime_error("Trying to subtract from an iterator with no owner!");
        }

        m_index = (value > m_index) ? 0 : m_index - value;

        return *this;
    }

    template<typename T>
    T& Vector<T>::Iterator::operator*() const
    {
        if (m_owner == nullptr)
        {
            throw std::runtime_error("Trying to dereference an iterator with no owner!");
        }

        if (m_index >= m_owner->Size())
        {
            throw std::runtime_error("Trying to dereference an iterator that is out of bounds!");
        }

        return m_owner->At(m_index);
    }

    template<typename T>
    inline T& Vector<T>::Iterator::operator[](size_t index) const
    {
        if (m_owner == nullptr)
        {
            throw std::runtime_error("Trying to dereference an iterator with no owner!");
        }

        return m_owner->At(index);
    }

    template<typename T>
    inline T* Vector<T>::Iterator::operator->()
    {
        return &(**this);
    }

#pragma endregion Iterator

#pragma region ConstIterator
    template<typename T>
    inline Vector<T>::ConstIterator::ConstIterator(const Iterator& other)
        : m_owner(other.m_owner), m_index(other.m_index)
    {
    }

    template<typename T>
    typename Vector<T>::ConstIterator& Vector<T>::ConstIterator::operator=(const Iterator& other)
    {
        m_owner = other.m_owner;
        m_index = other.m_index;

        return *this;
    }

    template<typename T>
    inline bool Vector<T>::ConstIterator::operator==(const ConstIterator& other) const
    {
        return !(operator != (other));
    }

    template<typename T>
    inline bool Vector<T>::ConstIterator::operator!=(const ConstIterator& other) const
    {
        return ((m_owner != other.m_owner) || (m_index != other.m_index));
    }

    template<typename T>
    typename Vector<T>::ConstIterator& Vector<T>::ConstIterator::operator++()
    {
        if (m_owner == nullptr)
        {
            throw std::runtime_error("You should not increment an uninitialized Iterator!");
        }

        if (m_index < m_owner->Size())
        {
            ++m_index;
        }

        return *this;
    }

    template<typename T>
    typename Vector<T>::ConstIterator Vector<T>::ConstIterator::operator++(int)
    {
        ConstIterator tempIterator = *this;
        ++(*this);
        return tempIterator;
    }

    template<typename T>
    typename Vector<T>::ConstIterator& Vector<T>::ConstIterator::operator--()
    {
        if (m_owner == nullptr)
        {
            throw std::runtime_error("You should not decrement an uninitialized Iterator!");
        }

        if (m_index > 0)
        {
            --m_index;
        }

        return *this;
    }

    template<typename T>
    typename Vector<T>::ConstIterator Vector<T>::ConstIterator::operator--(int)
    {
        ConstIterator tempIterator = *this;
        --(*this);
        return tempIterator;
    }

    template<typename T>
    typename Vector<T>::ConstIterator Vector<T>::ConstIterator::operator+(size_t value) const
    {
        ConstIterator tempIterator = *this;
        tempIterator += value;
        return tempIterator;
    }

    template<typename T>
    typename Vector<T>::ConstIterator Vector<T>::ConstIterator::operator-(size_t value) const
    {
        ConstIterator tempIterator = *this;
        tempIterator -= value;
        return tempIterator;
    }

    template<typename T>
    typename Vector<T>::ConstIterator& Vector<T>::ConstIterator::operator+=(size_t value)
    {
        if (m_owner == nullptr)
        {
            throw std::runtime_error("Trying to add to an iterator with no owner!");
        }

        size_t newIndex = m_index + value;
        m_index = (newIndex > m_owner->m_size) ? m_owner->m_size : newIndex;

        return *this;
    }

    template<typename T>
    typename Vector<T>::ConstIterator& Vector<T>::ConstIterator::operator-=(size_t value)
    {
        if (m_owner == nullptr)
        {
            throw std::runtime_error("Trying to subtract from an iterator with no owner!");
        }

        m_index = (value > m_index) ? 0 : m_index - value;

        return *this;
    }

    template<typename T>
    inline bool Vector<T>::ConstIterator::operator<(const ConstIterator& other) const
    {
        if (m_owner == nullptr || other.m_owner == nullptr)
        {
            throw std::runtime_error("Trying to compare an iterator with no owner!");
        }

        if (m_owner != other.m_owner)
        {
            throw std::runtime_error("Trying to compare two iterators with different owners!");
        }

        return m_index < other.m_index;
    }

    template<typename T>
    inline bool Vector<T>::ConstIterator::operator>(const ConstIterator& other) const
    {
        if (m_owner == nullptr || other.m_owner == nullptr)
        {
            throw std::runtime_error("Trying to compare an iterator with no owner!");
        }

        if (m_owner != other.m_owner)
        {
            throw std::runtime_error("Trying to compare two iterators with different owners!");
        }

        return m_index > other.m_index;
    }

    template<typename T>
    const T& Vector<T>::ConstIterator::operator*() const
    {
        if (m_owner == nullptr)
        {
            throw std::runtime_error("Trying to dereference an iterator with no owner!");
        }

        if (m_index >= m_owner->Size())
        {
            throw std::runtime_error("Trying to dereference an iterator that is out of bounds!");
        }

        return m_owner->At(m_index);
    }

    template<typename T>
    inline const T& Vector<T>::ConstIterator::operator[](size_t index) const
    {
        if (m_owner == nullptr)
        {
            throw std::runtime_error("Trying to dereference an iterator with no owner!");
        }

        return m_owner->At(index);
    }

    template<typename T>
    inline const T* Vector<T>::ConstIterator::operator->()
    {
        return &(**this);
    }

#pragma endregion ConstIterator
}