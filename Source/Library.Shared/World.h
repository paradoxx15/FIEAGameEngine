#pragma once
#include "Attributed.h"
#include "WorldState.h"
#include "Vector.h"
#include "EventQueue.h"

namespace Library
{
	class World final : public Attributed
	{
		RTTI_DECLARATIONS(World, Attributed);

	public:

		/// <summary>
		/// The index of the Sectors attribute within the Ordered Vector
		/// </summary>
		inline static int SectorsKey = 2;

		/// <summary>
		/// Returns the signatures of the prescribed attributed of this class.
		/// </summary>
		/// <returns>Vector of signatures of the prescribed attributes</returns>
		static const Vector<Signature> GetSignatures();

		/// <summary>
		/// This method will find a Datum given a base Scope and a string represented relative path to the sought after Datum.
		/// </summary>
		/// <param name="baseScope">The Scope where the pathing begins</param>
		/// <param name="path">The relative path of the Datum being looked for. GRAMMAR: ScopeName.AttributeName</param>
		/// <param name="foundScope">An optional output parameter for what Scope the Datum was found in</param>
		/// <returns>A pointer to the found Datum. Nullptr otherwise</returns>
		static Datum* FindRelativeDatum(Scope& baseScope, const std::string& path, Scope** foundScope = nullptr);

		/// <summary>
		/// Default constructor.
		/// </summary>
		World();
		/// <summary>
		/// Default copy constructor.
		/// </summary>
		/// <param name="rhs">The World being copied into this one</param>
		World(const World& rhs) = default;
		/// <summary>
		/// Default move constructor.
		/// </summary>
		/// <param name="rhs">The World being moved into this one</param>
		World(World&& rhs) = default;
		/// <summary>
		/// Default copy assignment operator.
		/// </summary>
		/// <param name="rhs">The World beign copied into this one</param>
		/// <returns>A reference to this World after its been mutated</returns>
		World& operator=(const World & rhs) = default;
		/// <summary>
		/// Default move assignment operator.
		/// </summary>
		/// <param name="rhs">The World beign copied into this one</param>
		/// <returns>A reference to this World after its been mutated</returns>
		World& operator=(World && rhs) = default;
		/// <summary>
		/// Default destructor.
		/// </summary>
		~World() = default;

		/// <summary>
		/// Returns a reference to the name of the World.
		/// </summary>
		/// <returns>Const std::string reference to the name of the World</returns>
		const std::string& Name() const;
		/// <summary>
		/// Sets the name of this World to the passed in name.
		/// </summary>
		/// <param name="name">The new name of the World</param>
		void SetName(const std::string & name);

		/// <summary>
		/// Returns the Datum reference that stores the contained entities in the World.
		/// </summary>
		/// <returns>A reference to the Datum that contains the Sectors of this World</returns>
		Datum& Sectors();

		/// <summary>
		/// Returns the Datum reference that stores the contained entities in the World.
		/// </summary>
		/// <returns>A const reference to the Datum that contains the Sectors of this World</returns>
		const Datum& Sectors() const;

		/// <summary>
		/// Gets a reference to this Worlds EventQueue
		/// </summary>
		/// <returns>An EventQueue reference</returns>
		EventQueue& GetEventQueue();
		/// <summary>
		/// Gets a const reference to this Worlds EventQueue
		/// </summary>
		/// <returns>A const EventQueue reference</returns>
		const EventQueue& GetEventQueue() const;

		/// <summary>
		/// Gets a reference to this Worlds WorldState
		/// </summary>
		/// <returns>A WorldState reference</returns>
		WorldState& GetWorldState();
		/// <summary>
		/// Gets a const reference to this Worlds WorldState
		/// </summary>
		/// <returns>A const WorldState reference</returns>
		const WorldState& GetWorldState() const;

		/// <summary>
		/// Creates a new Sector in this World and returns the address of the newly created Entity.
		/// </summary>
		/// <param name="sectorName">The name of the Sector being created</param>
		/// <returns>A pointer to the newly crated Sector within this World</returns>
		Sector* CreateSector(const std::string& sectorName);

		/// <summary>
		/// Given a Scope contained within the world mark it for delete so that it is removed on the next iteration of the game loop.
		/// </summary>
		/// <param name="scope">The scope to be marked for delete</param>
		void MarkScopeForDelete(Scope& scope);

		/// <summary>
		/// Updates all of the contained Sectors within the World based on the current WorldState.
		/// </summary>
		void Update();

		/// <summary>
		/// Creates and returns a clone of this World.
		/// </summary>
		/// <returns>The address of the newly allocated clone of this World</returns>
		gsl::owner<Scope*> Clone() const override;

		/// <summary>
		/// Returns a string representation of the World (its name).
		/// </summary>
		/// <returns>A string representation of the World</returns>
		std::string ToString() const override;

		/// <summary>
		/// Checks whether or not two Entities are equal.
		/// </summary>
		/// <param name="rhs">An RTTI* to the World object this is being compared to</param>
		/// <returns>True if the objects are logically equal, false otherwise</returns>
		bool Equals(const RTTI * rhs) const override;

	protected:

		/// <summary>
		/// The name of the World.
		/// </summary>
		std::string mName;

		/// <summary>
		/// WorldState that holds the game time and pointers to the current world, sector, entity, and action.
		/// </summary>
		WorldState mWorldState;

		/// <summary>
		/// The game clock that is used to update the GameTime contained within the WorldState	
		/// </summary>
		GameClock mGameClock;

		/// <summary>
		/// The EventQueue used to manage events within the world	
		/// </summary>
		EventQueue mEventQueue;

		/// <summary>
		/// An array of Scopes pending delete
		/// </summary>
		Vector<Scope*> mPendingDelete;
	};
}
