#pragma once
#include "ActionList.h"

namespace Library
{
	class ActionListWhile final : public ActionList
	{
		RTTI_DECLARATIONS(ActionIncrement, ActionList)

	public:

		/// <summary>
		/// Returns the signatures of the prescribed attributed of this class.
		/// </summary>
		/// <returns>Vector of signatures of the prescribed attributes</returns>
		static const Vector<Signature> GetSignatures();

		/// <summary>
		/// Default constructor
		/// </summary>
		ActionListWhile();
		/// <summary>
		/// Explicit constructor that takes a name for this ActionList
		/// </summary>
		/// <param name="name">The name of the action</param>
		explicit ActionListWhile(const std::string& name);
		/// <summary>
		/// Default copy constructor.
		/// </summary>
		/// <param name="rhs">The ActionList being copied into this one</param>
		ActionListWhile(const ActionListWhile& rhs) = default;
		/// <summary>
		/// Default move constructor.
		/// </summary>
		/// <param name="rhs">The ActionList being moved into this one</param>
		ActionListWhile(ActionListWhile&& rhs) = default;
		/// <summary>
		/// Default copy assignment operator.
		/// </summary>
		/// <param name="rhs">The Actionlist beign copied into this one</param>
		/// <returns>A reference to this ActionList after its been mutated</returns>
		ActionListWhile& operator=(const ActionListWhile& rhs) = default;
		/// <summary>
		/// Default move assignment operator.
		/// </summary>
		/// <param name="rhs">The ActionList being copied into this one</param>
		/// <returns>A reference to this ActionList after its been mutated</returns>
		ActionListWhile& operator=(ActionListWhile&& rhs) = default;
		/// <summary>
		/// Defaulted destructor.
		/// </summary>
		~ActionListWhile() = default;

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
		/// Checks whether or not two ActionListWhile are equal.
		/// </summary>
		/// <param name="rhs">An RTTI* to the ActionListWhile object this is being compared to</param>
		/// <returns>True if the objects are logically equal, false otherwise</returns>
		bool Equals(const RTTI* rhs) const override;

	protected:

		/// <summary>
		/// The relative path to the condition variable
		/// </summary>
		std::string mCondition;
	};

	ConcreteFactory(ActionListWhile, Scope)
}

