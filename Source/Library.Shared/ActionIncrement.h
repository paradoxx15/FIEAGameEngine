#pragma once
#include "Action.h"
#include "Factory.h"

namespace Library
{
	class ActionIncrement final : public Action
	{
		RTTI_DECLARATIONS(ActionIncrement, Action)

	public:

		/// <summary>
		/// Returns the signatures of the prescribed attributed of this class.
		/// </summary>
		/// <returns>Vector of signatures of the prescribed attributes</returns>
		static const Vector<Signature> GetSignatures();

		/// <summary>
		/// Default constructor
		/// </summary>
		ActionIncrement();
		/// <summary>
		/// Explicit constructor that takes a name for this ActionIncrement
		/// </summary>
		/// <param name="name">The name of the action</param>
		explicit ActionIncrement(const std::string& name);
		/// <summary>
		/// Default copy constructor.
		/// </summary>
		/// <param name="rhs">The ActionIncrement being copied into this one</param>
		ActionIncrement(const ActionIncrement& rhs) = default;
		/// <summary>
		/// Default move constructor.
		/// </summary>
		/// <param name="rhs">The ActionIncrement being moved into this one</param>
		ActionIncrement(ActionIncrement&& rhs) = default;
		/// <summary>
		/// Default copy assignment operator.
		/// </summary>
		/// <param name="rhs">The Actionlist beign copied into this one</param>
		/// <returns>A reference to this ActionIncrement after its been mutated</returns>
		ActionIncrement& operator=(const ActionIncrement& rhs) = default;
		/// <summary>
		/// Default move assignment operator.
		/// </summary>
		/// <param name="rhs">The ActionIncrement being copied into this one</param>
		/// <returns>A reference to this ActionIncrement after its been mutated</returns>
		ActionIncrement& operator=(ActionIncrement&& rhs) = default;
		/// <summary>
		/// Defaulted destructor.
		/// </summary>
		~ActionIncrement() = default;

		/// <summary>
		/// Updates each action contained within the list
		/// </summary>
		/// <param name="state">The curent state of the world</param>
		void Update(WorldState& state) override;

		/// <summary>
		/// Creates and returns a clone of this ActionIncrement.
		/// </summary>
		/// <returns>The address of the newly allocated clone of this ActionIncrement</returns>
		gsl::owner<Scope*> Clone() const override;

		/// <summary>
		/// Checks whether or not two ActionIncrement are equal.
		/// </summary>
		/// <param name="rhs">An RTTI* to the ActionIncrement object this is being compared to</param>
		/// <returns>True if the objects are logically equal, false otherwise</returns>
		bool Equals(const RTTI* rhs) const override;

	protected:

		/// <summary>
		/// The amount by which this action increments
		/// </summary>
		int mStep = 1;

		/// <summary>
		/// The string representation of the target this Action is incrementing
		/// </summary>
		std::string mTarget;
	};

	ConcreteFactory(ActionIncrement, Scope)
}

