#pragma once
#include "HashMap.h"
#include "Datum.h"
#include "RTTI.h"
#include <functional>
#include <gsl/gsl>
#include "Factory.h"

namespace FieaGameEngine
{
	class Scope : public FieaGameEngine::RTTI
	{
		friend Attributed;

		RTTI_DECLARATIONS(Scope, RTTI);
	public:
		using MapType = HashMap<const std::string, Datum>;
		using MapIteratorType = MapType::Iterator;
		using ConstMapIteratorType = MapType::ConstIterator;
		using PairType = std::pair<const std::string, Datum>;
		using NestedScopeFunction = std::function<bool(const Scope&, Datum&, size_t)>;

		#pragma region RuleOf6
		/// <summary>
		/// Default constructor with optional starting capacity parameter
		/// </summary>
		/// <param name="capacity"> Optional capacity parameter that defines the starting
		/// size of the inner hashmap "bucket" array </param>
		explicit Scope(size_t capacity = 11);

		/// <summary>
		/// Copy constructor. This will make a deep copy of all nested Scopes but will not
		/// copy anything up the hierarchy. The copied Scope will have no parent even if the
		/// provided Scope had one.
		/// </summary>
		/// <param name="other"> The Scope to copy </param>
		Scope(const Scope& other);

		/// <summary>
		/// Move constructor. This MUST be used on a heap allocated Scope. Additionally, you are 
		/// now responsible for deleting the allocated memory of the passed in Scope.
		/// </summary>
		/// <param name="other"> The Scope to move </param>
		Scope(Scope&& other) noexcept;

		/// <summary>
		/// Copy assignment operator. This will clear all of the contents of this scope and then
		/// deep copy the contents of the other scope (meaning all current nested scopes in this 
        /// scope will be destroyed). The copied Scope will have no parent even if the provided 
        /// Scope had one.
		/// </summary>
		/// <param name="other"> The Scope to copy </param>
		/// <returns> A reference to the newly copied Scope </returns>
		Scope& operator=(const Scope& other);

		/// <summary>
        /// Move assignment operator. This MUST be used on a heap allocated Scope. Additionally, 
        /// you are now responsible for deleting the allocated memory of the passed in Scope.
		/// </summary>
		/// <param name="other"> The Scope to move </param>
		Scope& operator=(Scope&& other) noexcept;

		/// <summary>
		/// Destructor that will recursively delete all children of this Scope.
		/// </summary>
		virtual ~Scope();
		#pragma endregion

		/// <summary>
		/// Override of RTTI's Equals method. Recursively checks to make sure all nested data is
		/// equivalent in order to determine if two Scopes are equal.
		/// </summary>
		/// <param name="rhs"> The other Scope to compare to </param>
		/// <returns> True if the two Scopes are equal, false otherwise </returns>
		bool Equals(const RTTI* rhs) const override;

		/// <summary>
        /// A syntactically convenient wrapper for the Equals method. Recursively checks to make 
        /// sure all nested data is equivalent in order to determine if two Scopes are equal.
		/// </summary>
		/// <param name="other"> The other Scope to compare to </param>
		/// <returns> True if the two Scopes are equal, false otherwise </returns>
		bool operator==(const Scope& other) const;

		/// <summary>
		/// Checks if two Scopes are not equivalent. Recursively checks nested Scopes looking for
		/// differences.
		/// </summary>
		/// <param name="other"> The other Scope to compare to </param>
		/// <returns> True if the two Scopes are different, false otherwise </returns>
		bool operator!=(const Scope& other) const;

		/// <summary>
		/// Gets the Datum at the given index and returns it. The index given corresponds to the 
		/// insert order.
		/// </summary>
		/// <param name="index"> The index to retrieve </param>
		/// <returns> A reference to the Datum found at the index </returns>
        Datum& operator[](size_t index);

		/// <summary>
		/// A syntactically convenient wrapper for the Append method. Attempts to append the given
		/// key into the Scope with a default constructed Datum as its value.
		/// </summary>
		/// <param name="key"> The key to use for insertion </param>
		/// <returns> A reference to the appended Datum in the case of insertion. If the key already 
		/// existed, a reference to the existing Datum is returned. </returns>
        Datum& operator[](const std::string key);

        /// <summary>
        /// Attempts to append the given key into the Scope with a default constructed Datum as 
        /// its value.
        /// </summary>
        /// <param name="key"> The key to use for insertion </param>
        /// <returns> A reference to the appended Datum in the case of insertion. If the key already 
        /// existed, a reference to the existing Datum is returned. </returns>
        /// <exception cref="std::runtime_error"> Throws if the key is an empty string </exception>
		Datum& Append(const std::string& key);

		/// <summary>
		/// Attempts to append the given key into the Scope as a nested Scope child. This will allocate
		/// a new heap allocated Scope as a child Scope. If the key already exists, this Scope will
		/// be appended to the existing Datum.
		/// </summary>
		/// <param name="key"> The key to use for insertion </param>
		/// <returns> A reference to the newly created Scope </returns>
		/// <exception cref="std::runtime_error"> Throws if the key is an empty string or the key 
		/// already exists but the Datum type isn't Table </exception>
		Scope& AppendScope(const std::string& key);

		/// <summary>
		/// Attempts to adopt the given Scope as a child as this Scope under the given key. If this
		/// child already had a parent, it will be disconnected from that parent. If the key already
		/// exists, this child will be appended to that existing Datum.
		/// </summary>
		/// <param name="child"> The child Scope to adopt </param>
		/// <param name="key"> The key to use for insertion </param>
		/// <exception cref="std::runtime_error"> Throws if the key is an empty string, if trying to
		/// adopt yourself or your parent, or if the key already exists but the Datum type isn't
		/// table </exception>
		void Adopt(Scope& child, const std::string& key);

		/// <summary>
		/// Removes this child from its parent. This will sever the reference the child had to the parent
		/// and also remove the child from its parent's nested Scope list. You are now responsible
		/// for deleting this child's memory. Has no effect on a Scope with no parent.
		/// </summary>
		void Orphan();

		/// <summary>
		/// Get the parent of this Scope if it has one
		/// </summary>
		/// <returns> A pointer to this Scope's parent, nullptr if the Scope has no parent </returns>
        Scope* GetParent();

        /// <summary>
        /// Get the parent of this Scope if it has one
        /// </summary>
        /// <returns> A pointer to this Scope's parent, nullptr if the Scope has no parent </returns>
		const Scope* GetParent() const;

		/// <summary>
		/// Checks if this Scope is a descendant of the passed in Scope
		/// </summary>
		/// <param name="potentialAncestor"> The Scope that we want to check if its an ancestor </param>
		/// <returns> True if this Scope is a descendant of the passed in Scope, false otherwise </returns>
		bool IsDescendantOf(const Scope& potentialAncestor) const;

        /// <summary>
        /// Checks if this Scope is an ancestor of the passed in Scope
        /// </summary>
        /// <param name="potentialAncestor"> The Scope that we want to check if its an descendant </param>
        /// <returns> True if this Scope is a ancestor of the passed in Scope, false otherwise </returns>
		bool IsAncestorOf(const Scope& potentialDescendant) const;

		/// <summary>
		/// Gets the size of this Scope, which is equivalent to the number of entries in its hashmap
		/// </summary>
		/// <returns> The size of this Scope </returns>
        size_t Size() const;

		/// <summary>
		/// Attempts to find the Datum at the given key inside this Scope. This does not check inside
		/// of nested Scopes or parent Scopes, just this one.
		/// </summary>
		/// <param name="key"> The key to search for </param>
		/// <returns> A pointer to the found Datum, or nullptr if nothing was found </returns>
		Datum* Find(const std::string& key);

        /// <summary>
        /// Attempts to find the Datum at the given key inside this Scope. This does not check inside
        /// of nested Scopes or parent Scopes, just this one.
        /// </summary>
        /// <param name="key"> The key to search for </param>
        /// <returns> A pointer to the found const Datum, or nullptr if nothing was found </returns>
		const Datum* Find(const std::string& key) const;

		/// <summary>
		/// Searches this Scope's children for the given nested Scope
		/// </summary>
		/// <param name="scopeToFind"> The Scope to search for </param>
        /// <returns>A std::tuple containing a pointer to the found Datum and the index in that Datum
        /// that the nested Scope was found. If the Scope wasn't found, the std::tuple will contain
        /// a nullptr and the index 0. </returns>
		std::tuple<Datum*, size_t> FindContainedScope(const Scope& scopeToFind);

        /// <summary>
        /// Searches this Scope and all of its ancestors for the given key. This does not look into
        /// this Scope's children or any siblings.
        /// </summary>
        /// <param name="scopeToFind"> The key to search for </param>
        /// <returns>A std::tuple containing a pointer to the found Datum and a pointer to the Scope it
        /// was found in. If the key wasn't found, the std::tuple will contain a nullptr and the 
        /// Scope this was originally called on </returns>
		std::tuple<Datum*, Scope*> Search(const std::string& key);

        /// <summary>
        /// Searches this Scope and all of its ancestors for the given key. This does not look into
        /// this Scope's children or any siblings.
        /// </summary>
        /// <param name="scopeToFind"> The key to search for </param>
        /// <returns>A std::tuple containing a pointer to the found Datum and a pointer to the Scope it
        /// was found in. If the key wasn't found, the std::tuple will contain a nullptr and the 
        /// Scope this was originally called on </returns>
		std::tuple<const Datum*, const Scope*> Search(const std::string& key) const;

		/// <summary>
		/// Clears all of the data contained in this Scope including recursively deleting all nested
		/// Scopes.
		/// </summary>
		void Clear();

		virtual gsl::owner<Scope*> Clone() const;

	protected:
		/// <summary>
		/// This Scope's parent Scope (or nullptr if this is a root Scope)
		/// </summary>
		Scope* m_parent = nullptr;

		/// <summary>
		/// Internal hashmap used to store string, Datum pairs to keep track of all children
		/// </summary>
		MapType m_map;

		/// <summary>
		/// Internal vector used to keep the order of inserted pairs (since the hashmap is unordered
		/// we would have no way of tracking the order things were inserted without this)
		/// </summary>
		Vector<PairType*> m_order;

	private:

		/// <summary>
		/// Helper function that enacts the given function on every nested Scope inside this Scope
		/// </summary>
		/// <param name="func"> The function to enact on every nested Scope </param>
		void ForEachNestedScopeIn(NestedScopeFunction func) const;

		/// <summary>
		/// Helper function that attempts to append the given key to the hashmap with a default
		/// constructed Datum.
		/// </summary>
		/// <param name="key"> The key to use for insertion </param>
		/// <returns> A std::tuple with a hashmap iterator pointing to the newly created pair or to
		/// the existing pair if the key already exists. The second part is a boolean that is true if
		/// the pair was inserted and false if the pair already existed </returns>
		std::tuple<MapIteratorType, bool> AppendHelper(const std::string& key);
	};

	ConcreteFactory(Scope, Scope);
}

