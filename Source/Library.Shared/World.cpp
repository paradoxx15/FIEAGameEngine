#include "pch.h"
#include "World.h"
#include "Vector.h"
#include "Sector.h"
#include <sstream>

namespace Library
{
	RTTI_DEFINITIONS(World);

	const Vector<Signature> World::GetSignatures()
	{
		return Vector<Signature>
		{
			{"Name"s, Datum::DatumTypes::String, true, 1, offsetof(World, mName)},
			{ "Sectors"s, Datum::DatumTypes::Table, false, 1, 0}
		};
	}

	Datum* World::FindRelativeDatum(Scope& baseScope, const std::string& path, Scope** foundScope)
	{
		Vector<std::string> splitPath;
		std::stringstream ss(path);
		
		std::string token;
		while (std::getline(ss, token, '.'))
		{
			splitPath.PushBack(token);
		}

		Scope* context = &baseScope;
		Datum* retDatum = nullptr;

		for (size_t i = 0; i < splitPath.Size(); ++i)
		{
			if (i == splitPath.Size() - 1)
			{
				retDatum = context->Search(splitPath[i], &context);
			}
			else
			{
				retDatum = context->SearchForValue("Name", Datum(splitPath[i]), &context);
			}
		}

		if (foundScope != nullptr)
		{
			*foundScope = context;
		}

		return retDatum;
	}

	World::World() : Attributed(TypeIdClass()) 
	{
		mWorldState.World = this;
	}

	const std::string& World::Name() const
	{
		return mName;
	}

	void World::SetName(const std::string& name)
	{
		mName = name;
	}

	Datum& World::Sectors()
	{
		return mOrderVector[SectorsKey]->second;
	}

	const Datum& World::Sectors() const
	{
		return mOrderVector[SectorsKey]->second;
	}

	EventQueue& World::GetEventQueue()
	{
		return mEventQueue;
	}

	const EventQueue& World::GetEventQueue() const
	{
		return mEventQueue;
	}

	WorldState& World::GetWorldState()
	{
		return mWorldState;
	}

	const WorldState& World::GetWorldState() const
	{
		return mWorldState;
	}

	Sector* World::CreateSector(const std::string& sectorName)
	{
		Sector* sector = new Sector;

		sector->SetName(sectorName);
		Adopt(*sector, "Sectors");

		return sector;
	}

	void World::MarkScopeForDelete(Scope& scope)
	{
		mPendingDelete.PushBack(&scope);
	}

	void World::Update()
	{
		mGameClock.UpdateGameTime(mWorldState.GetGameTime());
		mEventQueue.Update(mWorldState.GetGameTime());

		for (size_t i = 0; i < Sectors().Size(); i++)
		{
			assert(Sectors().GetScope(i)->Is(Sector::TypeIdClass()));
			Sector* sector = static_cast<Sector*>(Sectors().GetScope(i));
			mWorldState.Sector = sector;
			sector->Update(mWorldState);
			mWorldState.Sector = nullptr;
		}

		if (mPendingDelete.Size() > 0)
		{
			for (size_t i = 0; i < mPendingDelete.Size(); ++i)
			{
				mPendingDelete[i]->Orphan();
				delete mPendingDelete[i];
			}

			mPendingDelete.Clear();
		}
	}

	gsl::owner<Scope*> World::Clone() const
	{
		return new World(*this);
	}

	std::string World::ToString() const
	{
		return mName;
	}

	bool World::Equals(const RTTI* rhs) const
	{
		const World* world = rhs->As<World>();
		if (world == nullptr)
		{
			return false;
		}

		return *this == *world;
	}



}