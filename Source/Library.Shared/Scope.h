#pragma once

#include <string>
#include <gsl/gsl>
#include "RTTI.h"
#include "HashMap.h"
#include "Datum.h"
#include "Vector.h"

namespace Library
{
	class Scope : public RTTI
	{
		RTTI_DECLARATIONS(Scope, RTTI);

	public:
		using PairType = std::pair<const std::string, Datum>;

#pragma region MemberMethods

		/// <summary>
		/// Default constructor. Sets the HashMap and Vector to their default sizes unless the user specifies a starting capacity.
		/// </summary>
		/// <param name="capacity">User defined capacity for the table</param>
		Scope(size_t bucketSize = 11);

		/// <summary>
		/// Copy constructor. Makes a deep copy of the data contained within the right hand side Scope.
		/// Any deep copies of contained Scopes will have their parent changed to this Scope since they are not owned by the right hand side Scope. 
		/// </summary>
		/// <param name="rhs">The Scope that will be deep copied</param>
		Scope(const Scope& rhs);
		/// <summary>
		/// Move constructor. Moves all data from the right hand side Scope to this Scope and updates its Parents pointer to this new Scope.
		/// Warning: After calling move the user now owns the right hand side Scopes memory and is responsible for its management
		/// </summary>
		/// <param name="rhs">The Scope that will have its data moved into this Scope</param>
		Scope(Scope&& rhs) noexcept;

		/// <summary>
		/// Copy assignment operator. Clears the current data and makes a deep copy of the data contained within the right hand side Scope.
		/// Any deep copies of contained Scopes will have their parent changed to this Scope since they are not owned by the right hand side Scope. 
		/// </summary>
		/// <param name="rhs">The Scope that will be deep copied</param>
		/// <returns>A reference to the newly modified Scope</returns>
		Scope& operator=(const Scope& rhs);
		/// <summary>
		/// Move assignment operator. Clears the current data inside this Scope and then moves all data from the right hand side Scope to this Scope and updates its Parents pointer to this new Scope.
		/// Warning: After calling move the user now owns the right hand side Scopes memory and is responsible for its management.
		/// </summary>
		/// <param name="rhs">The Scope that will have its data moved into this Scope</param>
		/// <returns>A reference to the newly modified Scope</returns>
		Scope& operator=(Scope&& rhs) noexcept;

		// <summary>
		/// Clears all allocated memory that this Scope owns. This includes calling delete on all owned Scope children.
		/// </summary>
		virtual ~Scope();

#pragma endregion

#pragma region Equality

		/// <summary>
		/// Compares this scope to another scope for logical equivalence
		/// </summary>
		/// <param name="rhs">The scope to be compared to this one</param>
		/// <returns>True if the contents of both scope match, false otherwise</returns>
		bool operator==(const Scope& rhs) const;
		/// <summary>
		/// Compares this scope to another scope for logical inequality
		/// </summary>
		/// <param name="rhs">The scope to be compared to this one</param>
		/// <returns>True if the contents don't match, false otherwise</returns>
		bool operator!=(const Scope& rhs) const;

#pragma endregion

#pragma region ScopeInformation
		/// <summary>
		/// Returns a pointer to the Scope who is the parent of this Scope. Will be nullptr if this Scope has no parent.
		/// </summary>
		/// <returns>Returns a pointer to the Scope who is the parent of this Scope. Will be nullptr if this Scope has no parent.</returns>
		Scope* GetParent() const;

		/// <summary>
		/// Returns a number representing the number of slots (with chaining) contained within the internal HashMap
		/// </summary>
		/// <returns>The number of buckets within the internal HashMap</returns>
		size_t BucketSize() const;
		/// <summary>
		/// Returns the number of string datum pairs (attributes) contained within this scope
		/// </summary>
		/// <returns>The number of attributes contained within this scope</returns>
		size_t NumAttributes() const;

		/// <summary>
		/// Checks whether or not this Scope is an ancestor of the passed in Scope.
		/// </summary>
		/// <param name="scope">The Scope being determined if it is a child of this Scope</param>
		/// <returns>True if this Scope is an ancestor of the passed in Scope, False otherwise</returns>
		bool IsAncestorOf(const Scope& scope) const;
		/// <summary>
		/// Checks whether or not this Scope is a descendant of the passed in Scope
		/// </summary>
		/// <param name="scope">The Scope being determined if it is this Scopes ancestor</param>
		/// <returns>True if this Scope is a descendant of the passed in Scope, False otherwise</returns>
		bool IsDescendantOf(const Scope& scope) const;

#pragma endregion

#pragma region AtributeAccess

		/// <summary>
		/// Gives access to the order vector of attributes contained within this Scope
		/// </summary>
		/// <returns>A const reference to the order vector of name datum pairs contained within this Scope</returns>
		const Vector<PairType*>& GetOrderVector() const;

		/// <summary>
		/// Finds and returns a pointer to the Datum associated with the passed in name.
		/// This function only looks within this Scope's attributes.
		/// </summary>
		/// <param name="name">The string key of the Datum being searched for</param>
		/// <returns>A pointer to the Datum associated with the name passed in. Will return nullptr if nothing is found.</returns>
		Datum* Find(const std::string& name);

		/// <summary>
		/// Finds and returns a const Datum pointer associated with the passed in name.
		/// This function only looks within this Scope's attributes.
		/// </summary>
		/// <param name="name">The string key of the Datum being searched for</param>
		/// <returns>A const Datum pointer of the Datum associated with the name passed in. Will return nullptr if nothing is found.</returns>
		const Datum* Find(const std::string& name) const;

		/// <summary>
		/// Finds and returns a pointer to the Datum associated with the passed in name.
		/// Will not only search this Scope's attributes but will also search the hierarchy of its parents.
		/// </summary>
		/// <param name="name">The string key of the Datum being searched for</param>
		/// <param name="foundScope">An output parameter that, if not nullptr, will be set to the address of the Scope in which the returned Datum was found</param>
		/// <returns>A pointer to the Datum associated with the name passed in. Will return nullptr if nothing is found.</returns>
		Datum* Search(const std::string& name, Scope** foundScope = nullptr);

		/// <summary>
		/// Finds and returns a const Datum pointer associated with the passed in name.
		/// Will not only search this Scope's attributes but will also search the hierarchy of its parents.
		/// </summary>
		/// <param name="name">The string key of the Datum being searched for</param>
		/// <param name="foundScope">An output parameter that, if not nullptr, will be set to the address of the Scope in which the returned Datum was found</param>
		/// <returns>A const Datum pointer of the Datum associated with the name passed in. Will return nullptr if nothing is found.</returns>
		const Datum* Search(const std::string& name, Scope** foundScope = nullptr) const;

		/// <summary>
		/// Searches this Scope as well as up the hierarchy for the Datum associated with the passed in name and has the passed in value.
		/// Note: Method is virtual so that child classes can implement lateral hierarchal look up.
		/// </summary>
		/// <param name="name">The name of the attribute being looked for</param>
		/// <param name="value">The value of the attribute being looked for</param>
		/// <param name="foundScope">An optional pointer to a scope pointer that will point to the Scope where the desired attribute was found.</param>
		/// <returns>A pointer to the Datum found or nullptr otherwise</returns>
		virtual Datum* SearchForValue(const std::string& name, const Datum& value, Scope** foundScope = nullptr);

		/// <summary>
		/// Finds the Datum* and index within that Datum of which the passed in Scope* belongs to. 
		/// </summary>
		/// <param name="scope">A pointer to the Scope attempting to be found within this Scope</param>
		/// <returns>A std::pair that contains the Datum* and index of where the passed in Scope* was found. The Datum* will be nullptr if nothing was found</returns>
		std::pair<Datum*, size_t> FindNestedScope(const Scope* scope);

#pragma endregion

#pragma region ModifyAtributes

		/// <summary>
		/// Takes a constant string and returns a reference to a Datum with the associated name. If one doesn't exist it will
		/// create one, otherwise it will return the already existing one. 
		/// </summary>
		/// <param name="name">The name that the returned Datum should be associated with</param>
		/// <returns>The new or existing Datum associated with the passed in name</returns>
		/// <exception cref="std::runtime_error">Throws an exception if you pass an empty string to this method</exception>
		Datum& Append(const std::string& name);

		/// <summary>
		/// Takes a constant string and adds a new Scope to either an existing Table Datum associated with that string or
		/// to a newly created Table Datum associated with that string.
		/// </summary>
		/// <param name="name">The key name whose associated Datum the new Scope will be appended to</param>
		/// <returns>A reference to the newly appended Scope</returns>
		/// <exception cref="std::runtime_error">
		/// Throws an exception if there already exists a Datum not of type Table already associated with the passed in name,
		/// or if you pass it an empty string for name.
		/// </exception>
		Scope& AppendScope(const std::string& name, size_t bucketSize = 11);

		/// <summary>
		/// Takes a Scope pointer and Orphans it from its current parent and Appends it to this Scope inside a Datum associated with the passed in name.
		/// Only pass a heap allocated Scope child. Stack allocated children will break Scope.
		/// </summary>
		/// <param name="child">The reference to the Scope that will be Orphaned from its current parent and appended to this Scope</param>
		/// <param name="name">The std::string key whose associated Datum will have the child Scope inserted into it</param>
		/// <exception cref="std::runtime_error">Throws an exception if there already exists a Datum not of type Table associated with the name passed in</exception>
		void Adopt(Scope& child, const std::string& name);
		/// <summary>
		/// Removes the passed in Scope from its current parent and sets its parent to nullptr
		/// If you call Orphan on a child and don't adopt it into another Scope, you are responsible for deleting its memory
		/// </summary>
		void Orphan();

		/// <summary>
		/// Thin wrapper to the Append function for syntactic convenience. 
		/// Will append a new Datum or return the existing one associated with the passed in name. 
		/// </summary>
		/// <param name="name">The name that the returned Datum should be associated with</param>
		/// <returns>The new or existing Datum associated with the passed in name</returns>
		/// <exception cref="std::runtime_error">Throws an exception if you pass an empty string to this method</exception>
		Datum& operator[](const std::string& name);
		/// <summary>
		/// Takes an unsigned integer and returns a reference to a Datum contained within this Scope at the given index.
		/// Index values correspond to the order in which items were appended.
		/// </summary>
		/// <param name="index">The index of the Datum that should be returned</param>
		/// <returns>A reference to the Datum associated with the passed in index</returns>
		/// <exception cref="std::runtime_error">Throws an exception if the index is outside the range of this Scopes number of attributes</exception>
		Datum& operator[](size_t index);
		/// <summary>
		/// Takes an unsigned integer and returns a const reference to a Datum contained within this Scope at the given index.
		/// Index values correspond to the order in which items were appended.
		/// </summary>
		/// <param name="index">The index of the Datum that should be returned</param>
		/// <returns>A const reference to the Datum associated with the passed in index</returns>
		/// <exception cref="std::runtime_error">Throws an exception if the index is outside the range of this Scopes number of attributes</exception>
		const Datum& operator[](size_t index) const;

		/// <summary>
		/// Clears all allocated memory that this Scope owns. This includes calling delete on all owned Scope children.
		/// </summary>
		void Clear();

		/// <summary>
		/// Clones the Scope by creating a new deep copied version of this Scope.
		/// Children of Scope should override this method so that it clones the correct object type.
		/// </summary>
		/// <returns>A pointer to the new Scope copy</returns>
		virtual gsl::owner<Scope*> Clone() const;

#pragma endregion

#pragma region RTTIOverloads
		/// <summary>
		/// ToString override for RTTI so that the interface returns the correct string representation
		/// </summary>
		/// <returns>The string representation of this Scope</returns>
		std::string ToString() const override;

		/// <summary>
		/// Equals override for RTTI so that the interface can compare two scopes for equality.
		/// </summary>
		/// <param name="rhs">The RTTI* being compared to this Scope</param>
		/// <returns>True if the passed in RTTI* is a Scope and is equal to this Scope, false otherwise</returns>
		bool Equals(const RTTI* rhs) const override;
#pragma endregion

	protected:
		/// <summary>
		/// Constructor for creating a scope with default capacity and a known parent
		/// </summary>
		/// <param name="parent">The address of the Scope that this Scope belongs to</param>
		Scope(Scope* parent);

		/// <summary>
		/// A single function for the Copy Semantics so that the Copy constructor and assignment operator don't duplicate code
		/// </summary>
		/// <param name="rhs">The Scope being copied into this one</param>
		void CopyHelper(const Scope& rhs);

		/// <summary>
		/// A single function for the Move Semantics so that the Move constructor and assignment operator don't duplicate code
		/// </summary>
		void MoveHelper(const Scope* rhs) noexcept;

		Scope* mParent = nullptr;
		HashMap<std::string, Datum> mMap;
		Vector<PairType*> mOrderVector;
	};
}