#include "pch.h"
#include "ActionListWhile.h"
#include "World.h"

namespace Library
{
	RTTI_DEFINITIONS(ActionListWhile);

	const Vector<Signature> ActionListWhile::GetSignatures()
	{
		return Vector<Signature>
		{
			{ "Condition"s, Datum::DatumTypes::String, true, 1, offsetof(ActionListWhile, mCondition) }
		};
	}

	ActionListWhile::ActionListWhile() : ActionList(TypeIdClass()) {}

	ActionListWhile::ActionListWhile(const std::string& name) : ActionList(TypeIdClass(), name) {}

	void ActionListWhile::Update(WorldState& state)
	{
		Datum* preambleScope = Search("Preamble", state);
		if (preambleScope != nullptr)
		{
			assert(preambleScope->GetScope()->Is(Action::TypeIdClass()));
			Action* preambleAction = static_cast<Action*>(preambleScope->GetScope());

			preambleAction->Update(state);
		}

		Datum* condition = World::FindRelativeDatum(*this, Search("Condition", state)->GetString());
		if (condition != nullptr)
		{
			while (condition->GetInt() != 0)
			{
				ActionList::Update(state);
			}
		}

		Datum* postambleScope = Search("Postamble", state);
		if (postambleScope != nullptr)
		{
			assert(postambleScope->GetScope()->Is(Action::TypeIdClass()));
			Action* postambleAction = static_cast<Action*>(postambleScope->GetScope());

			postambleAction->Update(state);
		}

		Action::Update(state);
	}

	gsl::owner<Scope*> ActionListWhile::Clone() const
	{
		return new ActionListWhile(*this);
	}

	bool ActionListWhile::Equals(const RTTI* rhs) const
	{
		const ActionListWhile* actionListWhile = rhs->As<ActionListWhile>();
		if (actionListWhile == nullptr)
		{
			return false;
		}

		return *this == *actionListWhile;
	}
}