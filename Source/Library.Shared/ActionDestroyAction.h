#pragma once
#include "Action.h"
#include "Factory.h"

namespace Library
{
	class ActionDestroyAction : public Action
	{
		RTTI_DECLARATIONS(ActionDestroyAction, Action)

	public:

		/// <summary>
		/// Returns the signatures of the prescribed attributed of this class.
		/// </summary>
		/// <returns>Vector of signatures of the prescribed attributes</returns>
		static const Vector<Signature> GetSignatures();

		/// <summary>
		/// Default constructor
		/// </summary>
		ActionDestroyAction();
		/// <summary>
		/// Explicit constructor that takes a name for this ActionList
		/// </summary>
		/// <param name="name">The name of the action</param>
		explicit ActionDestroyAction(const std::string& name);
		/// <summary>
		/// Default copy constructor.
		/// </summary>
		/// <param name="rhs">The ActionDestroyAction being copied into this one</param>
		ActionDestroyAction(const ActionDestroyAction& rhs) = default;
		/// <summary>
		/// Default move constructor.
		/// </summary>
		/// <param name="rhs">The ActionDestroyAction being moved into this one</param>
		ActionDestroyAction(ActionDestroyAction&& rhs) = default;
		/// <summary>
		/// Default copy assignment operator.
		/// </summary>
		/// <param name="rhs">The ActionDestroyAction beign copied into this one</param>
		/// <returns>A reference to this ActionDestroyAction after its been mutated</returns>
		ActionDestroyAction& operator=(const ActionDestroyAction& rhs) = default;
		/// <summary>
		/// Default move assignment operator.
		/// </summary>
		/// <param name="rhs">The ActionDestroyAction being copied into this one</param>
		/// <returns>A reference to this ActionDestroyAction after its been mutated</returns>
		ActionDestroyAction& operator=(ActionDestroyAction&& rhs) = default;
		/// <summary>
		/// Defaulted destructor.
		/// </summary>
		~ActionDestroyAction() = default;

		/// <summary>
		/// Updates each action contained within the list
		/// </summary>
		/// <param name="state">The curent state of the world</param>
		void Update(WorldState& state) override;

		/// <summary>
		/// Creates and returns a clone of this ActionDestroyAction.
		/// </summary>
		/// <returns>The address of the newly allocated clone of this ActionDestroyAction</returns>
		gsl::owner<Scope*> Clone() const override;

		/// <summary>
		/// Returns a string representation of the ActionIncrement.
		/// </summary>
		/// <returns>A string representation of the ActionIncrement</returns>
		std::string ToString() const override;

		/// <summary>
		/// Checks whether or not two ActionIncrement are equal.
		/// </summary>
		/// <param name="rhs">An RTTI* to the ActionIncrement object this is being compared to</param>
		/// <returns>True if the objects are logically equal, false otherwise</returns>
		bool Equals(const RTTI* rhs) const override;

	protected:

		/// <summary>
		/// The name of the Action being created.
		/// </summary>
		std::string mAction;
	};

	ConcreteFactory(ActionDestroyAction, Scope)
}
