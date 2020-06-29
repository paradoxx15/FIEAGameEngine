#include "pch.h"
#include "Action.h"
#include "World.h"

using namespace std::string_literals;

namespace Library
{
	RTTI_DEFINITIONS(Action);

	const Vector<Signature> Action::GetSignatures()
	{
		return Vector<Signature>
		{
			{"Name"s, Datum::DatumTypes::String, true, 1, offsetof(Action, mName)},
			{ "RunOnce"s, Datum::DatumTypes::Integer, true, 1, offsetof(Action, mRunOnce) },
		};
	}

	Action::Action(RTTI::IdType typeId) : Attributed(typeId) {}

	Action::Action(RTTI::IdType typeId, const std::string& name) :
		Attributed(typeId), mName(name)
	{
	}

	void Action::Update(WorldState& state)
	{
		if (mRunOnce != 0)
		{
			state.World->MarkScopeForDelete(*this);
		}
	}

	Datum* Action::SearchForValue(const std::string& name, const Datum& value, Scope** foundScope)
	{
		Datum* ret = Find(name);

		if (ret != nullptr && foundScope != nullptr && value == *ret)
		{
			*foundScope = this;
		}

		if (mParent != nullptr && (ret == nullptr || *ret != value))
		{
			Scope* scopeAction;
			Datum* actions = mParent->Find("Actions");

			if (actions != nullptr)
			{
				for (size_t i = 0; i < actions->Size(); ++i)
				{
					scopeAction = actions->GetScope(i);
					ret = scopeAction->Find(name);

					if (ret != nullptr && scopeAction != nullptr && value == *ret)
					{
						if (foundScope != nullptr)
						{
							*foundScope = scopeAction;
						}
						return ret;
					}
				}
			}

			ret = mParent->SearchForValue(name, value, foundScope);
		}

		return ret;
	}

	Datum* Action::Search(const std::string& name, WorldState& worldState)
	{
		Datum* retDatum = nullptr;

		if (!worldState.GetArgumentStack().IsEmpty())
		{
			Scope& argumentScope = worldState.GetArgumentStack().Peek();
			retDatum = argumentScope.Find(name);
		}
		
		if (retDatum == nullptr)
		{
			retDatum = Scope::Search(name);
		}

		return retDatum;
	}

	const std::string& Action::Name() const
	{
		return mName;
	}

	void Action::SetName(const std::string& name)
	{
		mName = name;
	}

	std::string Action::ToString() const
	{
		return mName;
	}
}