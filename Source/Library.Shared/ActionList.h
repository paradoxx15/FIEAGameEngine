#pragma once

#include "Factory.h"
#include "Action.h"

namespace Library
{
	class ActionList : public Action
	{
		RTTI_DECLARATIONS(ActionList, Action)

	public:

		/// <summary>
		/// The index of the Actions attribute in the ordered vector
		/// </summary>
		inline const static size_t ActionsKey = 3;

		/// <summary>
		/// Returns the signatures of the prescribed attributed of this class.
		/// </summary>
		/// <returns>Vector of signatures of the prescribed attributes</returns>
		static const Vector<Signature> GetSignatures();

		/// <summary>
		/// Default constructor
		/// </summary>
		ActionList();
		/// <summary>
		/// Explicit constructor that takes a name for this ActionList
		/// </summary>
		/// <param name="name">The name of the action</param>
		explicit ActionList(const std::string& name);
		/// <summary>
		/// Default copy constructor.
		/// </summary>
		/// <param name="rhs">The ActionList being copied into this one</param>
		ActionList(const ActionList& rhs) = default;
		/// <summary>
		/// Default move constructor.
		/// </summary>
		/// <param name="rhs">The ActionList being moved into this one</param>
		ActionList(ActionList&& rhs) = default;
		/// <summary>
		/// Default copy assignment operator.
		/// </summary>
		/// <param name="rhs">The Actionlist beign copied into this one</param>
		/// <returns>A reference to this ActionList after its been mutated</returns>
		ActionList& operator=(const ActionList& rhs) = default;
		/// <summary>
		/// Default move assignment operator.
		/// </summary>
		/// <param name="rhs">The ActionList beign copied into this one</param>
		/// <returns>A reference to this ActionList after its been mutated</returns>
		ActionList& operator=(ActionList&& rhs) = default;
		/// <summary>
		/// Defaulted destructor.
		/// </summary>
		virtual ~ActionList() = default;

		/// <summary>
		/// Returns the Datum reference that stores the contained actions in this list.
		/// </summary>
		/// <returns>A reference to the Datum that contains the actions in this list</returns>
		Datum& Actions();
		/// <summary>
		/// Returns the Datum reference that stores the contained actions in this list.
		/// </summary>
		/// <returns>A const reference to the Datum that contains the actions in this list</returns>
		const Datum& Actions() const; 

		/// <summary>
		/// Updates each action contained within the list
		/// </summary>
		/// <param name="state">The curent state of the world</param>
		void Update(WorldState& state) override;

		/// <summary>
		/// Creates and returns a clone of this ActionList.
		/// </summary>
		/// <returns>The address of the newly allocated clone of this ActionList</returns>
		gsl::owner<Scope*> Clone() const override;

		/// <summary>
		/// Returns a string representation of the ActionList (its name).
		/// </summary>
		/// <returns>A string representation of the ActionList</returns>
		std::string ToString() const override;

		/// <summary>
		/// Checks whether or not two ActionLists are equal.
		/// </summary>
		/// <param name="rhs">An RTTI* to the ActionList object this is being compared to</param>
		/// <returns>True if the objects are logically equal, false otherwise</returns>
		bool Equals(const RTTI* rhs) const override;

	protected:
		/// <summary>
		/// Explicit protected constructor that takes a passed in typeId to be passed to the base Action constructor.
		/// </summary>
		/// <param name="typeId">The typeId of the child class</param>
		explicit ActionList(RTTI::IdType typeId);
		/// <summary>
		/// Explicit protected constructor that takes a passed in typeId to be passed to the base Action constructor.
		/// </summary>
		/// <param name="typeId">The typeId of the child class</param>
		/// <param name="name">The name of this ActionIncrement</param>
		explicit ActionList(RTTI::IdType typeId, const std::string& name);
	};

	ConcreteFactory(ActionList, Scope)
}