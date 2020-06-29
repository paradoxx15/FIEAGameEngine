#include "pch.h"
#include "Reaction.h"

namespace Library
{
	RTTI_DEFINITIONS(Reaction);

	Reaction::Reaction(RTTI::IdType typeId) : ActionList(typeId) {}

	Reaction::Reaction(RTTI::IdType typeId, const std::string& name) : ActionList(typeId, name) {}

	const Vector<Signature> Reaction::GetSignatures()
	{
		return Vector<Signature>();
	}

	void Reaction::Update([[maybe_unused]] WorldState& state)
	{
	}
}
