#include "pch.h"
#include "ActionDestroyAction.h"
#include "WorldState.h"
#include "World.h"

namespace Library
{
	RTTI_DEFINITIONS(ActionDestroyAction);

	const Vector<Signature> ActionDestroyAction::GetSignatures()
	{
		return Vector<Signature>
		{
			{ "Action"s, Datum::DatumTypes::String, true, 1, offsetof(ActionDestroyAction, mAction) }
		};
	}

	ActionDestroyAction::ActionDestroyAction() : Action(TypeIdClass()) {}

	ActionDestroyAction::ActionDestroyAction(const std::string& name) : Action(TypeIdClass(), name) {}

	void ActionDestroyAction::Update(WorldState& state)
	{
		Scope* toBeDeleted;
		SearchForValue("Name", *Search("Action", state), &toBeDeleted);
		if (toBeDeleted != nullptr)
		{
			state.World->MarkScopeForDelete(*toBeDeleted);
		}

		state.World->MarkScopeForDelete(*this);
	}

	gsl::owner<Scope*> ActionDestroyAction::Clone() const
	{
		return new ActionDestroyAction(*this);
	}
	std::string ActionDestroyAction::ToString() const
	{
		return mName;
	}
	bool ActionDestroyAction::Equals(const RTTI* rhs) const
	{
		const ActionDestroyAction* actionCreateAction = rhs->As<ActionDestroyAction>();
		if (actionCreateAction == nullptr)
		{
			return false;
		}

		return *this == *actionCreateAction;
	}
}