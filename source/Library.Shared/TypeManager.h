#pragma once
#include "Datum.h"
#include "HashMap.h"
#include "RTTI.h"

namespace FieaGameEngine
{
	#define RegisterType(Type, ParentType) (TypeManager::AddType(Type::TypeIdClass(), Type::Signatures(), ParentType::TypeIdClass()));

	/// <summary>
	/// Signature struct that holds the data needed to construct a key, Datum pair for a particular
	/// attribute
	/// </summary>
	struct Signature final
	{
		/// <summary>
		/// Default constructor
		/// </summary>
		Signature() = default;

		/// <summary>
		/// Default copy constructor
		/// </summary>
		/// <param name="other"> The other Signature to copy </param>
		Signature(const Signature& other) = default;

		/// <summary>
		/// Default move constructor
		/// </summary>
		/// <param name="other"> The other Signature to move </param>
		Signature(Signature&& other) noexcept = default;

		/// <summary>
		/// Default copy assignment operator
		/// </summary>
		/// <param name="other"> The other Signature to copy </param>
		Signature& operator=(const Signature& other) = default;

		/// <summary>
		/// Default move assignment
		/// </summary>
		/// <param name=""> The other Signature to move </param>
		/// <returns></returns>
		Signature& operator=(Signature&&) noexcept = default;

		/// <summary>
		/// Default destructor
		/// </summary>
		~Signature() = default;

		/// <summary>
		/// Constructor for creating a signature based on its 4 data members
		/// </summary>
		/// <param name="name"> The name of the attribute </param>
		/// <param name="type"> The type of the attribute </param>
		/// <param name="size"> The size of the atribute </param>
		/// <param name="storageOffset"> The offset (in bytes) of the attribute from
		/// the base class address </param>
        Signature(std::string name, Datum::DatumType type, size_t size, size_t storageOffset)
            : m_name(name), m_type(type), m_size(size), m_storageOffset(storageOffset) {}

		/// <summary>
		/// The name of the attribute
		/// </summary>
		std::string m_name;

		/// <summary>
		/// The type of the attribute
		/// </summary>
		Datum::DatumType m_type;

		/// <summary>
		/// The size of the attribute
		/// </summary>
		size_t m_size;

		/// <summary>
        /// The offset (in bytes) of the attribute from the base class address
		/// </summary>
		size_t m_storageOffset;
	};

	/// <summary>
	/// TypeManager static class that handles maintaining a map of types and their signatures
	/// </summary>
	class TypeManager final
	{
	public:
		/// <summary>
		/// TypeInfo struct that serves as a wrapper for two pieces of information on a type:
		/// the Vector of signatures and the parent type ID
		/// </summary>
		struct TypeInfo
		{
			Vector<Signature> m_signatures;
			RTTI::IdType m_parentType;
		};

		TypeManager() = delete;
		TypeManager(const TypeManager&) = delete;
		TypeManager(TypeManager&&) = delete;
		TypeManager& operator=(const TypeManager&) = delete;
		TypeManager& operator=(TypeManager&&) = delete;
		~TypeManager() = default;
		
		/// <summary>
		/// Static method to retrieve a list of Signatures for the given type
		/// </summary>
		/// <param name="typeId"> The typeId to get the Signatures for </param>
        /// <returns> A Vector of Signatures corresponding to the typeId </returns>
        /// <exception cref="std::runtime_error"> Throws if the passed in typeId does
        /// not correspond to a type that is registered with the TypeManager </exception>
		static const Vector<Signature> GetSignatures(RTTI::IdType typeId);

        /// <summary>
        /// Static method to check if the passed in type has been registered
        /// </summary>
        /// <param name="typeId"> The typeId to check </param>
        /// <returns> True if the type has been registered, false otherwise </returns>
		static bool ContainsType(RTTI::IdType typeId);

        /// <summary>
        /// Static method to register a type with the type manager. If the type has already been
        /// registered, nothing occurs. See RegisterType macro for a cleaner way to call this.
        /// </summary>
        /// <param name="typeId"> The typeId of the type you are registering </param>
        /// <param name="signatures"> The Signature Vector of the type you are registering </param>
        /// <param name="parentType"> The typeId of the parent of the type you are
        /// registering </param>
        /// <returns> True if the type was registered, false if it had already been
        /// registered </returns>
        /// <exception cref="std::runtime_error"> Throws if you try to register a type and
        /// pass itself as the parent </exception>
		static bool AddType(RTTI::IdType typeId, const Vector<Signature>& signatures, RTTI::IdType parentType);

		/// <summary>
		/// Static method that attempts to unregister a type with the TypeManager. 
		/// </summary>
		/// <param name="typeId"> The typeId of the type you are unregistering </param>
        /// <returns> True if the type was unregistered, false if it wasn't registered in
        /// the first place </returns>
		static bool RemoveType(RTTI::IdType typeId);

		/// <summary>
		/// Static method that unregisters all types with the TypeManager
		/// </summary>
		static void Clear();

		/// <summary>
		/// Static method that returns how many types are registered with the TypeManager
		/// </summary>
		static size_t Size();

	private:

		/// <summary>
		/// Internal hashmap that keeps track of all registered types and their associated
		/// info
		/// </summary>
		static HashMap<RTTI::IdType, TypeInfo> m_types;
	};
}

