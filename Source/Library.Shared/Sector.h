#pragma once
#include "Attributed.h"
#include "Factory.h"
#include "World.h"
#include "WorldState.h"
#include "Entity.h"

namespace Library
{
	class Sector final : public Attributed
	{
		RTTI_DECLARATIONS(Sector, Attributed);

	public:

		/// <summary>
		/// The index of the Entities attribute in the ordered vector
		/// </summary>
		inline static size_t EntitiesKey = 2;

		/// <summary>
		/// Returns the signatures of the prescribed attributed of this class.
		/// </summary>
		/// <returns>Vector of signatures of the prescribed attributes</returns>
		static const Vector<Signature> GetSignatures();

		/// <summary>
		/// Default constructor.
		/// </summary>
		Sector();
		/// <summary>
		/// Default copy constructor.
		/// </summary>
		/// <param name="rhs">The Sector being copied into this one</param>
		Sector(const Sector& rhs) = default;
		/// <summary>
		/// Default move constructor.
		/// </summary>
		/// <param name="rhs">The Sector being moved into this one</param>
		Sector(Sector&& rhs) = default;
		/// <summary>
		/// Default copy assignment operator.
		/// </summary>
		/// <param name="rhs">The Sector beign copied into this one</param>
		/// <returns>A reference to this Sector after its been mutated</returns>
		Sector& operator=(const Sector& rhs) = default;
		/// <summary>
		/// Default move assignment operator.
		/// </summary>
		/// <param name="rhs">The Sector beign copied into this one</param>
		/// <returns>A reference to this Sector after its been mutated</returns>
		Sector& operator=(Sector&& rhs) = default;
		/// <summary>
		/// Defaulted destructor.
		/// </summary>
		~Sector() = default;

		/// <summary>
		/// Searches this Scope as well as up the hierarchy and laterally within the other Sectors contained within this Sectors parent World
		/// for the Datum associated with the passed in name and value.
		/// </summary>
		/// <param name="name">The name of the attribute being looked for</param>
		/// <param name="value">The value of the attribute being looked for</param>
		/// <param name="foundScope">An optional pointer to a scope pointer that will point to the Scope where the desired attribute was found.</param>
		/// <returns>A pointer to the Datum found or nullptr otherwise</returns>
		Datum* SearchForValue(const std::string& name, const Datum& value, Scope** foundScope = nullptr) override;

		/// <summary>
		/// Returns a reference to the name of the Sector.
		/// </summary>
		/// <returns>Const std::string reference to the name of the Sector</returns>
		const std::string& Name() const;
		/// <summary>
		/// Sets the name of this Sector to the passed in name.
		/// </summary>
		/// <param name="name">The new name of the Sector</param>
		void SetName(const std::string & name);

		/// <summary>
		/// Returns the Datum reference that stores the contained entities in the sector.
		/// </summary>
		/// <returns>A reference to the Datum that containes the entities of this sector</returns>
		Datum& Entities();

		/// <summary>
		/// Returns the Datum reference that stores the contained entities in the sector.
		/// </summary>
		/// <returns>A const reference to the Datum that containes the entities of this sector</returns>
		const Datum& Entities() const;

		/// <summary>
		/// Creates a new entity in this Sector and returns the address of the newly created Entity.
		/// </summary>
		/// <param name="className">The string representation of the child class of Entity that should be created</param>
		/// <param name="instanceName">The name of the instance of the Entity</param>
		/// <returns>A pointer to the newly crated Entity within this Sector</returns>
		/// <exception cref="std::runtime_error">Throws an exception if the user passes in a class name that doesn't exist</exception>
		Entity* CreateEntity(const std::string& className, const std::string& instanceName);

		/// <summary>
		/// Returns the address of the World that owns this Sector.
		/// </summary>
		/// <returns>The address of the owning World</returns>
		World* GetWorld() const;
		/// <summary>
		/// Adopts this Sector into the passed in World.
		/// </summary>
		/// <param name="sector">The Sector that this Sector is being adopted into</param>
		void SetWorld(World& world);

		/// <summary>
		/// Updates the Sector based on the current WorldState.
		/// </summary>
		/// <param name="state">A reference to the current WorldState that this Sector exists within</param>
		void Update(WorldState& state);

		/// <summary>
		/// Creates and returns a clone of this Sector.
		/// </summary>
		/// <returns>The address of the newly allocated clone of this Sector</returns>
		gsl::owner<Scope*> Clone() const override;

		/// <summary>
		/// Returns a string representation of the Sector (its name).
		/// </summary>
		/// <returns>A string representation of the Sector</returns>
		std::string ToString() const override;

		/// <summary>
		/// Checks whether or not two Entities are equal.
		/// </summary>
		/// <param name="rhs">An RTTI* to the Sector object this is being compared to</param>
		/// <returns>True if the objects are logically equal, false otherwise</returns>
		bool Equals(const RTTI * rhs) const override;

	protected:

		/// <summary>
		/// The name of the Sector.
		/// </summary>
		std::string mName;
	};

	ConcreteFactory(Sector, Scope);
}

