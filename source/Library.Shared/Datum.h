#pragma once

#include <glm/glm.hpp>
#include "RTTI.h"
#include <map>

namespace FieaGameEngine
{

    class Scope;
    class Attributed;
	/// <summary>
	/// Datum class that holds one or more pieces of data. Supported types are int,
	/// std::string, float, glm::vec4, glm::mat4, and RTTI*
	/// </summary>
	class Datum final
	{
	public:
        friend Scope;
        friend Attributed;

		/// <summary>
		/// Enum that defines the type of the datum. A datum can only ever have one
		/// type
		/// </summary>
		enum class DatumType
		{
			Integer = 0,
			String = 1,
			Float = 2,
			Vector = 3,
			Matrix = 4,
			RTTI = 5,
            Table = 6,
			Unknown
		};

        inline static const std::map<std::string, DatumType> m_nameToType = {
            {"integer", DatumType::Integer},
            {"float", DatumType::Float},
            {"string", DatumType::String},
            {"vector", DatumType::Vector},
            {"matrix", DatumType::Matrix},
            {"pointer", DatumType::RTTI},
            {"table", DatumType::Table},
            {"unknown", DatumType::Unknown}
        };

		/// <summary>
		/// Constructor. Doesn't allocate any space by default.
		/// </summary>
		/// <param name="type"> Optional parameter that defines what type the datum 
		/// should be </param>
		Datum(DatumType type = DatumType::Unknown);

		/// <summary>
		/// Copy constructor
		/// </summary>
		/// <param name="other"> The other datum to construct this one from</param>
		Datum(const Datum& other);

		/// <summary>
		/// Copy assignment operator
		/// </summary>
		/// <param name="other"> The other datum to copy into this one </param>
		/// <returns> A reference to this newly changed datum </returns>
		Datum& operator=(const Datum& other);

		/// <summary>
		/// Move constructor
		/// </summary>
		/// <param name="other"> The other datum to move into this one </param>
		Datum(Datum&& other) noexcept;

		/// <summary>
		/// Move assignment operator 
		/// </summary>
		/// <param name="other"> The other datum to move into this one </param>
		/// <returns></returns>
		Datum& operator=(Datum&& other) noexcept;

		/// <summary>
		/// Destructor
		/// </summary>
		~Datum();

		#pragma region Assignment Overloads
		/// <summary>
		/// Scalar assignment operator. Doesn't have any effect on any elements other
		/// than the first one. Creates one element if the datum is empty.
		/// </summary>
		/// <param name="value"> The int to assign this Datum to</param>
		/// <returns> A reference to this newly assigned datum</returns>
		Datum& operator=(int value);

        /// <summary>
        /// Scalar assignment operator. Doesn't have any effect on any elements other
        /// than the first one. Creates one element if the datum is empty.
        /// </summary>
        /// <param name="value"> The std::string to assign this Datum to</param>
        /// <returns> A reference to this newly assigned datum</returns>
		Datum& operator=(std::string value);

        /// <summary>
        /// Scalar assignment operator. Doesn't have any effect on any elements other
        /// than the first one. Creates one element if the datum is empty.
        /// </summary>
        /// <param name="value"> The float to assign this Datum to</param>
        /// <returns> A reference to this newly assigned datum</returns>
		Datum& operator=(float value);

        /// <summary>
        /// Scalar assignment operator. Doesn't have any effect on any elements other
        /// than the first one. Creates one element if the datum is empty.
        /// </summary>
        /// <param name="value"> The glm::vec4 to assign this Datum to</param>
        /// <returns> A reference to this newly assigned datum</returns>
		Datum& operator=(glm::vec4 value);

        /// <summary>
        /// Scalar assignment operator. Doesn't have any effect on any elements other
        /// than the first one. Creates one element if the datum is empty.
        /// </summary>
        /// <param name="value"> The glm::mat4 to assign this Datum to</param>
        /// <returns> A reference to this newly assigned datum</returns>
		Datum& operator=(glm::mat4 value);

        /// <summary>
        /// Scalar assignment operator. Doesn't have any effect on any elements other
        /// than the first one. Creates one element if the datum is empty.
        /// </summary>
        /// <param name="value"> The RTTI* to assign this Datum to</param>
        /// <returns> A reference to this newly assigned datum</returns>
		Datum& operator=(RTTI* value);

        Datum& operator=(Scope& value);
		#pragma endregion

		#pragma region Equality
		/// <summary>
		/// Compares two datums to see if they are equal. Checks if everything
		/// other than capacity are equivalent.
		/// </summary>
		/// <param name="other"> The other datum to compare to </param>
		/// <returns> True if the datums are equal, false otherwise </returns>
		bool operator==(const Datum& other) const;

        /// <summary>
        /// Compares two datums to see if they aren't equal. Checks if everything
        /// other than capacity are equivalent.
        /// </summary>
        /// <param name="other"> The other datum to compare to </param>
        /// <returns> True if the datums aren't equal, false otherwise </returns>
		bool operator!=(const Datum& other) const;

        /// <summary>
        /// Compares a Datum to a scalar to see if they are equal. Only checks against
        /// the first element in the Datum if there are multiple.
        /// </summary>
        /// <param name="other"> A reference to the int to compare to </param>
        /// <returns> True if the datum is equal to the scalar, false otherwise </returns>
		bool operator==(const int& other) const;

        /// <summary>
        /// Compares a Datum to a scalar to see if they are equal. Only checks against
        /// the first element in the Datum if there are multiple.
        /// </summary>
        /// <param name="other"> A reference to the std::string to compare to </param>
        /// <returns> True if the datum is equal to the scalar, false otherwise </returns>
		bool operator==(const std::string& other) const;

        /// <summary>
        /// Compares a Datum to a scalar to see if they are equal. Only checks against
        /// the first element in the Datum if there are multiple.
        /// </summary>
        /// <param name="other"> A reference to the float to compare to </param>
        /// <returns> True if the datum is equal to the scalar, false otherwise </returns>
		bool operator==(const float& other) const;

        /// <summary>
        /// Compares a Datum to a scalar to see if they are equal. Only checks against
        /// the first element in the Datum if there are multiple.
        /// </summary>
        /// <param name="other"> A reference to the glm::vec4 to compare to </param>
        /// <returns> True if the datum is equal to the scalar, false otherwise </returns>
		bool operator==(const glm::vec4& other) const;

        /// <summary>
        /// Compares a Datum to a scalar to see if they are equal. Only checks against
        /// the first element in the Datum if there are multiple.
        /// </summary>
        /// <param name="other"> A reference to the glm::mat4 to compare to </param>
        /// <returns> True if the datum is equal to the scalar, false otherwise </returns>
		bool operator==(const glm::mat4& other) const;

        /// <summary>
        /// Compares a Datum to a scalar to see if they are equal. Only checks against
        /// the first element in the Datum if there are multiple.
        /// </summary>
        /// <param name="other"> A reference to the RTTI* to compare to </param>
        /// <returns> True if the datum is equal to the scalar, false otherwise </returns>
		bool operator==(RTTI* const& other) const;

        /// <summary>
        /// Compares a Datum to a scalar to see if they are not equal. Only checks against
        /// the first element in the Datum if there are multiple.
        /// </summary>
        /// <param name="other"> A reference to the int to compare to </param>
        /// <returns> True if the datum is not equal to the scalar, false otherwise </returns>
        bool operator!=(const int& other) const;

        /// <summary>
        /// Compares a Datum to a scalar to see if they are not equal. Only checks against
        /// the first element in the Datum if there are multiple.
        /// </summary>
        /// <param name="other"> A reference to the std::string to compare to </param>
        /// <returns> True if the datum is not equal to the scalar, false otherwise </returns>
        bool operator!=(const std::string& other) const;

        /// <summary>
        /// Compares a Datum to a scalar to see if they are not equal. Only checks against
        /// the first element in the Datum if there are multiple.
        /// </summary>
        /// <param name="other"> A reference to the float to compare to </param>
        /// <returns> True if the datum is not equal to the scalar, false otherwise </returns>
        bool operator!=(const float& other) const;

        /// <summary>
        /// Compares a Datum to a scalar to see if they are not equal. Only checks against
        /// the first element in the Datum if there are multiple.
        /// </summary>
        /// <param name="other"> A reference to the glm::vec4 to compare to </param>
        /// <returns> True if the datum is not equal to the scalar, false otherwise </returns>
        bool operator!=(const glm::vec4& other) const;

        /// <summary>
        /// Compares a Datum to a scalar to see if they are not equal. Only checks against
        /// the first element in the Datum if there are multiple.
        /// </summary>
        /// <param name="other"> A reference to the glm::mat4 to compare to </param>
        /// <returns> True if the datum is not equal to the scalar, false otherwise </returns>
        bool operator!=(const glm::mat4& other) const;

        /// <summary>
        /// Compares a Datum to a scalar to see if they are not equal. Only checks against
        /// the first element in the Datum if there are multiple.
        /// </summary>
        /// <param name="other"> A reference to the RTTI* to compare to </param>
        /// <returns> True if the datum is not equal to the scalar, false otherwise </returns>
        bool operator!=(RTTI* const& other) const;
		#pragma endregion

		#pragma region Get
		/// <summary>
		/// Template for non-const Get function. Only specializations of this function should
		/// ever be used.
		/// </summary>
        template<typename T> T& Get(size_t index = 0);

		/// <summary>
		/// Get template specialization for int
		/// </summary>
		/// <param name="index"> The index to get </param>
		/// <returns> A reference to the int at index </returns>
		/// <exception cref="std::runtime_error"> Throws if index is out of bounds 
		/// or type is doesn't match </exception>
        template<> int& Get(size_t index);

        /// <summary>
        /// Get template specialization for std::string
        /// </summary>
        /// <param name="index"> The index to get </param>
        /// <returns> A reference to the std::string at index </returns>
        /// <exception cref="std::runtime_error"> Throws if index is out of bounds 
        /// or type is doesn't match </exception>
		template<> std::string& Get(size_t index);

        /// <summary>
        /// Get template specialization for float
        /// </summary>
        /// <param name="index"> The index to get </param>
        /// <returns> A reference to the float at index </returns>
        /// <exception cref="std::runtime_error"> Throws if index is out of bounds 
        /// or type is doesn't match </exception>
        template<> float& Get(size_t index);

        /// <summary>
        /// Get template specialization for glm::vec4
        /// </summary>
        /// <param name="index"> The index to get </param>
        /// <returns> A reference to the glm::vec4 at index </returns>
        /// <exception cref="std::runtime_error"> Throws if index is out of bounds 
        /// or type is doesn't match </exception>
        template<> glm::vec4& Get(size_t index);

        /// <summary>
        /// Get template specialization for glm::mat4
        /// </summary>
        /// <param name="index"> The index to get </param>
        /// <returns> A reference to the glm::mat4 at index </returns>
        /// <exception cref="std::runtime_error"> Throws if index is out of bounds 
        /// or type is doesn't match </exception>
        template<> glm::mat4& Get(size_t index);

        /// <summary>
        /// Get template specialization for RTTI*
        /// </summary>
        /// <param name="index"> The index to get </param>
        /// <returns> A reference to the RTTI* at index </returns>
        /// <exception cref="std::runtime_error"> Throws if index is out of bounds 
        /// or type is doesn't match </exception>
        template<> RTTI* & Get(size_t index);

        template<> Scope& Get(size_t index);

        /// <summary>
        /// Template for const Get function. Only specializations of this function should
        /// ever be used.
        /// </summary>
        template<typename T> const T& Get(size_t index = 0) const;

        /// <summary>
        /// Get template specialization for int
        /// </summary>
        /// <param name="index"> The index to get </param>
        /// <returns> A const reference to the int at index </returns>
        /// <exception cref="std::runtime_error"> Throws if index is out of bounds 
        /// or type is doesn't match </exception>
        template<> const int& Get(size_t index) const;

        /// <summary>
        /// Get template specialization for std::string
        /// </summary>
        /// <param name="index"> The index to get </param>
        /// <returns> A const reference to the std::string at index </returns>
        /// <exception cref="std::runtime_error"> Throws if index is out of bounds 
        /// or type is doesn't match </exception>
        template<> const std::string& Get(size_t index) const;

        /// <summary>
        /// Get template specialization for float
        /// </summary>
        /// <param name="index"> The index to get </param>
        /// <returns> A const reference to the float at index </returns>
        /// <exception cref="std::runtime_error"> Throws if index is out of bounds 
        /// or type is doesn't match </exception>
        template<> const float& Get(size_t index) const;

        /// <summary>
        /// Get template specialization for glm::vec4
        /// </summary>
        /// <param name="index"> The index to get </param>
        /// <returns> A const reference to the glm::vec4 at index </returns>
        /// <exception cref="std::runtime_error"> Throws if index is out of bounds 
        /// or type is doesn't match </exception>
        template<> const glm::vec4& Get(size_t index) const;

        /// <summary>
        /// Get template specialization for glm::mat4
        /// </summary>
        /// <param name="index"> The index to get </param>
        /// <returns> A const reference to the glm::mat4 at index </returns>
        /// <exception cref="std::runtime_error"> Throws if index is out of bounds 
        /// or type is doesn't match </exception>
        template<> const glm::mat4& Get(size_t index) const;

        /// <summary>
        /// Get template specialization for RTTI*
        /// </summary>
        /// <param name="index"> The index to get </param>
        /// <returns> A const reference to the RTTI* at index </returns>
        /// <exception cref="std::runtime_error"> Throws if index is out of bounds 
        /// or type is doesn't match </exception>
        template<> RTTI* const& Get(size_t index) const;

        template<> const Scope& Get(size_t index) const;
		#pragma endregion

		#pragma region Set
        /// <summary>
        /// Sets the given index in the Datum to the given value
        /// </summary>
        /// <param name="value"> The value to set index to </param>
        /// <param name="index"> The index to use (default 0) </param>
        /// <exception cref="std::runtime_error"> Throws if index is out of bounds 
        /// or type is doesn't match </exception>
		void Set(int value, size_t index = 0);

        /// <summary>
        /// Sets the given index in the Datum to the given value
        /// </summary>
        /// <param name="value"> The value to set index to </param>
        /// <param name="index"> The index to use (default 0) </param>
        /// <exception cref="std::runtime_error"> Throws if index is out of bounds 
        /// or type is doesn't match </exception>
		void Set(std::string value, size_t index = 0);

        /// <summary>
        /// Sets the given index in the Datum to the given value
        /// </summary>
        /// <param name="value"> The value to set index to </param>
        /// <param name="index"> The index to use (default 0) </param>
        /// <exception cref="std::runtime_error"> Throws if index is out of bounds 
        /// or type is doesn't match </exception>
		void Set(float value, size_t index = 0);

        /// <summary>
        /// Sets the given index in the Datum to the given value
        /// </summary>
        /// <param name="value"> The value to set index to </param>
        /// <param name="index"> The index to use (default 0) </param>
        /// <exception cref="std::runtime_error"> Throws if index is out of bounds 
        /// or type is doesn't match </exception>
		void Set(glm::vec4 value, size_t index = 0);

        /// <summary>
        /// Sets the given index in the Datum to the given value
        /// </summary>
        /// <param name="value"> The value to set index to </param>
        /// <param name="index"> The index to use (default 0) </param>
        /// <exception cref="std::runtime_error"> Throws if index is out of bounds 
        /// or type is doesn't match </exception>
		void Set(glm::mat4 value, size_t index = 0);

        /// <summary>
        /// Sets the given index in the Datum to the given value
        /// </summary>
        /// <param name="value"> The value to set index to </param>
        /// <param name="index"> The index to use (default 0) </param>
        /// <exception cref="std::runtime_error"> Throws if index is out of bounds 
        /// or type is doesn't match </exception>
		void Set(RTTI* value, size_t index = 0);

        /// <summary>
        /// Sets the given index in the Datum to the given value
        /// </summary>
        /// <param name="value"> The value to set index to </param>
        /// <param name="index"> The index to use (default 0) </param>
        /// <exception cref="std::runtime_error"> Throws if index is out of bounds, type 
        /// doesn't match, or the Scope pointer is nullptr </exception>
        void Set(Scope* value, size_t index = 0);
		#pragma endregion

		#pragma region PushBack
		/// <summary>
		/// Pushes the given value into the back of the Datum
		/// </summary>
		/// <param name="value"> The value to push onto the Datum </param>
        /// <exception cref="std::runtime_error"> Throws if this Datum has external
        /// storage or the type doesn't match </exception>
		void PushBack(int value);

        /// <summary>
        /// Pushes the given value into the back of the Datum
        /// </summary>
        /// <param name="value"> The value to push onto the Datum </param>
        /// <exception cref="std::runtime_error"> Throws if this Datum has external
        /// storage or the type doesn't match </exception>
		void PushBack(const std::string& value);

        /// <summary>
        /// Pushes the given value into the back of the Datum
        /// </summary>
        /// <param name="value"> The value to push onto the Datum </param>
        /// <exception cref="std::runtime_error"> Throws if this Datum has external
        /// storage or the type doesn't match </exception>
		void PushBack(float value);

        /// <summary>
        /// Pushes the given value into the back of the Datum
        /// </summary>
        /// <param name="value"> The value to push onto the Datum </param>
        /// <exception cref="std::runtime_error"> Throws if this Datum has external
        /// storage or the type doesn't match </exception>
		void PushBack(glm::vec4 value);

        /// <summary>
        /// Pushes the given value into the back of the Datum
        /// </summary>
        /// <param name="value"> The value to push onto the Datum </param>
        /// <exception cref="std::runtime_error"> Throws if this Datum has external
        /// storage or the type doesn't match </exception>
		void PushBack(glm::mat4 value);

        /// <summary>
        /// Pushes the given value into the back of the Datum
        /// </summary>
        /// <param name="value"> The value to push onto the Datum </param>
        /// <exception cref="std::runtime_error"> Throws if this Datum has external
        /// storage or the type doesn't match </exception>
		void PushBack(RTTI* value);

        /// <summary>
        /// Pushes the given value into the back of the Datum using move semantics
        /// </summary>
        /// <param name="value"> The std::string to push onto the Datum </param>
        /// <exception cref="std::runtime_error"> Throws if this Datum has external
        /// storage or the type doesn't match </exception>
		void PushBack(std::string&& value);
		#pragma endregion

		#pragma region Front
        /// <summary>
        /// Template of Front function. Only the specializations of this function should
        /// be used. This will throw a static assert if called.
        /// </summary>
        template<typename T> T& Front();

		/// <summary>
		/// Returns the first element of the Datum
		/// </summary>
		/// <returns> A reference to the first element of the Datum</returns>
        /// <exception cref="std::runtime_error"> Throws if this Datum is empty 
        /// or the type doesn't match </exception>
		template<> int& Front();

        /// <summary>
        /// Returns the first element of the Datum
        /// </summary>
        /// <returns> A reference to the first element of the Datum</returns>
        /// <exception cref="std::runtime_error"> Throws if this Datum is empty 
        /// or the type doesn't match </exception>
        template<> std::string& Front();

        /// <summary>
        /// Returns the first element of the Datum
        /// </summary>
        /// <returns> A reference to the first element of the Datum</returns>
        /// <exception cref="std::runtime_error"> Throws if this Datum is empty 
        /// or the type doesn't match </exception>
        template<> float& Front();

        /// <summary>
        /// Returns the first element of the Datum
        /// </summary>
        /// <returns> A reference to the first element of the Datum</returns>
        /// <exception cref="std::runtime_error"> Throws if this Datum is empty 
        /// or the type doesn't match </exception>
        template<> glm::vec4& Front();

        /// <summary>
        /// Returns the first element of the Datum
        /// </summary>
        /// <returns> A reference to the first element of the Datum</returns>
        /// <exception cref="std::runtime_error"> Throws if this Datum is empty 
        /// or the type doesn't match </exception>
        template<> glm::mat4& Front();

        /// <summary>
        /// Returns the first element of the Datum
        /// </summary>
        /// <returns> A reference to the first element of the Datum</returns>
        /// <exception cref="std::runtime_error"> Throws if this Datum is empty 
        /// or the type doesn't match </exception>
        template<> RTTI*& Front();

        template<> Scope& Front();

        /// <summary>
        /// Template of const Front function. Only the specializations of this function should
        /// be used. This will throw a static assert if called.
        /// </summary>
		template<typename T> const T& Front() const;

        /// <summary>
        /// Returns the first element of the Datum
        /// </summary>
        /// <returns> A const reference to the first element of the Datum</returns>
        /// <exception cref="std::runtime_error"> Throws if this Datum is empty 
        /// or the type doesn't match </exception>
        template<> const int& Front() const;

        /// <summary>
        /// Returns the first element of the Datum
        /// </summary>
        /// <returns> A const reference to the first element of the Datum</returns>
        /// <exception cref="std::runtime_error"> Throws if this Datum is empty 
        /// or the type doesn't match </exception>
        template<> const std::string& Front() const;

        /// <summary>
        /// Returns the first element of the Datum
        /// </summary>
        /// <returns> A const reference to the first element of the Datum</returns>
        /// <exception cref="std::runtime_error"> Throws if this Datum is empty 
        /// or the type doesn't match </exception>
        template<> const float& Front() const;

        /// <summary>
        /// Returns the first element of the Datum
        /// </summary>
        /// <returns> A const reference to the first element of the Datum</returns>
        /// <exception cref="std::runtime_error"> Throws if this Datum is empty 
        /// or the type doesn't match </exception>
        template<> const glm::vec4& Front() const;

        /// <summary>
        /// Returns the first element of the Datum
        /// </summary>
        /// <returns> A const reference to the first element of the Datum</returns>
        /// <exception cref="std::runtime_error"> Throws if this Datum is empty 
        /// or the type doesn't match </exception>
        template<> const glm::mat4& Front() const;

        /// <summary>
        /// Returns the first element of the Datum
        /// </summary>
        /// <returns> A const reference to the first element of the Datum</returns>
        /// <exception cref="std::runtime_error"> Throws if this Datum is empty 
        /// or the type doesn't match </exception>
		template<> RTTI* const& Front() const;

        template<> const Scope& Front() const;
        
		#pragma endregion

		#pragma region Back
        /// <summary>
        /// Template of Back function. Only the specializations of this function should
        /// be used. This will throw a static assert if called.
        /// </summary>
		template<typename T> T& Back();

		/// <summary>
		/// Returns the last element of the Datum
		/// </summary>
		/// <returns> A reference to the last element of the Datum </returns>
        /// <exception cref="std::runtime_error"> Throws if this Datum is empty 
        /// or the type doesn't match </exception>
        template<> int& Back();

        /// <summary>
        /// Returns the last element of the Datum
        /// </summary>
        /// <returns> A reference to the last element of the Datum </returns>
        /// <exception cref="std::runtime_error"> Throws if this Datum is empty 
        /// or the type doesn't match </exception>
        template<> std::string& Back();

        /// <summary>
        /// Returns the last element of the Datum
        /// </summary>
        /// <returns> A reference to the last element of the Datum </returns>
        /// <exception cref="std::runtime_error"> Throws if this Datum is empty 
        /// or the type doesn't match </exception>
        template<> float& Back();

        /// <summary>
        /// Returns the last element of the Datum
        /// </summary>
        /// <returns> A reference to the last element of the Datum </returns>
        /// <exception cref="std::runtime_error"> Throws if this Datum is empty 
        /// or the type doesn't match </exception>
        template<> glm::vec4& Back();

        /// <summary>
        /// Returns the last element of the Datum
        /// </summary>
        /// <returns> A reference to the last element of the Datum </returns>
        /// <exception cref="std::runtime_error"> Throws if this Datum is empty 
        /// or the type doesn't match </exception>
        template<> glm::mat4& Back();

        /// <summary>
        /// Returns the last element of the Datum
        /// </summary>
        /// <returns> A reference to the last element of the Datum </returns>
        /// <exception cref="std::runtime_error"> Throws if this Datum is empty 
        /// or the type doesn't match </exception>
        template<> RTTI*& Back();

        template<> Scope& Back();

        /// <summary>
        /// Template of const Back function. Only the specializations of this function should
        /// be used. This will throw a static assert if called.
        /// </summary>
		template<typename T> const T& Back() const;

        /// <summary>
        /// Returns the last element of the Datum
        /// </summary>
        /// <returns> A const reference to the last element of the Datum </returns>
        /// <exception cref="std::runtime_error"> Throws if this Datum is empty 
        /// or the type doesn't match </exception>
        template<> const int& Back() const;

        /// <summary>
        /// Returns the last element of the Datum
        /// </summary>
        /// <returns> A const reference to the last element of the Datum </returns>
        /// <exception cref="std::runtime_error"> Throws if this Datum is empty 
        /// or the type doesn't match </exception>
        template<> const std::string& Back() const;

        /// <summary>
        /// Returns the last element of the Datum
        /// </summary>
        /// <returns> A const reference to the last element of the Datum </returns>
        /// <exception cref="std::runtime_error"> Throws if this Datum is empty 
        /// or the type doesn't match </exception>
        template<> const float& Back() const;

        /// <summary>
        /// Returns the last element of the Datum
        /// </summary>
        /// <returns> A const reference to the last element of the Datum </returns>
        /// <exception cref="std::runtime_error"> Throws if this Datum is empty 
        /// or the type doesn't match </exception>
        template<> const glm::vec4& Back() const;

        /// <summary>
        /// Returns the last element of the Datum
        /// </summary>
        /// <returns> A const reference to the last element of the Datum </returns>
        /// <exception cref="std::runtime_error"> Throws if this Datum is empty 
        /// or the type doesn't match </exception>
        template<> const glm::mat4& Back() const;

        /// <summary>
        /// Returns the last element of the Datum
        /// </summary>
        /// <returns> A const reference to the last element of the Datum </returns>
        /// <exception cref="std::runtime_error"> Throws if this Datum is empty 
        /// or the type doesn't match </exception>
        template<> RTTI* const& Back() const;

        template<> const Scope& Back() const;
		#pragma endregion

		#pragma region Find
		/// <summary>
		/// Tries to find the element with the given value and returns its index.
		/// </summary>
		/// <param name="value"> The value to search for </param>
		/// <returns> The index of the element. If the element could not be found, returns
		/// the index past the end of the list </returns>
        /// <exception cref="std::runtime_error"> Throws if the type doesn't match </exception>
		size_t Find(int value) const;

        /// <summary>
        /// Tries to find the element with the given value and returns its index.
        /// </summary>
        /// <param name="value"> The value to search for </param>
        /// <returns> The index of the element. If the element could not be found, returns
        /// the index past the end of the list </returns>
        /// <exception cref="std::runtime_error"> Throws if the type doesn't match </exception>
		size_t Find(std::string value) const;

        /// <summary>
        /// Tries to find the element with the given value and returns its index.
        /// </summary>
        /// <param name="value"> The value to search for </param>
        /// <returns> The index of the element. If the element could not be found, returns
        /// the index past the end of the list </returns>
        /// <exception cref="std::runtime_error"> Throws if the type doesn't match </exception>
		size_t Find(float value) const;

        /// <summary>
        /// Tries to find the element with the given value and returns its index.
        /// </summary>
        /// <param name="value"> The value to search for </param>
        /// <returns> The index of the element. If the element could not be found, returns
        /// the index past the end of the list </returns>
        /// <exception cref="std::runtime_error"> Throws if the type doesn't match </exception>
		size_t Find(glm::vec4 value) const;

        /// <summary>
        /// Tries to find the element with the given value and returns its index.
        /// </summary>
        /// <param name="value"> The value to search for </param>
        /// <returns> The index of the element. If the element could not be found, returns
        /// the index past the end of the list </returns>
        /// <exception cref="std::runtime_error"> Throws if the type doesn't match </exception>
		size_t Find(glm::mat4 value) const;

        /// <summary>
        /// Tries to find the element with the given value and returns its index.
        /// </summary>
        /// <param name="value"> The value to search for </param>
        /// <returns> The index of the element. If the element could not be found, returns
        /// the index past the end of the list </returns>
        /// <exception cref="std::runtime_error"> Throws if the type doesn't match </exception>
		size_t Find(RTTI* value) const;
		#pragma endregion

		#pragma region Remove
		/// <summary>
		/// Attempts to remove the first element found with the given value.
		/// </summary>
		/// <param name="value"> The element to try to remove </param>
		/// <returns> True if the element was removed, false otherwise </returns>
        /// <exception cref="std::runtime_error"> Throws if this Datum contains 
        /// external storage </exception>
		bool Remove(int value);

        /// <summary>
        /// Attempts to remove the first element found with the given value.
        /// </summary>
        /// <param name="value"> The element to try to remove </param>
        /// <returns> True if the element was removed, false otherwise </returns>
        /// <exception cref="std::runtime_error"> Throws if this Datum contains 
        /// external storage </exception>
		bool Remove(std::string value);

        /// <summary>
        /// Attempts to remove the first element found with the given value.
        /// </summary>
        /// <param name="value"> The element to try to remove </param>
        /// <returns> True if the element was removed, false otherwise </returns>
        /// <exception cref="std::runtime_error"> Throws if this Datum contains 
        /// external storage </exception>
		bool Remove(float value);

        /// <summary>
        /// Attempts to remove the first element found with the given value.
        /// </summary>
        /// <param name="value"> The element to try to remove </param>
        /// <returns> True if the element was removed, false otherwise </returns>
        /// <exception cref="std::runtime_error"> Throws if this Datum contains 
        /// external storage </exception>
		bool Remove(glm::vec4 value);

        /// <summary>
        /// Attempts to remove the first element found with the given value.
        /// </summary>
        /// <param name="value"> The element to try to remove </param>
        /// <returns> True if the element was removed, false otherwise </returns>
        /// <exception cref="std::runtime_error"> Throws if this Datum contains 
        /// external storage </exception>
		bool Remove(glm::mat4 value);

        /// <summary>
        /// Attempts to remove the first element found with the given value.
        /// </summary>
        /// <param name="value"> The element to try to remove </param>
        /// <returns> True if the element was removed, false otherwise </returns>
        /// <exception cref="std::runtime_error"> Throws if this Datum contains 
        /// external storage </exception>
		bool Remove(RTTI* value);
		#pragma endregion

		/// <summary>
		/// Removes the element located at index. All data that follows the element will
		/// be shifted down.
		/// </summary>
		/// <param name="index"> The index of the element to remove</param>
		/// <returns> True if the remove was successful, false otherwise </returns>
        /// <exception cref="std::runtime_error"> Throws if index is out of bounds  or this
        /// Datum contains external storage </exception>
		bool RemoveAt(size_t index);

		/// <summary>
		/// Removes the last element from the Datum. Has no effect if the Datum is empty.
		/// </summary>
        /// <exception cref="std::runtime_error"> Throws if this Datum contains external 
        /// storage </exception>
		void PopBack();

		/// <summary>
		/// Sets the type of the Datum. Type cannot be set more than once.
		/// </summary>
		/// <param name="type"> The type to set this datum to </param>
		void SetType(DatumType type);

		/// <summary>
		/// Gets the current number of elements in the Datum
		/// </summary>
		/// <returns> The current number of elements in the Datum </returns>
		size_t Size() const;

		/// <summary>
		/// Gets the current capacity of the Datum
		/// </summary>
		/// <returns> The current capacity of the Datum </returns>
		size_t Capacity() const;

		/// <summary>
		/// Gets the current type of the Datum
		/// </summary>
		/// <returns> The current type of the Datum </returns>
		DatumType Type() const;

		/// <summary>
		/// Changes the size of the Datum to match the passed in size. If the current Datum is 
		/// larger than size, any extra elements will be destroyed. If the current Datum is smaller
		/// than size, the extra elements will be default constructed.
		/// </summary>
		/// <param name="size"> The size to set this Datum to </param>
        /// <exception cref="std::runtime_error"> Throws if this Datum contains external 
        /// storage or has no type </exception>
		void Resize(size_t size);
		
		/// <summary>
		/// Reserves capacity for this Datum. Has no effect if the passed in capacity is smaller 
		/// than the Datum's current capacity.
		/// </summary>
		/// <param name="capacity"> The new capacity of the Datum </param>
        /// <exception cref="std::runtime_error"> Throws if this Datum contains external 
        /// storage or has no type </exception>
		void Reserve(size_t capacity);

		/// <summary>
		/// Clears all of the items out of the Datum and sets size to 0
		/// </summary>
        /// <exception cref="std::runtime_error"> Throws if this Datum contains external 
        /// storage </exception>
		void Clear();

		/// <summary>
		/// Converts the element at the given index into a std::string
		/// </summary>
		/// <param name="index"> The index of the element to convert into a string </param>
		/// <returns> A std::string with the contents of the element </returns>
        /// <exception cref="std::runtime_error"> Throws if index is out of bounds </exception>
		std::string ToString(size_t index = 0) const;

		/// <summary>
		/// Sets the element at the given index to and element based on the string passed in
		/// </summary>
		/// <param name="input"> The string with the info to construct an element </param>
		/// <param name="index"> The index of the element to set (default 0) </param>
		void SetFromString(std::string& input, size_t index = 0);

        void PushBackFromString(std::string& input);

        bool IsExternal() const;

        Scope& operator[](size_t index);

		#pragma region SetStorage
        /// <summary>
        /// Sets the storage of this Datum to be external and the hold the contents
        /// of the passed in array. This Datum will not longer be able to call any methods
        /// that change size or capacity.
        /// </summary>
        /// <param name="externalArray"> The array for this Datum to point to </param>
        /// <param name="numElements"> The number of elements in externalArray </param>
        /// <exception cref="std::runtime_error"> Throws if the type doesn't match
        /// or if this Datum already has internal storage </exception>
		void SetStorage(int* externalArray, size_t numElements);

        /// <summary>
        /// Sets the storage of this Datum to be external and the hold the contents
        /// of the passed in array. This Datum will not longer be able to call any methods
        /// that change size or capacity.
        /// </summary>
        /// <param name="externalArray"> The array for this Datum to point to </param>
        /// <param name="numElements"> The number of elements in externalArray </param>
        /// <exception cref="std::runtime_error"> Throws if the type doesn't match
        /// or if this Datum already has internal storage </exception>
		void SetStorage(std::string* externalArray, size_t numElements);

        /// <summary>
        /// Sets the storage of this Datum to be external and the hold the contents
        /// of the passed in array. This Datum will not longer be able to call any methods
        /// that change size or capacity.
        /// </summary>
        /// <param name="externalArray"> The array for this Datum to point to </param>
        /// <param name="numElements"> The number of elements in externalArray </param>
        /// <exception cref="std::runtime_error"> Throws if the type doesn't match
        /// or if this Datum already has internal storage </exception>
		void SetStorage(float* externalArray, size_t numElements);

        /// <summary>
        /// Sets the storage of this Datum to be external and the hold the contents
        /// of the passed in array. This Datum will not longer be able to call any methods
        /// that change size or capacity.
        /// </summary>
        /// <param name="externalArray"> The array for this Datum to point to </param>
        /// <param name="numElements"> The number of elements in externalArray </param>
        /// <exception cref="std::runtime_error"> Throws if the type doesn't match
        /// or if this Datum already has internal storage </exception>
		void SetStorage(glm::vec4* externalArray, size_t numElements);

        /// <summary>
        /// Sets the storage of this Datum to be external and the hold the contents
        /// of the passed in array. This Datum will not longer be able to call any methods
        /// that change size or capacity.
        /// </summary>
        /// <param name="externalArray"> The array for this Datum to point to </param>
        /// <param name="numElements"> The number of elements in externalArray </param>
        /// <exception cref="std::runtime_error"> Throws if the type doesn't match
        /// or if this Datum already has internal storage </exception>
		void SetStorage(glm::mat4* externalArray, size_t numElements);

        /// <summary>
        /// Sets the storage of this Datum to be external and the hold the contents
        /// of the passed in array. This Datum will not longer be able to call any methods
        /// that change size or capacity.
        /// </summary>
        /// <param name="externalArray"> The array for this Datum to point to </param>
        /// <param name="numElements"> The number of elements in externalArray </param>
        /// <exception cref="std::runtime_error"> Throws if the type doesn't match
        /// or if this Datum already has internal storage </exception>
		void SetStorage(RTTI** externalArray, size_t numElements);
		#pragma endregion

	private:

		/// <summary>
		/// Union of different array types this Datum can hold
		/// </summary>
		union DatumValue
		{
			char* c;
			int* i;
			float* f;
			std::string* s;
			glm::vec4* v;
			glm::mat4* m;
			RTTI** r;
            Scope** sc;
            uint8_t* b;
			void* vp;
		};

		/// <summary>
		/// Static array that defines the sizes of each type that this Datum supports.
		/// Used so we can map sizes to our enum types
		/// </summary>
        inline static const size_t m_DataSizes[8] =
        { 
            sizeof(int),
            sizeof(std::string),
            sizeof(float),
            sizeof(glm::vec4),
            sizeof(glm::mat4),
            sizeof(RTTI*),
            sizeof(Scope*),
            0
        };

        void PushBack(Scope& value);

		/// <summary>
		/// Gets the size of the current type in bytes
		/// </summary>
		/// <returns> The size of the current type in bytes </returns>
		size_t GetTypeSize() const;

		/// <summary>
		/// Set storage helper function that actually sets the storage 
		/// </summary>
		/// <param name="externalArray"> The array to set our pointer to </param>
		/// <param name="numElements"> The number of elements in the array </param>
		/// <exception cref="std::runtime_error"> Throws if this Datum already has
		/// internal storage </exception>
		void SetStorage(void* externalArray, size_t numElements);

		/// <summary>
		/// The internal type of the Datum. This cannot be changed once set
		/// </summary>
		DatumType m_type = DatumType::Unknown;

		/// <summary>
		/// Whether or not this Datum contains external storage
		/// </summary>
		bool m_externalStorage = false;

		/// <summary>
		/// How many elements are in this Datum
		/// </summary>
		size_t m_size = 0;

		/// <summary>
		/// How much space is allocated for this Datum
		/// </summary>
		size_t m_capacity = 0;

		/// <summary>
		/// The actual pointer to the data for this Datum
		/// </summary>
		DatumValue m_data{ nullptr };

		#pragma region CreateFunctions
		/// <summary>
		/// Create functions used to create an element of the given type at the given index.
		/// These use placement new, they don't allocate any space.
		/// </summary>
        using CreateFunction = void(Datum::*)(size_t index);
        inline void CreateInteger(size_t index);
		inline void CreateString(size_t index);
		inline void CreateFloat(size_t index);
		inline void CreateVector(size_t index);
		inline void CreateMatrix(size_t index);
		inline void CreatePointer(size_t index);

        inline static const CreateFunction m_CreateFunctions[] =
        {
			&Datum::CreateInteger,
			&Datum::CreateString,
			&Datum::CreateFloat,
			&Datum::CreateVector,
			&Datum::CreateMatrix,
			&Datum::CreatePointer,
            nullptr,
            nullptr
        };
		#pragma endregion

		#pragma region CompareFunctions
		/// <summary>
		/// Compare functions used to compare all the data in the Datum based on what
		/// type it is storing
		/// </summary>
		using CompareFunction = bool(Datum::*)(const Datum& other) const;
		bool CompareData(const Datum& other) const;
		bool CompareStrings(const Datum& other) const;
		bool ComparePointers(const Datum& other) const;

		inline static const CompareFunction m_CompareFunctions[] =
		{
			&Datum::CompareData,
			&Datum::CompareStrings,
			&Datum::CompareData,
			&Datum::CompareData,
			&Datum::CompareData,
			&Datum::ComparePointers,
            &Datum::ComparePointers,
			nullptr
		};
		#pragma endregion

		#pragma region SetFromStringFunctions
		/// <summary>
		/// FromString functions used to create elements from strings. 
		/// </summary>
		using SetFromStringFunction = void(Datum::*)(std::string& input, size_t index);
        void SetIntFromString(std::string& input, size_t index = 0);
        void SetStringFromString(std::string& input, size_t index = 0);
        void SetFloatFromString(std::string& input, size_t index = 0);
        void SetVectorFromString(std::string& input, size_t index = 0);
        void SetMatrixFromString(std::string& input, size_t index = 0);

        inline static const SetFromStringFunction m_SetFromStringFunctions[] =
        {
			&Datum::SetIntFromString,
            &Datum::SetStringFromString,
            &Datum::SetFloatFromString,
            &Datum::SetVectorFromString,
            &Datum::SetMatrixFromString,
			nullptr,
            nullptr
        };
		#pragma endregion

        #pragma region PushFromStringFunctions
        /// <summary>
        /// FromString functions used to create elements from strings. 
        /// </summary>
        using PushFromStringFunction = void(Datum::*)(std::string& input);
        void PushIntFromString(std::string& input);
        void PushStringFromString(std::string& input);
        void PushFloatFromString(std::string& input);
        void PushVectorFromString(std::string& input);
        void PushMatrixFromString(std::string& input);

        inline static const PushFromStringFunction m_PushFromStringFunctions[] =
        {
            &Datum::PushIntFromString,
            &Datum::PushStringFromString,
            &Datum::PushFloatFromString,
            &Datum::PushVectorFromString,
            &Datum::PushMatrixFromString,
            nullptr,
            nullptr
        };
        #pragma endregion
	};	
}

#include "Datum.inl"