#pragma once
#include "Attributed.h"
#include "WorldState.h"

namespace Library
{
	class Action : public Attributed
	{
		RTTI_DECLARATIONS(Action, Attributed)

	public:
		/// <summary>
		/// Returns the signatures of the prescribed attributed of this class.
		/// </summary>
		/// <returns>Vector of signatures of the prescribed attributes</returns>
		static const Vector<Signature> GetSignatures();

		/// <summary>
		/// Default constructor.
		/// </summary>
		Action() = delete;
		/// <summary>
		/// Default copy constructor.
		/// </summary>
		/// <param name="rhs">The Action being copied into this one</param>
		Action(const Action& rhs) = default;
		/// <summary>
		/// Default move constructor.
		/// </summary>
		/// <param name="rhs">The Action being moved into this one</param>
		Action(Action&& rhs) = default;
		/// <summary>
		/// Default copy assignment operator.
		/// </summary>
		/// <param name="rhs">The Action being copied into this one</param>
		/// <returns>A reference to this Action after its been mutated</returns>
		Action& operator=(const Action& rhs) = default;
		/// <summary>
		/// Default move assignment operator.
		/// </summary>
		/// <param name="rhs">The Action being copied into this one</param>
		/// <returns>A reference to this Action after its been mutated</returns>
		Action& operator=(Action&& rhs) = default;
		/// <summary>
		/// Virtual destructor. Defaulted for the base class.
		/// </summary>
		virtual ~Action() = default;

		/// <summary>
		/// Searches this Action as well as up the hierarchy and laterally within the other Actions contained within this Actors parent Entity
		/// for the Datum associated with the passed in name and value.
		/// </summary>
		/// <param name="name">The name of the attribute being looked for</param>
		/// <param name="value">The value of the attribute being looked for</param>
		/// <param name="foundScope">An optional pointer to a scope pointer that will point to the Scope where the desired attribute was found.</param>
		/// <returns>A pointer to the Datum found or nullptr otherwise</returns>
		Datum* SearchForValue(const std::string& name, const Datum& value, Scope** foundScope = nullptr) override;

		/// <summary>
		/// Searches the ArgumentStack contained within the WorldState as well as 
		/// this Action and its hierarchy for the Datum associated with the passed in name. 
		/// </summary>
		/// <param name="name">The name of the Datum being searched for</param>
		/// <param name="worldState">The current WorldState that holds the ArgumentStack</param>
		/// <returns></returns>
		Datum* Search(const std::string& name, WorldState& worldState);

		/// <summary>
		/// Returns a reference to the name of the Action.
		/// </summary>
		/// <returns>Const std::string reference to the name of the Action</returns>
		const std::string& Name() const;
		/// <summary>
		/// Sets the name of this Action to the passed in name.
		/// </summary>
		/// <param name="name">The new name of the Action</param>
		void SetName(const std::string & name);

		/// <summary>
		/// Updates the Action based on the current WorldState.
		/// </summary>
		/// <param name="state">A reference to the current WorldState that this Action exists within</param>
		virtual void Update(WorldState & state) = 0;

		/// <summary>
		/// Creates and returns a clone of this Action.
		/// </summary>
		/// <returns>The address of the newly allocated clone of this Action</returns>
		gsl::owner<Scope*> Clone() const override = 0;

		/// <summary>
		/// Returns a string representation of the Action (its name).
		/// </summary>
		/// <returns>A string representation of the Action</returns>
		std::string ToString() const override;

	protected:
		/// <summary>
		/// Explicit protected constructor that takes a passed in typeId to be passed to the base Attributed constructor.
		/// </summary>
		/// <param name="typeId">The typeId of the child class</param>
		explicit Action(RTTI::IdType typeId);
		/// <summary>
		/// Explicit protected constructor that takes a passed in typeId to be passed to the base Attributed constructor.
		/// </summary>
		/// <param name="typeId">The typeId of the child class</param>
		/// <param name="name">The name of this Action</param>
		explicit Action(RTTI::IdType typeId, const std::string & name);

		/// <summary>
		/// The name of the Action.
		/// </summary>
		std::string mName;

		/// <summary>
		/// Used to determine if this action runs once and then deletes itself or continues to run
		/// </summary>
		int mRunOnce = 0;
	};
}

