#include "pch.h"
#include "ActionList.h"

using namespace std::string_literals;

namespace Library
{
	RTTI_DEFINITIONS(ActionList);

	const Vector<Signature> ActionList::GetSignatures()
	{
		return Vector<Signature>
		{
			{ "Actions"s, Datum::DatumTypes::Table, false, 1, 0 }
		};
	}

	ActionList::ActionList() : Action(TypeIdClass()) {}

	ActionList::ActionList(const std::string& name) : Action(TypeIdClass(), name) {}

	ActionList::ActionList(RTTI::IdType typeId) : Action(typeId) {}

	ActionList::ActionList(RTTI::IdType typeId, const std::string& name) : Action(typeId, name) {}

	Datum& ActionList::Actions()
	{
		return mOrderVector[ActionsKey]->second;
	}

	const Datum& ActionList::Actions() const
	{
		return mOrderVector[ActionsKey]->second;
	}

	void ActionList::Update(WorldState& state)
	{
		for (size_t i = 0; i < Actions().Size(); i++)
		{
			assert(Actions().GetScope(i)->Is(Action::TypeIdClass()));
			Action* action = static_cast<Action*>(Actions().GetScope(i));
			state.Action = action;
			action->Update(state);
			state.Action = this;
		}

		Action::Update(state);
	}

	gsl::owner<Scope*> ActionList::Clone() const
	{
		return new ActionList(*this);
	}

	std::string ActionList::ToString() const
	{
		return mName;
	}

	bool ActionList::Equals(const RTTI* rhs) const
	{
		const ActionList* actionList = rhs->As<ActionList>();
		if (actionList == nullptr)
		{
			return false;
		}

		return *this == *actionList;
	}
}
