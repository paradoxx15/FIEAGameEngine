#include "pch.h"
#include "Entity.h"

namespace Library
{
	RTTI_DEFINITIONS(Entity);

	const Library::Vector<Signature> Entity::GetSignatures()
	{
		return Vector<Signature>
		{
			{ "Name"s, Datum::DatumTypes::String, true, 1, offsetof(Entity, mName) },
			{ "Actions"s, Datum::DatumTypes::Table, false, 1, 0 },
		};
	}

	Entity::Entity() : Attributed(this->TypeIdClass()) {}

	Entity::Entity(const std::string& name) : 
		Attributed(this->TypeIdClass()), mName(name)
	{
	}

	Entity::Entity(RTTI::IdType typeId) : Attributed(typeId) {}

	Entity::Entity(RTTI::IdType typeId, const std::string& name) :
		Attributed(typeId), mName(name)
	{
	}

	Datum* Entity::SearchForValue(const std::string& name, const Datum& value, Scope** foundScope)
	{
		Datum* ret = Find(name);

		if (ret != nullptr && foundScope != nullptr && value == *ret)
		{
			*foundScope = this;
		}

		if (mParent != nullptr && (ret == nullptr || *ret != value))
		{
			Scope* scopeEntity;
			assert(GetSector() != nullptr);
			Datum& entities = GetSector()->Entities();

			for (size_t i = 0; i < entities.Size(); ++i)
			{
				scopeEntity = entities.GetScope(i);
				ret = scopeEntity->Find(name);

				if (ret != nullptr && scopeEntity != nullptr && value == *ret)
				{
					if (foundScope != nullptr)
					{
						*foundScope = scopeEntity;
					}
					return ret;
				}
			}

			ret = mParent->SearchForValue(name, value, foundScope);
		}

		return ret;
	}

	Datum& Entity::Actions()
	{
		return mOrderVector[ActionsKey]->second;
	}

	const Datum& Entity::Actions() const
	{
		return mOrderVector[ActionsKey]->second;
	}

	Action* Entity::CreateAction(const std::string& className, const std::string& instanceName)
	{
		Scope* scope = Factory<Scope>::Create(className);
		if (scope == nullptr)
		{
			throw std::runtime_error("Can't create an entity for a class that does not exist");
		}

		assert(scope->Is(Action::TypeIdClass()));
		Action* action = static_cast<Action*>(scope);
		action->SetName(instanceName);
		Adopt(*action, "Entities");

		return action;
	}

	const std::string& Entity::Name() const
	{
		return mName;
	}

	void Entity::SetName(const std::string& name)
	{
		mName = name;
	}

	Sector* Entity::GetSector() const
	{
		if (mParent == nullptr)
		{
			return nullptr;
		}

		assert(GetParent()->Is(Sector::TypeIdClass()));
		return static_cast<Sector*>(GetParent());
	}

	void Entity::SetSector(Sector& sector)
	{
		sector.Adopt(*this, "Entities");
	}

	void Entity::Update(WorldState& state) 
	{
		for (size_t i = 0; i < Actions().Size(); i++)
		{
			assert(Actions().GetScope(i)->Is(Action::TypeIdClass()));
			Action* action = static_cast<Action*>(Actions().GetScope(i));
			state.Action = action;
			action->Update(state);
			state.Action = nullptr;
		}
	}

	gsl::owner<Scope*> Entity::Clone() const
	{
		return new Entity(*this);
	}

	std::string Entity::ToString() const
	{
		return mName;
	}

	bool Entity::Equals(const RTTI* rhs) const
	{
		const Entity* entityPointer = rhs->As<Entity>();
		if (entityPointer == nullptr)
		{
			return false;
		}

		return *this == *entityPointer;
	}
}