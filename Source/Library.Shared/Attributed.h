#pragma once

#include "Scope.h"
#include "TypeManager.h"
#include <gsl/gsl>

namespace Library
{
	class Attributed : public Scope
	{
		RTTI_DECLARATIONS(Attributed, Scope);

	public:
		/// <summary>
		/// Copy constructor. Copies all data from the rhs Attributed object and then fixes up the this attribute to point to this object,
		/// and fixes all external storage to be pointing to this objects storage.
		/// </summary>
		/// <param name="rhs">The Attributed object being copied into this object</param>
		Attributed(const Attributed& rhs);
		/// <summary>
		/// Move constructor. Moves all data from the rhs Attributed object and then fixes up the this attribute to point to this object,
		/// and fixes all external storage to be pointing to this objects storage.
		/// </summary>
		/// <param name="rhs">The Attributed object being moved into this object</param>
		Attributed(Attributed&& rhs) noexcept;

		/// <summary>
		/// Copy assignment operator. Clears all current data and copies all data from the rhs Attributed object and then fixes up the this attribute to point to this object,
		/// and fixes all external storage to be pointing to this objects storage.
		/// </summary>
		/// <param name="rhs">The Attributed object being copied into this object</param>
		Attributed& operator=(const Attributed& rhs);
		/// <summary>
		/// Move assignment operator. Clears all current data and then moves all data from the rhs Attributed object and then fixes up the this attribute to point to this object,
		/// and fixes all external storage to be pointing to this objects storage.
		/// </summary>
		/// <param name="rhs">The Attributed object being moved into this object</param>
		Attributed& operator=(Attributed&& rhs) noexcept;

		/// <summary>
		/// Checks if this Attributed object contains an attribute associated with the passed in name.
		/// </summary>
		/// <param name="name">The name used to check if there is an attribute associated with it</param>
		/// <returns>True if this Attributed object contains an attribute with the passed in name, false otherwise</returns>
		bool IsAttribute(const std::string& name) const;

		/// <summary>
		/// Checks if this Attributed object contains a prescribed attribute associated with the passed in name.
		/// </summary>
		/// <param name="name">The name used to check if there is a prescribed attribute associated with it</param>
		/// <returns>True if this Attributed object contains a prescribed attribute with the passed in name, false otherwise</returns>
		bool IsPrescribedAttribute(const std::string& name) const;

		/// <summary>
		/// Checks if this Attributed object contains an auxiliary attribute associated with the passed in name.
		/// </summary>
		/// <param name="name">The name used to check if there is an auxiliary attribute associated with it</param>
		/// <returns>True if this Attributed object contains an auxiliary attribute with the passed in name, false otherwise</returns>
		bool IsAuxiliaryAttribute(const std::string& name) const;

		/// <summary>
		/// Appends a new attribute with the associated passed in name and a default constructed Datum.
		/// If the attribute already exists it will return a reference to the existing Datum. 
		/// </summary>
		/// <param name="name">The name which is the key of the new appended attribute</param>
		/// <returns>A reference to the Datum associated with either the newly created Attribute or the one that already existed</returns>
		/// <exception cref="std::runtime_error">Will throw an exception if the name is already associated with a prescribed attribute</exception>
		Datum& AppendAuxilaryAttribute(const std::string& name);

		/// <summary>
		/// Accessor method to get a Vector of all of the attributes of this Attributed object
		/// </summary>
		/// <returns>Vector of pointers to each attribute contained within this object</returns>
		const Vector<PairType*>& GetAttributes() const;

		/// <summary>
		/// Virtual clone method. Required in order for derived attributed classes to be cloned with the correct type instead of becoming a Scope.
		/// </summary>
		/// <returns>The Scope* of the clone of this object</returns>
		gsl::owner<Scope*> Clone() const override = 0;

	protected:
		/// <summary>
		/// Attributed constructor. Creates the object and populates it with all of its prescribed attributes
		/// </summary>
		/// <param name="typeId">The RTTI::TypeId used to determine what prescribed attriutes this object will have</param>
		explicit Attributed(RTTI::IdType typeId);

		/// <summary>
		/// Populates this object with all of the prescribed attributes of itself and its parents
		/// </summary>
		/// <param name="typeId">The typeId of this object. Used to determine what presecribed attributes this method grabs from the TypeManager</param>
		void Populate(RTTI::IdType typeId);

		/// <summary>
		/// Used during moves and copies to update the external storage prescribed Attributes to point to the correct data members of this object.
		/// </summary>
		/// <param name="typeId">The typeId of this object instance used to get the correct signatures</param>
		void UpdateExternalStorage(RTTI::IdType typeId);
	};
}

