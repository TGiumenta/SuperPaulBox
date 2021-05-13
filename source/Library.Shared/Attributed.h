#pragma once
#include "Scope.h"

namespace FieaGameEngine
{
    /// <summary>
    /// Attributed class that serves as a way of converting C++ classes that contain
    /// signatures for their member variables into a Scope with key, Datum pairs.
    /// </summary>
	class Attributed : public Scope 
	{
		RTTI_DECLARATIONS(Attributed, Scope);

    public:
        /// <summary>
        /// Default constructor is deleted
        /// </summary>
        Attributed() = delete;

        /// <summary>
        /// Copy constructor. The underlying Scope is copied from the passed in
        /// Attributed and then the external storage pointers are adjusted to 
        /// point to the new object instance.
        /// </summary>
        /// <param name="other"> The other Attributed to copy from </param>
        Attributed(const Attributed& other);

        /// <summary>
        /// Move constructor. The underlying Scope's memory is stolen from 
        /// the passed in Attributed and then the external storage pointers are 
        /// adjusted to point to the new object instance.
        /// </summary>
        /// <param name="other"> The other Attributed to move from </param>
        Attributed(Attributed&& other) noexcept;

        /// <summary>
        /// Copy assignment. This Attributed is cleared and then the underlying 
        /// Scope is copied from the passed in Attributed and then the external 
        /// storage pointers are adjusted to point to the new object instance.
        /// </summary>
        /// <param name="other"> The other Attributed to copy from </param>
        Attributed& operator=(const Attributed& other);

        /// <summary>
        /// Move constructor. This attributed is cleared and then the underlying 
        /// Scope's memory is stolen from the passed in Attributed and then the 
        /// external storage pointers are adjusted to point to the new object 
        /// instance.
        /// </summary>
        /// <param name="other"> The other Attributed to move from </param>
        Attributed& operator=(Attributed&& other) noexcept;

        /// <summary>
        /// Pure virtual destructor to make this class abstract
        /// </summary>
        virtual ~Attributed() = 0 {};

        /// <summary>
        /// Determines whether the passed in string is the name of an attribute
        /// for this object.
        /// </summary>
        /// <param name="key"> The string to check </param>
        /// <returns> True if this is an attribute, false otherwise </returns>
        bool IsAttribute(const std::string& key) const;

        /// <summary>
        /// Determines whether the passed in string is the name of a prescribed
        /// attribute for this class.
        /// </summary>
        /// <param name="key"> The string to check </param>
        /// <returns> True if this is a prescribed attribute, false otherwise 
        /// </returns>
        bool IsPrescribedAttribute(const std::string& key) const;

        /// <summary>
        /// Determines whether the passed in string is the name of an auxiliary
        /// attribute for this object.
        /// </summary>
        /// <param name="key"> The string to check </param>
        /// <returns> True if this is a auxiliary attribute, false otherwise
        /// </returns>
        bool IsAuxiliaryAttribute(const std::string& key) const;

        /// <summary>
        /// Appends the given key as an auxiliary attribute for this object. This
        /// is a simple wrapper for Scope's append method that also enforces not
        /// appending an auxiliary attribute that is already prescribed.
        /// </summary>
        /// <param name="key"> The key to append </param>
        /// <returns> The datum of the appended attribute </returns>
        /// <exception cref="std::runtime_error"> Throws if the passed in key
        /// is already the key of a prescribed attribute </exception>
        Datum& AppendAuxiliaryAttribute(const std::string& key);

        /// <summary>
        /// Gets all of the attributes currently associated with this object, which
        /// includes both prescribed and auxiliary attributes
        /// </summary>
        /// <returns> A vector of pointers to pairs, where each pair represents
        /// one attribute </returns>
        const Vector<Scope::PairType*>& GetAttributes() const;

        /// <summary>
        /// Gets all of the prescribed attributes associated with this class, including 
        /// the "this" pointer
        /// </summary>
        /// <returns> A vector of pointer to pairs, where each pair represents
        /// one attribute </returns>
        Vector<Scope::PairType*> GetPrescribedAttributes() const;

        /// <summary>
        /// Gets all of the auxiliary attributes associated with this object
        /// </summary>
        /// <returns> A vector of pointer to pairs, where each pair represents
        /// one attribute </returns>
        Vector<Scope::PairType*> GetAuxiliaryAttributes() const;

    protected:
        /// <summary>
        /// Explicit constructor for Attributed that takes in the underlying
        /// type of the object. This is intended to only ever be called from the
        /// constructors of child classes.
        /// </summary>
        /// <param name="type"> The underlying type of the child class </param>
        explicit Attributed(RTTI::IdType type);

    private:

        /// <summary>
        /// Helper method that populates this Scope with all of the prescribed
        /// attributes based on the passed in IdType
        /// </summary>
        /// <param name="type"> The IdType to use for retrieving prescribed
        /// attributes </param>
        void PopulateScope(RTTI::IdType type);

        /// <summary>
        /// Helper method that updates external storage pointers to point to the
        /// this new instance of the object
        /// </summary>
        /// <param name="other"> The old object that the pointers pointed to </param>
        void UpdateExternalStorage(const Attributed& other);
	};
}

