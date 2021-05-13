#pragma once
#include "Datum.h"

namespace FieaGameEngine
{
#pragma region Get
    // Non const
    template<typename T>
    inline T& Datum::Get(size_t /*index*/)
    {
        static_assert(false, "Data type not supported!");
    }

    template<>
    inline int& Datum::Get(size_t index)
    {
        if (m_type != DatumType::Integer)
        {
            throw std::runtime_error("Trying to get the wrong type!");
        }

        if (index >= m_size)
        {
            throw std::runtime_error("Trying to get an out of bounds object!");
        }

        return m_data.i[index];
    }

    template<>
    inline std::string& Datum::Get(size_t index)
    {
        if (m_type != DatumType::String)
        {
            throw std::runtime_error("Trying to get the wrong type!");
        }

        if (index >= m_size)
        {
            throw std::runtime_error("Trying to get an out of bounds object!");
        }

        return m_data.s[index];
    }

    template<>
    inline float& Datum::Get(size_t index)
    {
        if (m_type != DatumType::Float)
        {
            throw std::runtime_error("Trying to get the wrong type!");
        }

        if (index >= m_size)
        {
            throw std::runtime_error("Trying to get an out of bounds object!");
        }

        return m_data.f[index];
    }

    template<>
    inline glm::vec4& Datum::Get(size_t index)
    {
        if (m_type != DatumType::Vector)
        {
            throw std::runtime_error("Trying to get the wrong type!");
        }

        if (index >= m_size)
        {
            throw std::runtime_error("Trying to get an out of bounds object!");
        }

        return m_data.v[index];
    }

    template<>
    inline glm::mat4& Datum::Get(size_t index)
    {
        if (m_type != DatumType::Matrix)
        {
            throw std::runtime_error("Trying to get the wrong type!");
        }

        if (index >= m_size)
        {
            throw std::runtime_error("Trying to get an out of bounds object!");
        }

        return m_data.m[index];
    }

    template<>
    inline RTTI*& Datum::Get(size_t index)
    {
        if (m_type != DatumType::RTTI)
        {
            throw std::runtime_error("Trying to get the wrong type!");
        }

        if (index >= m_size)
        {
            throw std::runtime_error("Trying to get an out of bounds object!");
        }

        return m_data.r[index];
    }

    template<>
    inline Scope& Datum::Get(size_t index)
    {
        if (m_type != DatumType::Table)
        {
            throw std::runtime_error("Trying to get the wrong type!");
        }

        if (index >= m_size)
        {
            throw std::runtime_error("Trying to get an out of bounds object!");
        }

        return (*m_data.sc[index]);
    }

    // Const
    template<typename T>
    inline const T& Datum::Get(size_t /*index*/) const
    {
        static_assert(false, "Data type not supported!");
    }

    template<>
    inline const int& Datum::Get(size_t index) const
    {
        if (m_type != DatumType::Integer)
        {
            throw std::runtime_error("Trying to get the wrong type!");
        }

        if (index >= m_size)
        {
            throw std::runtime_error("Trying to get an out of bounds object!");
        }

        return m_data.i[index];
    }

    template<>
    inline const std::string& Datum::Get(size_t index) const
    {
        if (m_type != DatumType::String)
        {
            throw std::runtime_error("Trying to get the wrong type!");
        }

        if (index >= m_size)
        {
            throw std::runtime_error("Trying to get an out of bounds object!");
        }

        return m_data.s[index];
    }

    template<>
    inline const float& Datum::Get(size_t index) const
    {
        if (m_type != DatumType::Float)
        {
            throw std::runtime_error("Trying to get the wrong type!");
        }

        if (index >= m_size)
        {
            throw std::runtime_error("Trying to get an out of bounds object!");
        }

        return m_data.f[index];
    }

    template<>
    inline const glm::vec4& Datum::Get(size_t index) const
    {
        if (m_type != DatumType::Vector)
        {
            throw std::runtime_error("Trying to get the wrong type!");
        }

        if (index >= m_size)
        {
            throw std::runtime_error("Trying to get an out of bounds object!");
        }

        return m_data.v[index];
    }

    template<>
    inline const glm::mat4& Datum::Get(size_t index) const
    {
        if (m_type != DatumType::Matrix)
        {
            throw std::runtime_error("Trying to get the wrong type!");
        }

        if (index >= m_size)
        {
            throw std::runtime_error("Trying to get an out of bounds object!");
        }

        return m_data.m[index];
    }

    template<>
    inline RTTI* const& Datum::Get(size_t index) const
    {
        if (m_type != DatumType::RTTI)
        {
            throw std::runtime_error("Trying to get the wrong type!");
        }

        if (index >= m_size)
        {
            throw std::runtime_error("Trying to get an out of bounds object!");
        }

        return m_data.r[index];
    }

    template<>
    inline const Scope& Datum::Get(size_t index) const
    {
        if (m_type != DatumType::Table)
        {
            throw std::runtime_error("Trying to get the wrong type!");
        }

        if (index >= m_size)
        {
            throw std::runtime_error("Trying to get an out of bounds object!");
        }

        return (*m_data.sc[index]);
    }


#pragma endregion

#pragma region Front
    // non-const
    template<typename T>
    inline T& Datum::Front()
    {
        static_assert(false, "Data type not supported!");
    }

    template<>
    inline int& Datum::Front()
    {
        return Get<int>();
    }

    template<>
    inline std::string& Datum::Front()
    {
        return Get<std::string>();
    }

    template<>
    inline float& Datum::Front()
    {
        return Get<float>();
    }

    template<>
    inline glm::vec4& Datum::Front()
    {
        return Get<glm::vec4>();
    }

    template<>
    inline glm::mat4& Datum::Front()
    {
        return Get<glm::mat4>();
    }

    template<>
    inline RTTI*& Datum::Front()
    {
        return Get<RTTI*>();
    }

    template<>
    inline Scope& Datum::Front()
    {
        return Get<Scope>();
    }

    // const
    template<typename T>
    inline const T& Datum::Front() const
    {
        static_assert(false, "Data type not supported!");
    }

    template<>
    inline const int& Datum::Front() const
    {
        return Get<int>();
    }

    template<>
    inline const std::string& Datum::Front() const
    {
        return Get<std::string>();
    }

    template<>
    inline const float& Datum::Front() const
    {
        return Get<float>();
    }

    template<>
    inline const glm::vec4& Datum::Front() const
    {
        return Get<glm::vec4>();
    }

    template<>
    inline const glm::mat4& Datum::Front() const
    {
        return Get<glm::mat4>();
    }

    template<>
    inline RTTI* const& Datum::Front() const
    {
        return Get<RTTI*>();
    }

    template<>
    inline const Scope& Datum::Front() const
    {
        return Get<Scope>();
    }
#pragma endregion

#pragma region Back
    template<typename T>
    inline T& Datum::Back()
    {
        static_assert(false, "Data type not supported!");
    }

    template<>
    inline int& Datum::Back()
    {
        return Get<int>(m_size - 1);
    }

    template<>
    inline std::string& Datum::Back()
    {
        return Get<std::string>(m_size - 1);
    }

    template<>
    inline float& Datum::Back()
    {
        return Get<float>(m_size - 1);
    }

    template<>
    inline glm::vec4& Datum::Back()
    {
        return Get<glm::vec4>(m_size - 1);
    }

    template<>
    inline glm::mat4& Datum::Back()
    {
        return Get<glm::mat4>(m_size - 1);
    }

    template<>
    inline RTTI*& Datum::Back()
    {
        return Get<RTTI*>(m_size - 1);
    }

    template<>
    inline Scope& Datum::Back()
    {
        return Get<Scope>(m_size - 1);
    }

    template<typename T>
    inline const T& Datum::Back() const
    {
        static_assert(false, "Data type not supported!");
    }

    template<>
    inline const int& Datum::Back() const
    {
        return Get<int>(m_size - 1);
    }

    template<>
    inline const std::string& Datum::Back() const
    {
        return Get<std::string>(m_size - 1);
    }

    template<>
    inline const float& Datum::Back() const
    {
        return Get<float>(m_size - 1);
    }

    template<>
    inline const glm::vec4& Datum::Back() const
    {
        return Get<glm::vec4>(m_size - 1);
    }

    template<>
    inline const glm::mat4& Datum::Back() const
    {
        return Get<glm::mat4>(m_size - 1);
    }

    template<>
    inline RTTI* const& Datum::Back() const
    {
        return Get<RTTI*>(m_size - 1);
    }

    template<>
    inline const Scope& Datum::Back() const
    {
        return Get<Scope>(m_size - 1);
    }
#pragma endregion


}
