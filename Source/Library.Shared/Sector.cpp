#include "pch.h"
#include "Sector.h"

namespace Library
{
	RTTI_DEFINITIONS(Sector);

	const Vector<Signature> Sector::GetSignatures()
	{
		return Vector<Signature>
		{
			{"Name"s, Datum::DatumTypes::String, true, 1, offsetof(Sector, mName)},
			{ "Entities"s, Datum::DatumTypes::Table, false, 1, 0}
		};
	}

	Sector::Sector() : Attributed(this->TypeIdClass()) {}

	Datum* Sector::SearchForValue(const std::string& name, const Datum& value, Scope** foundScope)
	{
		Datum* ret = Find(name);

		if (ret != nullptr && foundScope != nullptr && value == *ret)
		{
			*foundScope = this;
		}

		if (mParent != nullptr && (ret == nullptr || *ret != value))
		{
			Scope* scopeSector = nullptr;
			assert(GetWorld() != nullptr);
			Datum& sectors = GetWorld()->Sectors();

			for (size_t i = 0; i < sectors.Size(); ++i)
			{
				scopeSector = sectors.GetScope(i);
				ret = scopeSector->Find(name);

				if (ret != nullptr && scopeSector != nullptr && value == *ret)
				{
					if (foundScope != nullptr)
					{
						*foundScope = scopeSector;
					}
					return ret;
				}
			}

			ret = mParent->SearchForValue(name, value, foundScope);
		}

		return ret;
	}

	const std::string& Sector::Name() const
	{
		return mName;
	}

	void Sector::SetName(const std::string& name)
	{
		mName = name;
	}

	Datum& Sector::Entities()
	{
		return mOrderVector[EntitiesKey]->second;
	}

	const Datum& Sector::Entities() const
	{
		return mOrderVector[EntitiesKey]->second;
	}

	Entity* Sector::CreateEntity(const std::string& className, const std::string& instanceName)
	{
		Scope* scope = Factory<Scope>::Create(className);
		if (scope == nullptr)
		{
			throw std::runtime_error("Can't create an entity for a class that does not exist");
		}

		assert(scope->Is(Entity::TypeIdClass()));
		Entity* entity = static_cast<Entity*>(scope);
		entity->SetName(instanceName);
		Adopt(*entity, "Entities");

		return entity;
	}

	World* Sector::GetWorld() const
	{
		if (mParent == nullptr)
		{
			return nullptr;
		}

		assert(GetParent()->Is(World::TypeIdClass()));
		return static_cast<World*>(GetParent());
	}

	void Sector::SetWorld(World& world)
	{
		world.Adopt(*this, "Sectors");
	}

	void Sector::Update(WorldState& state)
	{
		for (size_t i = 0; i < Entities().Size(); i++)
		{
			assert(Entities().GetScope(i)->Is(Entity::TypeIdClass()));
			Entity* entity = static_cast<Entity*>(Entities().GetScope(i));
			state.Entity = entity;
			entity->Update(state);
			state.Entity = nullptr;
		}
	}

	gsl::owner<Scope*> Sector::Clone() const
	{
		return new Sector(*this);
	}

	std::string Sector::ToString() const
	{
		return mName;
	}

	bool Sector::Equals(const RTTI* rhs) const
	{
		const Sector* sector = rhs->As<Sector>();
		if (sector == nullptr)
		{
			return false;
		}

		return *this == *sector;
	}
}