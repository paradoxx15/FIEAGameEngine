#pragma once
#include "Action.h"
#include "Factory.h"
#include "WorldState.h"

namespace Library
{
	/// <summary>
	/// All auxiliary attributes are considered arguments for the created Action
	/// </summary>
	class ActionCreateAction final : public Action
	{
		RTTI_DECLARATIONS(ActionCreateAction, Action)

	public:

		/// <summary>
		/// The starting key of all auxiliary attributes to this Action
		/// </summary>
		inline const static size_t AuxAttributesKey = 4;

		/// <summary>
		/// Returns the signatures of the prescribed attributed of this class.
		/// </summary>
		/// <returns>Vector of signatures of the prescribed attributes</returns>
		static const Vector<Signature> GetSignatures();

		/// <summary>
		/// Default constructor
		/// </summary>
		ActionCreateAction();
		/// <summary>
		/// Explicit constructor that takes a name for this ActionCreateAction
		/// </summary>
		/// <param name="name">The name of the action</param>
		explicit ActionCreateAction(const std::string& name);
		/// <summary>
		/// Default copy constructor.
		/// </summary>
		/// <param name="rhs">The ActionCreateAction being copied into this one</param>
		ActionCreateAction(const ActionCreateAction& rhs) = default;
		/// <summary>
		/// Default move constructor.
		/// </summary>
		/// <param name="rhs">The ActionCreateAction being moved into this one</param>
		ActionCreateAction(ActionCreateAction&& rhs) = default;
		/// <summary>
		/// Default copy assignment operator.
		/// </summary>
		/// <param name="rhs">The ActionCreateAction beign copied into this one</param>
		/// <returns>A reference to this ActionCreateAction after its been mutated</returns>
		ActionCreateAction& operator=(const ActionCreateAction& rhs) = default;
		/// <summary>
		/// Default move assignment operator.
		/// </summary>
		/// <param name="rhs">The ActionCreateAction being copied into this one</param>
		/// <returns>A reference to this ActionCreateAction after its been mutated</returns>
		ActionCreateAction& operator=(ActionCreateAction&& rhs) = default;
		/// <summary>
		/// Defaulted destructor.
		/// </summary>
		~ActionCreateAction() = default;

		/// <summary>
		/// Updates each action contained within the list
		/// </summary>
		/// <param name="state">The curent state of the world</param>
		void Update(WorldState & state) override;

		/// <summary>
		/// Creates and returns a clone of this ActionCreateAction.
		/// </summary>
		/// <returns>The address of the newly allocated clone of this ActionCreateAction</returns>
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
		std::string mActionName;

		/// <summary>
		/// The name of the Action class being created.
		/// </summary>
		std::string mPrototype;
	};

	ConcreteFactory(ActionCreateAction, Scope)
}

