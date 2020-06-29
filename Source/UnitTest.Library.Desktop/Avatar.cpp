#include "pch.h"
#include "Avatar.h"

namespace UnitTests
{
	RTTI_DEFINITIONS(Avatar);

	Avatar::Avatar(int health) :
		Entity(this->TypeIdClass()), mHealth(health)
	{
	}

	Avatar::Avatar(const std::string& name, int health):
		Entity(this->TypeIdClass(), name), mHealth(health)
	{
	}

	const Vector<Signature> Avatar::GetSignatures()
	{
		return Vector<Signature>
		{
			{"Health", Datum::DatumTypes::Integer, true, 1, offsetof(Avatar, mHealth)}
		};
	}

	void UnitTests::Avatar::Update([[maybe_unused]] WorldState& state)
	{
		Entity::Update(state);
		mUpdateCount++;
	}

	gsl::owner<Scope*> UnitTests::Avatar::Clone() const
	{
		return new Avatar(*this);
	}
}