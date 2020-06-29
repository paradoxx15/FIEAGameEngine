#pragma once
#include "Attributed.h"
#include "Sector.h"
#include "WorldState.h"
#include "Action.h"
#include "Factory.h"

namespace Library
{
	class Entity : public Attributed
	{
		RTTI_DECLARATIONS(Entity, Attributed)

	public:
		/// <summary>
		/// The index of the Actions attribute in the ordered vector
		/// </summary>
		inline static size_t ActionsKey = 2;

		/// <summary>
		/// Returns the signatures of the prescribed attributed of this class.
		/// </summary>
		/// <returns>Vector of signatures of the prescribed attributes</returns>
		static const Vector<Signature> GetSignatures();

		/// <summary>
		/// Default constructor.
		/// </summary>
		Entity();
		/// <summary>
		/// Constructor that initializes the entities name with the passed in argument.
		/// </summary>
		/// <param name="name">The name of the entity</param>
		Entity(const std::string& name);
		/// <summary>
		/// Default copy constructor.
		/// </summary>
		/// <param name="rhs">The Entity being copied into this one</param>
		Entity(const Entity& rhs) = default;
		/// <summary>
		/// Default move constructor.
		/// </summary>
		/// <param name="rhs">The Entity being moved into this one</param>
		Entity(Entity&& rhs) = default;
		/// <summary>
		/// Default copy assignment operator.
		/// </summary>
		/// <param name="rhs">The entity beign copied into this one</param>
		/// <returns>A reference to this Entity after its been mutated</returns>
		Entity& operator=(const Entity& rhs) = default;
		/// <summary>
		/// Default move assignment operator.
		/// </summary>
		/// <param name="rhs">The entity beign copied into this one</param>
		/// <returns>A reference to this Entity after its been mutated</returns>
		Entity& operator=(Entity&& rhs) = default;
		/// <summary>
		/// Virtual destructor. Defaulted for the base class.
		/// </summary>
		virtual ~Entity() = default;


		/// <summary>
		/// Searches this Scope as well as up the hierarchy and laterally within the other Entities contained within this Entities parent Sector
		/// for the Datum associated with the passed in name and value.
		/// </summary>
		/// <param name="name">The name of the attribute being looked for</param>
		/// <param name="value">The value of the attribute being looked for</param>
		/// <param name="foundScope">An optional pointer to a scope pointer that will point to the Scope where the desired attribute was found.</param>
		/// <returns>A pointer to the Datum found or nullptr otherwise</returns>
		Datum* SearchForValue(const std::string& name, const Datum& value, Scope** foundScope = nullptr) override;

		/// <summary>
		/// Returns the Datum reference that stores the contained actions of this Entity.
		/// </summary>
		/// <returns>A reference to the Datum that contains the actions of this Entity</returns>
		Datum& Actions();
		/// <summary>
		/// Returns the Datum reference that stores the contained actions of this Entity.
		/// </summary>
		/// <returns>A const reference to the Datum that contains the actions of this Entity</returns>
		const Datum& Actions() const;

		/// <summary>
		/// Creates a new action for this Entity and returns the address of the newly created Action.
		/// </summary>
		/// <param name="className">The string representation of the child class of Action that should be created</param>
		/// <param name="instanceName">The name of the instance of the Action</param>
		/// <returns>A pointer to the newly crated Action</returns>
		/// <exception cref="std::runtime_error">Throws an exception if the user passes in a class name that doesn't exist</exception>
		Action* CreateAction(const std::string& className, const std::string& instanceName);

		/// <summary>
		/// Returns a reference to the name of the Entity.
		/// </summary>
		/// <returns>Const std::string reference to the name of the Entity</returns>
		const std::string& Name() const;
		/// <summary>
		/// Sets the name of this entity to the passed in name.
		/// </summary>
		/// <param name="name">The new name of the Entity</param>
		void SetName(const std::string& name);

		/// <summary>
		/// Returns the address of the sector that owns this Entity.
		/// </summary>
		/// <returns>The address of the owning Sector</returns>
		Sector* GetSector() const;
		/// <summary>
		/// Adopts this entity into the passed in Sector.
		/// </summary>
		/// <param name="sector">The Sector that this entity is being adopted into</param>
		void SetSector(Sector& sector);

		/// <summary>
		/// Updates the entity based on the current WorldState.
		/// </summary>
		/// <param name="state">A reference to the current WorldState that this Entity exists within</param>
		virtual void Update(WorldState& state);

		/// <summary>
		/// Creates and returns a clone of this Entity.
		/// </summary>
		/// <returns>The address of the newly allocated clone of this Entity</returns>
		gsl::owner<Scope*> Clone() const override;

		/// <summary>
		/// Returns a string representation of the Entity (its name).
		/// </summary>
		/// <returns>A string representation of the Entity</returns>
		std::string ToString() const override;

		/// <summary>
		/// Checks whether or not two Entities are equal.
		/// </summary>
		/// <param name="rhs">An RTTI* to the Entity object this is being compared to</param>
		/// <returns>True if the objects are logically equal, false otherwise</returns>
		bool Equals(const RTTI* rhs) const override;

	protected:
		/// <summary>
		/// Explicit protected constructor that takes a passed in typeId to be passed to the base Attributed constructor.
		/// </summary>
		/// <param name="typeId">The typeId of the child class</param>
		explicit Entity(RTTI::IdType typeId);
		/// <summary>
		/// Explicit protected constructor that takes a passed in typeId to be passed to the base Attributed constructor.
		/// </summary>
		/// <param name="typeId">The typeId of the child class</param>
		/// <param name="name">The name of this Entity</param>
		explicit Entity(RTTI::IdType typeId, const std::string& name);

		/// <summary>
		/// The name of the Entity.
		/// </summary>
		std::string mName;
	};

	ConcreteFactory(Entity, Scope)
}