#pragma once
#include "Entity.h"
#include "Factory.h"

using namespace Library;

namespace UnitTests
{
	class Avatar final : public Entity
	{
		RTTI_DECLARATIONS(Avatar, Entity);

	public:

		explicit Avatar(int health = 100);
		explicit Avatar(const std::string& name, int health = 100);
		Avatar(const Avatar& rhs) = default;
		Avatar(Avatar&& rhs) noexcept = default;
		Avatar& operator=(const Avatar& rhs) = default;
		Avatar& operator=(Avatar&& rhs) noexcept = default;
		~Avatar() = default;

		static const Vector<Signature> GetSignatures();

		void Update(WorldState& state) override;

		gsl::owner<Scope*> Clone() const override;

		int mHealth = 100;
		size_t mUpdateCount = 0;
	};
}

using namespace UnitTests;
namespace Library
{
	ConcreteFactory(Avatar, Scope);
}

