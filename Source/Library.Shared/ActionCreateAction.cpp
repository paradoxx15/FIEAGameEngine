#include "pch.h"
#include "ActionCreateAction.h"
#include "World.h"

namespace Library
{
	RTTI_DEFINITIONS(ActionCreateAction);

	const Vector<Signature> ActionCreateAction::GetSignatures()
	{
		return Vector<Signature>
		{
			{ "ActionName"s, Datum::DatumTypes::String, true, 1, offsetof(ActionCreateAction, mActionName) },
			{ "Prototype"s, Datum::DatumTypes::String, true, 1, offsetof(ActionCreateAction, mPrototype) }
		};
	}

	ActionCreateAction::ActionCreateAction() : Action(TypeIdClass()) {}

	ActionCreateAction::ActionCreateAction(const std::string& name) : Action(TypeIdClass(), name) {}

	void ActionCreateAction::Update(WorldState& state)
	{
		Scope* createdScope = Factory<Scope>::Create(Search("Prototype", state)->GetString());
		assert(createdScope->Is(Action::TypeIdClass()));

		Action* createdAction = static_cast<Action*>(createdScope);
		createdAction->SetName(Search("ActionName", state)->GetString());

		for (size_t i = AuxAttributesKey; i < mOrderVector.Size(); ++i)
		{
			const std::string& attribute = mOrderVector[i]->first;
			Datum& data = mOrderVector[i]->second;
			createdAction->Append(attribute) = data;
		}

		mParent->Adopt(*createdAction, "Actions");

		state.World->MarkScopeForDelete(*this);
	}

	gsl::owner<Scope*> ActionCreateAction::Clone() const
	{
		return new ActionCreateAction(*this);
	}

	std::string ActionCreateAction::ToString() const
	{
		return mName;
	}

	bool ActionCreateAction::Equals(const RTTI* rhs) const
	{
		const ActionCreateAction* actionCreateAction = rhs->As<ActionCreateAction>();
		if (actionCreateAction == nullptr)
		{
			return false;
		}

		return *this == *actionCreateAction;
	}
}