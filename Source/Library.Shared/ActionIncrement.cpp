#include "pch.h"
#include "ActionIncrement.h"
#include "World.h"

namespace Library
{
	RTTI_DEFINITIONS(ActionIncrement);

	const Vector<Signature> ActionIncrement::GetSignatures()
	{
		return Vector<Signature>
		{
			{ "Target"s, Datum::DatumTypes::String, true, 1, offsetof(ActionIncrement, mTarget) },
			{ "Step"s, Datum::DatumTypes::Integer, true, 1, offsetof(ActionIncrement, mStep) }
		};
	}

	ActionIncrement::ActionIncrement() : Action(TypeIdClass()) {}

	ActionIncrement::ActionIncrement(const std::string& name) : Action(TypeIdClass(), name) {}

	void ActionIncrement::Update([[maybe_unused]] WorldState& state)
	{
		Datum* target = World::FindRelativeDatum(*this, Search("Target", state)->GetString()); 
		if (target != nullptr)
		{
			target->GetInt() += Search("Step", state)->GetInt();
		}

		Action::Update(state);
	}

	gsl::owner<Scope*> ActionIncrement::Clone() const
	{
		return new ActionIncrement(*this);
	}

	bool ActionIncrement::Equals(const RTTI* rhs) const
	{
		const ActionIncrement* actionIncrement = rhs->As<ActionIncrement>();
		if (actionIncrement == nullptr)
		{
			return false;
		}

		return *this == *actionIncrement;
	}
}