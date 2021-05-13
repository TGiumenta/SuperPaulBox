#include "pch.h"
#include "Datum.h"
#include <stdexcept>
#include <glm/gtx/string_cast.hpp>


namespace FieaGameEngine
{
	/*size_t Datum::m_DataSizes[8] = 
	{	sizeof(int),
		sizeof(std::string),
		sizeof(float),
		sizeof(glm::vec4),
		sizeof(glm::mat4),
		sizeof(RTTI*),
        sizeof(Scope*),
        0
	};*/

#pragma region RuleOf6
	Datum::Datum(DatumType type)
		:m_type(type)
	{
	}

	Datum::Datum(const Datum& other)
	{
        m_type = other.m_type;

        if (other.m_externalStorage)
        {
            m_externalStorage = true;
            m_data.vp = other.m_data.vp;
            m_size = other.m_size;
            m_capacity = other.m_capacity;
        }
        else if (other.Size() > 0)
        {
            Reserve(other.m_capacity);

            if (m_type == DatumType::String)
            {
                for (size_t i = 0; i < other.m_size; ++i)
                {
                    PushBack(other.m_data.s[i]);
                }
            }
            else
            {
                size_t typeSize = m_DataSizes[static_cast<uint32_t>(m_type)];
                size_t numBytes = other.m_size * typeSize;

                memcpy(m_data.vp, other.m_data.vp, numBytes);

                m_size = other.m_size;
            }
        }
	}

    Datum& Datum::operator=(const Datum& other)
    {
		if (this != &other)
		{
            // If other has external storage
            if (other.m_externalStorage)
            {
                // If we have internal storage
                if (!m_externalStorage)
                {
                    Clear();
                    free(m_data.vp);
                }

                m_externalStorage = true;
                m_type = other.m_type;
                m_data.vp = other.m_data.vp;
                m_size = other.m_size;
                m_capacity = other.m_capacity;
            }
            // If other has internal storage
            else
            {
                // If we have internal storage
                if (!m_externalStorage)
                {
                    Clear();

                    // This is specifically for the case where the type has changed, we want to force the 
                    // reserve call to reallocate regardless of the number of elements since the size of 
                    // each element has changed
                    if (m_type != other.m_type)
                    {
                        m_capacity = 0;
                    }
                }
                // If we have external storage
                else
                {
                    m_data.vp = nullptr;

                    // Set our capacity to 0 so that the reserve call will actually work
                    m_capacity = 0;
                }

                m_externalStorage = false;
                m_type = other.m_type;
                Reserve(other.m_capacity);

                if (m_type == DatumType::String)
                {
                    for (size_t i = 0; i < other.m_size; ++i)
                    {
                        PushBack(other.m_data.s[i]);
                    }
                }
                else
                {
                    size_t typeSize = m_DataSizes[static_cast<uint32_t>(m_type)];
                    size_t numBytes = other.m_size * typeSize;

                    memcpy_s(m_data.vp, numBytes, other.m_data.vp, numBytes);

                    m_size = other.m_size;
                }
            }
		}

		return *this;
    }

	Datum::Datum(Datum&& other) noexcept
		: m_type(other.m_type), m_size(other.m_size), m_capacity(other.m_capacity)
	{
		m_data.vp = other.m_data.vp;
        m_externalStorage = other.m_externalStorage;

		other.m_type = DatumType::Unknown;
		other.m_size = 0;
		other.m_capacity = 0;
		other.m_data.vp = nullptr;
	}

	Datum& Datum::operator=(Datum&& other) noexcept
	{
		if (this != &other)
		{
            // If we had any internal memory, free that
            if (!m_externalStorage)
            {
                Clear();
                free(m_data.vp);
            }

			m_type = other.m_type;
			m_size = other.m_size;
			m_capacity = other.m_capacity;
			m_data.vp = other.m_data.vp;
            m_externalStorage = other.m_externalStorage;

            other.m_type = DatumType::Unknown;
            other.m_size = 0;
            other.m_capacity = 0;
            other.m_data.vp = nullptr;
		}

		return *this;
	}

	Datum::~Datum()
	{
        if (!m_externalStorage)
        {
            Clear();
            free(m_data.vp);
        }
	}
#pragma endregion

#pragma region AssignmentOverloads
	Datum& Datum::operator=(int value)
	{
		if (m_type == DatumType::Unknown)
		{
			m_type = DatumType::Integer;
		}

        if (m_type != DatumType::Integer)
        {
            throw std::runtime_error("Trying to set a non-Integer datum to an int!");
        }

        if (m_size == 0)
        {
            if (m_externalStorage)
            {
                throw std::runtime_error("Trying to call scalar equals on an empty datum with external storage!");
            }

            PushBack(value);
        }
        else
        {
            Set(value);
        }

		return *this;
	}

	Datum& Datum::operator=(std::string value)
	{
        if (m_type == DatumType::Unknown)
        {
            m_type = DatumType::String;
        }

        if (m_type != DatumType::String)
        {
            throw std::runtime_error("Trying to set a non-Integer datum to an int!");
        }

        if (m_size == 0)
        {
            if (m_externalStorage)
            {
                throw std::runtime_error("Trying to call scalar equals on an empty datum with external storage!");
            }

            PushBack(value);
        }
        else
        {
            Set(value);
        }

        return *this;
	}

	Datum& Datum::operator=(float value)
	{
        if (m_type == DatumType::Unknown)
        {
            m_type = DatumType::Float;
        }

        if (m_type != DatumType::Float)
        {
            throw std::runtime_error("Trying to set a non-float datum to an float!");
        }

        if (m_size == 0)
        {
            if (m_externalStorage)
            {
                throw std::runtime_error("Trying to call scalar equals on an empty datum with external storage!");
            }

            PushBack(value);
        }
        else
        {
            Set(value);
        }

        return *this;
	}

	Datum& Datum::operator=(glm::vec4 value)
	{
        if (m_type == DatumType::Unknown)
        {
            m_type = DatumType::Vector;
        }

        if (m_type != DatumType::Vector)
        {
            throw std::runtime_error("Trying to set a non-vector datum to an vector!");
        }

        if (m_size == 0)
        {
            if (m_externalStorage)
            {
                throw std::runtime_error("Trying to call scalar equals on an empty datum with external storage!");
            }

            PushBack(value);
        }
        else
        {
            Set(value);
        }

        return *this;
	}

	Datum& Datum::operator=(glm::mat4 value)
	{
        if (m_type == DatumType::Unknown)
        {
            m_type = DatumType::Matrix;
        }

        if (m_type != DatumType::Matrix)
        {
            throw std::runtime_error("Trying to set a non-matrix datum to an matrix!");
        }

        if (m_size == 0)
        {
            if (m_externalStorage)
            {
                throw std::runtime_error("Trying to call scalar equals on an empty datum with external storage!");
            }

            PushBack(value);
        }
        else
        {
            Set(value);
        }

        return *this;
	}

	Datum& Datum::operator=(RTTI* value)
	{
        if (m_type == DatumType::Unknown)
        {
            m_type = DatumType::RTTI;
        }

        if (m_type != DatumType::RTTI)
        {
            throw std::runtime_error("Trying to set a non-RTTI datum to an RTTI!");
        }

        if (m_size == 0)
        {
            if (m_externalStorage)
            {
                throw std::runtime_error("Trying to call scalar equals on an empty datum with external storage!");
            }

            PushBack(value);
        }
        else
        {
            Set(value);
        }

        return *this;
	}

    Datum& Datum::operator=(Scope& value)
    {
        if (m_type == DatumType::Unknown)
        {
            m_type = DatumType::Table;
        }

        if (m_type != DatumType::Table)
        {
            throw std::runtime_error("Trying to set a non-Scope datum to an scope!");
        }

        if (m_size == 0)
        {
            assert(!m_externalStorage);
            PushBack(value);
        }
        else
        {
            Set(&value);
        }

        return *this;
    }
#pragma endregion

#pragma region Equality
    bool Datum::operator==(const Datum& other) const
    {
        bool equality = false;
        if (m_type == other.m_type && m_size == other.m_size)
        {
            // If their types are unknown, they must be the same
            if (m_type == DatumType::Unknown)
            {
                equality = true;
            }
            else
            {
                Datum::CompareFunction func = m_CompareFunctions[static_cast<uint32_t>(m_type)];
                equality = (this->*func)(other);
            }
        }

        return equality;
    }

    bool Datum::operator!=(const Datum& other) const
    {
        return !(operator == (other));
    }

    bool Datum::operator==(const int& other) const
    {
        return Get<int>() == other;
    }

    bool Datum::operator==(const std::string& other) const
    {
        return Get<std::string>() == other;
    }

    bool Datum::operator==(const float& other) const
    {
        return Get<float>() == other;
    }

    bool Datum::operator==(const glm::vec4& other) const
    {
        return Get<glm::vec4>() == other;
    }

    bool Datum::operator==(const glm::mat4& other) const
    {
        return Get<glm::mat4>() == other;
    }

    bool Datum::operator==(RTTI* const& other) const
    {
        return Get<RTTI*>()->Equals(other);
    }

    bool Datum::operator!=(const int& other) const
    {
        return !(operator == (other));
    }

    bool Datum::operator!=(const std::string& other) const
    {
        return !(operator == (other));
    }

    bool Datum::operator!=(const float& other) const
    {
        return !(operator == (other));
    }

    bool Datum::operator!=(const glm::vec4& other) const
    {
        return !(operator == (other));
    }

    bool Datum::operator!=(const glm::mat4& other) const
    {
        return !(operator == (other));
    }

    bool Datum::operator!=(RTTI* const& other) const
    {
        return !(operator == (other));
    }
#pragma endregion

#pragma region Set
	void Datum::Set(int value, size_t index)
	{
		if (index >= m_size)
		{
			throw std::runtime_error("Trying to call Set with an out of bounds index!");
		}

		if (m_type != DatumType::Integer)
		{
			throw std::runtime_error("Trying to call Set on a datum with the wrong data type!");
		}

		m_data.i[index] = value;
	}

	void Datum::Set(std::string value, size_t index)
	{
        if (index >= m_size)
        {
            throw std::runtime_error("Trying to call Set with an out of bounds index!");
        }

        if (m_type != DatumType::String)
        {
            throw std::runtime_error("Trying to call Set on a datum with the wrong data type!");
        }

		m_data.s[index] = value;
	}

	void Datum::Set(float value, size_t index)
	{
        if (index >= m_size)
        {
            throw std::runtime_error("Trying to call Set with an out of bounds index!");
        }

        if (m_type != DatumType::Float)
        {
            throw std::runtime_error("Trying to call Set on a datum with the wrong data type!");
        }

        m_data.f[index] = value;
	}

	void Datum::Set(glm::vec4 value, size_t index)
	{
        if (index >= m_size)
        {
            throw std::runtime_error("Trying to call Set with an out of bounds index!");
        }

        if (m_type != DatumType::Vector)
        {
            throw std::runtime_error("Trying to call Set on a datum with the wrong data type!");
        }

		m_data.v[index] = value;
	}

	void Datum::Set(glm::mat4 value, size_t index)
	{
        if (index >= m_size)
        {
            throw std::runtime_error("Trying to call Set with an out of bounds index!");
        }

        if (m_type != DatumType::Matrix)
        {
            throw std::runtime_error("Trying to call Set on a datum with the wrong data type!");
        }

		m_data.m[index] = value;
	}

	void Datum::Set(RTTI* value, size_t index)
	{
        if (index >= m_size)
        {
            throw std::runtime_error("Trying to call Set with an out of bounds index!");
        }

        if (m_type != DatumType::RTTI)
        {
            throw std::runtime_error("Trying to call Set on a datum with the wrong data type!");
        }

		m_data.r[index] = value;
	}

    void Datum::Set(Scope* value, size_t index)
    {
        if (index >= m_size)
        {
            throw std::runtime_error("Trying to call Set with an out of bounds index!");
        }

        if (m_type != DatumType::Table)
        {
            throw std::runtime_error("Trying to call Set on a datum with the wrong data type!");
        }

        if (value == nullptr)
        {
            throw std::runtime_error("Cannot set a Scope pointer to a nullptr!");
        }

        m_data.sc[index] = value;
    }
#pragma endregion

#pragma region PushBack
	void Datum::PushBack(int value)
	{
        if (m_externalStorage)
        {
            throw std::runtime_error("Trying to call PushBack() on external storage!");
        }

        if (m_type != DatumType::Integer)
        {
            throw std::runtime_error("Trying to call PushBack on a datum with the wrong data type!");
        }

		// We are out of room
		if (m_size == m_capacity)
		{
			Reserve(m_capacity + 1);
		}

		new(m_data.i + m_size) int(value);
		++m_size;
	}

	void Datum::PushBack(const std::string& value)
	{
        if (m_externalStorage)
        {
            throw std::runtime_error("Trying to call PushBack() on external storage!");
        }

        if (m_type != DatumType::String)
        {
            throw std::runtime_error("Trying to call PushBack on a datum with the wrong data type!");
        }

        // We are out of room
        if (m_size == m_capacity)
        {
            Reserve(m_capacity + 1);
        }

        new(m_data.s + m_size) std::string(value);
        ++m_size;
	}

	void Datum::PushBack(float value)
	{
        if (m_externalStorage)
        {
            throw std::runtime_error("Trying to call PushBack() on external storage!");
        }

        if (m_type != DatumType::Float)
        {
            throw std::runtime_error("Trying to call PushBack on a datum with the wrong data type!");
        }

        // We are out of room
        if (m_size == m_capacity)
        {
            Reserve(m_capacity + 1);
        }

        new(m_data.f + m_size) float(value);
        ++m_size;
	}

	void Datum::PushBack(glm::vec4 value)
	{
        if (m_externalStorage)
        {
            throw std::runtime_error("Trying to call PushBack() on external storage!");
        }

        if (m_type != DatumType::Vector)
        {
            throw std::runtime_error("Trying to call PushBack on a datum with the wrong data type!");
        }

        // We are out of room
        if (m_size == m_capacity)
        {
            Reserve(m_capacity + 1);
        }

        new(m_data.v + m_size) glm::vec4(value);
        ++m_size;
	}

	void Datum::PushBack(glm::mat4 value)
	{
        if (m_externalStorage)
        {
            throw std::runtime_error("Trying to call PushBack() on external storage!");
        }

        if (m_type != DatumType::Matrix)
        {
            throw std::runtime_error("Trying to call PushBack on a datum with the wrong data type!");
        }

        // We are out of room
        if (m_size == m_capacity)
        {
            Reserve(m_capacity + 1);
        }

        new(m_data.m + m_size) glm::mat4(value);
        ++m_size;
	}

	void Datum::PushBack(RTTI* value)
	{
        if (m_externalStorage)
        {
            throw std::runtime_error("Trying to call PushBack() on external storage!");
        }

        if (m_type != DatumType::RTTI)
        {
            throw std::runtime_error("Trying to call PushBack on a datum with the wrong data type!");
        }

        // We are out of room
        if (m_size == m_capacity)
        {
            Reserve(m_capacity + 1);
        }

        new(m_data.r + m_size) RTTI*(value);
        ++m_size;
	}

	void Datum::PushBack(std::string&& value)
	{
        if (m_externalStorage)
        {
            throw std::runtime_error("Trying to call PushBack() on external storage!");
        }

        if (m_type != DatumType::String)
        {
            throw std::runtime_error("Trying to call Set on a datum with the wrong data type!");
        }

        // We are out of room
        if (m_size == m_capacity)
        {
            Reserve(m_capacity + 1);
        }

        new(m_data.s + m_size) std::string(std::forward<std::string>(value));
        ++m_size;
	}
#pragma endregion

#pragma region Find
	size_t Datum::Find(int value) const
	{
		if (m_type != DatumType::Integer)
		{
			throw std::runtime_error("Trying to find an integer in a datum whose type isn't integer!");
		}

        size_t i;
		for (i = 0; i < m_size; ++i)
		{
			if (m_data.i[i] == value)
			{
				break;
			}
		}

		return i;
	}

	size_t Datum::Find(std::string value) const
	{
        if (m_type != DatumType::String)
        {
            throw std::runtime_error("Trying to find an integer in a datum whose type isn't integer!");
        }

        size_t i;
        for (i = 0; i < m_size; ++i)
        {
            if (m_data.s[i] == value)
            {
                break;
            }
        }

        return i;
	}

	size_t Datum::Find(float value) const
	{
        if (m_type != DatumType::Float)
        {
            throw std::runtime_error("Trying to find an integer in a datum whose type isn't integer!");
        }

        size_t i;
        for (i = 0; i < m_size; ++i)
        {
            if (m_data.f[i] == value)
            {
                break;
            }
        }

        return i;
	}

	size_t Datum::Find(glm::vec4 value) const
	{
        if (m_type != DatumType::Vector)
        {
            throw std::runtime_error("Trying to find an integer in a datum whose type isn't integer!");
        }

        size_t i;
        for (i = 0; i < m_size; ++i)
        {
            if (m_data.v[i] == value)
            {
                break;
            }
        }

        return i;
	}

	size_t Datum::Find(glm::mat4 value) const
	{
        if (m_type != DatumType::Matrix)
        {
            throw std::runtime_error("Trying to find an integer in a datum whose type isn't integer!");
        }

        size_t i;
        for (i = 0; i < m_size; ++i)
        {
            if (m_data.m[i] == value)
            {
                break;
            }
        }

        return i;
	}

	size_t Datum::Find(RTTI* value) const
	{
        if (m_type != DatumType::RTTI)
        {
            throw std::runtime_error("Trying to find an integer in a datum whose type isn't integer!");
        }

        size_t i;
        for (i = 0; i < m_size; ++i)
        {
            if (m_data.r[i] == value)
            {
                break;
            }
        }

        return i;
	}
#pragma endregion

#pragma region Remove
	bool Datum::Remove(int value)
	{
		size_t index = Find(value);

		if (index < m_size)
		{
			RemoveAt(index);
			return true;
		}

		return false;
	}

	bool Datum::Remove(std::string value)
	{
        size_t index = Find(value);

        if (index < m_size)
        {
            RemoveAt(index);
            return true;
        }

        return false;
	}

	bool Datum::Remove(float value)
	{
        size_t index = Find(value);

        if (index < m_size)
        {
            RemoveAt(index);
            return true;
        }

        return false;
	}

	bool Datum::Remove(glm::vec4 value)
	{
        size_t index = Find(value);

        if (index < m_size)
        {
            RemoveAt(index);
            return true;
        }

        return false;
	}

	bool Datum::Remove(glm::mat4 value)
	{
        size_t index = Find(value);

        if (index < m_size)
        {
            RemoveAt(index);
            return true;
        }

        return false;
	}

	bool Datum::Remove(RTTI* value)
	{
        size_t index = Find(value);

        if (index < m_size)
        {
            RemoveAt(index);
            return true;
        }

        return false;
	}
#pragma endregion

	bool Datum::RemoveAt(size_t index)
	{
        using namespace std;

        if (m_externalStorage)
        {
            throw std::runtime_error("Trying to call RemoveAt() on external storage!");
        }

		if (index >= m_size)
		{
			throw std::runtime_error("Trying to remove an out of bounds index!");
		}

		if (m_type == DatumType::String)
		{
            m_data.s[index].~string();
		}

		size_t typeSize = GetTypeSize();
		size_t numBytes = (m_size - 1 - index) * typeSize;
		
        char* elementToRemove = &m_data.c[index * typeSize];
        char* startOfData = &m_data.c[(index * typeSize) + typeSize];

		memmove(elementToRemove, startOfData, numBytes);
		--m_size;

		return true;
	}

	void Datum::PopBack()
    {
		using namespace std;

        if (m_externalStorage)
        {
            throw std::runtime_error("Trying to call PopBack() on external storage!");
        }

		if (m_size > 0)
		{
            if (m_type == DatumType::String)
            {
				m_data.s[m_size - 1].~string();
            }

			--m_size;
		}
    }

    void Datum::SetType(DatumType type)
    {
        if (m_type != DatumType::Unknown && type != m_type)
        {
            throw std::runtime_error("You cannot change a datum's type once set.");
        }

        m_type = type;
    }

	size_t Datum::Size() const
	{
		return m_size;
	}

	size_t Datum::Capacity() const
	{
		return m_capacity;
	}

	Datum::DatumType Datum::Type() const
	{
		return m_type;
	}

	void Datum::Resize(size_t size)
	{
        if (m_externalStorage)
        {
            throw std::runtime_error("Trying to call resize on external storage!");
        }

        if (m_type == DatumType::Table)
        {
            throw std::runtime_error("Cannot call resize on a Datum of type table!");
        }

		if (size < m_size)
		{
			if (m_type == DatumType::String)
			{
				using namespace std;
				for (size_t i = size; i < m_size; ++i)
				{
					m_data.s[i].~string();
				}
			}
		}
		else
		{
			Reserve(size);
			Datum::CreateFunction func = m_CreateFunctions[static_cast<uint32_t>(m_type)];

			for (size_t i = m_size; i < size; ++i)
			{
				(this->*func)(i);
			}
		}

		m_size = size;
	}

	void Datum::Reserve(size_t capacity)
	{
        if (capacity > m_capacity)
        {
            if (m_externalStorage)
            {
                throw std::runtime_error("Trying to call reserve on external storage!");
            }

            if (m_type == DatumType::Unknown)
            {
                throw std::runtime_error("Can't reserve without a type");
            }

            size_t size = GetTypeSize();

            void* data = realloc(m_data.vp, capacity * size);
            assert(data != nullptr);
            m_data.vp = data;

            m_capacity = capacity;
        }
	}

	void Datum::Clear()
	{
		using namespace std;

        if (m_externalStorage)
        {
            throw std::runtime_error("Trying to call clear on external storage!");
        }

        if (m_type == DatumType::String)
        {
            for (size_t i = 0; i < m_size; ++i)
            {
                m_data.s[i].~string();
            }
        }

        m_size = 0;
	}

    std::string Datum::ToString(size_t index) const
    {
        std::string outputString;

        switch (m_type)
        {
        case FieaGameEngine::Datum::DatumType::Integer:
            outputString = std::to_string(Get<int>(index));
            break;
        case FieaGameEngine::Datum::DatumType::String:
            outputString = Get<std::string>(index);
            break;
        case FieaGameEngine::Datum::DatumType::Float:
            outputString = std::to_string(Get<float>(index));
            break;
        case FieaGameEngine::Datum::DatumType::Vector:
            outputString = glm::to_string(Get<glm::vec4>(index));
            break;
        case FieaGameEngine::Datum::DatumType::Matrix:
            outputString = glm::to_string(Get<glm::mat4>(index));
            break;
        case FieaGameEngine::Datum::DatumType::RTTI:
            outputString = Get<RTTI*>(index)->ToString();
            break;
        default:
            break;
        }

        return outputString;
    }

    void Datum::SetFromString(std::string& input, size_t index)
    {
        if (m_type == DatumType::Unknown || m_type == DatumType::RTTI || m_type == DatumType::Table)
        {
            throw std::runtime_error("Can't use SetFromString on types Unknown, RTTI, or Scope!");
        }

        Datum::SetFromStringFunction func = m_SetFromStringFunctions[static_cast<uint32_t>(m_type)];
        (this->*func)(input, index);
    }

    void Datum::PushBackFromString(std::string& input)
    {
        if (m_type == DatumType::Unknown || m_type == DatumType::RTTI || m_type == DatumType::Table)
        {
            throw std::runtime_error("Can't use PushBackFromString on types Unknown, RTTI, or Scope!");
        }

        Datum::PushFromStringFunction func = m_PushFromStringFunctions[static_cast<uint32_t>(m_type)];
        (this->*func)(input);
    }

    bool Datum::IsExternal() const
    {
        return m_externalStorage;
    }

    Scope& Datum::operator[](size_t index)
    {
        return Get<Scope>(index);
    }

    void Datum::PushBack(Scope& value)
    {
        assert(!m_externalStorage);
        assert(m_type == DatumType::Table);

        // We are out of room
        if (m_size == m_capacity)
        {
            Reserve(m_capacity + 1);
        }

        new(m_data.sc + m_size) Scope* (&value);
        ++m_size;
    }

    size_t Datum::GetTypeSize() const
	{
		return m_DataSizes[static_cast<uint32_t>(m_type)];
	}

#pragma region SetStorage
    void Datum::SetStorage(int* externalArray, size_t numElements)
    {
        if (m_type != DatumType::Integer && m_type != DatumType::Unknown)
        {
            throw std::runtime_error("Using SetStorage with the wrong data type!");
        }

        SetStorage((void*)externalArray, numElements);

        m_type = DatumType::Integer;
    }

    void Datum::SetStorage(std::string* externalArray, size_t numElements)
    {
        if (m_type != DatumType::String && m_type != DatumType::Unknown)
        {
            throw std::runtime_error("Using SetStorage with the wrong data type!");
        }

        SetStorage((void*)externalArray, numElements);

        m_type = DatumType::String;
    }

    void Datum::SetStorage(float* externalArray, size_t numElements)
    {
        if (m_type != DatumType::Float && m_type != DatumType::Unknown)
        {
            throw std::runtime_error("Using SetStorage with the wrong data type!");
        }

        SetStorage((void*)externalArray, numElements);

        m_type = DatumType::Float;
    }

    void Datum::SetStorage(glm::vec4* externalArray, size_t numElements)
    {
        if (m_type != DatumType::Vector && m_type != DatumType::Unknown)
        {
            throw std::runtime_error("Using SetStorage with the wrong data type!");
        }

        SetStorage((void*)externalArray, numElements);

        m_type = DatumType::Vector;
    }

    void Datum::SetStorage(glm::mat4* externalArray, size_t numElements)
    {
        if (m_type != DatumType::Matrix && m_type != DatumType::Unknown)
        {
            throw std::runtime_error("Using SetStorage with the wrong data type!");
        }

        SetStorage((void*)externalArray, numElements);

        m_type = DatumType::Matrix;
    }

    void Datum::SetStorage(RTTI** externalArray, size_t numElements)
    {
        if (m_type != DatumType::RTTI && m_type != DatumType::Unknown)
        {
            throw std::runtime_error("Using SetStorage with the wrong data type!");
        }

        SetStorage((void*)externalArray, numElements);

        m_type = DatumType::RTTI;
    }

    void Datum::SetStorage(void* externalArray, size_t numElements)
    {
        // If we already have internal memory
        if (m_capacity > 0 && !m_externalStorage)
        {
            throw std::runtime_error("Using SetStorage on a datum that already has memory!");
        }

        m_externalStorage = true;
        m_data.vp = externalArray;
        m_size = numElements;
        m_capacity = numElements;
    }
#pragma endregion

#pragma region CreateFunctions
    inline void Datum::CreateInteger(size_t index)
    {
		new(m_data.i + index)int(0);
    }

	inline void Datum::CreateString(size_t index)
	{
		new(m_data.s + index)std::string();
	}

	inline void Datum::CreateFloat(size_t index)
	{
		new(m_data.f + index)float(0.0f);
	}

	inline void Datum::CreateVector(size_t index)
	{
		new(m_data.v + index)glm::vec4(0);
	}

	inline void Datum::CreateMatrix(size_t index)
	{
		new(m_data.m + index)glm::mat4(0);
	}

	inline void Datum::CreatePointer(size_t index)
	{
		new(m_data.r + index)RTTI*(nullptr);
	}
#pragma endregion

#pragma region CompareFunctions
    bool Datum::CompareData(const Datum& other) const
    {
        size_t numBytes = m_size * GetTypeSize();
        return (memcmp(m_data.vp, other.m_data.vp, numBytes) == 0);
    }

    bool Datum::CompareStrings(const Datum& other) const
    {
        for (size_t i = 0; i < m_size; ++i)
        {
            if (m_data.s[i] != other.m_data.s[i])
            {
                return false;
            }
        }

        return true;
    }

    bool Datum::ComparePointers(const Datum& other) const
    {
        for (size_t i = 0; i < m_size; ++i)
        {
            if (!m_data.r[i]->Equals(other.m_data.r[i]))
            {
                return false;
            }
        }
        return true;
    }
#pragma endregion

#pragma region SetFromStringFunctions
    void Datum::SetIntFromString(std::string& input, size_t index)
    {
        int value = std::stoi(input);
        Set(value, index);
    }

    void Datum::SetStringFromString(std::string& input, size_t index)
    {
        Set(input, index);
    }

    void Datum::SetFloatFromString(std::string& input, size_t index)
    {
        float value = std::stof(input);
        Set(value, index);
    }

    void Datum::SetVectorFromString(std::string& input, size_t index)
    {
        glm::vec4 vec;
        
        sscanf_s(input.c_str(), "vec4(%f, %f, %f, %f)", &vec.x, &vec.y, &vec.z, &vec.w);

        Set(vec, index);
    }

    void Datum::SetMatrixFromString(std::string& input, size_t index)
    {
        glm::mat4 mat;

        sscanf_s(input.c_str(), "mat4x4((%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f))",
                                        &(mat[0].x), &(mat[0].y), &(mat[0].z), &(mat[0].w),
                                        &(mat[1].x), &(mat[1].y), &(mat[1].z), &(mat[1].w),
                                        &(mat[2].x), &(mat[2].y), &(mat[2].z), &(mat[2].w),
                                        &(mat[3].x), &(mat[3].y), &(mat[3].z), &(mat[3].w));

        Set(mat, index);
    }
    
#pragma endregion

#pragma region PushFromStringFunctions
    void Datum::PushIntFromString(std::string& input)
    {
        int value = std::stoi(input);
        PushBack(value);
    }

    void Datum::PushStringFromString(std::string& input)
    {
        PushBack(input);
    }

    void Datum::PushFloatFromString(std::string& input)
    {
        float value = std::stof(input);
        PushBack(value);
    }

    void Datum::PushVectorFromString(std::string& input)
    {
        glm::vec4 vec;
        sscanf_s(input.c_str(), "vec4(%f,%f,%f,%f)", &vec.x, &vec.y, &vec.z, &vec.w);
        PushBack(vec);
    }

    void Datum::PushMatrixFromString(std::string& input)
    {
        glm::mat4 mat;

        sscanf_s(input.c_str(), "mat4x4((%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f))",
            &(mat[0].x), &(mat[0].y), &(mat[0].z), &(mat[0].w),
            &(mat[1].x), &(mat[1].y), &(mat[1].z), &(mat[1].w),
            &(mat[2].x), &(mat[2].y), &(mat[2].z), &(mat[2].w),
            &(mat[3].x), &(mat[3].y), &(mat[3].z), &(mat[3].w));

        PushBack(mat);
    }
#pragma endregion
}
